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

//////////////////////////////////////////////////////////////////

HGLOBAL CGlobal::GetBinary(UINT nID, LPCTSTR szType, BYTE*& btBuff, DWORD& dwSize, HMODULE hInstance/* = NULL*/)
{
	// 加载资源内存
	HRSRC info = ::FindResource(hInstance, MAKEINTRESOURCE(nID), szType);
	if( info == NULL ) return NULL;
	HGLOBAL data = ::LoadResource(hInstance, info);
	if( data == NULL ) return NULL;
	// 获得资源内存块
	dwSize = ::SizeofResource(hInstance, info);
	if( dwSize == NULL )
	{
		ReleaseBinary(data);
		return NULL;
	}
	btBuff = (BYTE*)::LockResource(data);
	if( btBuff == NULL )
	{
		ReleaseBinary(data);
		return NULL;
	}
	return data;
}

BOOL CGlobal::ReleaseBinary(HGLOBAL hData)
{
	if (!hData) return TRUE;
	// 释放资源内存块
	UnlockResource(hData);
	return FreeResource(hData);
}