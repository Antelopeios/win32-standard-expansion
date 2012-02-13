#include "stdafx.h"

//////////////////////////////////////////////////////////////////

class CGuiLoader : public IApp
{
protected:
	CGC gc;

protected:
	void LoadGui()
	{
		CGuiSkin::Load(CString(GetPath()) + _T("ui.ui"));
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
		LoadGui();
		LinkGui();
	}
} g_GuiLoader;