#include "main.h"

// 적 우주선 모양 (문자열)
char enemyship_shape[5] = "^V^";

// 전역 배열 선언
ENEMYSHIP enemyship[MAX_ENEMY];            // 적 우주선 전체
UPOINT    ptOld[MAX_ENEMY];                // 이전 좌표 (지우기용)
BULLET    enemy_bullet[MAXENEMY_BULLET];   // 적의 총알
short     flag;                            // 좌우 이동 방향 플래그


/* =======================================
   적 우주선 초기화
   - 행렬 배치로 적 우주선 생성
   - 초기 좌표 저장
   ======================================= */
void Initenemyship()
{
    int i, j, k = 0;

    for (i = 0; i < MAXENEMY_BASE_ROW; i++) {
        for (j = 0; j < MAXENEMY_BASE_COL; j++) {
            enemyship[k].flag = TRUE;   // 활성화
            enemyship[k].pos.x = ENEMYSHIP_BASE_POSX + j * 4;
            enemyship[k].pos.y = ENEMYSHIP_BASE_POSY + i * 2;
            ptOld[k] = enemyship[k].pos; // 이전 위치 기억
            k++;
        }
    }
    InitBullet();  // 적 총알도 초기화
}


/* =======================================
   적 우주선 이동
   - 좌우 방향은 flag로 결정
   - 벽에 닿으면 아래로 한 칸 이동
   ======================================= */
void CalenemyshipPos()
{
    int i, j, k = 0;
    int XAdd, YAdd;

    YAdd = Calflag();                 // 벽에 닿았는지 확인
    XAdd = (flag == FALSE) ? 1 : -1;  // 방향 결정

    for (i = 0; i < MAXENEMY_BASE_ROW; i++) {
        for (j = 0; j < MAXENEMY_BASE_COL; j++) {
            ptOld[k] = enemyship[k].pos;   // 기존 위치 저장
            enemyship[k].pos.x += XAdd;    // 좌우 이동
            enemyship[k].pos.y += YAdd;    // 아래 이동
            k++;
        }
    }
}


/* =======================================
   벽 충돌 여부 확인
   - 좌측/우측 벽 닿으면 방향 반전
   - add = 1 → 아래로 한 칸 이동
   ======================================= */
int Calflag()
{
    int add;

    if (enemyship[0].pos.x < 1 || enemyship[0].pos.x > 40) {
        flag = (flag == TRUE) ? FALSE : TRUE;  // 방향 반전
        add = 1; // 한 칸 아래 이동
    }
    else {
        add = 0; // 이동 없음
    }
    return add;
}


/* =======================================
   적 우주선 출력
   - 이전 위치 지우고 새 위치 그리기
   ======================================= */
void Drawenemyship()
{
    UPOINT pos, posOld;
    int i;

    for (i = 0; i < MAX_ENEMY; i++) {
        if (enemyship[i].flag == TRUE) {
            posOld = ptOld[i];
            pos = enemyship[i].pos;

            gotoxy(posOld);           // 이전 위치 이동
            printf("   ");            // 지우기

            gotoxy(pos);              // 새 위치 이동
            printf("%s", enemyship_shape);
        }
    }
}


/* =======================================
   총알 발사할 적 선택
   - 랜덤으로 살아있는 적을 선택
   ======================================= */
int BulletNum()
{
    int num;
    while (1) {
        num = rand() % 40;
        if (enemyship[num].flag == FALSE)
            continue;
        break;
    }
    return num;
}


/* =======================================
   적 총알 초기화
   ======================================= */
void InitBullet()
{
    int i;
    for (i = 0; i < MAXENEMY_BULLET; i++) {
        enemy_bullet[i].flag = FALSE;
        enemy_bullet[i].pos.x = 0;
        enemy_bullet[i].pos.y = 0;
    }
}


/* =======================================
   적 총알 발사
   - BulletNum()으로 선택된 적에서 총알 생성
   ======================================= */
void Bulletshot()
{
    int i, num;
    num = BulletNum();

    for (i = 0; i < MAXENEMY_BULLET; i++) {
        if (enemy_bullet[i].flag == FALSE) {
            enemy_bullet[i].flag = TRUE;
            enemy_bullet[i].pos = enemyship[num].pos; // 적 위치에서 시작
            break;
        }
    }
}


/* =======================================
   적 총알 그리기/이동
   - 한 칸씩 아래로 이동
   - 화면 아래 도달 시 삭제
   ======================================= */
void DrawBullet()
{
    int i;
    UPOINT ptpos, oldpos;

    for (i = 0; i < MAXENEMY_BULLET; i++) {
        if (enemy_bullet[i].flag == TRUE) {
            if (enemy_bullet[i].pos.y > 23) { // 바닥 도달
                enemy_bullet[i].flag = FALSE;
                oldpos = enemy_bullet[i].pos;
                gotoxy(oldpos);
                printf(" ");
                break;
            }

            oldpos = enemy_bullet[i].pos;
            ++enemy_bullet[i].pos.y;   // 아래로 이동
            ptpos = enemy_bullet[i].pos;

            gotoxy(oldpos);
            printf(" ");               // 이전 위치 지우기
            gotoxy(ptpos);
            printf("|");               // 총알 표시
        }
    }
}


/* =======================================
   적이 바닥(y=23)에 닿았는지 확인
   - TRUE 반환 → 게임 오버 조건
   ======================================= */
int Checkenemypos()
{
    int flag = FALSE, i;

    for (i = 0; i < MAX_ENEMY; i++) {
        if (enemyship[i].flag == TRUE && enemyship[i].pos.y == 23) {
            flag = TRUE;
            break;
        }
    }
    return flag;
}


/* =======================================
   내 총알과 적 충돌 체크
   - 충돌 시 적 제거, 점수 추가, 폭발 이펙트 출력
   ======================================= */
void CheckenemyBullet(ENEMYSHIP* enemyship)
{
    int i, j;
    static BULLET boompos[MAXMY_BULLET]; // 폭발 이펙트 위치 저장

    // 이전 프레임 폭발 흔적 지우기
    for (i = 0; i < MAXMY_BULLET; i++) {
        if (boompos[i].flag == TRUE) {
            gotoxy(boompos[i].pos);
            printf("   ");
            boompos[i].flag = FALSE;
        }
    }

    // 충돌 판정
    for (i = 0; i < MAXMY_BULLET; i++) {
        if (myship_bullet[i].flag == TRUE) {
            for (j = 0; j < MAX_ENEMY; j++) {
                if (enemyship[j].flag == TRUE) {
                    if (enemyship[j].pos.x <= myship_bullet[i].pos.x &&
                        myship_bullet[i].pos.x <= (enemyship[j].pos.x + 2) &&
                        (enemyship[j].pos.y == myship_bullet[i].pos.y)) {

                        // 적 파괴
                        enemyship[j].flag = FALSE;
                        gotoxy(enemyship[j].pos);
                        printf(" *** ");   // 폭발 표시
                        myship_bullet[i].flag = FALSE;

                        score += 100;      // 점수
                        killnum++;         // 킬 수 증가

                        // 폭발 위치 저장
                        boompos[i].pos = enemyship[j].pos;
                        boompos[i].flag = TRUE;
                    }
                }
            }
        }
    }
}
