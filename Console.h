#pragma once
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct { int x; int y; } UPOINT;

	extern HANDLE hout;

	void InitConsole(void);
	void ClearScreen(void);               // 1행(타이틀) 유지 버전
	void DrawTitleBar(const char* title);

	// ★ 두 형태 모두 지원
	void gotoxy(UPOINT pt);               // 레거시: 구조체 한 개
	void gotoxy_xy(int x, int y);         // 신규: x, y 두 개

#ifdef __cplusplus
}
#endif
