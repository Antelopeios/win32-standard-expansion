// Copyright 2012, ľͷ��
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
// App - Ӧ�ó�����
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-02
// Version:	1.0.0006.2200
//
// History:
//	- 1.0.0001.1030(2012-01-09)	+ IApp�������nSingleID����,֧��ֱ��ͨ����ԴIDȷ��Ψһ���ַ���
//	- 1.0.0002.1400(2012-01-28)	= IThreadT����Appģ����ʵ��
//	- 1.0.0003.2209(2012-01-29)	+ ���IApp::OnTerm(),��IApp::OnExit()������ͬ
//	- 1.0.0004.1008(2012-01-30)	+ IThreadT֧����IApp��OnInit(),OnTerm()������ͬ���ⲿ�ӿ�
//	- 1.0.0005.1430(2012-02-15)	+ ʼ�ն���g_App����
//								+ ���ExApp(),��ֱ��ͨ���˽ӿڻ�ȡApp����ָ��
//	- 1.0.0006.2200(2012-03-02)	^ �����Ƴ��ڲ��Զ������g_App,�Ż��ڲ��߼�,��������ģʽ����Ҫʱ�Զ�����ȫ��App����
//////////////////////////////////////////////////////////////////

#ifndef __App_h__
#define __App_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread/ThreadPool.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename CreatorT = EXP_THREAD_CREATOR>
class IThreadT : public ISyncObject
{
public:
	typedef CreatorT creator_t;

protected:
	BOOL m_bUIThread;
	LPVOID m_lpParam;
	DWORD m_nIDThread;

	MSG* m_pMsg;

public:
	IThreadT()
		: ISyncObject()
		, m_bUIThread(FALSE)
		, m_lpParam(NULL)
		, m_nIDThread(0)
		, m_pMsg(NULL)
	{}
	IThreadT(_IN_ BOOL bUIThread, 
			 _IN_ LPVOID lpParam = NULL, 
			 _IN_ DWORD dwFlag = 0, 
			 _OT_ LPDWORD lpIDThread = NULL)
	{
		this->IThreadT::IThreadT();
		Create(bUIThread, lpParam, dwFlag, lpIDThread);
	}
	~IThreadT()
	{}

protected:
	static DWORD __stdcall ProxyProc(LPVOID lpParam)
	{
		IThread* _this = (IThread*)lpParam;
		if (_this->IsUIThread())
		{
			// ��ִ���̹߳���
			_this->OnThread(_this->GetParam());
			// ����UI��Ϣѭ��
			MSG msg = {0}; BOOL ret = FALSE;
			_this->m_pMsg = &msg;
			while ((ret = ::GetMessage(&msg, NULL, 0, 0)) != 0)
			{
				if (ret == -1)
					break;
				else
				{
					::TranslateMessage(&msg);
					if (!_this->OnMessage(&msg))
						::DispatchMessage(&msg);
				}
			}
			_this->m_pMsg = NULL;
			return _this->OnExit((DWORD)msg.wParam);
		}
		else
			return _this->OnExit(_this->OnThread(_this->GetParam()));
	}
	virtual BOOL OnMessage(const MSG* lpMsg) { return FALSE; }
	virtual DWORD OnThread(LPVOID lpParam) { return OnInit(); }
	virtual DWORD OnExit(DWORD nCode) { return OnTerm(nCode); }
	virtual DWORD OnInit() { return 0; }
	virtual DWORD OnTerm(DWORD nCode) { return nCode; }

public:
	BOOL Create(_IN_ BOOL bUIThread = FALSE, 
				_IN_ LPVOID lpParam = NULL, 
				_IN_ DWORD dwFlag = 0, 
				_OT_ LPDWORD lpIDThread = NULL)
	{
		if (!IsClosed()) return FALSE;
		m_bUIThread = bUIThread;
		m_lpParam = lpParam;
		m_hSync = creator_t::Create(ProxyProc, this, dwFlag, &m_nIDThread);
		if (lpIDThread) (*lpIDThread) = m_nIDThread;
		if (m_hSync == NULL) m_hSync = INVALID_HANDLE_VALUE;
		if (m_hSync == INVALID_HANDLE_VALUE) return FALSE;
		return TRUE;
	}

	MSG* GetMSG() const
	{ return m_pMsg; }

	BOOL IsUIThread() const
	{ return m_bUIThread; }
	LPVOID GetParam() const
	{ return m_lpParam; }
	DWORD GetID() const
	{ return m_nIDThread; }

	virtual BOOL PostMessage(UINT nMsg, WPARAM wParam = 0, LPARAM lParam = NULL)
	{
		if (!IsUIThread()) return FALSE;
		return ::PostThreadMessage(GetID(), nMsg, wParam, lParam);
	}

	virtual DWORD Suspend()
	{ return creator_t::Suspend(m_hSync); }
	virtual DWORD Resume()
	{ return creator_t::Resume(m_hSync); }

	virtual BOOL Terminate(DWORD dwExitCode = 0)
	{
		if (!creator_t::Terminate(m_hSync, dwExitCode)) return FALSE;
		return Close();
	}
};

typedef IThreadT<> IThread;

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

public:
	static int Run() { return (int)ProxyProc(GetApp()); }
	static MSG* GetMSG() { return GetApp()->IThread::GetMSG(); }

	BOOL m_IsSingle;
	TCHAR m_Full[MAX_PATH];
	TCHAR m_Path[MAX_PATH];
	TCHAR m_Name[MAX_PATH];

public:
	// App����
	EXP_INLINE static IApp* GetApp()
	{
		IApp*& app = EXP_SINGLETON<IApp*>::Instance();
		if (!app) app = &(EXP_SINGLETON<IApp>::Instance());
		return app;
	}

	// ���̾��
	EXP_INLINE static HINSTANCE GetInstance() { return (HINSTANCE)GetApp()->GetHandle(); }

	// �����Ƿ�������
	EXP_INLINE static BOOL IsSingle() { return GetApp()->m_IsSingle; }

	// ����·����������
	EXP_INLINE static LPCTSTR GetFull() { return GetApp()->m_Full; }
	EXP_INLINE static LPCTSTR GetPath() { return GetApp()->m_Path; }
	EXP_INLINE static LPCTSTR GetName() { return GetApp()->m_Name; }

	// ���̲���
	EXP_INLINE static LPCTSTR GetCmdLine() { return ::GetCommandLine(); }

public:
	IApp(BOOL bUIApp = TRUE, LPCTSTR sSingle = NULL/*�����жϽ���Ψһ�Ե��ַ���*/, UINT nSingleID = 0)
		: m_IsSingle(TRUE)
	{
		IApp*& app = EXP_SINGLETON<IApp*>::Instance();
		if (app) return;
		app = this;

		m_bUIThread = bUIApp;
		m_hSync = ::GetModuleHandle(NULL);
		m_nIDThread = ::GetCurrentProcessId();

		::GetModuleFileName(NULL, m_Full, _countof(m_Full));
		_tcscpy_s(m_Path, m_Full);
		TCHAR* p_name = _tcsrchr(m_Path, _T('\\')) + 1;
		_tcscpy_s(m_Name, p_name);
		(*p_name) = _T('\0');

		// �ڳ����ʼ����ʱ���ж������Ƿ�������
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
			m_IsSingle = FALSE; // �����Ѿ�����������
		}
	}

	~IApp()
	{
		/*
			�ڻ�������֮ǰ�Ƚ�����ÿ�
			��ֹ��������ʱ���Զ�����
		*/
		m_hSync = NULL;
	}

public:
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

#else /*_CONSOLE*/

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	return EXP::IApp::Run();
}

#endif/*_CONSOLE*/

#endif/*EXP_USING_APP*/

#define ExApp() EXP::IApp::GetApp()

//////////////////////////////////////////////////////////////////

#endif/*__App_h__*/