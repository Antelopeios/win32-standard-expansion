// TestGui10.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestGui10.h"

class CApp : public IApp
{
protected:
	CGC gc;

protected:
	DWORD OnInit()
	{
		DWORD ret = IApp::OnInit();

		ExReg<CText>(_T("font"), gcnew(gc, CText));
		ExGet<CText>(_T("font"))->SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));

		ExReg<IGuiCtl>(_T("scrl_v"), ExGui(_T("CGuiScroll")));
		ExGet<IGuiCtl>(_T("scrl_v"))->SetVisible(FALSE);
		ExDynCast<IGuiCtl>(ExGet<IGuiCtl>(_T("scrl_v"))->GetState(_T("up")))->SetWindowRect(CRect());
		ExDynCast<IGuiCtl>(ExGet<IGuiCtl>(_T("scrl_v"))->GetState(_T("down")))->SetWindowRect(CRect());
		ExGet<IGuiCtl>(_T("scrl_v"))->SetState(_T("sli_blk_thr_sta"), (void*)TRUE);
		ExGet<IGuiCtl>(_T("scrl_v"))->SetState(_T("sli_ori"), (void*)TRUE);
		ExGet<IGuiCtl>(_T("scrl_v"))->AddEvent(dbnew(CScrEventV));

		ExReg<IGuiCtl>(_T("scrl_h"), ExGui(_T("CGuiScroll")));
		ExGet<IGuiCtl>(_T("scrl_h"))->SetVisible(FALSE);
		ExDynCast<IGuiCtl>(ExGet<IGuiCtl>(_T("scrl_h"))->GetState(_T("up")))->SetWindowRect(CRect());
		ExDynCast<IGuiCtl>(ExGet<IGuiCtl>(_T("scrl_h"))->GetState(_T("down")))->SetWindowRect(CRect());
		ExGet<IGuiCtl>(_T("scrl_h"))->SetState(_T("sli_blk_thr_sta"), (void*)TRUE);
		ExGet<IGuiCtl>(_T("scrl_h"))->SetState(_T("sli_ori"), (void*)FALSE);
		ExGet<IGuiCtl>(_T("scrl_h"))->AddEvent(dbnew(CScrEventH));

		ExReg<IGuiCtl>(_T("tree"), ExGui(_T("CGuiTree")));
		ExGet<IGuiCtl>(_T("tree"))->SetState(_T("font"), (void*)ExGet<CText>(_T("font")));
		ExGet<IGuiCtl>(_T("tree"))->AddEvent(dbnew(CTreEvent));
		ExGet<IGuiCtl>(_T("tree"))->SetScroll(ExGet<IGuiCtl>(_T("scrl_v")), TRUE);
		ExGet<IGuiCtl>(_T("tree"))->SetScroll(ExGet<IGuiCtl>(_T("scrl_h")), FALSE);

		ExReg<IGuiWnd>(_T("main"), ExGui(_T("CGuiWnd"), &gc));
		ExGet<IGuiWnd>(_T("main"))->Create(_T("TestGui10"), CRect(0, 0, 600, 400), SW_HIDE, WS_OVERLAPPEDWINDOW);
		ExGet<IGuiWnd>(_T("main"))->CenterWindow();
		ExGet<IGuiWnd>(_T("main"))->AddComp(ExGet<IGuiCtl>(_T("scrl_v")));
		ExGet<IGuiWnd>(_T("main"))->AddComp(ExGet<IGuiCtl>(_T("scrl_h")));
		ExGet<IGuiWnd>(_T("main"))->AddComp(ExGet<IGuiCtl>(_T("tree")));
		ExGet<IGuiWnd>(_T("main"))->AddEvent(ExGui(_T("CQuitEvent")));
		ExGet<IGuiWnd>(_T("main"))->AddEvent(dbnew(CWndEvent));
		ExGet<IGuiWnd>(_T("main"))->ShowWindow(SW_SHOW);

		return ret;
	}
} g_App;