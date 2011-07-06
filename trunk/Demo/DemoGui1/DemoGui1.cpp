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
	CGC gc;
	/////////////////////////////////

	/////////////////////////////////
	// 相关资源定义
	RECT rc_dsk = {0};
	::GetClientRect(::GetDesktopWindow(), &rc_dsk);
	CRect rc_wnd(0, 0, 1000, 750);
	HICON ic_wnd = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMOGUI1));

	// 解码器
	ICoderObject* coder = CImgAnalyzer::GetCoder(CImgAnalyzer::png, &gc);
	if (!coder) return 0;

	// 图片资源
	file.Open(_T("ui/banner.png"));
	CImage banner(coder->Decode());

	file.Open(_T("ui/corner_lb.png"));
	CImage corner_lb(coder->Decode());
	file.Open(_T("ui/corner_rb.png"));
	CImage corner_rb(coder->Decode());
	file.Open(_T("ui/corner_rt.png"));
	CImage corner_rt(coder->Decode());
	file.Open(_T("ui/cornor_lt.png"));
	CImage cornor_lt(coder->Decode());

	file.Open(_T("ui/line_bottom.png"));
	CImage line_bottom(coder->Decode());
	file.Open(_T("ui/line_left.png"));
	CImage line_left(coder->Decode());
	file.Open(_T("ui/line_right.png"));
	CImage line_right(coder->Decode());
	file.Open(_T("ui/line_top.png"));
	CImage line_top(coder->Decode());
	/////////////////////////////////

	/////////////////////////////////
	// 创建窗口对象并设置
	IGuiBoard* wnd = ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc));
	wnd->Create(_T("DemoGui1"), rc_wnd, SW_HIDE);
	wnd->SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)ic_wnd);
	wnd->SendMessage(WM_SETICON, (WPARAM)FALSE, (LPARAM)ic_wnd);
	wnd->CenterWindow();
	//wnd->SetLayered();
	wnd->ShowWindow(SW_SHOW);
	wnd->GetClientRect(rc_wnd);
	/////////////////////////////////

	/////////////////////////////////
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
