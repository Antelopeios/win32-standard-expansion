// Copyright 2011, 木头云
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
// ThreadCreator - 线程创建器
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-09-26
// Version:	1.1.0005.1214
//
// History:
//	- 1.0.0002.1540(2011-07-20)	= 将CThreadAdapterT的单例独立到外部,定义EXP_SINGLETON_TRDCREATOR,可由外部按需要自行替换
//	- 1.0.0003.1529(2011-09-20)	^ 简化EXP_THREAD_CREATOR,直接调用_ThreadHeap来完成功能
//	- 1.1.0004.1600(2011-09-21)	+ 添加IThreadT对象接口模板,用于整合线程的所有操作,并对象化线程管理
//	- 1.1.0005.1214(2011-09-26)	+ IThreadT对象支持直接创建UI线程
//								= 调整IThreadT::ThreadProc线程回调接口参数
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
	bool m_bUIThread;
	LPVOID m_lpParam;
	DWORD m_nIDThread;

public:
	IThreadT()
		: ISyncObject()
		, m_bUIThread(false)
		, m_lpParam(NULL)
		, m_nIDThread(0)
	{}
	IThreadT(_IN_ bool bUIThread, 
			 _IN_ LPVOID lpParam = NULL, 
			 _IN_ DWORD dwFlag = 0, 
			 _OT_ LPDWORD lpIDThread = NULL)
		: ISyncObject()
		, m_bUIThread(false)
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
			MSG msg = {0}; BOOL ret = FALSE;
			while ((ret = ::GetMessage(&msg, NULL, 0, 0)) != 0)
			{
				if (ret == -1)
					break;
				else
				{
					::TranslateMessage(&msg);
					_this->ThreadProc(&msg);
				}
			}
			return (DWORD)msg.wParam;
		}
		else
			return _this->ThreadProc(_this->GetParam());
	}
	virtual DWORD ThreadProc(LPVOID lpParam) = 0;

public:
	bool Create(_IN_ bool bUIThread = false, 
				_IN_ LPVOID lpParam = NULL, 
				_IN_ DWORD dwFlag = 0, 
				_OT_ LPDWORD lpIDThread = NULL)
	{
		if (!IsClosed()) return false;
		m_bUIThread = bUIThread;
		m_lpParam = lpParam;
		m_hSync = creator_t::Create(ProxyProc, this, dwFlag, &m_nIDThread);
		if (lpIDThread) (*lpIDThread) = m_nIDThread;
		if (m_hSync == NULL) m_hSync = INVALID_HANDLE_VALUE;
		if (m_hSync == INVALID_HANDLE_VALUE) return false;
		return true;
	}

	bool IsUIThread()
	{ return m_bUIThread; }
	LPVOID GetParam()
	{ return m_lpParam; }
	DWORD GetID()
	{ return m_nIDThread; }

	bool PostMessage(UINT nMsg, WPARAM wParam = 0, LPARAM lParam = NULL)
	{
		if (!IsUIThread()) return false;
		return ::PostThreadMessage(GetID(), nMsg, wParam, lParam);
	}

	DWORD Suspend()
	{ return creator_t::Suspend(m_hSync); }
	DWORD Resume()
	{ return creator_t::Resume(m_hSync); }

	bool Terminate(DWORD dwExitCode = 0)
	{
		if (!creator_t::Terminate(m_hSync, dwExitCode)) return false;
		return Close();
	}
};

typedef IThreadT<> IThread;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ThreadCreator_h__*/