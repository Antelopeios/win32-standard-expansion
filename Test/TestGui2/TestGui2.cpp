// TestGui2.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestGui2.h"

#pragma comment(lib, "Imm32.lib")

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 垃圾回收器
	CGC gc;

	// 相关资源定义
	RECT rc_dsk = {0};
	::GetClientRect(::GetDesktopWindow(), &rc_dsk);
	CRect rc_wnd(0, 0, 500, 300);
	HICON ic_wnd = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTGUI2));
	CText txt_edt[5];
	txt_edt[0].SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
	txt_edt[0].SetColor(ExRGBA(0, 60, 116, 200));
	for(int i = 1; i < _countof(txt_edt); ++i)
		txt_edt[i] = txt_edt[0];

	// 创建控件
	IGuiCtrl* edit = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiEdit"), &gc));
	edit->SetWindowRect(rc_wnd);
	edit->SetState(_T("text"), txt_edt);

	// 创建窗口对象并设置
	IGuiBoard* wnd = ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc));
	wnd->Create(_T("TestGui2"), rc_wnd, SW_HIDE);
	wnd->SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)ic_wnd);
	wnd->SendMessage(WM_SETICON, (WPARAM)FALSE, (LPARAM)ic_wnd);
	wnd->CenterWindow();
	wnd->SetLayered();
	HIMC context = ::ImmCreateContext();
	::ImmAssociateContext(wnd->GethWnd(), context);
	wnd->ShowWindow(SW_SHOW);

	// 关联对象
	wnd->AddComp(edit);

	// 主消息循环:
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	::ImmDestroyContext(context);

	return (int) msg.wParam;
}