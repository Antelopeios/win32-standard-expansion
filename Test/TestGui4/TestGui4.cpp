// TestGui4.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestGui4.h"


int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 垃圾回收器
	CGC& gc = ExGC();

	// 相关资源定义
	RECT rc_dsk = {0};
	::GetClientRect(::GetDesktopWindow(), &rc_dsk);
	CRect rc_wnd(0, 0, 500, 300);
	HICON ic_wnd = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTGUI4));

	// 创建窗口对象并设置
	IGuiBoard* wnd = ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc));
	wnd->Create(_T("TestGui2"), rc_wnd, SW_HIDE, WS_OVERLAPPEDWINDOW);
	wnd->SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)ic_wnd);
	wnd->SendMessage(WM_SETICON, (WPARAM)FALSE, (LPARAM)ic_wnd);
	wnd->CenterWindow();
	//wnd->SetLayered();
	wnd->ShowWindow(SW_SHOW);
	wnd->GetClientRect(rc_wnd);

	// 创建控件
	IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiSlider"), &gc));
	ctrl->SetWindowRect(CRect(10, 10, 475, 30));
	pixel_t pix[5] = {0};
	pix[0] = ExRGBA(240, 240, 240, 255);
	ctrl->SetState(_T("color"), pix);
	pix[0] = pix[1] = pix[2] = pix[3] = pix[4] = ExRGBA(120, 120, 120, 255);
	ctrl->SetState(_T("sli_color"), pix);
	ctrl->SetState(_T("all"), (void*)100);
	ctrl->SetState(_T("fra"), (void*)20);
	//ctrl->SetState(_T("ori"), (void*)true);

	// 创建事件对象并设置
	CCustomEvent cus_evt;
	CFillEvent	 fil_evt;

	// 关联对象
	wnd->AddComp(ctrl);
	wnd->AddEvent(&cus_evt);
	//wnd->AddEvent(&fil_evt);

	// 主消息循环:
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}
