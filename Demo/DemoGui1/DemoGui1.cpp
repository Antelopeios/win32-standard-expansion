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

	// 相关资源定义
	RECT rc_dsk = {0};
	::GetClientRect(::GetDesktopWindow(), &rc_dsk);
	CRect rc_wnd(0, 0, 1000, 750);
	HICON ic_wnd = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMOGUI1));

	/////////////////////////////////

	// 创建窗口对象并设置
	IGuiBoard* wnd = ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc));
	wnd->Create(_T("DemoGui1"), rc_wnd, SW_HIDE);
	wnd->SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)ic_wnd);
	wnd->SendMessage(WM_SETICON, (WPARAM)FALSE, (LPARAM)ic_wnd);
	wnd->CenterWindow();
	wnd->SetLayered();
	wnd->GetClientRect(rc_wnd);
	wnd->AddEvent(ExDynCast<IGuiEvent>(ExDynCreate(_T("CEvent_wnd"), &gc)));

	// 创建控件对象并设置

#define EXP_GUI_PIC(pic_name) pic_##pic_name
#define ExGuiLoadPic(pic_name) \
		IGuiCtrl* EXP_GUI_PIC(pic_name) = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc)); \
		EXP_GUI_PIC(pic_name)->SetState(_T("image"), &CResManager::##pic_name); \
		EXP_GUI_PIC(pic_name)->AddEvent(ExDynCast<IGuiEvent>(ExDynCreate(CString(_T("CEvent_")) + _T(#pic_name), &gc)))
//#define ExGuiLoadPic()

	ExGuiLoadPic(banner);
	ExGuiLoadPic(corner_lb);
	ExGuiLoadPic(corner_rb);
	ExGuiLoadPic(corner_rt);
	ExGuiLoadPic(corner_lt);
	ExGuiLoadPic(line_bottom);
	ExGuiLoadPic(line_left);
	ExGuiLoadPic(line_right);
	ExGuiLoadPic(line_top);
	ExGuiLoadPic(tag_bg);
	ExGuiLoadPic(toolbar_bg);

#define EXP_GUI_BTN(btn_name) btn_##btn_name
#define ExGuiLoadBtn(btn_name, thr_sta) \
		IGuiCtrl* EXP_GUI_BTN(btn_name) = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiButton"), &gc)); \
		EXP_GUI_BTN(btn_name)->SetState(_T("thr_sta"), (void*)thr_sta); \
		{ \
			CImage tmp[9]; \
			tmp[4] = CResManager::##btn_name; \
			EXP_GUI_BTN(btn_name)->SetState(_T("image"), tmp); \
		} \
		EXP_GUI_BTN(btn_name)->AddEvent(ExDynCast<IGuiEvent>(ExDynCreate(CString(_T("CEvent_")) + _T(#btn_name), &gc)))
//#define ExGuiLoadBtn()

	ExGuiLoadBtn(win_sysbtn_close, true);
	ExGuiLoadBtn(win_sysbtn_max, true);
	ExGuiLoadBtn(win_sysbtn_restore, true);
	ExGuiLoadBtn(win_sysbtn_min, true);

	//IGuiCtrl* grp_topbar_btn = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiGroup"), &gc));
	//CArrayT<IGuiCtrl*> itm_topbar_btn;
	//for(int i = 0; i < 8; ++i)
	//{
	//	IGuiCtrl* itm = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiButton"), &gc));
	//	grp_topbar_btn->AddComp(btn);
	//	itm_topbar_btn.Add(btn);
	//}
	//grp_topbar_btn->SetState(_T("items"), &itm_topbar_btn);
	//grp_topbar_btn->SetState(_T("image"), &CResManager::topbar_btn);

	// 将窗口与控件关联

	// 内容打底
	wnd->AddComp(EXP_GUI_PIC(banner));
	wnd->AddComp(EXP_GUI_PIC(tag_bg));
	wnd->AddComp(EXP_GUI_PIC(toolbar_bg));

	// 三态按钮
	wnd->AddComp(EXP_GUI_BTN(win_sysbtn_close));
	wnd->AddComp(EXP_GUI_BTN(win_sysbtn_max));
	wnd->AddComp(EXP_GUI_BTN(win_sysbtn_restore));
	wnd->AddComp(EXP_GUI_BTN(win_sysbtn_min));

	// 窗口边框
	wnd->AddComp(EXP_GUI_PIC(line_bottom));
	wnd->AddComp(EXP_GUI_PIC(line_left));
	wnd->AddComp(EXP_GUI_PIC(line_right));
	wnd->AddComp(EXP_GUI_PIC(line_top));
	wnd->AddComp(EXP_GUI_PIC(corner_lb));
	wnd->AddComp(EXP_GUI_PIC(corner_rb));
	wnd->AddComp(EXP_GUI_PIC(corner_rt));
	wnd->AddComp(EXP_GUI_PIC(corner_lt));

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
