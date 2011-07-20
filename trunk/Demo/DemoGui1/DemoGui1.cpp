// DemoGui1.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "DemoGui1.h"

//////////////////////////////////////////////////////////////////

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	/////////////////////////////////

	// 垃圾回收器
	CGC& gc = ExGC();

	/////////////////////////////////

	// 相关资源定义
	RECT rc_dsk = {0};
	::GetClientRect(::GetDesktopWindow(), &rc_dsk);
	CRect rc_wnd(0, 0, 1000, 750);
	HICON ic_wnd = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMOGUI1));

	/////////////////////////////////

	// 创建窗口对象并设置
	IGuiBoard* wnd = ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc));
	wnd->Create(_T("DemoGui1"), rc_wnd, SW_HIDE, 
		WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	wnd->SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)ic_wnd);
	wnd->SendMessage(WM_SETICON, (WPARAM)FALSE, (LPARAM)ic_wnd);
	wnd->CenterWindow();
	wnd->SetLayered(false);
	wnd->GetClientRect(rc_wnd);
	wnd->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_wnd"), &gc)));

	// 将窗口与控件关联

	// 内容打底
	wnd->AddComp(GUI_CTRL(banner));
	wnd->AddComp(GUI_CTRL(tag_bg));
	wnd->AddComp(GUI_CTRL(toolbar_bg));
	wnd->AddComp(GUI_CTRL(list));

	// 功能按钮
	wnd->AddComp(GUI_CTRL(topbar_btn));
	wnd->AddComp(GUI_CTRL(gamesearch_charmap));
	wnd->AddComp(GUI_CTRL(toolbar_tools));

	// 标签栏
	wnd->AddComp(GUI_CTRL(tag_qb));
	wnd->AddComp(GUI_CTRL(tag_zx));
	wnd->AddComp(GUI_CTRL(tag_wl));
	wnd->AddComp(GUI_CTRL(tag_dz));
	wnd->AddComp(GUI_CTRL(tag_wy));
	wnd->AddComp(GUI_CTRL(tag_dj));

	// 编辑框
	wnd->AddComp(GUI_CTRL(search_bg));
	wnd->AddComp(GUI_CTRL(search_button));
	wnd->AddComp(GUI_CTRL(google_bg));
	wnd->AddComp(GUI_CTRL(google_button));

	// 窗口边框
	wnd->AddComp(GUI_CTRL(line_bottom));
	wnd->AddComp(GUI_CTRL(line_left));
	wnd->AddComp(GUI_CTRL(line_right));
	wnd->AddComp(GUI_CTRL(line_top));
	wnd->AddComp(GUI_CTRL(corner_lb));
	wnd->AddComp(GUI_CTRL(corner_rb));
	wnd->AddComp(GUI_CTRL(corner_rt));
	wnd->AddComp(GUI_CTRL(corner_lt));

	// 三态按钮
	wnd->AddComp(GUI_CTRL(win_sysbtn_close));
	wnd->AddComp(GUI_CTRL(win_sysbtn_max));
	wnd->AddComp(GUI_CTRL(win_sysbtn_restore));
	wnd->AddComp(GUI_CTRL(win_sysbtn_min));

	/////////////////////////////////

	// 显示主窗口
	wnd->ShowWindow(SW_SHOW);

	// 主消息循环:
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	/////////////////////////////////

	return (int) msg.wParam;
}
