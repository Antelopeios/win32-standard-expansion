// TestGui7.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestGui7.h"

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

		// 滚动条
		ExReg<IGuiCtl>(_T("scrl"), ExGui(_T("CGuiScroll")));
		ExGet<IGuiCtl>(_T("scrl"))->SetVisible(FALSE);
		ExDynCast<IGuiCtl>(ExGet<IGuiCtl>(_T("scrl"))->GetState(_T("up")))->SetWindowRect(CRect());
		ExDynCast<IGuiCtl>(ExGet<IGuiCtl>(_T("scrl"))->GetState(_T("down")))->SetWindowRect(CRect());
		ExGet<IGuiCtl>(_T("scrl"))->SetState(_T("sli_blk_thr_sta"), (void*)TRUE);
		ExGet<IGuiCtl>(_T("scrl"))->SetState(_T("sli_ori"), (void*)TRUE);
		ExGet<IGuiCtl>(_T("scrl"))->AddEvent(dbnew(CScrEvent));

		// 列表框
		ExReg<IGuiCtl>(_T("list"), ExGui(_T("CGuiList")));
		IGuiCtl::items_t items;
		for(int i = 0; i < 10; ++i)
		{
			IGuiCtl* btn = ExGui<IGuiCtl>(_T("CGuiListItem"));
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
			btn->SetState(_T("text"), &CString(_T("Dark C.at Dark C.at Dark C.at")));
			btn->SetState(_T("locate"), (void*)3);
			btn->SetState(_T("loc_off"), (void*)5);
			btn->SetState(_T("ico_off"), (void*)5);
			btn->SetState(_T("shake_ico"), (void*)TRUE);
			btn->SetWindowRect(CRect(0, 0, 0, 70));
			items.Add(btn);
		}
		ExGet<IGuiCtl>(_T("list"))->SetState(_T("items"), &items);
		ExGet<IGuiCtl>(_T("list"))->SetState(_T("space"), (void*)5);
		ExGet<IGuiCtl>(_T("list"))->SetScroll(ExGet<IGuiCtl>(_T("scrl")));
		ExGet<IGuiCtl>(_T("list"))->AddEvent(ExGui(_T("CScrollEvent")));

		// 主窗口
		ExReg<IGuiWnd>(_T("main"), ExGui(_T("CGuiWnd"), &gc));
		ExGet<IGuiWnd>(_T("main"))->Create(_T("TestGui7"), CRect(0, 0, 800, 500), SW_HIDE, WS_OVERLAPPEDWINDOW);
		ExGet<IGuiWnd>(_T("main"))->CenterWindow();
		ExGet<IGuiWnd>(_T("main"))->AddComp(ExGet<IGuiCtl>(_T("scrl")));
		ExGet<IGuiWnd>(_T("main"))->AddComp(ExGet<IGuiCtl>(_T("list")));
		ExGet<IGuiWnd>(_T("main"))->AddEvent(ExGui(_T("CQuitEvent")));
		ExGet<IGuiWnd>(_T("main"))->AddEvent(dbnew(CWndEvent));
		ExGet<IGuiWnd>(_T("main"))->ShowWindow(SW_SHOW);

		coder->Free();
		return ret;
	}
} g_App;