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

class CResLoader
{
public:
	CResLoader()
	{
		// 垃圾回收器
		CGC gc;

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
} g_loader;

//////////////////////////////////////////////////////////////////

CCtlManager::key_map_t CCtlManager::m_Ctrls(101);