// 人物移动demo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "人物移动demo.h"

#define MAX_LOADSTRING 100
#define IMAGE_SIZE 4
#define WIN_WIDTH   641
#define WIN_HEIGHT  480 

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
HBITMAP g_background;
HBITMAP g_character[IMAGE_SIZE];
HDC g_hdc;
HDC g_memdc;
int oldtime;
int newtime;
int g_direction=0;
int g_ix=200;
int g_iy=200;
enum DIRECTION
{
	up,down,left,right
};

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void game_init(HWND hwnd);
void paint_game();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	MSG msg = {0};
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	//hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEMO));

	// 主消息循环: 
	while (msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //没有消息的情况
		{
			newtime = GetTickCount();
			if (newtime - oldtime > 80)
			{
				paint_game();
			}
		}
	}
	/*while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}*/

	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DEMO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, WIN_WIDTH, WIN_HEIGHT+60, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		game_init(hWnd);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			g_direction = up;
			if (g_iy > 0)
				g_iy -= 20;
			else
				g_iy = 0;
			break;
		case VK_DOWN:
			g_direction = down;
			if (g_iy < WIN_HEIGHT-108)
				g_iy += 20;
			else
				g_iy = WIN_HEIGHT-108;
			break;
		case VK_LEFT:
			g_direction = left;
			if (g_ix>0)
				g_ix -= 20;
			else
				g_ix = 0;
			break;
		case VK_RIGHT:
			g_direction = right;
			if (g_ix < WIN_WIDTH-60)
				g_ix += 20;
			else
				g_ix = WIN_WIDTH-60;
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
void game_init(HWND hwnd)
{
	g_background = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	g_character[up] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	g_character[down] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
	g_character[left] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
	g_character[right] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
	g_hdc = GetDC(hwnd);
	g_memdc = CreateCompatibleDC(g_hdc);
	paint_game();
}
void paint_game()
{
	static int num = 1;
	if (num == 8)
		num = 1;
	SelectObject(g_memdc, g_background);
	BitBlt(g_hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT, g_memdc, 0, 0, SRCCOPY);
	SelectObject(g_memdc, g_character[g_direction]);
	BitBlt(g_hdc, g_ix, g_iy, 60, 108, g_memdc, 60 * num, 108, SRCAND);
	BitBlt(g_hdc, g_ix, g_iy, 60, 108, g_memdc, 60 * num, 0, SRCPAINT);
	num++;
	oldtime = GetTickCount();
}