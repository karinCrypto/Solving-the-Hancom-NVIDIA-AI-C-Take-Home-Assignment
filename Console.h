#pragma once
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct { int x; int y; } UPOINT;

	extern HANDLE hout;

	void InitConsole(void);
	void ClearScreen(void);               // 1��(Ÿ��Ʋ) ���� ����
	void DrawTitleBar(const char* title);

	// �� �� ���� ��� ����
	void gotoxy(UPOINT pt);               // ���Ž�: ����ü �� ��
	void gotoxy_xy(int x, int y);         // �ű�: x, y �� ��

#ifdef __cplusplus
}
#endif
