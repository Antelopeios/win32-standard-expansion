// TestGui11.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestGui11.h"

//////////////////////////////////////////////////////////////////
/*
	�Զ���ؼ�ʾ��
	�Զ���һ��֧����ʾ3�����ֵİ�ť
	��1,2�����ֿ������;��3�����ֿ��Ҷ���
*/
//////////////////////////////////////////////////////////////////

class CApp : public IApp
{
protected:
	CGC gc;

protected:
	DWORD OnInit()
	{
		DWORD ret = IApp::OnInit();

		// ������
		CIOFile file;
		ICoderObject* coder = CImgAnalyzer::GetCoder(CImgAnalyzer::png);
		if (!coder) return ret;
		coder->SetFile(&file);
		// ͼƬ��Դ
		file.Open(_T("TestGui3/icon.png"));
		ExReg<CImage>(_T("icon"), gcnew(gc, CImage));
		ExGet<CImage>(_T("icon"))->Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_cap_left.png"));
		ExReg<CImage>(_T("btn_bg_cap_left"), gcnew(gc, CImage));
		ExGet<CImage>(_T("btn_bg_cap_left"))->Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_cap.png"));
		ExReg<CImage>(_T("btn_bg_cap"), gcnew(gc, CImage));
		ExGet<CImage>(_T("btn_bg_cap"))->Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_cap_right.png"));
		ExReg<CImage>(_T("btn_bg_cap_right"), gcnew(gc, CImage));
		ExGet<CImage>(_T("btn_bg_cap_right"))->Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_left.png"));
		ExReg<CImage>(_T("btn_bg_left"), gcnew(gc, CImage));
		ExGet<CImage>(_T("btn_bg_left"))->Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg.png"));
		ExReg<CImage>(_T("btn_bg"), gcnew(gc, CImage));
		ExGet<CImage>(_T("btn_bg"))->Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_right.png"));
		ExReg<CImage>(_T("btn_bg_right"), gcnew(gc, CImage));
		ExGet<CImage>(_T("btn_bg_right"))->Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_bottom_left.png"));
		ExReg<CImage>(_T("btn_bg_bottom_left"), gcnew(gc, CImage));
		ExGet<CImage>(_T("btn_bg_bottom_left"))->Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_bottom.png"));
		ExReg<CImage>(_T("btn_bg_bottom"), gcnew(gc, CImage));
		ExGet<CImage>(_T("btn_bg_bottom"))->Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_bottom_right.png"));
		ExReg<CImage>(_T("btn_bg_bottom_right"), gcnew(gc, CImage));
		ExGet<CImage>(_T("btn_bg_bottom_right"))->Set(coder->Decode());
		// ������Դ
		ExReg<CText>(_T("font"), gcnew(gc, CText));
		ExGet<CText>(_T("font"))->SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));

		// �Զ��尴ť
		IGuiCtl* btn = ExReg<IGuiCtl>(_T("btn"), ExGui(_T("CCusButton")));
		btn->SetState(_T("icon"), ExGet<CImage>(_T("icon")));
		btn->Execute(_T("image"), _T("\
			btn_bg_cap_left,\
			btn_bg_cap,\
			btn_bg_cap_right,\
			btn_bg_left,\
			btn_bg,\
			btn_bg_right,\
			btn_bg_bottom_left,\
			btn_bg_bottom,\
			btn_bg_bottom_right"));
		btn->Execute(_T("font"), 
			_T("font,font,font,font,font,font,font,font"));
		btn->SetState(_T("text"), &CString(_T("���ǵ�һ������ ���ǵ�һ������ ���ǵ�һ������")));
		btn->SetState(_T("text2"), &CString(_T("���ǵڶ������� ���ǵڶ������� ���ǵڶ�������")));
		btn->SetState(_T("text3"), &CString(_T("���ǵ��������� ���ǵ��������� ���ǵ���������")));
		btn->AddEvent(dbnew(CBtnEvent));

		// ������
		ExReg<IGuiWnd>(_T("main"), ExGui(_T("CGuiWnd"), &gc));
		ExGet<IGuiWnd>(_T("main"))->Create(_T("TestGui11"), CRect(0, 0, 600, 400), SW_HIDE, WS_OVERLAPPEDWINDOW);
		ExGet<IGuiWnd>(_T("main"))->CenterWindow();
		ExGet<IGuiWnd>(_T("main"))->AddComp(ExGet<IGuiCtl>(_T("btn")));
		ExGet<IGuiWnd>(_T("main"))->AddEvent(ExGui(_T("CQuitEvent")));
		ExGet<IGuiWnd>(_T("main"))->AddEvent(dbnew(CWndEvent));
		ExGet<IGuiWnd>(_T("main"))->ShowWindow(SW_SHOW);

		coder->Free();
		return ret;
	}
} g_App;