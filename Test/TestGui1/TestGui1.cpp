// TestGui1.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestGui1.h"

//////////////////////////////////////////////////////////////////

class CApp : public IApp
{
protected:
	CGC gc;
	CImage img_pic, img_btn[9];
	CText txt_btn[8];

protected:
	DWORD OnInit()
	{
		DWORD ret = IApp::OnInit();

		// 加载资源
		CIOFile file(_T("TestImg1/ground.png"));
		// 解码器
		ICoderObject* coder = CImgAnalyzer::GetCoder(&file, &gc);
		if (!coder) return 0;
		// 图片资源
		img_pic.Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_cap_left.png"));
		img_btn[0].Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_cap.png"));
		img_btn[1].Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_cap_right.png"));
		img_btn[2].Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_left.png"));
		img_btn[3].Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg.png"));
		img_btn[4].Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_right.png"));
		img_btn[5].Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_bottom_left.png"));
		img_btn[6].Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_bottom.png"));
		img_btn[7].Set(coder->Decode());
		file.Open(_T("TestGui1/btn_bg_bottom_right.png"));
		img_btn[8].Set(coder->Decode());
		// 文字资源
		txt_btn[0].SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
		txt_btn[0].SetColor(ExRGBA(0, 60, 116, 200));
		for(int i = 1; i < _countof(txt_btn); ++i) txt_btn[i] = txt_btn[0];

		// 创建按钮控件对象并设置
		IGuiCtl* btn = ExGui<IGuiCtl>(_T("CGuiButton"));
		btn->SetState(_T("image"), img_btn);
		btn->SetState(_T("font"), txt_btn);
		btn->SetState(_T("text"), &CString(_T("Dark C.at")));
		//btn->SetState(_T("locate"), (void*)4);
		//btn->SetState(_T("loc_off"), (void*)10);
		btn->SetWindowRect(CRect(70, 100, 180, 140));
		//btn->SetState(_T("comf"), (void*)TRUE);
		//btn->SetEffect(ExGui(_T("CGuiFade")));

		// 创建图片控件对象并设置
		IGuiCtl* pic = ExGui<IGuiCtl>(_T("CGuiPicture"));
		pic->SetState(_T("image"), &img_pic);
		CRect rect(0, 0, img_pic.GetWidth(), img_pic.GetHeight());
		pic->SetWindowRect(rect);
		pic->SetEffect(ExGui(_T("CGuiFade")));

		// 创建窗口对象并设置
		IGuiWnd* wnd = ExGui<IGuiWnd>(_T("CGuiWnd"), &gc);
		wnd->Create(_T("Test"), /*CRect(0, 0, 500, 300)*/rect, SW_HIDE);
		wnd->CenterWindow();
		wnd->SetLayered();
		wnd->ShowWindow(SW_SHOW);

		// 将窗口与控件及事件对象关联
		pic->AddEvent(dbnew(CCusPicEvent));
		btn->AddEvent(dbnew(CCusBtnEvent));
		wnd->AddComp(pic);
		pic->AddComp(btn);
		wnd->AddEvent(dbnew(CCustomEvent));

		return ret;
	}
} g_App;