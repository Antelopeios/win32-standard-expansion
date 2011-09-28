#pragma once

#include "Resource/resource.h"
#include "GUI/GuiManager.h"
#include "GUI/GuiLoader.h"
#include "Data/Data.h"

//////////////////////////////////////////////////////////////////

class CGlobal
{
protected:
	HINSTANCE m_AppInst;
	CString m_AppPath;
	CString m_AppName;

public:
	CGlobal();

	static CGlobal* Instance();

public:
	void Init();
	void Term();

	HINSTANCE AppInst();
	CString AppPath();
	CString AppName();

	HGLOBAL GetBinary(UINT nID, LPCTSTR szType, BYTE*& btBuff, DWORD& dwSize, HMODULE hInstance = NULL);
	BOOL ReleaseBinary(HGLOBAL hData);
};

//////////////////////////////////////////////////////////////////

#define GBL() CGlobal::Instance()