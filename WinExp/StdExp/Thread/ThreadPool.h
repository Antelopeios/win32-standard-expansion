// Copyright 2011-2012, 木头云
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
// ThreadPool - 线程池
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-28
// Version:	1.0.0009.2356
//
// History:
//	- 1.0.0004.0400(2011-02-25)	^ 简化CallProc中锁的调用
//	- 1.0.0005.2136(2011-02-27)	+ CThreadPoolT::Create()支持返回线程ID
//	- 1.0.0006.1520(2011-03-02)	+ 支持通过策略控制CThreadPoolT是否限制最大线程数
//								# 修正当线程数为空时,CThreadPoolT::Clear(INFINITE)死锁的问题
//	- 1.0.0007.1048(2011-06-13)	# 修正CThreadPoolT的限制最大线程数策略无效的问题
//	- 1.0.0008.1554(2011-09-21)	+ 对_ThreadPool添加一一对应_ThreadHeap的常规接口定义
//	- 1.0.0009.2356(2012-01-28)	= _ThreadPoolPolicyT改名为_TrdPoolPolicyT
//								= CThreadPoolCreator改写为CPoolCreatorT
//////////////////////////////////////////////////////////////////

#ifndef __ThreadPool_h__
#define __ThreadPool_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread/Semaphore.h"
#include "Thread/Lock.h"
#include "Thread/ThreadCreator.h"
#include "Container/List.h"
#include "Pattern/Singleton.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename CreatorT = EXP_THREAD_CREATOR>
struct _TrdPoolPolicyT
{
	typedef typename CreatorT::creator_t creator_t;
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

	static const BOOL LIMIT_MAX = TRUE;
};

//////////////////////////////////////////////////////////////////

template <typename PolicyT = _TrdPoolPolicyT<> >
class CThreadPoolT
{
public:
	typedef typename PolicyT::creator_t creator_t;
	typedef typename creator_t::call_t call_t;
	typedef typename PolicyT::model_t model_t;
	typedef typename PolicyT::mutex_t mutex_t;

	// 内部线程对象
	typedef struct _Trd
	{
		HANDLE hTrd;

		_Trd(HANDLE trd = NULL)
			: hTrd(trd)
		{}
		operator HANDLE()
		{ return hTrd; }
	} trd_t;

	// 线程对象链表
	typedef CListT<trd_t> trd_list_t;

	// 内部任务对象
	typedef struct _Tsk
	{
		call_t pCal;
		LPVOID pPar;
		CEvent* evtR;	// 任务执行信号量
		HANDLE* hdlR;	// 任务执行返回值
		LPDWORD tidR;

		_Tsk(call_t cal = NULL, LPVOID par = NULL)
			: pCal(cal)
			, pPar(par)
			, evtR(NULL)
			, hdlR(NULL)
			, tidR(NULL)
		{}
	} tsk_t;

	// 任务对象链表
	typedef CListT<tsk_t> tsk_list_t;

	// 内部参数传递对象
	typedef struct _Par
	{
		CThreadPoolT* pThs;
		typename trd_list_t::iterator_t tIte;

		_Par()
			: pThs(NULL)
		{}
	} par_t;

protected:
	// 线程分配
	creator_t m_Creator;

	// 线程同步
	mutex_t	m_Mutex;
	CSemaphore m_TaskSmph;
	CEvent	m_ComplEvt;
	CEvent	m_ClearEvt;

	trd_list_t m_TrdList;	// 线程对象链表
	tsk_list_t m_TskList;	// 线程任务链表

	DWORD	m_nMaxSize;		// 池大小上限
	DWORD	m_nUseSize;		// 已分配数量

protected:
	static DWORD __stdcall CallProc(LPVOID lpParam)
	{
		par_t* par = (par_t*)(lpParam);
		ExAssert(par);
		CThreadPoolT* ths = par->pThs;
		ExAssert(ths);
		HANDLE hdl = par->tIte->Val().hTrd;
		ExAssert(hdl);

		HANDLE sync[2] =
		{
			ths->m_TaskSmph, 
			ths->m_ClearEvt
		};

		// 无限的任务等待循环
		for(;;)
		{
			DWORD wr = ISyncObject::Wait(sync, FALSE);
			// 响应线程清空事件
			if (wr == WAIT_OBJECT_0 + 1)
				break;
			// 响应用户调用事件
			ExLock(ths->m_Mutex, FALSE, mutex_t);
			if(!ths->m_TskList.Empty())
			{
				tsk_t tsk = ths->m_TskList.HeadItem();
				ths->m_TskList.Del(ths->m_TskList.Head());
				++(ths->m_nUseSize);
				if (tsk.hdlR) (*(tsk.hdlR)) = hdl;
				if (tsk.tidR) (*(tsk.tidR)) = GetCurrentThreadId();
				ths->m_Mutex.Unlock(FALSE);	// 解写锁
				if (tsk.evtR) tsk.evtR->Set();
				tsk.pCal(tsk.pPar);			// 用户调用
				ths->m_Mutex.Lock(FALSE);	// 加写锁
				--(ths->m_nUseSize);
			}
			// 检查是否超出线程池大小
			if (ths->m_nMaxSize < ths->m_TrdList.GetCount())
				break;
		}

		// 清理自身
		ths->m_Creator.Close(hdl);
		ths->m_Mutex.Lock(FALSE);		// 加写锁
		ths->m_TrdList.Del(par->tIte);
		if (ths->m_TrdList.GetCount() == 0)
		{
			ths->m_Mutex.Unlock(FALSE);	// 解写锁
			ths->m_ComplEvt.Set();
			ths->m_ClearEvt.Reset();
		}
		else
			ths->m_Mutex.Unlock(FALSE);	// 解写锁
		del(par);
		return 0;
	}

	HANDLE AddTrd(DWORD dwFlag, LPDWORD lpIDThread)
	{
		m_TrdList.Add(trd_t());
		par_t* par = dbnew(par_t);
		par->pThs = this;
		par->tIte = m_TrdList.Last();
		return (par->tIte->Val().hTrd = m_Creator.Create(CallProc, par, dwFlag, lpIDThread));
	}

public:
	CThreadPoolT(DWORD nDefSize = PolicyT::DefSize(), 
				 DWORD nMaxSize = PolicyT::MaxSize())
		: m_nMaxSize(0)
		, m_nUseSize(0)
		, m_TaskSmph(0, 0x7FFFFFFF)
		, m_ComplEvt(FALSE)
		, m_ClearEvt(TRUE)
	{
		SetMaxSize(nMaxSize);
		SetPoolSize(nDefSize);
	}
	~CThreadPoolT()
	{ Clear(); }

public:
	// 池大小设置
	DWORD GetPoolSize()
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		return m_TrdList.GetCount();
	}
	void SetPoolSize(DWORD nSize = PolicyT::DefSize())
	{
		ExLock(m_Mutex, FALSE, mutex_t);
		if( nSize > m_nMaxSize )
			nSize = m_nMaxSize;
		if( m_TrdList.GetCount() >= nSize ) return;
		DWORD need_size = (nSize - m_TrdList.GetCount());
		for(DWORD i = 0; i < need_size; ++i)
			Create(NULL);
	}
	DWORD GetUsedSize()
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		return m_nUseSize;
	}
	DWORD GetMaxSize()
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		return m_nMaxSize;
	}
	void SetMaxSize(DWORD nMaxSize)
	{
		ExLock(m_Mutex, FALSE, mutex_t);
		m_nMaxSize = nMaxSize;
	}

	// 创建线程
	HANDLE Create(_IN_ call_t lpStartAddr, 
				  _IN_ LPVOID lpParam = NULL, 
				  _IN_ DWORD dwFlag = 0, 
				  _OT_ LPDWORD lpIDThread = NULL, 
				  _IN_ DWORD dwWaitTime = 100)
	{
		HANDLE hdl = NULL;
		if (lpStartAddr)
		{	// 给用户分配线程
			m_Mutex.Lock(FALSE);
			BOOL is_limited = FALSE;
			if (m_nUseSize >= m_TrdList.GetCount())
			{
				if (m_nMaxSize <= m_TrdList.GetCount())
					is_limited = PolicyT::LIMIT_MAX;
				if(!is_limited)
					hdl = AddTrd(dwFlag, lpIDThread);
			}
			// 添加任务
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
			{	// 等待线程响应
				if (hdl == NULL && dwWaitTime)
				{
					m_Mutex.Unlock(FALSE);
					ExAssert(tsk.evtR);
					tsk.evtR->Wait(dwWaitTime);
					alloc_t::Free(tsk.evtR);
				}
				else
					m_Mutex.Unlock(FALSE);
			}
			else
				m_Mutex.Unlock(FALSE);
		}
		else
		{	// 为池增加线程
			ExLock(m_Mutex, FALSE, mutex_t);
			if (m_TrdList.GetCount() >= m_nMaxSize)
				return NULL;
			hdl = AddTrd(dwFlag, lpIDThread);
		}
		return hdl;
	}
	// 清空池
	BOOL Clear(DWORD dwWaitTime = INFINITE)
	{
		if (GetPoolSize() == 0) return TRUE;
		m_ClearEvt.Set();
		return (m_ComplEvt.Wait(dwWaitTime) == WAIT_OBJECT_0);
	}
};

typedef CThreadPoolT<> CThreadPool;

//////////////////////////////////////////////////////////////////

template <typename CreatorT = EXP_THREAD_CREATOR>
class CPoolCreatorT
{
public:
	typedef CPoolCreatorT creator_t;
	typedef CThreadPoolT<_TrdPoolPolicyT<CreatorT> > trd_creator_t;
	typedef typename trd_creator_t::call_t call_t;
	typedef typename trd_creator_t::creator_t base_creator_t;

	EXP_INLINE static trd_creator_t& GetPool()
	{
		return ExSingleton<trd_creator_t>();
	}

public:
	EXP_INLINE static HANDLE Create(_IN_ call_t lpStartAddr, 
									_IN_ LPVOID lpParam = NULL, 
									_IN_ DWORD dwFlag = 0, 
									_OT_ LPDWORD lpIDThread = NULL)
	{ return GetPool().Create(lpStartAddr, lpParam, dwFlag, lpIDThread); }
	EXP_INLINE static BOOL Close(HANDLE hTrd)
	{ return base_creator_t::Close(hTrd); }

	EXP_INLINE static DWORD Suspend(HANDLE hTrd)
	{ return base_creator_t::Suspend(hTrd); }
	EXP_INLINE static DWORD Resume(HANDLE hTrd)
	{ return base_creator_t::Resume(hTrd); }

	EXP_INLINE static BOOL Terminate(HANDLE hTrd, DWORD dwExitCode = 0)
	{ return base_creator_t::Terminate(hTrd, dwExitCode); }
};

typedef CPoolCreatorT<> CPoolCreator;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ThreadPool_h__*/