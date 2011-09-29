// Tagger.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Tagger.h"

//////////////////////////////////////////////////////////////////

CGlobal::CGlobal()
	: m_AppInst(NULL)
{
}

CGlobal* g_instance = NULL;
CGlobal* CGlobal::Instance()
{
	if (g_instance == NULL)
		g_instance = ExMem::Alloc<CGlobal>();
	return g_instance;
}

void CGlobal::Init()
{
	m_AppInst = ::GetModuleHandle(NULL);

	::GetModuleFileName(NULL, m_AppPath.GetCStr(MAX_PATH), MAX_PATH);
	int inx = (m_AppPath.RevFind(_T('\\')) + 1)->Index();
	m_AppName = ((LPCTSTR)m_AppPath) + inx;
	m_AppPath[inx] = _T('\0');
}

void CGlobal::Term()
{
	ExMem::Free(this);
	g_instance = NULL;
}

HINSTANCE CGlobal::AppInst()
{
	return m_AppInst;
}

CString CGlobal::AppPath()
{
	return m_AppPath;
}

CString CGlobal::AppName()
{
	return m_AppName;
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

	// 初始化
	GBL()->Init();	// 全局初始化
	DAT()->Init();	// 数据层初始化
	GUI()->Init();	// 界面层初始化

	// 主消息循环
	MSG msg = {0}; BOOL ret = FALSE;
	while ((ret = ::GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (ret == -1)
			break;
		else
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	// 释放资源
	GUI()->Term();
	DAT()->Term();
	GBL()->Term();

	return (int) msg.wParam;
}