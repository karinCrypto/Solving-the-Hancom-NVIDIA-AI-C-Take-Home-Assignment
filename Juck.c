#include "main.h"

// �� ���ּ� ��� (���ڿ�)
char enemyship_shape[5] = "^V^";

// ���� �迭 ����
ENEMYSHIP enemyship[MAX_ENEMY];            // �� ���ּ� ��ü
UPOINT    ptOld[MAX_ENEMY];                // ���� ��ǥ (������)
BULLET    enemy_bullet[MAXENEMY_BULLET];   // ���� �Ѿ�
short     flag;                            // �¿� �̵� ���� �÷���


/* =======================================
   �� ���ּ� �ʱ�ȭ
   - ��� ��ġ�� �� ���ּ� ����
   - �ʱ� ��ǥ ����
   ======================================= */
void Initenemyship()
{
    int i, j, k = 0;

    for (i = 0; i < MAXENEMY_BASE_ROW; i++) {
        for (j = 0; j < MAXENEMY_BASE_COL; j++) {
            enemyship[k].flag = TRUE;   // Ȱ��ȭ
            enemyship[k].pos.x = ENEMYSHIP_BASE_POSX + j * 4;
            enemyship[k].pos.y = ENEMYSHIP_BASE_POSY + i * 2;
            ptOld[k] = enemyship[k].pos; // ���� ��ġ ���
            k++;
        }
    }
    InitBullet();  // �� �Ѿ˵� �ʱ�ȭ
}


/* =======================================
   �� ���ּ� �̵�
   - �¿� ������ flag�� ����
   - ���� ������ �Ʒ��� �� ĭ �̵�
   ======================================= */
void CalenemyshipPos()
{
    int i, j, k = 0;
    int XAdd, YAdd;

    YAdd = Calflag();                 // ���� ��Ҵ��� Ȯ��
    XAdd = (flag == FALSE) ? 1 : -1;  // ���� ����

    for (i = 0; i < MAXENEMY_BASE_ROW; i++) {
        for (j = 0; j < MAXENEMY_BASE_COL; j++) {
            ptOld[k] = enemyship[k].pos;   // ���� ��ġ ����
            enemyship[k].pos.x += XAdd;    // �¿� �̵�
            enemyship[k].pos.y += YAdd;    // �Ʒ� �̵�
            k++;
        }
    }
}


/* =======================================
   �� �浹 ���� Ȯ��
   - ����/���� �� ������ ���� ����
   - add = 1 �� �Ʒ��� �� ĭ �̵�
   ======================================= */
int Calflag()
{
    int add;

    if (enemyship[0].pos.x < 1 || enemyship[0].pos.x > 40) {
        flag = (flag == TRUE) ? FALSE : TRUE;  // ���� ����
        add = 1; // �� ĭ �Ʒ� �̵�
    }
    else {
        add = 0; // �̵� ����
    }
    return add;
}


/* =======================================
   �� ���ּ� ���
   - ���� ��ġ ����� �� ��ġ �׸���
   ======================================= */
void Drawenemyship()
{
    UPOINT pos, posOld;
    int i;

    for (i = 0; i < MAX_ENEMY; i++) {
        if (enemyship[i].flag == TRUE) {
            posOld = ptOld[i];
            pos = enemyship[i].pos;

            gotoxy(posOld);           // ���� ��ġ �̵�
            printf("   ");            // �����

            gotoxy(pos);              // �� ��ġ �̵�
            printf("%s", enemyship_shape);
        }
    }
}


/* =======================================
   �Ѿ� �߻��� �� ����
   - �������� ����ִ� ���� ����
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
   �� �Ѿ� �ʱ�ȭ
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
   �� �Ѿ� �߻�
   - BulletNum()���� ���õ� ������ �Ѿ� ����
   ======================================= */
void Bulletshot()
{
    int i, num;
    num = BulletNum();

    for (i = 0; i < MAXENEMY_BULLET; i++) {
        if (enemy_bullet[i].flag == FALSE) {
            enemy_bullet[i].flag = TRUE;
            enemy_bullet[i].pos = enemyship[num].pos; // �� ��ġ���� ����
            break;
        }
    }
}


/* =======================================
   �� �Ѿ� �׸���/�̵�
   - �� ĭ�� �Ʒ��� �̵�
   - ȭ�� �Ʒ� ���� �� ����
   ======================================= */
void DrawBullet()
{
    int i;
    UPOINT ptpos, oldpos;

    for (i = 0; i < MAXENEMY_BULLET; i++) {
        if (enemy_bullet[i].flag == TRUE) {
            if (enemy_bullet[i].pos.y > 23) { // �ٴ� ����
                enemy_bullet[i].flag = FALSE;
                oldpos = enemy_bullet[i].pos;
                gotoxy(oldpos);
                printf(" ");
                break;
            }

            oldpos = enemy_bullet[i].pos;
            ++enemy_bullet[i].pos.y;   // �Ʒ��� �̵�
            ptpos = enemy_bullet[i].pos;

            gotoxy(oldpos);
            printf(" ");               // ���� ��ġ �����
            gotoxy(ptpos);
            printf("|");               // �Ѿ� ǥ��
        }
    }
}


/* =======================================
   ���� �ٴ�(y=23)�� ��Ҵ��� Ȯ��
   - TRUE ��ȯ �� ���� ���� ����
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
   �� �Ѿ˰� �� �浹 üũ
   - �浹 �� �� ����, ���� �߰�, ���� ����Ʈ ���
   ======================================= */
void CheckenemyBullet(ENEMYSHIP* enemyship)
{
    int i, j;
    static BULLET boompos[MAXMY_BULLET]; // ���� ����Ʈ ��ġ ����

    // ���� ������ ���� ���� �����
    for (i = 0; i < MAXMY_BULLET; i++) {
        if (boompos[i].flag == TRUE) {
            gotoxy(boompos[i].pos);
            printf("   ");
            boompos[i].flag = FALSE;
        }
    }

    // �浹 ����
    for (i = 0; i < MAXMY_BULLET; i++) {
        if (myship_bullet[i].flag == TRUE) {
            for (j = 0; j < MAX_ENEMY; j++) {
                if (enemyship[j].flag == TRUE) {
                    if (enemyship[j].pos.x <= myship_bullet[i].pos.x &&
                        myship_bullet[i].pos.x <= (enemyship[j].pos.x + 2) &&
                        (enemyship[j].pos.y == myship_bullet[i].pos.y)) {

                        // �� �ı�
                        enemyship[j].flag = FALSE;
                        gotoxy(enemyship[j].pos);
                        printf(" *** ");   // ���� ǥ��
                        myship_bullet[i].flag = FALSE;

                        score += 100;      // ����
                        killnum++;         // ų �� ����

                        // ���� ��ġ ����
                        boompos[i].pos = enemyship[j].pos;
                        boompos[i].flag = TRUE;
                    }
                }
            }
        }
    }
}
