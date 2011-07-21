#include "stdafx.h"
#include "DemoGui1.h"

//////////////////////////////////////////////////////////////////

CImage CResManager::banner;
CImage CResManager::corner_lb;
CImage CResManager::corner_rb;
CImage CResManager::corner_rt;
CImage CResManager::corner_lt;
CImage CResManager::line_bottom;
CImage CResManager::line_left;
CImage CResManager::line_right;
CImage CResManager::line_top;
CImage CResManager::tag_bg;
CImage CResManager::toolbar_bg;

CImage CResManager::list;
CImage CResManager::list_item;

CImage CResManager::win_sysbtn_close;
CImage CResManager::win_sysbtn_max;
CImage CResManager::win_sysbtn_min;
CImage CResManager::win_sysbtn_restore;

CImage CResManager::topbar_btn;
CImage CResManager::gamesearch_charmap;
CImage CResManager::toolbar_tools;

CImage CResManager::tag_qb;
CImage CResManager::tag_zx;
CImage CResManager::tag_wl;
CImage CResManager::tag_dz;
CImage CResManager::tag_wy;
CImage CResManager::tag_dj;

CImage CResManager::search_bg;
CImage CResManager::search_button;
CImage CResManager::google_bg;
CImage CResManager::google_button;

//////////////////////////////////////////////////////////////////

CCtlManager::key_map_t CCtlManager::m_Ctrls(101);

//////////////////////////////////////////////////////////////////

void CGuiLoader::LoadRes()
{
	// 解码器
	ICoderObject* coder = CImgAnalyzer::GetCoder(CImgAnalyzer::png, &gc);
	if (!coder) return;
	CIOFile file;
	coder->SetFile(&file);

	// 图片资源

	file.Open(_T("ui/banner.png"));
	CResManager::banner.Set(coder->Decode());
	file.Open(_T("ui/corner_lb.png"));
	CResManager::corner_lb.Set(coder->Decode());
	file.Open(_T("ui/corner_rb.png"));
	CResManager::corner_rb.Set(coder->Decode());
	file.Open(_T("ui/corner_rt.png"));
	CResManager::corner_rt.Set(coder->Decode());
	file.Open(_T("ui/corner_lt.png"));
	CResManager::corner_lt.Set(coder->Decode());
	file.Open(_T("ui/line_bottom.png"));
	CResManager::line_bottom.Set(coder->Decode());
	file.Open(_T("ui/line_left.png"));
	CResManager::line_left.Set(coder->Decode());
	file.Open(_T("ui/line_right.png"));
	CResManager::line_right.Set(coder->Decode());
	file.Open(_T("ui/line_top.png"));
	CResManager::line_top.Set(coder->Decode());
	file.Open(_T("ui/tag_bg.png"));
	CResManager::tag_bg.Set(coder->Decode());
	file.Open(_T("ui/toolbar_bg.png"));
	CResManager::toolbar_bg.Set(coder->Decode());

	file.Open(_T("ui/list.png"));
	CResManager::list.Set(coder->Decode());
	file.Open(_T("ui/list_item.png"));
	CResManager::list_item.Set(coder->Decode());

	file.Open(_T("ui/win_sysbtn_close.png"));
	CResManager::win_sysbtn_close.Set(coder->Decode());
	file.Open(_T("ui/win_sysbtn_max.png"));
	CResManager::win_sysbtn_max.Set(coder->Decode());
	file.Open(_T("ui/win_sysbtn_min.png"));
	CResManager::win_sysbtn_min.Set(coder->Decode());
	file.Open(_T("ui/win_sysbtn_restore.png"));
	CResManager::win_sysbtn_restore.Set(coder->Decode());

	file.Open(_T("ui/topbar_btn.png"));
	CResManager::topbar_btn.Set(coder->Decode());
	file.Open(_T("ui/gamesearch_charmap.png"));
	CResManager::gamesearch_charmap.Set(coder->Decode());
	file.Open(_T("ui/toolbar_tools.png"));
	CResManager::toolbar_tools.Set(coder->Decode());

	file.Open(_T("ui/tag_qb.png"));
	CResManager::tag_qb.Set(coder->Decode());
	file.Open(_T("ui/tag_zx.png"));
	CResManager::tag_zx.Set(coder->Decode());
	file.Open(_T("ui/tag_wl.png"));
	CResManager::tag_wl.Set(coder->Decode());
	file.Open(_T("ui/tag_dz.png"));
	CResManager::tag_dz.Set(coder->Decode());
	file.Open(_T("ui/tag_wy.png"));
	CResManager::tag_wy.Set(coder->Decode());
	file.Open(_T("ui/tag_dj.png"));
	CResManager::tag_dj.Set(coder->Decode());

	file.Open(_T("ui/search_bg.png"));
	CResManager::search_bg.Set(coder->Decode());
	file.Open(_T("ui/search_button.png"));
	CResManager::search_button.Set(coder->Decode());
	file.Open(_T("ui/google_bg.png"));
	CResManager::google_bg.Set(coder->Decode());
	file.Open(_T("ui/google_button.png"));
	CResManager::google_button.Set(coder->Decode());
}

void CGuiLoader::LoadCtl()
{
	// 创建控件对象并设置

#define GuiLoadPic(name) \
	REG_CTRL(name, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc))); \
	GUI_CTRL(name)->SetState(_T("image"), &CResManager::##name); \
	GUI_CTRL(name)->AddEvent(ExDynCast<IGuiEvent>(ExGui(CString(_T("CEvent_")) + _T(#name), &gc)))
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

	REG_CTRL(list, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiListView"), &gc)));
	GUI_CTRL(list)->SetState(_T("image"), &CResManager::list);
	CListT<IGuiCtrl*> items_list;
	for(int i = 0; i < 200; ++i)
	{
		CImage img_btn[9];
		img_btn[4] = CResManager::list_item;
		IGuiCtrl* btn = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiLVItem"), &gc));
		btn->SetState(_T("image"), img_btn);
		//btn->SetState(_T("icon"), &img_pic);
		//btn->SetState(_T("text"), txt_btn);
		//btn->SetState(_T("locate"), (void*)2);
		//btn->SetState(_T("loc_off"), (void*)5);
		btn->SetWindowRect(CRect(0, 0, 70, 70));
		items_list.Add(btn);
	}
	GUI_CTRL(list)->SetState(_T("items"), &items_list);
	GUI_CTRL(list)->SetState(_T("space"), (void*)5);
	GUI_CTRL(list)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_list"), &gc)));

#define GuiLoadBtn(name, thr_sta) \
	REG_CTRL(name, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiButton"), &gc))); \
	GUI_CTRL(name)->SetState(_T("thr_sta"), (void*)thr_sta); \
	{ \
		CImage tmp[9]; \
		tmp[4] = CResManager::##name; \
		GUI_CTRL(name)->SetState(_T("image"), tmp); \
	} \
	GUI_CTRL(name)->AddEvent(ExDynCast<IGuiEvent>(ExGui(CString(_T("CEvent_")) + _T(#name), &gc)))
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
		items_##name.Add(itm); \
	} \
	GUI_CTRL(name)->SetState(_T("items"), &items_##name); \
	GUI_CTRL(name)->SetState(_T("image"), &CResManager::##name); \
	GUI_CTRL(name)->AddEvent(ExDynCast<IGuiEvent>(ExGui(CString(_T("CEvent_")) + _T(#name), &gc)))
//#define GuiLoadGrp()

	GuiLoadGrp(gamesearch_charmap, 26, true);
	GuiLoadGrp(toolbar_tools, 8, true);

	REG_CTRL(topbar_btn, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiGroup"), &gc)));
	CArrayT<IGuiCtrl*> items_topbar_btn;
	IGuiEvent* items_topbar_btn_evt = ExDynCast<IGuiEvent>(ExGui(_T("CEvent_items_topbar_btn"), &gc));
	for(int i = 0; i < 8; ++i)
	{
		IGuiCtrl* itm = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPushBtn"), &gc));
		itm->AddEvent(items_topbar_btn_evt);
		GUI_CTRL(topbar_btn)->AddComp(itm);
		items_topbar_btn.Add(itm);
	}
	items_topbar_btn[0]->SetState(_T("status"), (void*)3);
	GUI_CTRL(topbar_btn)->SetState(_T("items"), &items_topbar_btn);
	GUI_CTRL(topbar_btn)->SetState(_T("sta_cnt"), (void*)4);
	GUI_CTRL(topbar_btn)->SetState(_T("sty_box"), (void*)false);
	GUI_CTRL(topbar_btn)->SetState(_T("image"), &CResManager::topbar_btn);
	GUI_CTRL(topbar_btn)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_topbar_btn"), &gc)));

#define GuiLoadPsh(name) \
	REG_CTRL(name, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPushBtn"), &gc))); \
	GUI_CTRL(name)->SetState(_T("image"), &CResManager::##name); \
	GUI_CTRL(name)->AddEvent(ExDynCast<IGuiEvent>(ExGui(CString(_T("CEvent_")) + _T(#name), &gc)))
//#define GuiLoadGrp()

	GuiLoadPsh(tag_qb);
	GUI_CTRL(tag_qb)->SetState(_T("status"), (void*)3);
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

CGuiLoader::CGuiLoader()
	: gc(ExGC())
{
	LoadRes();
	LoadCtl();
}

CGuiLoader g_GuiLoader;