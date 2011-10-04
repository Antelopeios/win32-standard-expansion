#include "stdafx.h"
#include "Main/Tagger.h"

//////////////////////////////////////////////////////////////////

// ������Դ
void CGuiLoader::LoadRes()
{
	CText* txt = ExMem::Alloc<CText>(&gc);
	txt->SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
	REG_TXT(search, txt);

	txt = ExMem::Alloc<CText>(&gc);
	txt->SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
	txt->SetColor(ExRGBA(0, 0, 0, 100));
	REG_TXT(cover, txt);

	// ��ȡ������
	ICoderObject* coder = CImgAnalyzer::GetCoder(CImgAnalyzer::png, &gc);
	if (!coder) return;
	CMemFile file;
	coder->SetFile(&file);
	// ����ͼƬǰ��׼������
	BYTE* buff = NULL; DWORD size = 0;
	HGLOBAL hres = NULL;
#define REG_RES_IMG(res_id, img_id) \
	hres = GBL()->GetBinary(res_id, _T("PNG"), buff, size); \
	if (hres) \
	{ \
		file.Open(buff, size); \
		REG_IMG(img_id, ExMem::Alloc<CImage>(&gc))->Set(coder->Decode()); \
		GBL()->ReleaseBinary(hres); \
	}
//#define REG_RES_IMG(res_id, img_id)
	// �����ļ���ͼƬ
	REG_RES_IMG(IDB_FILE_C, file_c);
	REG_RES_IMG(IDB_FILE_L, file_l);
	REG_RES_IMG(IDB_FILE_R, file_r);
	REG_RES_IMG(IDB_FILE_T, file_t);
	REG_RES_IMG(IDB_FILE_B, file_b);
	REG_RES_IMG(IDB_FILE_LT, file_lt);
	REG_RES_IMG(IDB_FILE_LB, file_lb);
	REG_RES_IMG(IDB_FILE_RT, file_rt);
	REG_RES_IMG(IDB_FILE_RB, file_rb);
	// ���ؽ�����ͼƬ
	REG_RES_IMG(IDB_FOC_L, foc_l);
	REG_RES_IMG(IDB_FOC_R, foc_r);
	REG_RES_IMG(IDB_FOC_T, foc_t);
	REG_RES_IMG(IDB_FOC_B, foc_b);
	REG_RES_IMG(IDB_FOC_LT, foc_lt);
	REG_RES_IMG(IDB_FOC_LB, foc_lb);
	REG_RES_IMG(IDB_FOC_RT, foc_rt);
	REG_RES_IMG(IDB_FOC_RB, foc_rb);
}

// ���ؿؼ�
void CGuiLoader::LoadCtl()
{
	// ���ݴ��
	REG_CTL(ground, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc)));
	GET_CTL(ground)->SetState(_T("color"), (void*)ExRGBA(242, 242, 242, 255));
	GET_CTL(ground)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_ground"), &gc)));

	// ��������
	REG_CTL(bottom1, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc)));
	GET_CTL(bottom1)->SetState(_T("color"), (void*)ExRGBA(222, 222, 222, 255));
	GET_CTL(bottom1)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_bottom1"), &gc)));
	REG_CTL(bottom2, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc)));
	GET_CTL(bottom2)->SetState(_T("color"), (void*)ExRGBA(255, 255, 255, 255));
	GET_CTL(bottom2)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_bottom2"), &gc)));
	// ������
	REG_CTL(search, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiEdit"), &gc)));
	GET_CTL(search)->SetState(_T("text"), GET_TXT(search));
	GET_CTL(search)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_search"), &gc)));
	// ���ǿ�
	REG_CTL(cover, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiEdit"), &gc)));
	GET_CTL(cover)->SetState(_T("text"), GET_TXT(cover));
	GET_CTL(cover)->SetState(_T("edit"), &CString(_T("���������ѯ��ǩ")));
	GET_CTL(cover)->SetVisible(false);
	GET_CTL(cover)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_cover"), &gc)));

	// ��ǩ��
	REG_CTL(cloud, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiListView"), &gc)));
	GET_CTL(cloud)->SetTrust(true);
	GET_CTL(cloud)->SetState(_T("align_top"), (void*)false);
	GET_CTL(cloud)->SetState(_T("color"), (void*)ExRGBA(0, 0, 0, 0));
	// ���ر�ǩ
	CListT<IGuiCtrl*> tags_list;
	for(int i = 0; i < 40; ++i)
	{
		IGuiCtrl* tag = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiLVItem"), GET_CTL(cloud)->GetGC()));
		tag->SetState(_T("thr_sta"), (void*)true);
		CText txt[8];
		txt[0].SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
		LOGFONT lf = {0};
		txt[0].GetLogFont(&lf);
		lf.lfHeight -= i;
		txt[0].Create(&lf);
		txt[0].SetColor(ExRGBA(160, 160, 160, 255));
		txt[0].SetString(_T("���Ա�ǩ"));
		lf.lfUnderline = true;
		txt[1].Create(&lf);
		txt[1].SetColor(ExRGBA(101, 177, 177, 255));
		txt[1].SetString(_T("���Ա�ǩ"));
		txt[2].Create(&lf);
		txt[2].SetColor(ExRGBA(0, 160, 160, 255));
		txt[2].SetString(_T("���Ա�ǩ"));
		CSize sz;
		txt[0].GetSize(sz);
		tag->SetState(_T("text"), txt);
		tag->SetWindowRect(CRect(0, 0, sz.cx + 8, sz.cy + 8));
		tags_list.Add(tag);
	}
	GET_CTL(cloud)->SetState(_T("items"), &tags_list);
	GET_CTL(cloud)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_cloud"), &gc)));
	// ��ǩ�ƹ�����
	REG_CTL(scr_cloud, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiScroll"), &gc)));
	ExDynCast<IGuiCtrl>(GET_CTL(scr_cloud)->GetState(_T("up")))->SetWindowRect(CRect());
	ExDynCast<IGuiCtrl>(GET_CTL(scr_cloud)->GetState(_T("down")))->SetWindowRect(CRect());
	GET_CTL(scr_cloud)->SetState(_T("sli_blk_thr_sta"), (void*)true);
	GET_CTL(scr_cloud)->SetState(_T("sli_ori"), (void*)true);
	GET_CTL(scr_cloud)->SetState(_T("sli_color"), (void*)ExRGBA(220, 220, 220, 255));
	pixel_t pix[8] = {0};
	for(int i = 0; i < _countof(pix); ++i) pix[i] = ExRGBA(180, 180, 180, 255);
	GET_CTL(scr_cloud)->SetState(_T("sli_blk_color"), pix);
	GET_CTL(scr_cloud)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_scr_cloud"), &gc)));
	// ����������
	GET_CTL(cloud)->SetScroll(GET_CTL(scr_cloud));

	// Ԥ��ȡ��Դ
	CImage img_btn[9];
	img_btn[0] = GET_IMG(file_lt)->Get();
	img_btn[1] = GET_IMG(file_t)->Get();
	img_btn[2] = GET_IMG(file_rt)->Get();
	img_btn[3] = GET_IMG(file_l)->Get();
	img_btn[4] = GET_IMG(file_c)->Get();
	img_btn[5] = GET_IMG(file_r)->Get();
	img_btn[6] = GET_IMG(file_lb)->Get();
	img_btn[7] = GET_IMG(file_b)->Get();
	img_btn[8] = GET_IMG(file_rb)->Get();
	CImage img_foc[9];
	img_foc[0] = GET_IMG(foc_lt)->Get();
	img_foc[1] = GET_IMG(foc_t)->Get();
	img_foc[2] = GET_IMG(foc_rt)->Get();
	img_foc[3] = GET_IMG(foc_l)->Get();
	//img_foc[4] = GET_IMG(foc_c)->Get();
	img_foc[5] = GET_IMG(foc_r)->Get();
	img_foc[6] = GET_IMG(foc_lb)->Get();
	img_foc[7] = GET_IMG(foc_b)->Get();
	img_foc[8] = GET_IMG(foc_rb)->Get();
	// �ļ��б�
	REG_CTL(files, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiListView"), &gc)));
	GET_CTL(files)->SetTrust(true);
	GET_CTL(files)->SetVisible(false);
	GET_CTL(files)->SetState(_T("align_top"), (void*)false);
	GET_CTL(files)->SetState(_T("space"), (void*)4);
	GET_CTL(files)->SetState(_T("color"), (void*)ExRGBA(0, 0, 0, 0));
	GET_CTL(files)->SetState(_T("foc_image"), img_foc);
	// �����ļ�
	CListT<IGuiCtrl*> file_list;
	for(int i = 0; i < 40; ++i)
	{
		IGuiCtrl* file = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiLVItem"), GET_CTL(files)->GetGC()));
		file->SetState(_T("image"), img_btn);
		file->SetState(_T("locate"), (void*)2);
		file->SetState(_T("loc_off"), (void*)18);
		file->SetState(_T("ico_off"), (void*)8);
		file->SetState(_T("shake_ico"), (void*)1);
		CText txt[8];
		txt[0].SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
		txt[0].SetColor(ExRGBA(0, 0, 0, 255));
		txt[0].SetString(_T("�����ļ�"));
		for (int n = 1; n < _countof(txt); ++n) { txt[n] = txt[0]; }
		file->SetState(_T("text"), txt);
		file->SetWindowRect(CRect(0, 0, 80, 90));
		file_list.Add(file);
	}
	GET_CTL(files)->SetState(_T("items"), &file_list);
	GET_CTL(files)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_files"), &gc)));
	// �ļ��б������
	REG_CTL(scr_files, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiScroll"), &gc)));
	GET_CTL(scr_files)->SetVisible(false);
	ExDynCast<IGuiCtrl>(GET_CTL(scr_files)->GetState(_T("up")))->SetWindowRect(CRect());
	ExDynCast<IGuiCtrl>(GET_CTL(scr_files)->GetState(_T("down")))->SetWindowRect(CRect());
	GET_CTL(scr_files)->SetState(_T("sli_blk_thr_sta"), (void*)true);
	GET_CTL(scr_files)->SetState(_T("sli_ori"), (void*)true);
	GET_CTL(scr_files)->SetState(_T("sli_color"), (void*)ExRGBA(220, 220, 220, 255));
	GET_CTL(scr_files)->SetState(_T("sli_blk_color"), pix);
	GET_CTL(scr_files)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_scr_files"), &gc)));
	// ����������
	GET_CTL(files)->SetScroll(GET_CTL(scr_files));
}

// ���ش���
void CGuiLoader::LoadWnd()
{
	// ������Դ����
	CRect rc_wnd(0, 0, GUI()->DefSize().cx, GUI()->DefSize().cy);
	HICON ic_wnd = ::LoadIcon(GBL()->AppInst(), MAKEINTRESOURCE(IDI_TAGGER));

	// �������ڶ�������
	REG_WND(main, ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc)));
	GET_WND(main)->Create(_T("Tagger"), rc_wnd, SW_HIDE, 
		/*WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX*/WS_OVERLAPPEDWINDOW);
	GET_WND(main)->SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)ic_wnd);
	GET_WND(main)->SendMessage(WM_SETICON, (WPARAM)FALSE, (LPARAM)ic_wnd);
	GET_WND(main)->CenterWindow();
	GET_WND(main)->SetLayered(false, false);
	GET_WND(main)->GetClientRect(rc_wnd);

	// �¼�����
	GET_WND(main)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_wnd"), &gc)));
}

// ��������
void CGuiLoader::LinkGui()
{
	// ��ʾ������
	GET_WND(main)->ShowWindow(SW_SHOW);

	// ���ݴ��
	GET_WND(main)->AddComp(GET_CTL(ground));
	// ��������
	GET_WND(main)->AddComp(GET_CTL(bottom1));
	GET_WND(main)->AddComp(GET_CTL(bottom2));
	// ������
	GET_WND(main)->AddComp(GET_CTL(search));
	// ���ǿ�
	GET_WND(main)->AddComp(GET_CTL(cover));

	// ��ǩ��
	GET_WND(main)->AddComp(GET_CTL(cloud));
	GET_WND(main)->AddComp(GET_CTL(scr_cloud));

	// �ļ��б�
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
		g_instance = ExMem::Alloc<CGuiLoader>();
	return g_instance;
}

void CGuiLoader::Init()
{
	CRect rc_dsk;
	::GetClientRect(::GetDesktopWindow(), (LPRECT)&rc_dsk);
	m_DefSize.cx = (LONG)(rc_dsk.Width() * 0.6);
	m_DefSize.cy = (LONG)(rc_dsk.Height() * 0.6);

	LoadRes();
	LoadCtl();
	LoadWnd();
	LinkGui();
}

void CGuiLoader::Term()
{
	ExMem::Free(this);
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

// ���»�ȡ����
void CGuiLoader::Refresh()
{
}