// TestGui2.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestGui2.h"

//////////////////////////////////////////////////////////////////

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// ����������
	CGC gc;

	// �����Դ����
	RECT rc_dsk = {0};
	::GetClientRect(::GetDesktopWindow(), &rc_dsk);
	CRect rc_wnd(0, 0, 500, 300);
	HICON ic_wnd = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTGUI2));
	CText txt_edt((font_t)::GetStockObject(DEFAULT_GUI_FONT), ExRGBA(0, 60, 116, 200));
	CText txt_ept((font_t)::GetStockObject(DEFAULT_GUI_FONT), ExRGBA(128, 128, 128, 255));

	// �������ڶ�������
	IGuiBoard* wnd = ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc));
	wnd->Create(_T("TestGui2"), rc_wnd, SW_HIDE, WS_OVERLAPPEDWINDOW);
	wnd->SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)ic_wnd);
	wnd->SendMessage(WM_SETICON, (WPARAM)FALSE, (LPARAM)ic_wnd);
	wnd->CenterWindow();
	//wnd->SetLayered();
	wnd->ShowWindow(SW_SHOW);
	wnd->GetClientRect(rc_wnd);

	// �����ؼ�
	IGuiCtrl* edit = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiEdit")));
	edit->SetWindowRect(rc_wnd);
	edit->SetState(_T("font"), &txt_edt);
	edit->SetState(_T("empty_font"), &txt_ept);
	edit->SetState(_T("empty_text"), &CString(_T("���ڵ�Edit����ʲô��û��Ŷ")));

	// ��������
	wnd->AddComp(edit);
	wnd->AddEvent(ExMem::Alloc<CCustomEvent>());
	wnd->AddEvent(ExMem::Alloc<CFillEvent>());

	// ����Ϣѭ��:
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}