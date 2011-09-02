#include "stdafx.h"
#include "Main/Tagger.h"

//////////////////////////////////////////////////////////////////

class CGuiLoader
{
protected:
	CGC& gc;

protected:
	// ������Դ
	void LoadRes()
	{
		CText* txt = ExMem::Alloc<CText>(&gc);
		txt->SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
		REG_TXT(search, txt);

		txt = ExMem::Alloc<CText>(&gc);
		txt->SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
		txt->SetColor(ExRGBA(0, 0, 0, 100));
		REG_TXT(cover, txt);
	}

	// ���ؿؼ�
	void LoadCtl()
	{
		// ���ݴ��
		REG_CTL(ground, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc)));
		GUI_CTL(ground)->SetState(_T("color"), (void*)ExRGBA(242, 242, 242, 255));
		GUI_CTL(ground)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_ground"), &gc)));

		// ��������
		REG_CTL(bottom1, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc)));
		GUI_CTL(bottom1)->SetState(_T("color"), (void*)ExRGBA(222, 222, 222, 255));
		GUI_CTL(bottom1)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_bottom1"), &gc)));
		REG_CTL(bottom2, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc)));
		GUI_CTL(bottom2)->SetState(_T("color"), (void*)ExRGBA(255, 255, 255, 255));
		GUI_CTL(bottom2)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_bottom2"), &gc)));
		// ������
		REG_CTL(search, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiEdit"), &gc)));
		GUI_CTL(search)->SetState(_T("text"), GUI_TXT(search));
		GUI_CTL(search)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_search"), &gc)));
		// ���ǿ�
		REG_CTL(cover, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiEdit"), &gc)));
		GUI_CTL(cover)->SetState(_T("text"), GUI_TXT(cover));
		GUI_CTL(cover)->SetState(_T("edit"), &CString(_T("���������ѯ��ǩ")));
		GUI_CTL(cover)->SetVisible(false);
		GUI_CTL(cover)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_cover"), &gc)));

		// ��ǩ��
		REG_CTL(cloud, ExDynCast<IGuiCtrl>(ExGui(_T("CGuiListView"), &gc)));
		GUI_CTL(cloud)->SetState(_T("align_top"), (void*)false);
		GUI_CTL(cloud)->SetState(_T("color"), (void*)ExRGBA(0, 0, 0, 0));
		// ���ر�ǩ
		CListT<IGuiCtrl*> tags_list;
		for(int i = 0; i < 40; ++i)
		{
			IGuiCtrl* tag = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiLVItem"), &gc));
			tag->SetState(_T("thr_sta"), (void*)true);

			CText txt[5];
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
		GUI_CTL(cloud)->SetState(_T("items"), &tags_list);
		GUI_CTL(cloud)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_cloud"), &gc)));
	}

	// ���ش���
	void LoadWnd()
	{
		// ������Դ����
		CRect rc_dsk;
		::GetClientRect(::GetDesktopWindow(), (LPRECT)&rc_dsk);
		CRect rc_wnd(0, 0, (LONG)(rc_dsk.Width() * 0.6), (LONG)(rc_dsk.Height() * 0.6));
		HICON ic_wnd = ::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_TAGGER));

		// �������ڶ�������
		REG_WND(main, ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc)));
		GUI_WND(main)->Create(_T("Tagger"), rc_wnd, SW_HIDE, 
			/*WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX*/WS_OVERLAPPEDWINDOW);
		GUI_WND(main)->SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)ic_wnd);
		GUI_WND(main)->SendMessage(WM_SETICON, (WPARAM)FALSE, (LPARAM)ic_wnd);
		GUI_WND(main)->CenterWindow();
		GUI_WND(main)->SetLayered(false, false);
		GUI_WND(main)->GetClientRect(rc_wnd);

		// �¼�����
		GUI_WND(main)->AddEvent(ExDynCast<IGuiEvent>(ExGui(_T("CEvent_wnd"), &gc)));
	}

	// ��������
	void LinkGui()
	{
		// ��ʾ������
		GUI_WND(main)->ShowWindow(SW_SHOW);

		// ���ݴ��
		GUI_WND(main)->AddComp(GUI_CTL(ground));
		// ��������
		GUI_WND(main)->AddComp(GUI_CTL(bottom1));
		GUI_WND(main)->AddComp(GUI_CTL(bottom2));
		// ������
		GUI_WND(main)->AddComp(GUI_CTL(search));
		// ���ǿ�
		GUI_WND(main)->AddComp(GUI_CTL(cover));

		// ��ǩ��
		GUI_WND(main)->AddComp(GUI_CTL(cloud));
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