#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "console.h"

HANDLE hout = NULL;   // ���� �ܼ� ��� �ڵ�

// ����: �ܼ� �� ���(���� �� 80)
static int get_console_width(void) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(hout, &info)) {
        return info.srWindow.Right - info.srWindow.Left + 1;
    }
    return 80;
}

void InitConsole(void)
{
    CONSOLE_CURSOR_INFO ci;
    hout = GetStdHandle(STD_OUTPUT_HANDLE);

    // Ŀ�� ����
    ci.bVisible = FALSE;
    ci.dwSize = 1;
    SetConsoleCursorInfo(hout, &ci);

    // �⺻ �ؽ�Ʈ ��ũ
    SetConsoleTextAttribute(
        hout,
        FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    );
}

// ���Ž� �ñ״�ó(����ü �� ��)
void gotoxy(UPOINT pt)
{
    if (!hout) hout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = (SHORT)pt.x;
    pos.Y = (SHORT)pt.y;
    SetConsoleCursorPosition(hout, pos);
}

// �ű� �ñ״�ó(x, y �� ��)
void gotoxy_xy(int x, int y)
{
    UPOINT p = { x, y };
    gotoxy(p);  // �� �Լ��� ����
}

// 1��(Ÿ��Ʋ)�� �����ϰ� ȭ�� �����
void ClearScreen(void)
{
    if (!hout) hout = GetStdHandle(STD_OUTPUT_HANDLE);
    int width = get_console_width();

    for (int y = 2; y <= 25; ++y) {         // 1���� Ÿ��Ʋ�̹Ƿ� ����
        for (int x = 1; x <= width; ++x) {
            gotoxy_xy(x, y);
            putchar(' ');
        }
    }
    gotoxy_xy(1, 3); // Ŀ���� Ÿ��Ʋ �Ʒ���
}

void DrawTitleBar(const char* title)
{
    if (!hout) hout = GetStdHandle(STD_OUTPUT_HANDLE);
    int width = get_console_width();
    int len = (int)strlen(title);
    if (len > width) len = width;

    // ���� �� ����
    CONSOLE_SCREEN_BUFFER_INFO info;
    WORD savedAttr = 0;
    if (GetConsoleScreenBufferInfo(hout, &info)) {
        savedAttr = info.wAttributes;
    }

    // ���: ���� ����Ÿ, ����: ���� ���
    SetConsoleTextAttribute(
        hout,
        BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY |
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    );

    // 1�� ��ü Ŭ����
    for (int x = 1; x <= width; ++x) {
        gotoxy_xy(x, 1);
        putchar(' ');
    }

    // �߾� ���ķ� Ÿ��Ʋ
    int startX = (width - len) / 2 + 1;
    gotoxy_xy(startX, 1);
    fwrite(title, 1, len, stdout);

    // Ŀ�� �Ʒ���
    gotoxy_xy(1, 3);

    // ���� ����
    if (savedAttr) SetConsoleTextAttribute(hout, savedAttr);
}
