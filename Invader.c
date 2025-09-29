#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>     // _getch(), _kbhit() 같은 콘솔 입력 함수
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>   // GetTickCount(), 콘솔 제어
#include "main.h"
#include "console.h"

// ==============================
// 전역 상태 변수
// ==============================
UPOINT ptthisMypos;           // 내 비행기의 현재 좌표
int    timeflag = FALSE;      // 게임 상태 플래그 (FALSE = 진행 중, TRUE = 미션 클리어)
int    score = 0;             // 현재 점수
int    hiscore = 2500;        // 최고 점수
int    killnum = 0;           // 처치한 적 수
char* Aboom[8];              // 폭발 애니메이션 프레임

// 대시/무적 관련 변수
DWORD g_lastDodgeTick = 0;    // 마지막 대시 입력 시각
DWORD g_invincibleUntil = 0;  // 무적이 지속되는 시간(밀리초)
const int DODGE_COOLDOWN_MS = 800;  // 대시 쿨타임 (0.8초)
const int INVINCIBLE_MS = 250;      // 무적 시간 (0.25초)

// 내부 함수 선언
static void GrantInvincible(int ms);  // 무적 상태 부여
static int  IsPlayerHitNow(UPOINT pos);   // 피격 판정
static int  WasHitRawNow(UPOINT pos);     // 무적 무시 피격 판정
static void ShowReaction(char* buf, DWORD* until, const char* msg, DWORD ms); // 리액션 메시지 출력
static const char* PickRand(const char** arr, int n); // 랜덤 문자열 선택
void play(void); // 게임 플레이 루프

// 발사할 때 랜덤하게 나올 리액션 메시지
static const char* RE_RANDOM[] = {
    "나이스샷!", "굿!", "깔끔해!", "가즈아!", "집중!", "좋아!", "멋지다!"
};

// ==============================
// 유틸 함수 정의
// ==============================
static void GrantInvincible(int ms) { g_invincibleUntil = GetTickCount() + ms; }

static int IsPlayerHitNow(UPOINT pos) {
    // 총알 충돌 체크
    int raw = (CheckMybullet(pos) == 0);
    if (!raw) return 0;
    // 무적 상태라면 피격 무효
    if (GetTickCount() < g_invincibleUntil) return 0;
    return 1;
}

static int WasHitRawNow(UPOINT pos) { return (CheckMybullet(pos) == 0); }

static void ShowReaction(char* buf, DWORD* until, const char* msg, DWORD ms) {
    // 메시지를 buf에 복사하고 일정 시간(ms) 동안 표시
    strcpy(buf, msg);
    *until = GetTickCount() + ms;
}

static const char* PickRand(const char** arr, int n) {
    // 배열에서 랜덤하게 하나 선택
    return arr[rand() % n];
}

// ==============================
// 프로그램 시작점
// ==============================
int main(void)
{
    srand((unsigned)time(NULL));  // 랜덤 초기화
    UPOINT ptend; int loop = 1;

    system("cls");        // 화면 클리어
    InitConsole();        // 콘솔 초기화
    DrawTitleBar("PinkBridge"); // 상단 타이틀 출력

    // 폭발 애니메이션 프레임 설정
    Aboom[0] = "i<^>i";  Aboom[1] = "i(*)i";
    Aboom[2] = "(* *)";  Aboom[3] = "(** **)";
    Aboom[4] = " (* *) "; Aboom[5] = "  (*)  ";
    Aboom[6] = "   *   "; Aboom[7] = "       ";

    ptend.x = 36; ptend.y = 12; // "게임 오버" 메시지 위치

    while (loop)
    {
        DWORD tick = GetTickCount(), bcount = tick; int bp = 0;
        score = 0; killnum = 0; timeflag = 0;  // 초기화

        play();  // 게임 실행

        // ==============================
        // 폭발 애니메이션 연출
        // ==============================
        for (;;)
        {
            DrawTitleBar("PinkBridge");
            if (timeflag == FALSE) // 적 총알에 맞은 경우
            {
                tick = GetTickCount();
                if (tick - bcount > 100) // 0.1초마다 프레임 변경
                {
                    gotoxy(ptthisMypos);
                    printf("%s", Aboom[bp]); // 폭발 출력
                    if (++bp > 7) break; // 끝나면 종료
                    bcount = tick;
                }
            }
            else break; // 미션 클리어 시 즉시 종료
        }

        // 게임 오버 메시지 출력
        DrawTitleBar("PinkBridge");
        gotoxy_xy(ptend.x, ptend.y);
        printf("당신의 비행기는 파괴되었습니다.");
        gotoxy_xy(ptend.x, ++ptend.y);
        printf("다시 할까요? (y/n)\n");

        // y 입력 시 재시작, n 입력 시 종료
        if (_getch() == 'y') {
            ClearScreen();
            DrawTitleBar("PinkBridge");
            ptend.y = 12;
        }
        else loop = 0;
    }
    return 0;
}

// ==============================
// 실제 게임 플레이 루프
// ==============================
void play(void)
{
    static UPOINT ptMyoldpos;
    DWORD gTick = GetTickCount();
    DWORD gCount = gTick;
    DWORD Count = gTick;
    DWORD lastFire = 0;
    const DWORD FIRE_RATE_MS = 150; // 발사 간격 (0.15초)

    // 리액션 메시지 저장
    char  reactionMsg[64] = "";
    DWORD reactionUntil = 0;

    // 점수, 하이스코어, 리액션 표시 위치
    UPOINT ptscore = { 68, 2 };
    UPOINT pthi = { 2, 2 };
    UPOINT ptreact = { 30, 25 };

    int juckspeed = 500;  // 적 이동 속도 초기값
    int prevSpeed = juckspeed;

    // 비행기, 적 초기화
    InitMyship();
    Initenemyship();

    // 플레이어 위치 초기화
    ptthisMypos.x = ptMyoldpos.x = MYSHIP_BASE_POSX;
    ptthisMypos.y = ptMyoldpos.y = MYSHIP_BASE_POSY;

    g_lastDodgeTick = 0;    // 대시 초기화
    g_invincibleUntil = 0;  // 무적 해제

    while (TRUE)
    {
        DrawTitleBar("PinkBridge");
        gTick = GetTickCount();

        // ==============================
        // 이동 & 대시 처리
        // ==============================
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 0 || ch == 224) { // 방향키 입력
                ch = _getch();
                switch (ch) {
                case 72: // ↑
                    ptMyoldpos.y = ptthisMypos.y;
                    if (--ptthisMypos.y < 3) ptthisMypos.y = 3;
                    DrawMyship(&ptthisMypos, &ptMyoldpos); break;
                case 80: // ↓
                    ptMyoldpos.y = ptthisMypos.y;
                    if (++ptthisMypos.y > 24) ptthisMypos.y = 24;
                    DrawMyship(&ptthisMypos, &ptMyoldpos); break;
                case 75: // ←
                    ptMyoldpos.x = ptthisMypos.x;
                    if (--ptthisMypos.x < 1) ptthisMypos.x = 1;
                    DrawMyship(&ptthisMypos, &ptMyoldpos); break;
                case 77: // →
                    ptMyoldpos.x = ptthisMypos.x;
                    if (++ptthisMypos.x > 78) ptthisMypos.x = 78;
                    DrawMyship(&ptthisMypos, &ptMyoldpos); break;
                }
            }
            else if (ch == 's' || ch == 'S') { // 대시 키 입력
                if (gTick - g_lastDodgeTick >= (DWORD)DODGE_COOLDOWN_MS) {
                    ptMyoldpos.x = ptthisMypos.x;
                    ptthisMypos.x += 3; // 오른쪽으로 순간이동
                    if (ptthisMypos.x > 78) ptthisMypos.x = 78;
                    DrawMyship(&ptthisMypos, &ptMyoldpos);
                    GrantInvincible(INVINCIBLE_MS); // 무적 부여
                    g_lastDodgeTick = gTick;
                    ShowReaction(reactionMsg, &reactionUntil, "대시!", 700);
                }
            }
        }

        // ==============================
        // 총알 발사 (Space, A키)
        // ==============================
        if ((GetAsyncKeyState(VK_SPACE) & 0x8000) || (GetAsyncKeyState('A') & 0x8000))
        {
            if (gTick - lastFire >= FIRE_RATE_MS) {
                MyBulletshot(ptthisMypos); // 내 총알 발사
                lastFire = gTick;
                ShowReaction(reactionMsg, &reactionUntil,
                    PickRand(RE_RANDOM, sizeof(RE_RANDOM) / sizeof(RE_RANDOM[0])),
                    1000);
            }
        }

        // ==============================
        // 점수 / 피격 처리
        // ==============================
        if (gTick - Count > 15)
        {
            if (IsPlayerHitNow(ptthisMypos)) { // 피격 판정
                if (score > hiscore) { // 최고 점수 갱신
                    hiscore = score;
                    ShowReaction(reactionMsg, &reactionUntil, "신기록!", 1100);
                }
                break; // 게임 종료
            }

            // 적 총알 처리
            CheckenemyBullet(enemyship);
            DrawMyBullet();
            DrawMyship(&ptthisMypos, &ptMyoldpos);

            gotoxy_xy(ptscore.x, ptscore.y);
            printf("점수 : %d   ", score);

            // 미션 클리어 조건
            if (killnum >= 30) {
                timeflag = TRUE;
                ShowReaction(reactionMsg, &reactionUntil, "MISSION COMPLETE!", 2500);
                score = 2500; // 강제 2500점 부여
                break;
            }

            // 적 속도 단계별 가속
            if (killnum > 20 && prevSpeed != 150) {
                juckspeed = 150;
                ShowReaction(reactionMsg, &reactionUntil, "Final Rush!", 1000);
            }
            else if (killnum > 10 && prevSpeed == 500) {
                juckspeed = 300;
                ShowReaction(reactionMsg, &reactionUntil, "Speed Up!", 1000);
            }
            prevSpeed = juckspeed;

            gotoxy_xy(pthi.x, pthi.y);
            printf("최고점수 : %d   ", hiscore);

            Count = gTick;
        }

        // ==============================
        // 적 이동 / 총알 처리
        // ==============================
        if (gTick - gCount > juckspeed)
        {
            Bulletshot();        // 적 총알 발사
            DrawBullet();        // 총알 그리기
            CalenemyshipPos();   // 적 이동(적의 위치를 계산한다.  Calculate enemy ship position
            Drawenemyship();     // 적 그리기
            if (Checkenemypos() == 1) break; // 적이 화면 끝 도달
            gCount = gTick;
        }

        // ==============================
        // 리액션 메시지 출력 / 삭제
        // ==============================
        if (reactionMsg[0] && gTick < reactionUntil) {
            gotoxy_xy(5, 20);  // 좌표 바꿔보기
            printf("%s      ", reactionMsg);
        }
        else if (reactionMsg[0] && gTick >= reactionUntil) {
            gotoxy_xy(ptreact.x, ptreact.y);
            printf("                        "); // 지우기
            reactionMsg[0] = '\0';
        }
    }
}
