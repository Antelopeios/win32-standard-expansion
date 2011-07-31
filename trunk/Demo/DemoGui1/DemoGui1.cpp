// DemoGui1.cpp : ����Ӧ�ó������ڵ㡣
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

	// ����������
	CGC& gc = ExGC();

	/////////////////////////////////

	// �����Դ����
	RECT rc_dsk = {0};
	::GetClientRect(::GetDesktopWindow(), &rc_dsk);
	CRect rc_wnd(0, 0, 1000, 750);
	HICON ic_wnd = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMOGUI1));

	/////////////////////////////////

	// �������ڶ�������
	IGuiBoard* wnd = ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc));
	wnd->Create(_T("DemoGui1"), rc_wnd, SW_HIDE, 
		WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	wnd->SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)ic_wnd);
	wnd->SendMessage(WM_SETICON, (WPARAM)FALSE, (LPARAM)ic_wnd);
	wnd->CenterWindow();
	wnd->SetLayered(false);
	wnd->GetClientRect(rc_wnd);
	wnd->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_wnd"), &gc)));

	// ��������ؼ�����

	// ���ݴ��
	wnd->AddComp(GUI_CTL(banner));
	wnd->AddComp(GUI_CTL(tag_bg));
	wnd->AddComp(GUI_CTL(toolbar_bg));

	// �б�
	wnd->AddComp(GUI_CTL(list));

	// ���ܰ�ť
	wnd->AddComp(GUI_CTL(topbar_btn));
	wnd->AddComp(GUI_CTL(gamesearch_charmap));
	wnd->AddComp(GUI_CTL(toolbar_tools));

	// ��ǩ��
	wnd->AddComp(GUI_CTL(tag_qb));
	wnd->AddComp(GUI_CTL(tag_zx));
	wnd->AddComp(GUI_CTL(tag_wl));
	wnd->AddComp(GUI_CTL(tag_dz));
	wnd->AddComp(GUI_CTL(tag_wy));
	wnd->AddComp(GUI_CTL(tag_dj));

	// �༭��
	wnd->AddComp(GUI_CTL(search_bg));
	wnd->AddComp(GUI_CTL(search_button));
	wnd->AddComp(GUI_CTL(google_bg));
	wnd->AddComp(GUI_CTL(google_button));

	// ���ڱ߿�
	wnd->AddComp(GUI_CTL(line_bottom));
	wnd->AddComp(GUI_CTL(line_left));
	wnd->AddComp(GUI_CTL(line_right));
	wnd->AddComp(GUI_CTL(line_top));
	wnd->AddComp(GUI_CTL(corner_lb));
	wnd->AddComp(GUI_CTL(corner_rb));
	wnd->AddComp(GUI_CTL(corner_rt));
	wnd->AddComp(GUI_CTL(corner_lt));

	// ��̬��ť
	wnd->AddComp(GUI_CTL(win_sysbtn_close));
	wnd->AddComp(GUI_CTL(win_sysbtn_max));
	wnd->AddComp(GUI_CTL(win_sysbtn_restore));
	wnd->AddComp(GUI_CTL(win_sysbtn_min));

	/////////////////////////////////

	// ��ʾ������
	wnd->ShowWindow(SW_SHOW);

	// ����Ϣѭ��:
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	/////////////////////////////////

	return (int) msg.wParam;
}
