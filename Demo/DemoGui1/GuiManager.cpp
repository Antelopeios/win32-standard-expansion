#include "stdafx.h"
#include "DemoGui1.h"

//////////////////////////////////////////////////////////////////

class CGuiLoader
{
protected:
	CGC& gc;

protected:
	// 加载资源
	void LoadRes()
	{
		// 解码器
		ICoderObject* coder = CImgAnalyzer::GetCoder(CImgAnalyzer::png, &gc);
		if (!coder) return;
		CIOFile file;
		coder->SetFile(&file);

		// 图片资源

		file.Open(_T("ui/banner.png"));
		REG_IMG(banner, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/corner_lb.png"));
		REG_IMG(corner_lb, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/corner_rb.png"));
		REG_IMG(corner_rb, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/corner_rt.png"));
		REG_IMG(corner_rt, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/corner_lt.png"));
		REG_IMG(corner_lt, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/line_bottom.png"));
		REG_IMG(line_bottom, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/line_left.png"));
		REG_IMG(line_left, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/line_right.png"));
		REG_IMG(line_right, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/line_top.png"));
		REG_IMG(line_top, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/tag_bg.png"));
		REG_IMG(tag_bg, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/toolbar_bg.png"));
		REG_IMG(toolbar_bg, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());

		file.Open(_T("ui/list.png"));
		REG_IMG(list, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/list_item.png"));
		REG_IMG(list_item, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/list_icon.png"));
		REG_IMG(list_icon, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());

		file.Open(_T("ui/win_sysbtn_close.png"));
		REG_IMG(win_sysbtn_close, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/win_sysbtn_max.png"));
		REG_IMG(win_sysbtn_max, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/win_sysbtn_min.png"));
		REG_IMG(win_sysbtn_min, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/win_sysbtn_restore.png"));
		REG_IMG(win_sysbtn_restore, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());

		file.Open(_T("ui/topbar_btn.png"));
		REG_IMG(topbar_btn, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/gamesearch_charmap.png"));
		REG_IMG(gamesearch_charmap, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/toolbar_tools.png"));
		REG_IMG(toolbar_tools, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());

		file.Open(_T("ui/tag_qb.png"));
		REG_IMG(tag_qb, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/tag_zx.png"));
		REG_IMG(tag_zx, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/tag_wl.png"));
		REG_IMG(tag_wl, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/tag_dz.png"));
		REG_IMG(tag_dz, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/tag_wy.png"));
		REG_IMG(tag_wy, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/tag_dj.png"));
		REG_IMG(tag_dj, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());

		file.Open(_T("ui/search_bg.png"));
		REG_IMG(search_bg, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/search_button.png"));
		REG_IMG(search_button, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/google_bg.png"));
		REG_IMG(google_bg, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(_T("ui/google_button.png"));
		REG_IMG(google_button, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
	}

	// 加载控件
	void LoadCtl()
	{
		// 创建控件对象并设置

	#define GuiLoadPic(name) \
		REG_CTL(name, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc))); \
		GUI_CTL(name)->SetState(_T("image"), GUI_IMG(name)); \
		GUI_CTL(name)->AddEvent(ExDynCast<IGuiEvent>(ExGui(CString(_T("CEvent_")) + _T(#name), &gc)))
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

		REG_CTL(list, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiListView"), &gc)));
		GUI_CTL(list)->SetState(_T("image"), GUI_IMG(list));
		CListT<IGuiCtrl*> items_list;
		for(int i = 0; i < 200; ++i)
		{
			CImage img_btn[9];
			img_btn[4] = GUI_IMG(list_item)->Get();
			IGuiCtrl* btn = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiLVItem"), &gc));
			btn->SetState(_T("image"), img_btn);
			btn->SetState(_T("icon"), GUI_IMG(list_icon));
			//btn->SetState(_T("text"), txt_btn);
			//btn->SetState(_T("locate"), (void*)2);
			//btn->SetState(_T("loc_off"), (void*)5);
			btn->SetWindowRect(CRect(0, 0, 70, 70));
			items_list.Add(btn);
		}
		GUI_CTL(list)->SetState(_T("items"), &items_list);
		GUI_CTL(list)->SetState(_T("space"), (void*)5);
		GUI_CTL(list)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_list"), &gc)));

	#define GuiLoadBtn(name, thr_sta) \
		REG_CTL(name, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiButton"), &gc))); \
		GUI_CTL(name)->SetState(_T("thr_sta"), (void*)thr_sta); \
		{ \
			CImage tmp[9]; \
			tmp[4] = GUI_IMG(name)->Get(); \
			GUI_CTL(name)->SetState(_T("image"), tmp); \
		} \
		GUI_CTL(name)->AddEvent(ExDynCast<IGuiEvent>(ExGui(CString(_T("CEvent_")) + _T(#name), &gc)))
	//#define GuiLoadBtn()

		GuiLoadBtn(win_sysbtn_close, true);
		GuiLoadBtn(win_sysbtn_max, true);
		GuiLoadBtn(win_sysbtn_restore, true);
		GuiLoadBtn(win_sysbtn_min, true);

	#define GuiLoadGrp(name, count, thr_sta) \
		REG_CTL(name, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiGroup"), &gc))); \
		CArrayT<IGuiCtrl*> items_##name; \
		for(int i = 0; i < count; ++i) \
		{ \
			IGuiCtrl* itm = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiButton"), &gc)); \
			itm->SetState(_T("thr_sta"), (void*)thr_sta); \
			items_##name.Add(itm); \
		} \
		GUI_CTL(name)->SetState(_T("items"), &items_##name); \
		GUI_CTL(name)->SetState(_T("image"), GUI_IMG(name)); \
		GUI_CTL(name)->AddEvent(ExDynCast<IGuiEvent>(ExGui(CString(_T("CEvent_")) + _T(#name), &gc)))
	//#define GuiLoadGrp()

		GuiLoadGrp(gamesearch_charmap, 26, true);
		GuiLoadGrp(toolbar_tools, 8, true);

		REG_CTL(topbar_btn, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiGroup"), &gc)));
		CArrayT<IGuiCtrl*> items_topbar_btn;
		IGuiEvent* items_topbar_btn_evt = ExDynCast<IGuiEvent>(ExGui(_T("CEvent_items_topbar_btn"), &gc));
		for(int i = 0; i < 8; ++i)
		{
			IGuiCtrl* itm = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPushBtn"), &gc));
			itm->AddEvent(items_topbar_btn_evt);
			GUI_CTL(topbar_btn)->AddComp(itm);
			items_topbar_btn.Add(itm);
		}
		items_topbar_btn[0]->SetState(_T("status"), (void*)3);
		GUI_CTL(topbar_btn)->SetState(_T("items"), &items_topbar_btn);
		GUI_CTL(topbar_btn)->SetState(_T("sta_cnt"), (void*)4);
		GUI_CTL(topbar_btn)->SetState(_T("sty_box"), (void*)false);
		GUI_CTL(topbar_btn)->SetState(_T("image"), GUI_IMG(topbar_btn));
		GUI_CTL(topbar_btn)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_topbar_btn"), &gc)));

	#define GuiLoadPsh(name) \
		REG_CTL(name, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPushBtn"), &gc))); \
		GUI_CTL(name)->SetState(_T("image"), GUI_IMG(name)); \
		GUI_CTL(name)->AddEvent(ExDynCast<IGuiEvent>(ExGui(CString(_T("CEvent_")) + _T(#name), &gc)))
	//#define GuiLoadGrp()

		GuiLoadPsh(tag_qb);
		GUI_CTL(tag_qb)->SetState(_T("status"), (void*)3);
		GuiLoadPsh(tag_zx);
		GuiLoadPsh(tag_wl);
		GuiLoadPsh(tag_dz);
		GuiLoadPsh(tag_wy);
		GuiLoadPsh(tag_dj);

		GuiLoadPic(search_bg);
		GuiLoadPic(search_button);
		GuiLoadPic(google_bg);
		GuiLoadPic(google_button);
	}

	// 加载窗口
	void LoadWnd()
	{
		// 相关资源定义
		RECT rc_dsk = {0};
		::GetClientRect(::GetDesktopWindow(), &rc_dsk);
		CRect rc_wnd(0, 0, 1000, 750);
		HICON ic_wnd = ::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_DEMOGUI1));

		// 创建窗口对象并设置
		REG_WND(main, ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc)));
		GUI_WND(main)->Create(_T("DemoGui1"), rc_wnd, SW_HIDE, 
			WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
		GUI_WND(main)->SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)ic_wnd);
		GUI_WND(main)->SendMessage(WM_SETICON, (WPARAM)FALSE, (LPARAM)ic_wnd);
		GUI_WND(main)->CenterWindow();
		GUI_WND(main)->SetLayered(false);
		GUI_WND(main)->GetClientRect(rc_wnd);
		GUI_WND(main)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_wnd"), &gc)));
	}

	// 关联对象
	void LinkGui()
	{
		// 内容打底
		GUI_WND(main)->AddComp(GUI_CTL(banner));
		GUI_WND(main)->AddComp(GUI_CTL(tag_bg));
		GUI_WND(main)->AddComp(GUI_CTL(toolbar_bg));

		// 列表
		GUI_WND(main)->AddComp(GUI_CTL(list));

		// 功能按钮
		GUI_WND(main)->AddComp(GUI_CTL(topbar_btn));
		GUI_WND(main)->AddComp(GUI_CTL(gamesearch_charmap));
		GUI_WND(main)->AddComp(GUI_CTL(toolbar_tools));

		// 标签栏
		GUI_WND(main)->AddComp(GUI_CTL(tag_qb));
		GUI_WND(main)->AddComp(GUI_CTL(tag_zx));
		GUI_WND(main)->AddComp(GUI_CTL(tag_wl));
		GUI_WND(main)->AddComp(GUI_CTL(tag_dz));
		GUI_WND(main)->AddComp(GUI_CTL(tag_wy));
		GUI_WND(main)->AddComp(GUI_CTL(tag_dj));

		// 编辑框
		GUI_WND(main)->AddComp(GUI_CTL(search_bg));
		GUI_WND(main)->AddComp(GUI_CTL(search_button));
		GUI_WND(main)->AddComp(GUI_CTL(google_bg));
		GUI_WND(main)->AddComp(GUI_CTL(google_button));

		// 窗口边框
		GUI_WND(main)->AddComp(GUI_CTL(line_bottom));
		GUI_WND(main)->AddComp(GUI_CTL(line_left));
		GUI_WND(main)->AddComp(GUI_CTL(line_right));
		GUI_WND(main)->AddComp(GUI_CTL(line_top));
		GUI_WND(main)->AddComp(GUI_CTL(corner_lb));
		GUI_WND(main)->AddComp(GUI_CTL(corner_rb));
		GUI_WND(main)->AddComp(GUI_CTL(corner_rt));
		GUI_WND(main)->AddComp(GUI_CTL(corner_lt));

		// 三态按钮
		GUI_WND(main)->AddComp(GUI_CTL(win_sysbtn_close));
		GUI_WND(main)->AddComp(GUI_CTL(win_sysbtn_max));
		GUI_WND(main)->AddComp(GUI_CTL(win_sysbtn_restore));
		GUI_WND(main)->AddComp(GUI_CTL(win_sysbtn_min));

		// 显示主窗口
		GUI_WND(main)->ShowWindow(SW_SHOW);
	}

public:
	CGuiLoader()
		: gc(ExGC())
	{
		LoadRes();
		LoadCtl();
		LoadWnd();
		LinkGui();
	}
} g_GuiLoader;