// ====================== ���� ���� ��ũ�� ���� ======================
//������ 25.09.25 (KARIN)

#define MAX_ENEMY           40    // �� ����� �� ���� (�� * ��)
#define MAXENEMY_BASE_ROW    4    // �� ��ġ �� ��
#define MAXENEMY_BASE_COL   10    // �� ��ġ �� ��
#define MAXENEMY_BULLET     10    // ���ÿ� ȭ�鿡 ���� ������ �� �Ѿ� ����
#define ENEMYSHIP_BASE_POSX 20    // �� ��� ���� X��ǥ
#define ENEMYSHIP_BASE_POSY  2    // �� ��� ���� Y��ǥ
#define TRUE                 1
#define FALSE                0

#define MAXMY_BULLET         3    // �Ʊ�(�÷��̾�) �Ѿ� �ִ� ����
#define MYSHIP_BASE_POSX    38    // �÷��̾�(�� �����) ���� X��ǥ
#define MYSHIP_BASE_POSY    23    // �÷��̾�(�� �����) ���� Y��ǥ (�ٴ� ����)


// ====================== �ý��� ��� ======================

#include <windows.h>   // Win32 API (�ܼ� ��ġ, Sleep ��)
#include <stdio.h>     // ǥ�� ����� (printf ��)
#include <conio.h>     // �ܼ� �Է� (_getch, _kbhit)
#include <wincon.h>    // �ܼ� Ŀ��/��ǥ ���� API
#include "console.h"   // gotoxy, ClearScreen ���� Ŀ���� �ܼ� �Լ�


// ====================== ����ü ���� ======================

// �÷��̾� ����� ����ü
typedef struct {
    int flag;     // Ȱ��ȭ ���� (1=����, 0=��Ȱ��)
    UPOINT pos;   // ���� ��ǥ (x, y)
} MYSHIP;

// �Ѿ� ����ü (�Ʊ�/���� ����)
typedef struct {
    int flag;     // Ȱ��ȭ ���� (�߻���=1, ����=0)
    UPOINT pos;   // �Ѿ� ��ġ
} BULLET;

// �� ����� ����ü
typedef struct {
    int flag;     // Ȱ��ȭ ����
    UPOINT pos;   // ���� ��ǥ
} ENEMYSHIP;


// ====================== ���� ���� �ܺ� ���� ======================

extern BULLET     myship_bullet[MAXMY_BULLET];      // �Ʊ� �Ѿ� �迭
extern BULLET     enemy_bullet[MAXENEMY_BULLET];    // ���� �Ѿ� �迭
extern ENEMYSHIP  enemyship[MAX_ENEMY];             // �� ����� �迭
extern int        score, killnum;                   // ����, óġ ��


// ====================== �Լ� ���� ���� ======================

// ---- �÷��̾� ���� ----
void DrawMyship(UPOINT* pt, UPOINT* old);  // �� ����� ��ġ �׸��� (���� ��ġ/���� ��ġ)
void InitMyship();                         // �� ����� �ʱ�ȭ
void MyBulletshot(UPOINT pos);             // �� ����� �Ѿ� �߻�
void DrawMyBullet();                       // �� �Ѿ� �̵�/�׸���
int  CheckMybullet(UPOINT ptthisMypos);    // �� �Ѿ��� �� ������� ����

// ---- �� ���� ----
void Drawenemyship();                      // �� ��ü �׸���
void Initenemyship();                      // �� �ʱ�ȭ
void CalenemyshipPos();                    // �� �̵� ��� (�¿�/�ϰ�)
int  Checkenemypos();                      // ���� �ٴڱ��� �����Դ��� ����
int  Calflag();                            // �� �̵� ���� ��� ���

// ---- �ý���/�ð� ���� ----
void CalTime();                            // �ð� �帧 ��� (tick ���, ���� �ʿ�)

// ---- �� �Ѿ� ���� ----
void Bulletshot();                         // �� �Ѿ� �߻�
void DrawBullet();                         // �� �Ѿ� �̵�/�׸���
void InitBullet();                         // �� �Ѿ� �ʱ�ȭ

// ---- �浹 ó�� ----
void CheckenemyBullet(ENEMYSHIP* enemy);   // �� �Ѿ˰� �� �浹 ����

// ---- ȿ�� ----
void boom(UPOINT pt, int type);            // ���� ����Ʈ ��� (pt ��ġ���� type�� ����)

// ---- ���� ���� ----
void play();                               // ���� ���� ���� ����
