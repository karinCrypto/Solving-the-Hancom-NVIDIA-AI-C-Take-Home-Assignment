#include "main.h"

// �÷��̾� ���༱
MYSHIP myship;

// �÷��̾� �Ѿ� �迭 (�ִ� �߻� �� ����)
BULLET myship_bullet[MAXMY_BULLET];

// �÷��̾� ���༱ ���
char myship_shape[10] = "-i^i-";


/* =======================================
   �÷��̾� �ʱ�ȭ
   - ���� ��ġ�� ���� ����
   ======================================= */
void InitMyship()
{
    myship.flag = TRUE;                   // ���༱ Ȱ��ȭ
    myship.pos.x = MYSHIP_BASE_POSX;      // �ʱ� X��ǥ
    myship.pos.y = MYSHIP_BASE_POSY;      // �ʱ� Y��ǥ
}


/* =======================================
   �÷��̾� ���༱ �׸���
   - ���� ��ġ ����� �� ��ġ�� �׸���
   ======================================= */
void DrawMyship(UPOINT* pt, UPOINT* oldpt)
{
    gotoxy(*oldpt);              // ���� ��ġ �̵�
    printf("     ");             // 5ĭ ����� (��� ���̿� ����)

    gotoxy(*pt);                 // �� ��ġ �̵�
    printf("%s", myship_shape);  // �÷��̾� ���
}


/* =======================================
   �÷��̾� �Ѿ� �̵�/�׸���
   - �������� �� ĭ�� �̵�
   - ȭ�� ��(y < 1) ���� �� ��Ȱ��ȭ
   ======================================= */
void DrawMyBullet()
{
    int i;
    UPOINT ptpos, oldpos;

    for (i = 0; i < MAXENEMY_BULLET; i++) {
        if (myship_bullet[i].flag == TRUE) {
            if (myship_bullet[i].pos.y < 1) { // ȭ�� �� ���
                myship_bullet[i].flag = FALSE;
                oldpos = myship_bullet[i].pos;
                gotoxy(oldpos);
                printf(" ");
                break;
            }

            oldpos = myship_bullet[i].pos;
            --myship_bullet[i].pos.y;      // ���� �̵�
            ptpos = myship_bullet[i].pos;

            gotoxy(oldpos);
            printf(" ");                   // ���� ��ġ �����
            gotoxy(ptpos);
            printf("!");                   // �Ѿ� ǥ��
        }
    }
}


/* =======================================
   �÷��̾� �Ѿ� �߻�
   - ���� �� ���༱ ��ġ �������� ����
   - X��ǥ�� ��ü ������� �߻�
   ======================================= */
void MyBulletshot(UPOINT ptthisMypos)
{
    int i;

    for (i = 0; i < MAXMY_BULLET; i++) {
        if (myship_bullet[i].flag == FALSE) {
            myship_bullet[i].flag = TRUE;
            myship_bullet[i].pos.x = ptthisMypos.x + 2; // �߾� ����
            myship_bullet[i].pos.y = ptthisMypos.y - 1; // �� ������ ����
            break;
        }
    }
}


/* =======================================
   �÷��̾� �ǰ� ����
   - �� �Ѿ�(enemy_bullet)�� �� ��ü�� ��Ҵ��� Ȯ��
   - ��ü ũ��(X��ǥ + 4����) ���� üũ
   - �浹 �� 0 ��ȯ (���� ���� ����)
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
        return 0;   // �ǰ� �� ���� ����
    else
        return 1;   // ����
}
