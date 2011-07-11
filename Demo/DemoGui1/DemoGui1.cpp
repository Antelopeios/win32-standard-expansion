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

#define GuiLoadPic(name) \
	REG_CTRL(name, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc))); \
	GUI_CTRL(name)->SetState(_T("image"), &CResManager::##name); \
	GUI_CTRL(name)->AddEvent(ExDynCast<IGuiEvent>(ExDynCreate(CString(_T("CEvent_")) + _T(#name), &gc)))
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

#define GuiLoadBtn(name, thr_sta) \
	REG_CTRL(name, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiButton"), &gc))); \
	GUI_CTRL(name)->SetState(_T("thr_sta"), (void*)thr_sta); \
	{ \
		CImage tmp[9]; \
		tmp[4] = CResManager::##name; \
		GUI_CTRL(name)->SetState(_T("image"), tmp); \
	} \
	GUI_CTRL(name)->AddEvent(ExDynCast<IGuiEvent>(ExDynCreate(CString(_T("CEvent_")) + _T(#name), &gc)))
//#define GuiLoadBtn()

	GuiLoadBtn(win_sysbtn_close, true);
	GuiLoadBtn(win_sysbtn_max, true);
	GuiLoadBtn(win_sysbtn_restore, true);
	GuiLoadBtn(win_sysbtn_min, true);

#define GuiLoadGrp(name, count, thr_sta) \
	REG_CTRL(name, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiGroup"), &gc))); \
	CArrayT<IGuiCtrl*> items_##name; \
	for(int i = 0; i < count; ++i) \
	{ \
		IGuiCtrl* itm = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiButton"), &gc)); \
		itm->SetState(_T("thr_sta"), (void*)thr_sta); \
		GUI_CTRL(name)->AddComp(itm); \
		items_##name.Add(itm); \
	} \
	GUI_CTRL(name)->SetState(_T("items"), &items_##name); \
	GUI_CTRL(name)->SetState(_T("image"), &CResManager::##name); \
	GUI_CTRL(name)->AddEvent(ExDynCast<IGuiEvent>(ExDynCreate(CString(_T("CEvent_")) + _T(#name), &gc)))
//#define GuiLoadGrp()

	GuiLoadGrp(gamesearch_charmap, 26, true);
	GuiLoadGrp(toolbar_tools, 8, true);

	REG_CTRL(topbar_btn, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiGroup"), &gc)));
	CArrayT<IGuiCtrl*> items_topbar_btn;
	IGuiEvent* items_topbar_btn_evt = ExDynCast<IGuiEvent>(ExDynCreate(_T("CEvent_items_topbar_btn"), &gc));
	for(int i = 0; i < 8; ++i)
	{
		IGuiCtrl* itm = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPushBtn"), &gc));
		itm->AddEvent(items_topbar_btn_evt);
		GUI_CTRL(topbar_btn)->AddComp(itm);
		items_topbar_btn.Add(itm);
	}
	((IGuiCtrl**)items_topbar_btn)[0]->SetState(_T("status"), (void*)3);
	GUI_CTRL(topbar_btn)->SetState(_T("items"), &items_topbar_btn);
	GUI_CTRL(topbar_btn)->SetState(_T("sta_cnt"), (void*)4);
	GUI_CTRL(topbar_btn)->SetState(_T("sty_box"), (void*)false);
	GUI_CTRL(topbar_btn)->SetState(_T("image"), &CResManager::topbar_btn);
	GUI_CTRL(topbar_btn)->AddEvent(ExDynCast<IGuiEvent>(ExDynCreate(_T("CEvent_topbar_btn"), &gc)));

#define GuiLoadPsh(name) \
	REG_CTRL(name, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPushBtn"), &gc))); \
	GUI_CTRL(name)->SetState(_T("image"), &CResManager::##name); \
	GUI_CTRL(name)->AddEvent(ExDynCast<IGuiEvent>(ExDynCreate(CString(_T("CEvent_")) + _T(#name), &gc)))
//#define GuiLoadGrp()

	GuiLoadPsh(tag_qb);
	GuiLoadPsh(tag_zx);
	GuiLoadPsh(tag_wl);
	GuiLoadPsh(tag_dz);
	GuiLoadPsh(tag_wy);
	GuiLoadPsh(tag_dj);

	GuiLoadPic(search_bg);
	GuiLoadPic(search_button);
	GuiLoadPic(google_bg);
	GuiLoadPic(google_button);

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
