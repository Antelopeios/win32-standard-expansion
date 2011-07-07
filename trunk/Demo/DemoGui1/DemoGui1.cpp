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

#define GUI_PIC(name) pic_##name
#define GuiLoadPic(name) \
		IGuiCtrl* GUI_PIC(name) = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc)); \
		GUI_PIC(name)->SetState(_T("image"), &CResManager::##name); \
		GUI_PIC(name)->AddEvent(ExDynCast<IGuiEvent>(ExDynCreate(CString(_T("CEvent_")) + _T(#name), &gc)))
//#define GuiLoadPic()

	GuiLoadPic(banner);
	GuiLoadPic(corner_lb);
	GuiLoadPic(corner_rb);
	GuiLoadPic(corner_rt);
	GuiLoadPic(corner_lt);
	GuiLoadPic(line_bottom);
	GuiLoadPic(line_left);
	GuiLoadPic(line_right);
	GuiLoadPic(line_top);
	GuiLoadPic(tag_bg);
	GuiLoadPic(toolbar_bg);
	GuiLoadPic(list);

#define GUI_BTN(name) btn_##name
#define GuiLoadBtn(name, thr_sta) \
		IGuiCtrl* GUI_BTN(name) = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiButton"), &gc)); \
		GUI_BTN(name)->SetState(_T("thr_sta"), (void*)thr_sta); \
		{ \
			CImage tmp[9]; \
			tmp[4] = CResManager::##name; \
			GUI_BTN(name)->SetState(_T("image"), tmp); \
		} \
		GUI_BTN(name)->AddEvent(ExDynCast<IGuiEvent>(ExDynCreate(CString(_T("CEvent_")) + _T(#name), &gc)))
//#define GuiLoadBtn()

	GuiLoadBtn(win_sysbtn_close, true);
	GuiLoadBtn(win_sysbtn_max, true);
	GuiLoadBtn(win_sysbtn_restore, true);
	GuiLoadBtn(win_sysbtn_min, true);

#define GUI_GRP(name) grp_##name
#define GUI_ITM(name) itm_##name
#define GuiLoadGrp(name, count, thr_sta) \
		IGuiCtrl* GUI_GRP(name) = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiGroup"), &gc)); \
		CArrayT<IGuiCtrl*> GUI_ITM(name); \
		for(int i = 0; i < count; ++i) \
		{ \
			IGuiCtrl* itm = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiButton"), &gc)); \
			itm->SetState(_T("thr_sta"), (void*)thr_sta); \
			GUI_GRP(name)->AddComp(itm); \
			GUI_ITM(name).Add(itm); \
		} \
		GUI_GRP(name)->SetState(_T("items"), &GUI_ITM(name)); \
		GUI_GRP(name)->SetState(_T("image"), &CResManager::##name); \
		GUI_GRP(name)->AddEvent(ExDynCast<IGuiEvent>(ExDynCreate(CString(_T("CEvent_")) + _T(#name), &gc)))
//#define GuiLoadGrp()

	GuiLoadGrp(topbar_btn, 8, true);

	// 将窗口与控件关联

	// 内容打底
	wnd->AddComp(GUI_PIC(banner));
	wnd->AddComp(GUI_PIC(tag_bg));
	wnd->AddComp(GUI_PIC(toolbar_bg));
	wnd->AddComp(GUI_PIC(list));

	// 三态按钮
	wnd->AddComp(GUI_BTN(win_sysbtn_close));
	wnd->AddComp(GUI_BTN(win_sysbtn_max));
	wnd->AddComp(GUI_BTN(win_sysbtn_restore));
	wnd->AddComp(GUI_BTN(win_sysbtn_min));

	// 功能按钮
	wnd->AddComp(GUI_GRP(topbar_btn));

	// 窗口边框
	wnd->AddComp(GUI_PIC(line_bottom));
	wnd->AddComp(GUI_PIC(line_left));
	wnd->AddComp(GUI_PIC(line_right));
	wnd->AddComp(GUI_PIC(line_top));
	wnd->AddComp(GUI_PIC(corner_lb));
	wnd->AddComp(GUI_PIC(corner_rb));
	wnd->AddComp(GUI_PIC(corner_rt));
	wnd->AddComp(GUI_PIC(corner_lt));

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
