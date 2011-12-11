// Copyright 2011, ľͷ��
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
// ThreadCreator - �̴߳�����
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-12-09
// Version:	1.1.0008.1712
//
// History:
//	- 1.0.0002.1540(2011-07-20)	= ��CThreadAdapterT�ĵ����������ⲿ,����EXP_SINGLETON_TRDCREATOR,�����ⲿ����Ҫ�����滻
//	- 1.0.0003.1529(2011-09-20)	^ ��EXP_THREAD_CREATOR,ֱ�ӵ���_ThreadHeap����ɹ���
//	- 1.1.0004.1600(2011-09-21)	+ ���IThreadT����ӿ�ģ��,���������̵߳����в���,�������̹߳���
//	- 1.1.0005.1214(2011-09-26)	+ IThreadT����֧��ֱ�Ӵ���UI�߳�
//								= ����IThreadT::ThreadProc�̻߳ص��ӿڲ���
//	- 1.1.0006.1524(2011-11-30)	+ UI�̴߳������Զ�����Ϣ���ͨ������ֵ�����Ƿ����Ĭ�ϵ���Ϣ����
//	- 1.1.0007.1150(2011-12-07)	+ ���OnMessage�麯���ص�,����UI�̴߳�����Ϣ֮ǰ�ĳ�ʼ������
//								= �����ص��ӿ�����ΪOnThread
//	- 1.1.0008.1712(2011-12-09)	= ΪOnThread�ӿ����Ĭ��ʵ��,����Ϊ���麯��
//////////////////////////////////////////////////////////////////

#ifndef __ThreadCreator_h__
#define __ThreadCreator_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

#ifndef EXP_THREAD_CREATOR
#define EXP_THREAD_CREATOR _ThreadHeap
#endif/*EXP_THREAD_CREATOR*/

typedef EXP_THREAD_CREATOR ExThread;

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

public:
	IThreadT()
		: ISyncObject()
		, m_bUIThread(FALSE)
		, m_lpParam(NULL)
		, m_nIDThread(0)
	{}
	IThreadT(_IN_ BOOL bUIThread, 
			 _IN_ LPVOID lpParam = NULL, 
			 _IN_ DWORD dwFlag = 0, 
			 _OT_ LPDWORD lpIDThread = NULL)
		: ISyncObject()
		, m_bUIThread(FALSE)
		, m_lpParam(NULL)
		, m_nIDThread(0)
	{ Create(lpParam, dwFlag, lpIDThread); }
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
			return (DWORD)msg.wParam;
		}
		else
			return _this->OnThread(_this->GetParam());
	}
	virtual DWORD OnThread(LPVOID lpParam) { return 0; }
	virtual BOOL OnMessage(const MSG* lpMsg) { return FALSE; }

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

	BOOL IsUIThread()
	{ return m_bUIThread; }
	LPVOID GetParam()
	{ return m_lpParam; }
	DWORD GetID()
	{ return m_nIDThread; }

	BOOL PostMessage(UINT nMsg, WPARAM wParam = 0, LPARAM lParam = NULL)
	{
		if (!IsUIThread()) return FALSE;
		return ::PostThreadMessage(GetID(), nMsg, wParam, lParam);
	}

	DWORD Suspend()
	{ return creator_t::Suspend(m_hSync); }
	DWORD Resume()
	{ return creator_t::Resume(m_hSync); }

	BOOL Terminate(DWORD dwExitCode = 0)
	{
		if (!creator_t::Terminate(m_hSync, dwExitCode)) return FALSE;
		return Close();
	}
};

typedef IThreadT<> IThread;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ThreadCreator_h__*/