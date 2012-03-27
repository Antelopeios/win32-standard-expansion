// TestGui10.cpp : 定义应用程序的入口点。
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

		CPrgEvent* prg_evt = NULL;
		ExReg<IGuiCtl>(_T("prog1"), ExGui(_T("CGuiProgress")));
		ExGet<IGuiCtl>(_T("prog1"))->AddEvent((prg_evt = dbnew(CPrgEvent), prg_evt->m_Off = 2, prg_evt));
		ExReg<IGuiCtl>(_T("prog2"), ExGui(_T("CGuiProgress")));
		ExGet<IGuiCtl>(_T("prog2"))->AddEvent((prg_evt = dbnew(CPrgEvent), prg_evt->m_Off = 1, prg_evt));
		ExReg<IGuiCtl>(_T("prog3"), ExGui(_T("CGuiProgress")));
		ExGet<IGuiCtl>(_T("prog3"))->AddEvent((prg_evt = dbnew(CPrgEvent), prg_evt->m_Off = 3, prg_evt));

		ExReg<IGuiWnd>(_T("main"), ExGui(_T("CGuiWnd"), &gc));
		ExGet<IGuiWnd>(_T("main"))->Create(_T("TestGui10"), CRect(0, 0, 600, 300), SW_HIDE, WS_OVERLAPPEDWINDOW);
		ExGet<IGuiWnd>(_T("main"))->CenterWindow();
		ExGet<IGuiWnd>(_T("main"))->AddComp(ExGet<IGuiCtl>(_T("prog1")));
		ExGet<IGuiWnd>(_T("main"))->AddComp(ExGet<IGuiCtl>(_T("prog2")));
		ExGet<IGuiWnd>(_T("main"))->AddComp(ExGet<IGuiCtl>(_T("prog3")));
		ExGet<IGuiWnd>(_T("main"))->AddEvent(ExGui(_T("CQuitEvent")));
		ExGet<IGuiWnd>(_T("main"))->AddEvent(dbnew(CWndEvent));
		ExGet<IGuiWnd>(_T("main"))->ShowWindow(SW_SHOW);

		return ret;
	}
} g_App;