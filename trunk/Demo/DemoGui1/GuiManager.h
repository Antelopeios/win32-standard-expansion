#pragma once

//////////////////////////////////////////////////////////////////

class CResManager
{
public:
	static CImage banner;
	static CImage corner_lb;
	static CImage corner_rb;
	static CImage corner_rt;
	static CImage corner_lt;
	static CImage line_bottom;
	static CImage line_left;
	static CImage line_right;
	static CImage line_top;
	static CImage tag_bg;
	static CImage toolbar_bg;
	static CImage list;

	static CImage win_sysbtn_close;
	static CImage win_sysbtn_max;
	static CImage win_sysbtn_min;
	static CImage win_sysbtn_restore;

	static CImage topbar_btn;
	static CImage gamesearch_charmap;
	static CImage toolbar_tools;

	static CImage tag_qb;
	static CImage tag_zx;
	static CImage tag_wl;
	static CImage tag_dz;
	static CImage tag_wy;
	static CImage tag_dj;

	static CImage search_bg;
	static CImage search_button;
	static CImage google_bg;
	static CImage google_button;
};

//////////////////////////////////////////////////////////////////

class CCtlManager
{
private:
	typedef CMapT<CString, IGuiCtrl*> key_map_t;
	static key_map_t m_Ctrls;

public:
	static bool RegCtrl(LPCTSTR c_key, IGuiCtrl* inf)
	{
		if (!c_key) return false;
		CString key(c_key);
		if (m_Ctrls.Locate(key) == m_Ctrls.Tail())
		{
			m_Ctrls.Add(key, inf);
			return true;
		}
		else
			return false;
	}
	static IGuiCtrl* GetCtrl(LPCTSTR c_key)
	{
		if (!c_key) return false;
		CString key(c_key);
		key_map_t::iterator_t ite = m_Ctrls.Locate(key);
		if (ite == m_Ctrls.Tail())
			return NULL;
		else
			return ite->Val();
	}
};

#define REG_CTRL(name, inf) CCtlManager::RegCtrl(_T(#name), inf)
#define GUI_CTRL(name) CCtlManager::GetCtrl(_T(#name))

//////////////////////////////////////////////////////////////////

class CGuiLoader
{
protected:
	CGC& gc;

protected:
	void LoadRes();
	void LoadCtl();

public:
	CGuiLoader();
};