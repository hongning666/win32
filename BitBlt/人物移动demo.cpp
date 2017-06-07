// �����ƶ�demo.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "�����ƶ�demo.h"

#define MAX_LOADSTRING 100
#define IMAGE_SIZE 4
#define WIN_WIDTH   641
#define WIN_HEIGHT  480 

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
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

// �˴���ģ���а����ĺ�����ǰ������: 
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

 	// TODO:  �ڴ˷��ô��롣
	MSG msg = {0};
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	//hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEMO));

	// ����Ϣѭ��: 
	while (msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //û����Ϣ�����
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
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
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
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��: 
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
		// TODO:  �ڴ���������ͼ����...
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

// �����ڡ������Ϣ�������
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