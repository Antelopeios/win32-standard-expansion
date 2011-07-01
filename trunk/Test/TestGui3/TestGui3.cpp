// TestGui3.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestGui3.h"

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
	HICON ic_wnd = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTGUI3));
	CText txt_lst;
	txt_lst.SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
	txt_lst.SetColor(ExRGBA(0, 60, 116, 200));

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
	IGuiCtrl* list = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiListView"), &gc));
	list->SetWindowRect(rc_wnd);

	// �����¼���������
	CCustomEvent cus_evt;
	CFillEvent	 fil_evt;

	// ��������
	wnd->AddComp(list);
	wnd->AddEvent(&cus_evt);
	wnd->AddEvent(&fil_evt);

	// ����Ϣѭ��:
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}