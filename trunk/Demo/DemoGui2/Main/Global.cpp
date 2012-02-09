#include "stdafx.h"
#include "Tagger.h"

//////////////////////////////////////////////////////////////////

CGlobal g_Global;

//////////////////////////////////////////////////////////////////

DWORD CGlobal::OnInit()
{
	DWORD ret = IApp::OnInit();

	TAG()->Init();

	return ret;
}

DWORD CGlobal::OnExit(DWORD nCode)
{
	TAG()->Term();
	return nCode;
}