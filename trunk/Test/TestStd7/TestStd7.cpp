// TestStd7.cpp : 定义控制台应用程序的入口点。
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