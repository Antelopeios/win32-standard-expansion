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
// ThreadPool - �̳߳�
//
// Author:	ľͷ��
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-03-02
// Version:	1.0.0006.1520
//
// History:
//	- 1.0.0004.0400(2011-02-25)	^ ��CallProc�����ĵ���
//	- 1.0.0005.2136(2011-02-27)	+ CThreadPoolT::Create()֧�ַ����߳�ID
//	- 1.0.0006.1520(2011-03-02)	+ ֧��ͨ�����Կ���CThreadPoolT�Ƿ���������߳���
//								# �������߳���Ϊ��ʱ,CThreadPoolT::Clear(INFINITE)����������
//////////////////////////////////////////////////////////////////

#ifndef __ThreadPool_h__
#define __ThreadPool_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread/Semaphore.h"
#include "Thread/Lock.h"
#include "Thread/ThreadHeap.h"
#include "Container/List.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename CreatorT = CThreadHeapCreator, typename AllocT = EXP_MEMORY_ALLOC>
struct _ThreadPoolPolicyT
{
	typedef typename CreatorT::creator_t creator_t;
	typedef AllocT alloc_t;
	typedef _MultiModel model_t;
	typedef typename model_t::_LockPolicy mutex_policy_t;
	typedef CLockT<mutex_policy_t> mutex_t;

	EXP_INLINE static DWORD DefSize()
	{
		SYSTEM_INFO system_info = {0};
		GetSystemInfo(&system_info);
		return (system_info.dwNumberOfProcessors * 2 + 2);
	}
	EXP_INLINE static DWORD MaxSize()
	{ return DefSize(); }

	static const bool s_bLimitMax = true;
};

//////////////////////////////////////////////////////////////////

template <typename PolicyT = _ThreadPoolPolicyT<> >
class CThreadPoolT
{
public:
	typedef typename PolicyT::creator_t creator_t;
	typedef typename creator_t::handle_t handle_t;
	typedef typename creator_t::call_t call_t;
	typedef typename PolicyT::alloc_t alloc_t;
	typedef typename PolicyT::model_t model_t;
	typedef typename PolicyT::mutex_t mutex_t;

	// �ڲ��̶߳���
	typedef struct _Trd
	{
		handle_t hTrd;

		_Trd(handle_t trd = NULL)
			: hTrd(trd)
		{}
		operator handle_t()
		{ return hTrd; }
	} trd_t;
	typedef CListT<trd_t, _ListPolicyT<alloc_t> > trd_list_t;

	// �ڲ��������
	typedef struct _Tsk
	{
		call_t pCal;
		LPVOID pPar;
		CEvent* evtR;	// ����ִ���ź���
		handle_t* hdlR;	// ����ִ�з���ֵ
		LPDWORD tidR;

		_Tsk(call_t cal = NULL, LPVOID par = NULL)
			: pCal(cal)
			, pPar(par)
			, evtR(NULL)
			, hdlR(NULL)
			, tidR(NULL)
		{}
	} tsk_t;
	typedef CListT<tsk_t, _ListPolicyT<alloc_t> > tsk_list_t;

	// �ڲ��������ݶ���
	typedef struct _Par
	{
		CThreadPoolT* pThs;
		typename trd_list_t::iterator_t tIte;

		_Par()
			: pThs(NULL)
		{}
	} par_t;

protected:
	// �̷߳���
	creator_t m_Creator;

	// �߳�ͬ��
	mutex_t	m_Mutex;
	CSemaphore m_TaskSmph;
	CEvent	m_ComplEvt;
	CEvent	m_ClearEvt;

	trd_list_t m_TrdList;	// �̶߳�������
	tsk_list_t m_TskList;	// �߳���������

	DWORD	m_nMaxSize;		// �ش�С����
	DWORD	m_nUseSize;		// �ѷ�������

protected:
	static DWORD __stdcall CallProc(LPVOID lpParam)
	{
		par_t* par = (par_t*)(lpParam);
		ExAssert(par);
		CThreadPoolT* ths = par->pThs;
		ExAssert(ths);
		handle_t hdl = par->tIte->Val().hTrd;
		ExAssert(hdl);

		HANDLE sync[2] =
		{
			ths->m_TaskSmph, 
			ths->m_ClearEvt
		};

		// ���޵�����ȴ�ѭ��
		for(;;)
		{
			DWORD wr = CSyncObject::Wait(sync, false);
			// ��Ӧ�߳�����¼�
			if (wr == WAIT_OBJECT_0 + 1)
				break;
			// ��Ӧ�û������¼�
			ExLock(ths->m_Mutex, false, mutex_t);
			if(!ths->m_TskList.Empty())
			{
				tsk_t tsk = ths->m_TskList.HeadItem();
				ths->m_TskList.Del(ths->m_TskList.Head());
				++(ths->m_nUseSize);
				if (tsk.hdlR) (*(tsk.hdlR)) = hdl;
				if (tsk.tidR) (*(tsk.tidR)) = GetCurrentThreadId();
				ths->m_Mutex.Unlock(false);	// ��д��
				if (tsk.evtR) tsk.evtR->Set();
				tsk.pCal(tsk.pPar);			// �û�����
				ths->m_Mutex.Lock(false);	// ��д��
				--(ths->m_nUseSize);
			}
			// ����Ƿ񳬳��̳߳ش�С
			if (ths->m_nMaxSize < ths->m_TrdList.GetCount())
				break;
		}

		// ��������
		ths->m_Creator.Close(hdl);
		ths->m_Mutex.Lock(false);		// ��д��
		ths->m_TrdList.Del(par->tIte);
		if (ths->m_TrdList.GetCount() == 0)
		{
			ths->m_Mutex.Unlock(false);	// ��д��
			ths->m_ComplEvt.Set();
			ths->m_ClearEvt.Reset();
		}
		else
			ths->m_Mutex.Unlock(false);	// ��д��
		alloc_t::Free(par);
		return 0;
	}

	handle_t AddTrd(DWORD dwFlag, LPDWORD lpIDThread)
	{
		m_TrdList.Add(trd_t());
		par_t* par = alloc_t::Alloc<par_t>();
		par->pThs = this;
		par->tIte = m_TrdList.Last();
		return (par->tIte->Val().hTrd = m_Creator.Create(CallProc, par, dwFlag, lpIDThread));
	}

public:
	CThreadPoolT(DWORD nSize = PolicyT::DefSize())
		: m_nMaxSize(PolicyT::MaxSize())
		, m_nUseSize(0)
		, m_TaskSmph(0, 0x7FFFFFFF)
		, m_ComplEvt(false)
		, m_ClearEvt(true)
	{ SetPoolSize(nSize); }
	~CThreadPoolT()
	{ Clear(); }

public:
	// �ش�С����
	DWORD GetPoolSize()
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_TrdList.GetCount();
	}
	void SetPoolSize(DWORD nSize = PolicyT::DefSize())
	{
		ExLock(m_Mutex, false, mutex_t);
		if( nSize > m_nMaxSize )
			nSize = m_nMaxSize;
		if( m_TrdList.GetCount() >= nSize ) return;
		DWORD need_size = (nSize - m_TrdList.GetCount());
		for(DWORD i = 0; i < need_size; ++i)
			Create(NULL);
	}
	DWORD GetUsedSize()
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_nUseSize;
	}
	DWORD GetMaxSize()
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_nMaxSize;
	}
	void SetMaxSize(DWORD nMaxSize)
	{
		ExLock(m_Mutex, false, mutex_t);
		m_nMaxSize = nMaxSize;
	}

	// �����߳�
	handle_t Create(_IN_ call_t lpStartAddr, 
					_IN_ LPVOID lpParam = NULL, 
					_IN_ DWORD dwFlag = 0, 
					_OT_ LPDWORD lpIDThread = NULL, 
					_IN_ DWORD dwWaitTime = 100)
	{
		handle_t hdl = NULL;
		if (lpStartAddr)
		{	// ���û������߳�
			m_Mutex.Lock(false);
			bool is_limited = false;
			if (m_nUseSize >= m_TrdList.GetCount())
			{
				if (m_nMaxSize <= m_TrdList.GetCount())
					is_limited = true;
				else
					hdl = AddTrd(dwFlag, lpIDThread);
			}
			// �������
			tsk_t tsk(lpStartAddr, lpParam);
			if (!is_limited && hdl == NULL && dwWaitTime)
			{
				CEvent* evt = alloc_t::Alloc<CEvent>();
				ExAssert(evt);
				evt->Create();
				tsk.evtR = evt;
				tsk.hdlR = &hdl;
				tsk.tidR = lpIDThread;
			}
			m_TskList.Add(tsk);
			m_TaskSmph.Release();
			if (!is_limited)
			{	// �ȴ��߳���Ӧ
				if (hdl == NULL && dwWaitTime)
				{
					m_Mutex.Unlock(false);
					ExAssert(tsk.evtR);
					tsk.evtR->Wait(dwWaitTime);
					alloc_t::Free(tsk.evtR);
				}
				else
					m_Mutex.Unlock(false);
			}
			else
				m_Mutex.Unlock(false);
		}
		else
		{	// Ϊ�������߳�
			ExLock(m_Mutex, false, mutex_t);
			if (m_TrdList.GetCount() >= m_nMaxSize)
				return NULL;
			hdl = AddTrd(dwFlag, lpIDThread);
		}
		return hdl;
	}
	// ��ճ�
	bool Clear(DWORD dwWaitTime = INFINITE)
	{
		if (GetPoolSize() == 0) return true;
		m_ClearEvt.Set();
		return (m_ComplEvt.Wait(dwWaitTime) == WAIT_OBJECT_0);
	}
};

typedef CThreadPoolT<> CThreadPool;

//////////////////////////////////////////////////////////////////

class CThreadPoolCreator : public CThreadPool
{
public:
	typedef CThreadPool creator_t;
};

typedef CThreadPoolCreator _ThreadPool;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ThreadPool_h__*/