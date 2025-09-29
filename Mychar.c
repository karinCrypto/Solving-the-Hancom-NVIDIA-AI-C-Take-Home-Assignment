#include "main.h"

// 플레이어 비행선
MYSHIP myship;

// 플레이어 총알 배열 (최대 발사 수 제한)
BULLET myship_bullet[MAXMY_BULLET];

// 플레이어 비행선 모양
char myship_shape[10] = "-i^i-";


/* =======================================
   플레이어 초기화
   - 시작 위치와 상태 설정
   ======================================= */
void InitMyship()
{
    myship.flag = TRUE;                   // 비행선 활성화
    myship.pos.x = MYSHIP_BASE_POSX;      // 초기 X좌표
    myship.pos.y = MYSHIP_BASE_POSY;      // 초기 Y좌표
}


/* =======================================
   플레이어 비행선 그리기
   - 이전 위치 지우고 새 위치에 그리기
   ======================================= */
void DrawMyship(UPOINT* pt, UPOINT* oldpt)
{
    gotoxy(*oldpt);              // 이전 위치 이동
    printf("     ");             // 5칸 지우기 (모양 길이에 맞춤)

    gotoxy(*pt);                 // 새 위치 이동
    printf("%s", myship_shape);  // 플레이어 출력
}


/* =======================================
   플레이어 총알 이동/그리기
   - 위쪽으로 한 칸씩 이동
   - 화면 밖(y < 1) 도달 시 비활성화
   ======================================= */
void DrawMyBullet()
{
    int i;
    UPOINT ptpos, oldpos;

    for (i = 0; i < MAXENEMY_BULLET; i++) {
        if (myship_bullet[i].flag == TRUE) {
            if (myship_bullet[i].pos.y < 1) { // 화면 위 벗어남
                myship_bullet[i].flag = FALSE;
                oldpos = myship_bullet[i].pos;
                gotoxy(oldpos);
                printf(" ");
                break;
            }

            oldpos = myship_bullet[i].pos;
            --myship_bullet[i].pos.y;      // 위로 이동
            ptpos = myship_bullet[i].pos;

            gotoxy(oldpos);
            printf(" ");                   // 이전 위치 지우기
            gotoxy(ptpos);
            printf("!");                   // 총알 표시
        }
    }
}


/* =======================================
   플레이어 총알 발사
   - 현재 내 비행선 위치 기준으로 생성
   - X좌표는 기체 가운데에서 발사
   ======================================= */
void MyBulletshot(UPOINT ptthisMypos)
{
    int i;

    for (i = 0; i < MAXMY_BULLET; i++) {
        if (myship_bullet[i].flag == FALSE) {
            myship_bullet[i].flag = TRUE;
            myship_bullet[i].pos.x = ptthisMypos.x + 2; // 중앙 정렬
            myship_bullet[i].pos.y = ptthisMypos.y - 1; // 내 위에서 시작
            break;
        }
    }
}


/* =======================================
   플레이어 피격 판정
   - 적 총알(enemy_bullet)이 내 기체에 닿았는지 확인
   - 기체 크기(X좌표 + 4까지) 범위 체크
   - 충돌 시 0 반환 (게임 오버 조건)
   ======================================= */
int CheckMybullet(UPOINT ptthisMypos)
{
    int i, flag = FALSE;

    for (i = 0; i < MAXENEMY_BULLET; i++) {
        if (enemy_bullet[i].flag == TRUE) {
            if (ptthisMypos.x <= enemy_bullet[i].pos.x &&
                enemy_bullet[i].pos.x <= ptthisMypos.x + 4 &&
                enemy_bullet[i].pos.y == ptthisMypos.y) {
                flag = TRUE;
                break;
            }
        }
    }

    if (flag == TRUE)
        return 0;   // 피격 → 게임 종료
    else
        return 1;   // 안전
}
