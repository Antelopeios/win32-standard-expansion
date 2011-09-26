// Tagger.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Tagger.h"

//////////////////////////////////////////////////////////////////

CGlobal::CGlobal()
	: m_AppInst(NULL)
{
}

CGlobal* CGlobal::Instance()
{
	static CGlobal glb;
	return &glb;
}

void CGlobal::Init()
{
	m_AppInst = ::GetModuleHandle(NULL);

	::GetModuleFileName(NULL, m_AppPath.GetCStr(MAX_PATH), MAX_PATH);
	int inx = (m_AppPath.RevFind(_T('\\')) + 1)->Index();
	m_AppName = ((LPCTSTR)m_AppPath) + inx;
	m_AppPath[inx] = _T('\0');

	CRect rc_dsk;
	::GetClientRect(::GetDesktopWindow(), (LPRECT)&rc_dsk);
	m_DefSize.cx = (LONG)(rc_dsk.Width() * 0.6);
	m_DefSize.cy = (LONG)(rc_dsk.Height() * 0.6);
}

HINSTANCE CGlobal::GetAppInst()
{
	return m_AppInst;
}

CString CGlobal::GetAppPath()
{
	return m_AppPath;
}

CString CGlobal::GetAppName()
{
	return m_AppName;
}

CSize CGlobal::GetDefSize()
{
	return m_DefSize;
}

int CGlobal::GetGuiScrW()
{
	return 10;
}

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

//////////////////////////////////////////////////////////////////

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 主消息循环
	MSG msg = {0};
	while (::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return (int) msg.wParam;
}
