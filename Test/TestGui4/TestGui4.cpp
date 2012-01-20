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
	CGC gc;

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

	pixel_t pix[8] = {0};

	IGuiCtrl* scroll1 = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiScroll")));
	scroll1->SetWindowRect(CRect(10, 230, 475, 250));
	scroll1->SetState(_T("sli_color"), (void*)ExRGBA(220, 220, 220, 255));
	CImgASM::PixSetP(pix, _countof(pix), ExRGBA(120, 120, 120, 255));
	scroll1->SetState(_T("sli_blk_color"), pix);
	scroll1->SetState(_T("sli_all"), (void*)100);
	scroll1->SetState(_T("sli_fra"), (void*)10);
	//scroll1->SetState(_T("sli_ori"), (void*)TRUE);
	CImgASM::PixSetP(pix, _countof(pix), ExRGBA(180, 180, 180, 255));
	scroll1->SetState(_T("up_color"), pix);
	scroll1->SetState(_T("dn_color"), pix);

	IGuiCtrl* scroll2 = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiScroll")));
	scroll2->SetWindowRect(CRect(455, 10, 475, 225));
	scroll2->SetState(_T("sli_color"), (void*)ExRGBA(220, 220, 220, 255));
	CImgASM::PixSetP(pix, _countof(pix), ExRGBA(120, 120, 120, 255));
	scroll2->SetState(_T("sli_blk_color"), pix);
	scroll2->SetState(_T("sli_all"), (void*)100);
	scroll2->SetState(_T("sli_fra"), (void*)20);
	scroll2->SetState(_T("sli_ori"), (void*)TRUE);
	CImgASM::PixSetP(pix, _countof(pix), ExRGBA(180, 180, 180, 255));
	scroll2->SetState(_T("up_color"), pix);
	scroll2->SetState(_T("dn_color"), pix);

	// 关联对象
	wnd->AddComp(scroll1);
	wnd->AddComp(scroll2);
	wnd->AddEvent(ExMem::Alloc<CCustomEvent>());

	// 主消息循环:
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}
