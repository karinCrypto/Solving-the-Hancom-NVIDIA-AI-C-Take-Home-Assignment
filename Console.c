#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "console.h"

HANDLE hout = NULL;   // 전역 콘솔 출력 핸들

// 내부: 콘솔 폭 얻기(실패 시 80)
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

    // 커서 숨김
    ci.bVisible = FALSE;
    ci.dwSize = 1;
    SetConsoleCursorInfo(hout, &ci);

    // 기본 텍스트 핑크
    SetConsoleTextAttribute(
        hout,
        FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    );
}

// 레거시 시그니처(구조체 한 개)
void gotoxy(UPOINT pt)
{
    if (!hout) hout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = (SHORT)pt.x;
    pos.Y = (SHORT)pt.y;
    SetConsoleCursorPosition(hout, pos);
}

// 신규 시그니처(x, y 두 개)
void gotoxy_xy(int x, int y)
{
    UPOINT p = { x, y };
    gotoxy(p);  // 위 함수를 재사용
}

// 1행(타이틀)을 제외하고 화면 지우기
void ClearScreen(void)
{
    if (!hout) hout = GetStdHandle(STD_OUTPUT_HANDLE);
    int width = get_console_width();

    for (int y = 2; y <= 25; ++y) {         // 1행은 타이틀이므로 유지
        for (int x = 1; x <= width; ++x) {
            gotoxy_xy(x, y);
            putchar(' ');
        }
    }
    gotoxy_xy(1, 3); // 커서는 타이틀 아래로
}

void DrawTitleBar(const char* title)
{
    if (!hout) hout = GetStdHandle(STD_OUTPUT_HANDLE);
    int width = get_console_width();
    int len = (int)strlen(title);
    if (len > width) len = width;

    // 현재 색 저장
    CONSOLE_SCREEN_BUFFER_INFO info;
    WORD savedAttr = 0;
    if (GetConsoleScreenBufferInfo(hout, &info)) {
        savedAttr = info.wAttributes;
    }

    // 배경: 밝은 마젠타, 글자: 밝은 흰색
    SetConsoleTextAttribute(
        hout,
        BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY |
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    );

    // 1행 전체 클리어
    for (int x = 1; x <= width; ++x) {
        gotoxy_xy(x, 1);
        putchar(' ');
    }

    // 중앙 정렬로 타이틀
    int startX = (width - len) / 2 + 1;
    gotoxy_xy(startX, 1);
    fwrite(title, 1, len, stdout);

    // 커서 아래로
    gotoxy_xy(1, 3);

    // 색상 원복
    if (savedAttr) SetConsoleTextAttribute(hout, savedAttr);
}
