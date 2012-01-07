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
// Date:	2012-01-08
// Version:	1.0.0000.0030
//////////////////////////////////////////////////////////////////

#ifndef __App_h__
#define __App_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread/SyncObject.h"
#include "Thread/ThreadHeap.h"
#include "Thread/ThreadCreator.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class IApp : public IThread
{
private:
	BOOL Create(_IN_ BOOL bUIThread = FALSE, 
				_IN_ LPVOID lpParam = NULL, 
				_IN_ DWORD dwFlag = 0, 
				_OT_ LPDWORD lpIDThread = NULL) { return FALSE; }
	DWORD Suspend() { return 0; }
	DWORD Resume() { return 0; }

private:
	static IApp* m_pThis;
public:
	static int Run()
	{
		return (int)ProxyProc(m_pThis);
	}

public:
	// 进程句柄
	EXP_INLINE static HINSTANCE GetInstance() { return (HINSTANCE)m_pThis->GetHandle(); }

	// 自身是否已运行
	static BOOL m_IsSingle;
	EXP_INLINE static BOOL IsSingle() { return m_IsSingle; }

	// 进程路径及进程名
	static TCHAR m_Full[MAX_PATH];
	static TCHAR m_Path[MAX_PATH];
	static TCHAR m_Name[MAX_PATH];
	EXP_INLINE static LPCTSTR GetFull() { return m_Full; }
	EXP_INLINE static LPCTSTR GetPath() { return m_Path; }
	EXP_INLINE static LPCTSTR GetName() { return m_Name; }

	// 进程参数
	EXP_INLINE static LPCTSTR GetCmdLine() { return ::GetCommandLine(); }

public:
	IApp(BOOL bUIApp = TRUE, LPCTSTR sSingle = NULL/*用于判断进程唯一性的字符串*/)
	{
		if (m_pThis) return;

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
	~IApp()
	{
		m_hSync = NULL;
	}

private:
	DWORD OnThread(LPVOID lpParam)
	{
		return OnInit();
	}
protected:
	virtual DWORD OnInit() { return 0; }
	BOOL Terminate(DWORD dwExitCode = 0)
	{
		return TerminateProcess(GetCurrentProcess(), dwExitCode);
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#ifdef	EXP_USING_APP

#ifdef	_CONSOLE

int _tmain(int argc, _TCHAR* argv[])
{
	return EXP::IApp::Run();
}

#else

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	return EXP::IApp::Run();
}

#endif/*_CONSOLE*/

#endif/*EXP_USING_APP*/

//////////////////////////////////////////////////////////////////

#endif/*__App_h__*/