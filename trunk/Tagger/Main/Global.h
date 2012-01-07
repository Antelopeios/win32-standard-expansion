#pragma once

//////////////////////////////////////////////////////////////////

class CGlobal : public IApp
{
protected:
	DWORD OnInit();
	DWORD OnExit(DWORD nCode);

public:
	static HGLOBAL GetBinary(UINT nID, LPCTSTR szType, BYTE*& btBuff, DWORD& dwSize, HMODULE hInstance = NULL);
	static BOOL ReleaseBinary(HGLOBAL hData);
};