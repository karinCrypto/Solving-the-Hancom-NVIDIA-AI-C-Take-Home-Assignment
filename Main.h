// ====================== 게임 전역 매크로 정의 ======================
//이은지 25.09.25 (KARIN)

#define MAX_ENEMY           40    // 적 비행기 총 개수 (행 * 열)
#define MAXENEMY_BASE_ROW    4    // 적 배치 행 수
#define MAXENEMY_BASE_COL   10    // 적 배치 열 수
#define MAXENEMY_BULLET     10    // 동시에 화면에 존재 가능한 적 총알 개수
#define ENEMYSHIP_BASE_POSX 20    // 적 편대 시작 X좌표
#define ENEMYSHIP_BASE_POSY  2    // 적 편대 시작 Y좌표
#define TRUE                 1
#define FALSE                0

#define MAXMY_BULLET         3    // 아군(플레이어) 총알 최대 개수
#define MYSHIP_BASE_POSX    38    // 플레이어(내 비행기) 시작 X좌표
#define MYSHIP_BASE_POSY    23    // 플레이어(내 비행기) 시작 Y좌표 (바닥 라인)


// ====================== 시스템 헤더 ======================

#include <windows.h>   // Win32 API (콘솔 위치, Sleep 등)
#include <stdio.h>     // 표준 입출력 (printf 등)
#include <conio.h>     // 콘솔 입력 (_getch, _kbhit)
#include <wincon.h>    // 콘솔 커서/좌표 관련 API
#include "console.h"   // gotoxy, ClearScreen 같은 커스텀 콘솔 함수


// ====================== 구조체 정의 ======================

// 플레이어 비행기 구조체
typedef struct {
    int flag;     // 활성화 여부 (1=출현, 0=비활성)
    UPOINT pos;   // 현재 좌표 (x, y)
} MYSHIP;

// 총알 구조체 (아군/적군 공통)
typedef struct {
    int flag;     // 활성화 여부 (발사중=1, 꺼짐=0)
    UPOINT pos;   // 총알 위치
} BULLET;

// 적 비행기 구조체
typedef struct {
    int flag;     // 활성화 여부
    UPOINT pos;   // 현재 좌표
} ENEMYSHIP;


// ====================== 전역 변수 외부 선언 ======================

extern BULLET     myship_bullet[MAXMY_BULLET];      // 아군 총알 배열
extern BULLET     enemy_bullet[MAXENEMY_BULLET];    // 적군 총알 배열
extern ENEMYSHIP  enemyship[MAX_ENEMY];             // 적 비행기 배열
extern int        score, killnum;                   // 점수, 처치 수


// ====================== 함수 원형 선언 ======================

// ---- 플레이어 관련 ----
void DrawMyship(UPOINT* pt, UPOINT* old);  // 내 비행기 위치 그리기 (현재 위치/이전 위치)
void InitMyship();                         // 내 비행기 초기화
void MyBulletshot(UPOINT pos);             // 내 비행기 총알 발사
void DrawMyBullet();                       // 내 총알 이동/그리기
int  CheckMybullet(UPOINT ptthisMypos);    // 내 총알이 적 맞췄는지 판정

// ---- 적 관련 ----
void Drawenemyship();                      // 적 전체 그리기
void Initenemyship();                      // 적 초기화
void CalenemyshipPos();                    // 적 이동 계산 (좌우/하강)
int  Checkenemypos();                      // 적이 바닥까지 내려왔는지 판정
int  Calflag();                            // 적 이동 방향 토글 계산

// ---- 시스템/시간 관련 ----
void CalTime();                            // 시간 흐름 계산 (tick 기반, 구현 필요)

// ---- 적 총알 관련 ----
void Bulletshot();                         // 적 총알 발사
void DrawBullet();                         // 적 총알 이동/그리기
void InitBullet();                         // 적 총알 초기화

// ---- 충돌 처리 ----
void CheckenemyBullet(ENEMYSHIP* enemy);   // 내 총알과 적 충돌 판정

// ---- 효과 ----
void boom(UPOINT pt, int type);            // 폭발 이펙트 출력 (pt 위치에서 type에 따라)

// ---- 게임 루프 ----
void play();                               // 실제 게임 루프 실행
