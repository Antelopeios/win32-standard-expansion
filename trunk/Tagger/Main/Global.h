#pragma once

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

#define GLB() CGlobal::Instance()