//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
//It rained on us at 4am.
#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include "Picture.h"
#include "debugger.h"
#include "resource.h"
#include "Callback.h"
#include "Status.h"
#include "windowsize.h"
#pragma comment(lib, "WINMM.LIB")
//FILE* fl = fopen("debug.log", "w");
HBITMAP hBRosa, hBDog, hBHeart, hBGame, hBWelcome, hBFail, hBWin, hBRing, hBHelp;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	Status* p = Status::currSta;
	int res = -1;
	switch (message){
	case WM_CREATE:
		SetTimer(hWnd, 2136, wd::delay, NULL);
		res = p->OnCreate();
		break;
	case WM_KEYDOWN:
		res = p->OnKeyDown(wParam);
		break;
	case WM_KEYUP:
		res = p->OnKeyUp(wParam);
		break;
	case WM_PAINT:
		res = p->OnPaint(hWnd);
		break;
	case WM_TIMER:
		p->OnTimer();
		if (p->needFlush())
			InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		res = 0;
		break;
	}
	return ~res ? 0 : DefWindowProc(hWnd, message, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow){
	hBGame = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GAME));
	hBDog = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DOG));
	hBFail = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FAIL));
	hBHeart = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_HEART));
	hBRosa = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ROSA));
	hBWelcome = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WELCOME));
	hBWin = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WIN));
	hBRing = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RING));
	hBHelp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_HELP));
	srand((int)time(NULL));
	WNDCLASS wcex;
	TCHAR lpszClassName[] = "dyc520app";
	TCHAR lpszTitle[] = "¸ø½ã½ãµÄ520ÀñÎï£¡";
	wcex.style = wcex.cbClsExtra = wcex.cbWndExtra = 0;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInst;
	wcex.hIcon = LoadIcon(hInst, "AppIcon");
	wcex.hCursor = LoadCursor(hInst, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = lpszClassName;
	if (!RegisterClass(&wcex)){
		MessageBox(NULL, "Fail Register", "Title", MB_OK);
		return -1;
	}
	DWORD style = WS_EX_LAYERED | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	RECT rc;
	rc.left = rc.top = 0, rc.bottom = wd::height, rc.right = wd::width;
	AdjustWindowRect(&rc, style, FALSE);
	//debug("%d %d %d %d\n", rc.left, rc.top, rc.right, rc.bottom);
	HWND hWnd = CreateWindow(lpszClassName, lpszTitle, style, 20, 20, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInst, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	Status::hWnd = hWnd;
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
