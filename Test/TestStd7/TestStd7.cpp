// TestStd7.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

class CApp : public IApp
{
public:
	CApp(BOOL bUIApp = TRUE)
		: IApp(bUIApp)
	{}

protected:
	DWORD OnInit()
	{
		DWORD ret = IApp::OnInit();

		ExCPrintf(_T("Hello World~\n"));
		ExCPrintf(_T("%s\n"), GetFull());

		_tsystem(_T("pause"));
		PostQuitMessage(0);

		return ret;
	}

	DWORD OnExit(DWORD nCode)
	{
		ExCPrintf(_T("Bye~\n"));
		return nCode;
	}
} g_App;