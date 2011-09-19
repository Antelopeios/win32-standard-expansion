#pragma once

#include "Resource/resource.h"
#include "Manager/GuiManager.h"
#include "Data/Data.h"

//////////////////////////////////////////////////////////////////

class CGlobal
{
protected:
	HINSTANCE m_AppInst;
	CString m_AppPath;
	CString m_AppName;
	CSize m_DefSize;

public:
	CGlobal();

	static CGlobal* Instance();

public:
	void Init();
	HINSTANCE GetAppInst();
	CString GetAppPath();
	CString GetAppName();
	CSize GetDefSize();
};

//////////////////////////////////////////////////////////////////

#define GET_GBL() CGlobal::Instance()