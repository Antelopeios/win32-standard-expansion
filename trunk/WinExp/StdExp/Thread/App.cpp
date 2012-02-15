// Copyright 2012, 木头云
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////////////
// App - 应用程序类
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-15
// Version:	1.0.0005.1430
//////////////////////////////////////////////////////////////////

#include "Common/Common.h"
#include "App.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

IApp* IApp::m_pThis = NULL;
BOOL IApp::m_IsSingle = TRUE;
TCHAR IApp::m_Full[MAX_PATH] = {0};
TCHAR IApp::m_Path[MAX_PATH] = {0};
TCHAR IApp::m_Name[MAX_PATH] = {0};

IApp g_App;

IApp::IApp(BOOL bUIApp/* = TRUE*/, LPCTSTR sSingle/* = NULL*/, UINT nSingleID/* = 0*/)
{
	if (m_pThis)
	{
		if (this != &g_App)
			m_pThis = this;
		return;
	}

	::GetModuleFileName(NULL, m_Full, _countof(m_Full));
	_tcscpy_s(m_Path, m_Full);
	TCHAR* p_name = _tcsrchr(m_Path, _T('\\')) + 1;
	_tcscpy_s(m_Name, p_name);
	(*p_name) = _T('\0');

	m_bUIThread = bUIApp;
	m_hSync = ::GetModuleHandle(NULL);
	m_nIDThread = ::GetCurrentProcessId();
	m_pThis = this;

	// 在程序初始化的时候判断自身是否已运行
	TCHAR str_name[MAX_PATH];
	if (sSingle && _tcslen(sSingle) > 0)
		_tcscpy_s(str_name, sSingle);
	else
	if (nSingleID > 0)
		::LoadString((HINSTANCE)m_hSync, nSingleID, str_name, _countof(str_name));
	else
	{
		_tcscpy_s(str_name, m_Full);
		while(p_name = _tcsrchr(str_name, _T('\\'))) (*p_name) = _T('_');
	}
	HANDLE mutex = ::CreateMutex(NULL, FALSE, str_name);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		::CloseHandle(mutex);
		m_IsSingle = FALSE; // 程序已经处于运行中
	}
}

//////////////////////////////////////////////////////////////////

EXP_END
