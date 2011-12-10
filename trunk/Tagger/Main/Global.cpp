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
	// ������Դ�ڴ�
	HRSRC info = ::FindResource(hInstance, MAKEINTRESOURCE(nID), szType);
	if( info == NULL ) return NULL;
	HGLOBAL data = ::LoadResource(hInstance, info);
	if( data == NULL ) return NULL;
	// �����Դ�ڴ��
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
	// �ͷ���Դ�ڴ��
	UnlockResource(hData);
	return FreeResource(hData);
}