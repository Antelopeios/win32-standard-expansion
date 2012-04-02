#include "stdafx.h"
#include "../Main/Tagger.h"

//////////////////////////////////////////////////////////////////

// 加载资源
void CGuiLoader::LoadRes()
{
	CText* txt = gcnew(gc, CText);
	txt->SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
	REG_TXT(search, txt);

	txt = gcnew(gc, CText);
	txt->SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
	txt->SetColor(ExRGBA(0, 0, 0, 100));
	REG_TXT(cover, txt);

	// 获取解码器
	ICoderObject* coder = CImgAnalyzer::GetCoder(CImgAnalyzer::png, &gc);
	if (!coder) return;
	CMemFile file;
	coder->SetFile(&file);
	// 加载图片前的准备工作
	BYTE* buff = NULL; DWORD size = 0;
	HGLOBAL hres = NULL;
#define REG_RES_IMG(res_id, img_id) \
	hres = ExGetBinary(res_id, _T("PNG"), buff, size); \
	if (hres) \
	{ \
		file.Open(buff, size); \
		REG_IMG(img_id, gcnew(gc, CImage))->Set(coder->Decode()); \
		ExReleaseBinary(hres); \
	}
//#define REG_RES_IMG(res_id, img_id)
	// 加载文件项图片
	REG_RES_IMG(IDB_FILE_C, file_c);
	REG_RES_IMG(IDB_FILE_L, file_l);
	REG_RES_IMG(IDB_FILE_R, file_r);
	REG_RES_IMG(IDB_FILE_T, file_t);
	REG_RES_IMG(IDB_FILE_B, file_b);
	REG_RES_IMG(IDB_FILE_LT, file_lt);
	REG_RES_IMG(IDB_FILE_LB, file_lb);
	REG_RES_IMG(IDB_FILE_RT, file_rt);
	REG_RES_IMG(IDB_FILE_RB, file_rb);
	// 加载焦点项图片
	REG_RES_IMG(IDB_FOC_L, foc_l);
	REG_RES_IMG(IDB_FOC_R, foc_r);
	REG_RES_IMG(IDB_FOC_T, foc_t);
	REG_RES_IMG(IDB_FOC_B, foc_b);
	REG_RES_IMG(IDB_FOC_LT, foc_lt);
	REG_RES_IMG(IDB_FOC_LB, foc_lb);
	REG_RES_IMG(IDB_FOC_RT, foc_rt);
	REG_RES_IMG(IDB_FOC_RB, foc_rb);
}

// 加载控件
void CGuiLoader::LoadCtl()
{
	// 内容打底
	REG_CTL(ground, ExGui<IGuiCtl>(_T("CGuiPicture")));
	GET_CTL(ground)->SetState(_T("color"), (void*)ExRGBA(242, 242, 242, 255));
	GET_CTL(ground)->AddEvent(ExGui<IGuiEvent>(_T("CEvent_ground")));

	// 搜索框打底
	REG_CTL(bottom1, ExGui<IGuiCtl>(_T("CGuiPicture")));
	GET_CTL(bottom1)->SetState(_T("color"), (void*)ExRGBA(222, 222, 222, 255));
	GET_CTL(bottom1)->AddEvent(ExGui<IGuiEvent>(_T("CEvent_bottom1")));
	REG_CTL(bottom2, ExGui<IGuiCtl>(_T("CGuiPicture")));
	GET_CTL(bottom2)->SetState(_T("color"), (void*)ExRGBA(255, 255, 255, 255));
	GET_CTL(bottom2)->AddEvent(ExGui<IGuiEvent>(_T("CEvent_bottom2")));
	// 搜索框
	REG_CTL(search, ExGui<IGuiCtl>(_T("CGuiEdit")));
	GET_CTL(search)->SetState(_T("font"), GET_TXT(search));
	GET_CTL(search)->SetState(_T("empty_font"), GET_TXT(cover));
	GET_CTL(search)->SetState(_T("empty_text"), &CString(_T("请输入待查询标签")));
	GET_CTL(search)->AddEvent(ExGui<IGuiEvent>(_T("CEvent_search")));

	// 标签云
	REG_CTL(cloud, ExGui<IGuiCtl>(_T("CGuiListView")));
	GET_CTL(cloud)->SetState(_T("align_top"), (void*)FALSE);
	GET_CTL(cloud)->SetState(_T("color"), (void*)ExRGBA(0, 0, 0, 0));
	// 加载标签
	IGuiCtl::items_t tags_list;
	for(int i = 0; i < 40; ++i)
	{
		IGuiCtl* tag = ExGui<IGuiCtl>(_T("CGuiLVItem"));
		tag->SetState(_T("thr_sta"), (void*)(0x04 | 0x08));
		tag->SetState(_T("loc_off"), (void*)0);
		tag->SetState(_T("ico_off"), (void*)0);
		CText* txt = gcnew(gc, CText, 8);
		txt[0].SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
		LOGFONT lf = {0};
		txt[0].GetLogFont(&lf);
		lf.lfHeight -= i;
		txt[0].Create(&lf);
		txt[0].SetColor(ExRGBA(160, 160, 160, 255));
		lf.lfUnderline = TRUE;
		txt[1].Create(&lf);
		txt[1].SetColor(ExRGBA(101, 177, 177, 255));
		txt[2].Create(&lf);
		txt[2].SetColor(ExRGBA(0, 160, 160, 255));
		CString str(_T("测试标签"));
		CSize sz;
		txt[0].GetSize(str, sz);
		tag->SetState(_T("font"), txt);
		tag->SetState(_T("text"), &str);
		tag->SetWindowRect(CRect(0, 0, sz.cx + 8, sz.cy + 8));
		tags_list.Add(tag);
	}
	GET_CTL(cloud)->SetState(_T("items"), &tags_list);
	GET_CTL(cloud)->AddEvent(ExGui<IGuiEvent>(_T("CEvent_cloud")));
	// 标签云滚动条
	REG_CTL(scr_cloud, ExGui<IGuiCtl>(_T("CGuiScroll")));
	ExDynCast<IGuiCtl>(GET_CTL(scr_cloud)->GetState(_T("up")))->SetWindowRect(CRect());
	ExDynCast<IGuiCtl>(GET_CTL(scr_cloud)->GetState(_T("down")))->SetWindowRect(CRect());
	GET_CTL(scr_cloud)->SetState(_T("sli_blk_thr_sta"), (void*)TRUE);
	GET_CTL(scr_cloud)->SetState(_T("sli_ori"), (void*)TRUE);
	GET_CTL(scr_cloud)->SetState(_T("sli_color"), (void*)ExRGBA(220, 220, 220, 255));
	pixel_t pix[8] = {0};
	for(int i = 0; i < _countof(pix); ++i) pix[i] = ExRGBA(180, 180, 180, 255);
	GET_CTL(scr_cloud)->SetState(_T("sli_blk_color"), pix);
	GET_CTL(scr_cloud)->AddEvent(ExGui<IGuiEvent>(_T("CEvent_scr_cloud")));
	// 关联滚动条
	GET_CTL(cloud)->SetScroll(GET_CTL(scr_cloud));

	// 文件列表
	REG_CTL(files, ExGui<IGuiCtl>(_T("CGuiListView")));
	GET_CTL(files)->SetTrust(TRUE);
	GET_CTL(files)->SetVisible(FALSE);
	GET_CTL(files)->SetState(_T("align_top"), (void*)FALSE);
	GET_CTL(files)->SetState(_T("space"), (void*)4);
	GET_CTL(files)->SetState(_T("color"), (void*)ExRGBA(0, 0, 0, 0));
	GET_CTL(files)->Execute(_T("foc_image"), _T("foc_lt,foc_t,foc_rt,foc_l,,foc_r,foc_lb,foc_b,foc_rb"));
	// 加载文件
	IGuiCtl::items_t file_list;
	for(int i = 0; i < 40; ++i)
	{
		IGuiCtl* file = ExGui<IGuiCtl>(_T("CGuiLVItem"));
		file->Execute(_T("image"), _T("file_lt,file_t,file_rt,file_l,file_c,file_r,file_lb,file_b,file_rb"));
		file->SetState(_T("locate"), (void*)1);
		file->SetState(_T("loc_off"), (void*)5);
		file->SetState(_T("ico_off"), (void*)5);
		file->SetState(_T("shake_ico"), (void*)1);
		CText* txt = gcnew(gc, CText, 8);
		txt[0].SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
		txt[0].SetColor(ExRGBA(0, 0, 0, 255));
		for (int n = 1; n < 8; ++n) { txt[n] = txt[0]; }
		file->SetState(_T("font"), txt);
		file->SetState(_T("text"), &CString(_T("测试文件")));
		file->SetWindowRect(CRect(0, 0, 80, 90));
		file_list.Add(file);
	}
	GET_CTL(files)->SetState(_T("items"), &file_list);
	GET_CTL(files)->AddEvent(ExGui<IGuiEvent>(_T("CEvent_files")));
	// 文件列表滚动条
	REG_CTL(scr_files, ExGui<IGuiCtl>(_T("CGuiScroll")));
	GET_CTL(scr_files)->SetVisible(FALSE);
	ExDynCast<IGuiCtl>(GET_CTL(scr_files)->GetState(_T("up")))->SetWindowRect(CRect());
	ExDynCast<IGuiCtl>(GET_CTL(scr_files)->GetState(_T("down")))->SetWindowRect(CRect());
	GET_CTL(scr_files)->SetState(_T("sli_blk_thr_sta"), (void*)TRUE);
	GET_CTL(scr_files)->SetState(_T("sli_ori"), (void*)TRUE);
	GET_CTL(scr_files)->SetState(_T("sli_color"), (void*)ExRGBA(220, 220, 220, 255));
	GET_CTL(scr_files)->SetState(_T("sli_blk_color"), pix);
	GET_CTL(scr_files)->AddEvent(ExGui<IGuiEvent>(_T("CEvent_scr_files")));
	// 关联滚动条
	GET_CTL(files)->SetScroll(GET_CTL(scr_files));
}

// 加载窗口
void CGuiLoader::LoadWnd()
{
	// 窗口资源定义
	CRect rc_wnd(0, 0, GUI()->DefSize().cx, GUI()->DefSize().cy);
	HICON ic_wnd = ::LoadIcon(CGlobal::GetInstance(), MAKEINTRESOURCE(IDI_TAGGER));

	// 创建窗口对象并设置
	REG_WND(main, ExGui<IGuiWnd>(_T("CGuiWnd"), &gc));
	GET_WND(main)->Create(_T("Tagger"), rc_wnd, SW_HIDE, 
		/*WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX*/WS_OVERLAPPEDWINDOW);
	GET_WND(main)->SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)ic_wnd);
	GET_WND(main)->SendMessage(WM_SETICON, (WPARAM)FALSE, (LPARAM)ic_wnd);
	GET_WND(main)->CenterWindow();
	GET_WND(main)->SetLayered(FALSE, FALSE);
	GET_WND(main)->GetClientRect(rc_wnd);

	// 事件关联
	GET_WND(main)->AddEvent(ExGui<IGuiEvent>(_T("CEvent_wnd")));
}

// 关联对象
void CGuiLoader::LinkGui()
{
	// 显示主窗口
	GET_WND(main)->ShowWindow(SW_SHOW);

	// 内容打底
	GET_WND(main)->AddComp(GET_CTL(ground));
	// 搜索框打底
	GET_WND(main)->AddComp(GET_CTL(bottom1));
	GET_WND(main)->AddComp(GET_CTL(bottom2));
	// 搜索框
	GET_WND(main)->AddComp(GET_CTL(search));

	// 标签云
	GET_WND(main)->AddComp(GET_CTL(cloud));
	GET_WND(main)->AddComp(GET_CTL(scr_cloud));

	// 文件列表
	GET_WND(main)->AddComp(GET_CTL(files));
	GET_WND(main)->AddComp(GET_CTL(scr_files));
}

//////////////////////////////////////////////////////////////////

CGuiLoader::CGuiLoader()
{
}

CGuiLoader* g_instance = NULL;
CGuiLoader* CGuiLoader::Instance()
{
	if (g_instance == NULL)
		g_instance = dbnew(CGuiLoader);
	return g_instance;
}

void CGuiLoader::Init()
{
#ifdef	_CONSOLE
	m_Msg.Create();
	while(TRUE)
	{
		// 获得输入
		CStringT<char> s;
		char c = 0;
		putchar('>');
		do
		{
			c = getchar();
			if (c == '\n') break;
			s += c;
		} while(TRUE);
		// 判断输入
		if (s == "exit")
			break;
		else
		if (s == "get tags")
			TAG()->GetTags();
		else
		if (s == "get files")
			TAG()->GetFiles();
		else
		if (s.Left(8) == "get tag ")
			TAG()->GetTag(CString(s.Mid(8)));
		else
		if (s.Left(9) == "get file ")
			TAG()->GetFile(CString(s.Mid(9)));
		else
		if (s.Left(4) == "add ")
		{
			CString t(s.Mid(4));
			CString::iterator_t i = t.Find(_T(';'));
			if (i == t.Tail())
			{
				_tcprintf_s(_T("输入命令不正确\n\n"));
				continue;
			}
			TAG()->Add(CString(t.Mid(0, i->Index())), CString(t.Mid(i->Index() + 1)));
		}
		else
		if (s.Left(8) == "del tag ")
			TAG()->DelTag(CString(s.Mid(8)));
		else
		if (s.Left(9) == "del file ")
			TAG()->DelFile(CString(s.Mid(9)));
		else
		if (s.Left(4) == "del ")
		{
			CString t(s.Mid(4));
			CString::iterator_t i = t.Find(_T(';'));
			if (i == t.Tail())
			{
				_tcprintf_s(_T("输入命令不正确\n\n"));
				continue;
			}
			TAG()->Del(CString(t.Mid(0, i->Index())), CString(t.Mid(i->Index() + 1)));
		}
		else
		if (s.Left(8) == "set tag ")
		{
			CString t(s.Mid(8));
			CString::iterator_t i = t.Find(_T(';'));
			if (i == t.Tail())
			{
				_tcprintf_s(_T("输入命令不正确\n\n"));
				continue;
			}
			TAG()->SetTag(CString(t.Mid(0, i->Index())), CString(t.Mid(i->Index() + 1)));
		}
		else
		if (s.Left(9) == "set file ")
		{
			CString t(s.Mid(9));
			CString::iterator_t i = t.Find(_T(';'));
			if (i == t.Tail())
			{
				_tcprintf_s(_T("输入命令不正确\n\n"));
				continue;
			}
			TAG()->SetTag(CString(t.Mid(0, i->Index())), CString(t.Mid(i->Index() + 1)));
		}
		else
		{
			_tcprintf_s(_T("输入命令不正确\n\n"));
			continue;
		}
		// 获得并显示处理结果
		m_Msg.Wait();
		CTagger::list_t ret = TAG()->GetRet();
		if (ret.Empty())
			_tcprintf_s(_T("没有找到任何结果\n\n"));
		else
		{
			CTagger::list_t tmp;
			CTagger::list_t::iterator_t ite = ret.Head();
			for(; ite != ret.Tail(); ++ite)
			{
				if (tmp.Find(*ite) != tmp.Tail()) continue;
				tmp.Add(*ite);
				_tcprintf_s(_T("%s\n"), (*ite).GetCStr());
			}
			_tcprintf_s(_T("共找到%d条结果\n\n"), tmp.GetCount());
		}
	}
	::PostQuitMessage(0);
#else /*_CONSOLE*/
	CRect rc_dsk;
	::GetClientRect(::GetDesktopWindow(), (LPRECT)&rc_dsk);
	m_DefSize.cx = (LONG)(rc_dsk.Width() * 0.6);
	m_DefSize.cy = (LONG)(rc_dsk.Height() * 0.6);

	LoadRes();
	LoadCtl();
	LoadWnd();
	LinkGui();
#endif/*_CONSOLE*/
	//GET_WND(main)->SendMessage(WM_CLOSE);
	//::PostQuitMessage(0);
}

void CGuiLoader::Term()
{
	del(this);
	g_instance = NULL;
}

CSize CGuiLoader::DefSize()
{
	return m_DefSize;
}

int CGuiLoader::ScrWidth()
{
	return 10;
}

//////////////////////////////////////////////////////////////////

// 重新获取数据
void CGuiLoader::Refresh()
{
#ifdef	_CONSOLE
	m_Msg.Set();
#else /*_CONSOLE*/
#endif/*_CONSOLE*/
}