// TestGui6.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestGui6.h"

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

		ExReg<IGuiCtl>(_T("menu_grd"), ExGui(_T("CGuiButton")));
		ExGet<IGuiCtl>(_T("menu_grd"))->Execute(_T("color"), 
			_T("#ffffffff,#ffffffff,#ffffffff,#ffffffff,#ffffffff,#ffffffff,#ffffffff,#ffffffff"));
		ExGet<IGuiCtl>(_T("menu_grd"))->Execute(_T("font"), 
			_T("font,font,font,font,font,font,font,font"));
		ExGet<IGuiCtl>(_T("menu_grd"))->Execute(_T("text"), _T("菜单测试"));
		ExGet<IGuiCtl>(_T("menu_grd"))->SetState(_T("locate"), (void*)3);
		ExGet<IGuiCtl>(_T("menu_grd"))->SetWindowRect(CRect(0, 0, 100, 20));

		ExReg<IGuiWnd>(_T("menu"), ExGui(_T("CGuiMenu"), &gc));
		ExGet<IGuiWnd>(_T("menu"))->Create();
		ExGet<IGuiWnd>(_T("menu"))->AddComp(ExGet<IGuiCtl>(_T("menu_grd")));

		ExReg<IGuiCtl>(_T("menu_btn"), ExGui(_T("CGuiButton")));
		ExGet<IGuiCtl>(_T("menu_btn"))->Execute(_T("font"), 
			_T("font,font,font,font,font,font,font,font"));
		ExGet<IGuiCtl>(_T("menu_btn"))->Execute(_T("text"), _T("菜单测试"));
		ExGet<IGuiCtl>(_T("menu_btn"))->AddEvent(dbnew(CBtnEvent));

		ExReg<IGuiCtl>(_T("menu_bar"), ExGui(_T("CGuiPicture")));
		ExGet<IGuiCtl>(_T("menu_bar"))->SetState(_T("color"), (void*)ExRGBA(244, 247, 252, 255));
		ExGet<IGuiCtl>(_T("menu_bar"))->AddComp(ExGet<IGuiCtl>(_T("menu_btn")));
		ExGet<IGuiCtl>(_T("menu_bar"))->AddEvent(dbnew(CBarEvent));

		ExReg<IGuiCtl>(_T("gund"), ExGui(_T("CGuiPicture")));
		ExGet<IGuiCtl>(_T("gund"))->SetState(_T("color"), (void*)ExRGBA(171, 171, 171, 255));
		ExGet<IGuiCtl>(_T("gund"))->AddComp(ExGet<IGuiCtl>(_T("menu_bar")));
		ExGet<IGuiCtl>(_T("gund"))->AddEvent(dbnew(CPicEvent));

		ExReg<IGuiWnd>(_T("main"), ExGui(_T("CGuiWnd"), &gc));
		ExGet<IGuiWnd>(_T("main"))->Create(_T("TestGui6"), CRect(0, 0, 800, 500), SW_HIDE, WS_OVERLAPPEDWINDOW);
		ExGet<IGuiWnd>(_T("main"))->CenterWindow();
		ExGet<IGuiWnd>(_T("main"))->AddComp(ExGet<IGuiCtl>(_T("gund")));
		ExGet<IGuiWnd>(_T("main"))->AddEvent(ExGui(_T("CQuitEvent")));
		ExGet<IGuiWnd>(_T("main"))->AddEvent(dbnew(CWndEvent));
		ExGet<IGuiWnd>(_T("main"))->ShowWindow(SW_SHOW);

		return ret;
	}
} g_App;