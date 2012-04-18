// TestGui11.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestGui11.h"

//////////////////////////////////////////////////////////////////
/*
	自定义控件示例
	自定义一个支持显示3排文字的按钮
	第1,2排文字靠左对齐;第3排文字靠右对齐
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

		// 解码器
		CIOFile file;
		ICoderObject* coder = CImgAnalyzer::GetCoder(CImgAnalyzer::png);
		if (!coder) return ret;
		coder->SetFile(&file);
		// 图片资源
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
		// 文字资源
		ExReg<CText>(_T("font"), gcnew(gc, CText));
		ExGet<CText>(_T("font"))->SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));

		// 自定义按钮
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
		btn->SetState(_T("text"), &CString(_T("这是第一排文字 这是第一排文字 这是第一排文字")));
		btn->SetState(_T("text2"), &CString(_T("这是第二排文字 这是第二排文字 这是第二排文字")));
		btn->SetState(_T("text3"), &CString(_T("这是第三排文字 这是第三排文字 这是第三排文字")));
		btn->AddEvent(dbnew(CBtnEvent));

		// 主窗口
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