// TestGui8.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestGui8.h"

class CApp : public IApp
{
protected:
	CGC gc;

protected:
	DWORD OnInit()
	{
		DWORD ret = IApp::OnInit();

		// 文字资源
		ExReg<CText>(_T("font"), gcnew(gc, CText));
		ExGet<CText>(_T("font"))->SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));

		ExReg<IGuiCtl>(_T("scrl"), ExGui(_T("CGuiScroll")));
		ExGet<IGuiCtl>(_T("scrl"))->SetVisible(FALSE);
		ExDynCast<IGuiCtl>(ExGet<IGuiCtl>(_T("scrl"))->GetState(_T("up")))->SetWindowRect(CRect());
		ExDynCast<IGuiCtl>(ExGet<IGuiCtl>(_T("scrl"))->GetState(_T("down")))->SetWindowRect(CRect());
		ExGet<IGuiCtl>(_T("scrl"))->SetState(_T("sli_blk_thr_sta"), (void*)TRUE);
		ExGet<IGuiCtl>(_T("scrl"))->SetState(_T("sli_ori"), (void*)TRUE);
		ExGet<IGuiCtl>(_T("scrl"))->SetState(_T("sli_color"), (void*)ExRGBA(220, 220, 220, 255));
		pixel_t pix[8] = {0};
		for(int i = 0; i < _countof(pix); ++i) pix[i] = ExRGBA(180, 180, 180, 255);
		ExGet<IGuiCtl>(_T("scrl"))->SetState(_T("sli_blk_color"), pix);
		ExGet<IGuiCtl>(_T("scrl"))->AddEvent(dbnew(CScrEvent));

		CString str;
		for(int i = 0; i < 100; ++i)
		{
			CString tmp;
			tmp.Format(_T("这是一行文字, 文字测试第%d行\n"), i + 1);
			str += tmp;
		}
		ExReg<IGuiCtl>(_T("text"), ExGui(_T("CGuiText")));
		ExGet<IGuiCtl>(_T("text"))->SetState(_T("font"), (void*)ExGet<CText>(_T("font")));
		ExGet<IGuiCtl>(_T("text"))->SetState(_T("text"), (void*)&str);
		ExGet<IGuiCtl>(_T("text"))->AddEvent(dbnew(CTxtEvent));
		ExGet<IGuiCtl>(_T("text"))->SetScroll(ExGet<IGuiCtl>(_T("scrl")));

		ExReg<IGuiWnd>(_T("main"), ExGui(_T("CGuiWnd"), &gc));
		ExGet<IGuiWnd>(_T("main"))->Create(_T("TestGui8"), CRect(0, 0, 600, 400), SW_HIDE, WS_OVERLAPPEDWINDOW);
		ExGet<IGuiWnd>(_T("main"))->CenterWindow();
		ExGet<IGuiWnd>(_T("main"))->AddComp(ExGet<IGuiCtl>(_T("scrl")));
		ExGet<IGuiWnd>(_T("main"))->AddComp(ExGet<IGuiCtl>(_T("text")));
		ExGet<IGuiWnd>(_T("main"))->AddEvent(ExGui(_T("CQuitEvent")));
		ExGet<IGuiWnd>(_T("main"))->AddEvent(dbnew(CWndEvent));
		ExGet<IGuiWnd>(_T("main"))->ShowWindow(SW_SHOW);

		return ret;
	}
} g_App;