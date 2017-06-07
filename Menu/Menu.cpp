#include<Windows.h>
#include<tchar.h>
#include"resource.h"

HMENU hm;
HINSTANCE hins;

LRESULT WinProc(HWND, UINT, WPARAM, LPARAM);
void onSysMenuCreate(HWND hwnd);//系统菜单
void onPopupMenuCreate(HWND hwnd);//右键菜单
void onTopMenuCreate(HWND hwnd,HINSTANCE hInstance);//顶层菜单

int WinMain(HINSTANCE hInstance,
			HINSTANCE phInstance,
			LPSTR cmd,
			int show)
{
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
	wc.hCursor = nullptr;
	wc.hIcon = nullptr;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.lpszClassName = _T("ZCJ");
	wc.lpszMenuName = nullptr;
	wc.style = CS_HREDRAW;

	RegisterClass(&wc);
	HWND hwnd = CreateWindow(_T("ZCJ"), _T("菜单"), WS_OVERLAPPEDWINDOW, 300, 100, 800, 600, nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	MSG msg = { 0 };
	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, nullptr, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				return 0;
			}
		}
	}
	UnregisterClass(_T("ZCJ"), hInstance);
	return 0;
}

LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
		onSysMenuCreate(hwnd);
		onPopupMenuCreate(hwnd);
		onTopMenuCreate(hwnd,hins);
		break;
	case WM_SYSCOMMAND:
		if (wparam==1000)//wparam中保存了系统菜单ID
			MessageBox(nullptr, _T("点击了系统菜单"), _T("嘿嘿"), MB_OK);
		break;
	case WM_COMMAND:
		switch (wparam)
		{
		case 2003:
			MessageBox(NULL, _T("HelloWorld"), _T("你好世界"), MB_OK);
			break;
		case 3008:{
			static bool flag = false;
			if (!flag)
			{
				HMENU t_menu = GetSubMenu(GetSubMenu(GetMenu(hwnd), 1),3);
				CheckMenuItem(t_menu, 3008, MF_UNCHECKED);
				EnableMenuItem(t_menu, 3009, MF_ENABLED);
			}
			else
			{
				HMENU t_menu = GetSubMenu(GetSubMenu(GetMenu(hwnd), 1), 3);
				CheckMenuItem(t_menu, 3008, MF_CHECKED);
				EnableMenuItem(t_menu, 3009, MF_DISABLED);
			}
			flag = !flag;
			break; }
		}
		break;
	case WM_RBUTTONUP:{
		POINT pt = { LOWORD(lparam), HIWORD(lparam) };
		ClientToScreen(hwnd, &pt);
		TrackPopupMenu(hm, TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, nullptr);
		break;
	}
	/*case WM_CONTEXTMENU:{
		BOOL ret=TrackPopupMenu(hm, TPM_LEFTALIGN | WM_NOTIFY | TPM_RETURNCMD, LOWORD(lparam), HIWORD(lparam), 0, hwnd, nullptr);
		switch (ret)
		{
		case 2003:
			MessageBox(NULL, _T("HelloWorld"), _T("你好世界"), MB_OK);
			break;
		}
		break; 
	}*/
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void onSysMenuCreate(HWND hwnd)
{
	HMENU hmenu = GetSystemMenu(hwnd, false);
	AppendMenu(hmenu, MF_SEPARATOR, 520, _T(""));//分割线
	AppendMenu(hmenu, MF_STRING, 1000, _T("嘿嘿"));
	//DeleteMenu(hmenu, 1, MF_BYPOSITION);//通过索引删除
	//DeleteMenu(hmenu, 1000, MF_BYCOMMAND);//通过菜单ID删除
}

void onPopupMenuCreate(HWND hwnd)
{
	hm = CreatePopupMenu();
	AppendMenu(hm, MF_STRING, 2000, _T("Hello"));
	AppendMenu(hm, MF_STRING, 2001, _T("World"));
	AppendMenu(hm, MF_SEPARATOR, 520, _T(""));
	AppendMenu(hm, MF_STRING, 2002, _T("你好"));
	AppendMenu(hm,MF_STRING, 2003, _T("世界"));
}
void onTopMenuCreate(HWND hwnd,HINSTANCE hInstance)
{
	//1.创建顶层菜单
	HMENU top=CreateMenu();
	//2.创建弹出式菜单
	HMENU pop = CreatePopupMenu();
	//3.把弹出式菜单加到顶层菜单
	AppendMenu(top, MF_POPUP, (UINT)pop, _T("文件"));
	//4.在弹出式菜单中添加菜单项
	AppendMenu(pop, MF_STRING, 3000, L"新建");
	AppendMenu(pop, MF_STRING, 3001, L"打开");
	AppendMenu(pop, MF_STRING, 3002, L"保存");
	AppendMenu(pop, MF_BITMAP, 3000, (LPWSTR)LoadIcon(hInstance, MAKEINTRESOURCE(IDB_BITMAP1)));
	//AppendMenu(pop, MF_STRING | 1, 3004, L"灰机");//MF_MENUBARBREAK单独成列
	//顶层菜单中添加第二个弹出菜单
	HMENU pop2 = CreatePopupMenu();
	AppendMenu(top, MF_POPUP, (UINT)pop2, _T("编辑"));
	AppendMenu(pop2, MF_STRING, 3005, L"撤销");
	AppendMenu(pop2, MF_STRING, 3006, L"复制");
	AppendMenu(pop2, MF_STRING, 3007, L"粘贴");
	//在弹出式菜单中添加弹出式菜单
	HMENU popChild = CreatePopupMenu();
	AppendMenu(pop2, MF_POPUP, (UINT)popChild, _T("状态"));
	AppendMenu(popChild, MF_STRING|MF_CHECKED, 3008, L"状态1");
	AppendMenu(popChild, MF_STRING|MF_GRAYED, 3009, L"状态2");
	AppendMenu(popChild, MF_STRING, 3010, L"状态3");
	//5.把顶层菜单设置到窗口中显示
	SetMenu(hwnd, top);
}