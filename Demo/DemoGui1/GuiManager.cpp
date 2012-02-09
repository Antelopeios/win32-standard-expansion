#include "stdafx.h"

//////////////////////////////////////////////////////////////////

class CGuiLoader : public IApp
{
protected:
	CGC gc;

protected:
	void LoadRes()
	{
		CIOFile file(CString(GetPath()) + _T("ui\\ui_res.xml"));
		if (file.Error() || file.Size() == 0) return;
		CGuiSkin::Load(&file);
	}
	void LoadSty()
	{
		CIOFile file(CString(GetPath()) + _T("ui\\ui_sty.xml"));
		if (file.Error() || file.Size() == 0) return;
		CGuiSkin::Load(&file);
	}
	void LoadDiv()
	{
		CIOFile file(CString(GetPath()) + _T("ui\\ui_div.xml"));
		if (file.Error() || file.Size() == 0) return;
		CGuiSkin::Load(&file);
	}
	void LinkGui()
	{
		// ÏÔÊ¾Ö÷´°¿Ú
		if (GUI_WND(main))
			GUI_WND(main)->ShowWindow(SW_SHOW);
	}

public:
	CGuiLoader()
	{
		LoadRes();
		LoadSty();
		LoadDiv();
		LinkGui();
	}
} g_GuiLoader;