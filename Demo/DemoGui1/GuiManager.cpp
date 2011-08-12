#include "stdafx.h"
#include "DemoGui1.h"

//////////////////////////////////////////////////////////////////

class CGuiLoader
{
protected:
	CGC& gc;

	CListT<CImage*> list_ico;
	CListT<CString> list_str;

protected:
	void LoadDirImage(CListT<CImage*>& lstImage, CListT<CString>& lstString, LPCTSTR sPath, ICoderObject* pCoder, CGC* pGC)
	{
		if (!sPath || !pCoder || !pGC) return;
		CString path(sPath); path += _T("\\");
		CString path_find(path); path_find += _T("*");

		CIOFile file;
		IFileObject* old_file = pCoder->GetFile();
		pCoder->SetFile(&file);

		WIN32_FIND_DATA fd = {0};
		HANDLE find = ::FindFirstFile(path_find, &fd);
		if (find != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (0 == wcscmp(fd.cFileName, L".") || 
					0 == wcscmp(fd.cFileName, L"..")) continue;
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					LoadDirImage(lstImage, lstString, path + fd.cFileName, pCoder, pGC);
				else
				{
					file.Open(path + fd.cFileName);
					image_t img = pCoder->Decode();
					if (img)
					{
						CImage* image = ExMem::Alloc<CImage>(pGC);
						image->Set(img);
						lstImage.Add(image);
						lstString.Add(fd.cFileName);
					}
				}
			} while (::FindNextFile(find, &fd));
			::FindClose(find);
		}

		pCoder->SetFile(old_file);
	}

	// 加载资源
	void LoadRes()
	{
		// 解码器
		ICoderObject* coder = CImgAnalyzer::GetCoder(CImgAnalyzer::png, &gc);
		if (!coder) return;
		CIOFile file;
		coder->SetFile(&file);

		// 图片资源

		CString path;
		::GetCurrentDirectory(MAX_PATH, path.GetCStr(MAX_PATH));
		path += _T("\\ui");

		file.Open(path + _T("\\banner.png"));
		REG_IMG(banner, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\corner_lb.png"));
		REG_IMG(corner_lb, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\corner_rb.png"));
		REG_IMG(corner_rb, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\corner_rt.png"));
		REG_IMG(corner_rt, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\corner_lt.png"));
		REG_IMG(corner_lt, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\line_bottom.png"));
		REG_IMG(line_bottom, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\line_left.png"));
		REG_IMG(line_left, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\line_right.png"));
		REG_IMG(line_right, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\line_top.png"));
		REG_IMG(line_top, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\tag_bg.png"));
		REG_IMG(tag_bg, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\toolbar_bg.png"));
		REG_IMG(toolbar_bg, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());

		file.Open(path + _T("\\list.png"));
		REG_IMG(list, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_cor_lt.png"));
		REG_IMG(list_cor_lt, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_item_top.png"));
		REG_IMG(list_item_top, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_cor_rt.png"));
		REG_IMG(list_cor_rt, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_item_left.png"));
		REG_IMG(list_item_left, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_item.png"));
		REG_IMG(list_item, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_item_right.png"));
		REG_IMG(list_item_right, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_cor_lb.png"));
		REG_IMG(list_cor_lb, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_item_bottom.png"));
		REG_IMG(list_item_bottom, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_cor_rb.png"));
		REG_IMG(list_cor_rb, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_foc_lt.png"));
		REG_IMG(list_foc_lt, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_foc_top.png"));
		REG_IMG(list_foc_top, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_foc_rt.png"));
		REG_IMG(list_foc_rt, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_foc_left.png"));
		REG_IMG(list_foc_left, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_foc.png"));
		REG_IMG(list_foc, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_foc_right.png"));
		REG_IMG(list_foc_right, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_foc_lb.png"));
		REG_IMG(list_foc_lb, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_foc_bottom.png"));
		REG_IMG(list_foc_bottom, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\list_foc_rb.png"));
		REG_IMG(list_foc_rb, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		LoadDirImage(list_ico, list_str, path + _T("\\icon"), coder, &gc);

		file.Open(path + _T("\\scr_h.png"));
		REG_IMG(scr_h, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\scr_h_sli_top.png"));
		REG_IMG(scr_h_sli_top, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\scr_h_sli_bottom.png"));
		REG_IMG(scr_h_sli_bottom, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\scr_h_sli.png"));
		REG_IMG(scr_h_sli, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\scr_h_ico.png"));
		REG_IMG(scr_h_ico, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\scr_h_up.png"));
		REG_IMG(scr_h_up, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\scr_h_dn.png"));
		REG_IMG(scr_h_dn, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());

		file.Open(path + _T("\\win_sysbtn_close.png"));
		REG_IMG(win_sysbtn_close, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\win_sysbtn_max.png"));
		REG_IMG(win_sysbtn_max, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\win_sysbtn_min.png"));
		REG_IMG(win_sysbtn_min, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\win_sysbtn_restore.png"));
		REG_IMG(win_sysbtn_restore, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());

		file.Open(path + _T("\\topbar_btn.png"));
		REG_IMG(topbar_btn, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\gamesearch_charmap.png"));
		REG_IMG(gamesearch_charmap, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\toolbar_tools.png"));
		REG_IMG(toolbar_tools, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());

		file.Open(path + _T("\\tag_qb.png"));
		REG_IMG(tag_qb, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\tag_zx.png"));
		REG_IMG(tag_zx, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\tag_wl.png"));
		REG_IMG(tag_wl, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\tag_dz.png"));
		REG_IMG(tag_dz, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\tag_wy.png"));
		REG_IMG(tag_wy, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\tag_dj.png"));
		REG_IMG(tag_dj, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());

		file.Open(path + _T("\\search_bg.png"));
		REG_IMG(search_bg, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\search_button.png"));
		REG_IMG(search_button, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\google_bg.png"));
		REG_IMG(google_bg, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode());
		file.Open(path + _T("\\google_button.png"));
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

		CText txt_btn[5];
		txt_btn[0].SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
		txt_btn[0].SetColor(ExRGBA(0, 0, 0, 255));
		for(int i = 1; i < _countof(txt_btn); ++i) txt_btn[i] = txt_btn[0];

		REG_CTL(list, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiListView"), &gc)));
		GUI_CTL(list)->SetState(_T("image"), GUI_IMG(list));
		CImage img_btn[9];
		img_btn[0] = GUI_IMG(list_cor_lt)->Get();
		img_btn[1] = GUI_IMG(list_item_top)->Get();
		img_btn[2] = GUI_IMG(list_cor_rt)->Get();
		img_btn[3] = GUI_IMG(list_item_left)->Get();
		img_btn[4] = GUI_IMG(list_item)->Get();
		img_btn[5] = GUI_IMG(list_item_right)->Get();
		img_btn[6] = GUI_IMG(list_cor_lb)->Get();
		img_btn[7] = GUI_IMG(list_item_bottom)->Get();
		img_btn[8] = GUI_IMG(list_cor_rb)->Get();
		CListT<IGuiCtrl*> items_list;
		CListT<CImage*>::iterator_t ite_ico = list_ico.Head();
		CListT<CString>::iterator_t ite_str = list_str.Head();
		for(; ite_ico != list_ico.Tail() && ite_str != list_str.Tail(); ++ite_ico, ++ite_str)
		{
			IGuiCtrl* btn = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiLVItem"), &gc));
			btn->SetState(_T("image"), img_btn);
			btn->SetState(_T("icon"), *ite_ico);
			for(int i = 0; i < _countof(txt_btn); ++i) txt_btn[i].SetString(*ite_str);
			btn->SetState(_T("text"), txt_btn);
			btn->SetState(_T("locate"), (void*)2);
			btn->SetState(_T("loc_off"), (void*)18);
			btn->SetState(_T("ico_off"), (void*)8);
			btn->SetWindowRect(CRect(0, 0, 80, 90));
			items_list.Add(btn);
		}
		GUI_CTL(list)->SetState(_T("items"), &items_list);
		img_btn[0] = GUI_IMG(list_foc_lt)->Get();
		img_btn[1] = GUI_IMG(list_foc_top)->Get();
		img_btn[2] = GUI_IMG(list_foc_rt)->Get();
		img_btn[3] = GUI_IMG(list_foc_left)->Get();
		img_btn[4] = GUI_IMG(list_foc)->Get();
		img_btn[5] = GUI_IMG(list_foc_right)->Get();
		img_btn[6] = GUI_IMG(list_foc_lb)->Get();
		img_btn[7] = GUI_IMG(list_foc_bottom)->Get();
		img_btn[8] = GUI_IMG(list_foc_rb)->Get();
		GUI_CTL(list)->SetState(_T("foc_image"), img_btn);
		GUI_CTL(list)->SetState(_T("space"), (void*)15);
		GUI_CTL(list)->InsEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_list"), &gc)));
		if (!items_list.Empty()) items_list.HeadItem()->SetFocus();


		REG_CTL(scr_h, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiScroll"), &gc)));
		GUI_CTL(scr_h)->SetState(_T("sli_blk_thr_sta"), (void*)true);
		GUI_CTL(scr_h)->SetState(_T("up_thr_sta"), (void*)true);
		GUI_CTL(scr_h)->SetState(_T("dn_thr_sta"), (void*)true);
		if (!GUI_IMG(scr_h) || GUI_IMG(scr_h)->IsNull())
		{
			pixel_t pix[5] = {0};
			GUI_CTL(scr_h)->SetState(_T("sli_color"), (void*)ExRGBA(220, 220, 220, 255));
			pix[0] = pix[1] = pix[2] = pix[3] = pix[4] = ExRGBA(120, 120, 120, 255);
			GUI_CTL(scr_h)->SetState(_T("sli_blk_color"), pix);
			GUI_CTL(scr_h)->SetState(_T("sli_ori"), (void*)true);
			pix[0] = pix[1] = pix[2] = pix[3] = pix[4] = ExRGBA(180, 180, 180, 255);
			GUI_CTL(scr_h)->SetState(_T("up_color"), pix);
			GUI_CTL(scr_h)->SetState(_T("dn_color"), pix);
		}
		else
		{
			CImage img[9];
			GUI_CTL(scr_h)->SetState(_T("sli_image"), GUI_IMG(scr_h));
			img[1] = GUI_IMG(scr_h_sli_top)->Get();
			img[4] = GUI_IMG(scr_h_sli)->Get();
			img[7] = GUI_IMG(scr_h_sli_bottom)->Get();
			GUI_CTL(scr_h)->SetState(_T("sli_blk_image"), img);
			GUI_CTL(scr_h)->SetState(_T("sli_blk_icon"), GUI_IMG(scr_h_ico));
			img[4] = GUI_IMG(scr_h_up)->Get();
			GUI_CTL(scr_h)->SetState(_T("up_image"), img);
			img[4] = GUI_IMG(scr_h_dn)->Get();
			GUI_CTL(scr_h)->SetState(_T("dn_image"), img);
		}
		GUI_CTL(scr_h)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_scr_h"), &gc)));

		GUI_CTL(list)->SetScroll(GUI_CTL(scr_h));

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

		GuiLoadBtn(win_sysbtn_close, 1);
		GuiLoadBtn(win_sysbtn_max, 1);
		GuiLoadBtn(win_sysbtn_restore, 1);
		GuiLoadBtn(win_sysbtn_min, 1);

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

		GuiLoadGrp(gamesearch_charmap, 26, 1);
		GuiLoadGrp(toolbar_tools, 8, 1);

		REG_CTL(topbar_btn, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiGroup"), &gc)));
		CArrayT<IGuiCtrl*> items_topbar_btn;
		IGuiEvent* items_topbar_btn_evt = ExDynCast<IGuiEvent>(ExGui(_T("CEvent_items_topbar_btn"), &gc));
		for(int i = 0; i < 8; ++i)
		{
			IGuiCtrl* itm = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPushBtn"), &gc));
			itm->AddEvent(items_topbar_btn_evt);
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
		GUI_WND(main)->AddComp(GUI_CTL(scr_h));

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