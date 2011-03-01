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
// ThreadModel - 线程模型
//
// Author:	木头云
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-02-22
// Version:	1.0.0002.1700
//////////////////////////////////////////////////////////////////

#ifndef __ThreadModel_h__
#define __ThreadModel_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread/Event.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

struct _SingleModel
{
	struct _ExcPolicy
	{
		typedef volatile LONG lock_t;

		static void IntLock(lock_t& /*lc*/)
		{ __noop; }
		static void DelLock(lock_t& /*lc*/)
		{ __noop; }
		static void Lock(lock_t& /*lc*/, bool /*bRead*/)
		{ __noop; }
		static void Unlock(lock_t& /*lc*/, bool /*bRead*/)
		{ __noop; }
	};
	typedef _ExcPolicy _CriPolicy;
	typedef _ExcPolicy _ShrPolicy;
	typedef _ExcPolicy _LockPolicy;

	inline static void Switch()
	{ __noop; }

	inline static LONG Inc(volatile LONG* lpAddend)
	{
		ExAssert(lpAddend);
		return ++(*lpAddend);
	}
	inline static LONG Dec(volatile LONG* lpAddend)
	{
		ExAssert(lpAddend);
		return --(*lpAddend);
	}
	inline static LONG Add(volatile LONG* lpAddend, LONG lValue)
	{
		ExAssert(lpAddend);
		return (*lpAddend) += lValue;
	}
	inline static LONG Exc(volatile LONG* lpTarget, LONG lValue)
	{
		ExAssert(lpTarget);
		LONG tmp(*lpTarget);
		(*lpTarget) = lValue;
		return tmp;
	}
	inline static bool CAS(volatile LONG* lpTarget, LONG lComperand, LONG lValue)
	{
		ExAssert(lpTarget);
		if ((*lpTarget) == lComperand)
		{
			(*lpTarget) = lValue;
			return true;
		}
		return false;
	}
};

//////////////////////////////////////////////////////////////////

struct _MultiModel
{
	struct _ExcPolicy
	{	// 不可重入的自旋锁
		typedef volatile LONG lock_t;

		static void IntLock(lock_t& lc)
		{ _MultiModel::Exc(&lc, 0); }
		static void DelLock(lock_t& lc)
		{}
		static void Lock(lock_t& lc, bool /*bRead*/)
		{
			while( _MultiModel::Exc(&lc, 1) )
				_MultiModel::Switch();
		}
		static void Unlock(lock_t& lc, bool /*bRead*/)
		{ _MultiModel::Exc(&lc, 0); }
	};
	struct _CriPolicy
	{	// 临界区
		typedef CRITICAL_SECTION lock_t;

		static void IntLock(lock_t& lc)
		{ InitializeCriticalSectionAndSpinCount(&lc, 4000); }
		static void DelLock(lock_t& lc)
		{ DeleteCriticalSection(&lc); }
		static void Lock(lock_t& lc, bool /*bRead*/)
		{ EnterCriticalSection(&lc); }
		static void Unlock(lock_t& lc, bool /*bRead*/)
		{ LeaveCriticalSection(&lc); }
	};
	struct _ShrPolicy
	{	// 可重入的共享锁
		typedef _CriPolicy policy_t;
		typedef struct
		{
			CEvent rw_event;			// 读写事件
			long rc_w, rc_r;			// 读写状态
			DWORD id;					// 当前线程id
			policy_t::lock_t rc_lock;	// 状态锁
		} lock_t;

		static void IntLock(lock_t& lc)
		{
			lc.rw_event.Create(true, true);
			lc.rc_r = lc.rc_w = 0;
			lc.id = 0;
			policy_t::IntLock(lc.rc_lock);
		}
		static void DelLock(lock_t& lc)
		{
			lc.rw_event.Close();
			policy_t::DelLock(lc.rc_lock);
		}
		static void Lock(lock_t& lc, bool bRead)
		{
			policy_t::Lock(lc.rc_lock, false);
			if (bRead)
			{	// 加读锁
				if (lc.id == GetCurrentThreadId())
					++ lc.rc_r;
				else
				{
					while (lc.rc_w > 0)
					{	// 正在被写
						policy_t::Unlock(lc.rc_lock, false);
						lc.rw_event.Wait();		// 等待写结束
						policy_t::Lock(lc.rc_lock, false);
					}
					// 正在被读或无访问
					++ lc.rc_r;
					lc.id = GetCurrentThreadId();
				}
			}
			else
			{	// 加写锁
				if (lc.id == GetCurrentThreadId())
					++ lc.rc_w;
				else
				{
					while (lc.rc_w > 0 || lc.rc_r > 0)
					{	// 正在被写或正在被读
						policy_t::Unlock(lc.rc_lock, false);
						lc.rw_event.Wait();		// 等待访问结束
						lc.rw_event.Reset();	// 复位事件
						policy_t::Lock(lc.rc_lock, false);
					}
					// 无访问
					++ lc.rc_w;
					lc.id = GetCurrentThreadId();
				}
			}
			policy_t::Unlock(lc.rc_lock, false);
		}
		static void Unlock(lock_t& lc, bool bRead)
		{
			policy_t::Lock(lc.rc_lock, false);
			if (bRead)
			{
				if (lc.rc_r == 0)
				{
					policy_t::Unlock(lc.rc_lock, false);
					return;
				}
				else
					-- lc.rc_r;
			}
			else
			{
				if (lc.rc_w == 0)
				{
					policy_t::Unlock(lc.rc_lock, false);
					return;
				}
				else
					-- lc.rc_w;
			}
			if (lc.id != GetCurrentThreadId())
				lc.id = GetCurrentThreadId();
			policy_t::Unlock(lc.rc_lock, false);
			// 通知等待结束
			lc.rw_event.Set();
		}
	};
	typedef _CriPolicy _LockPolicy;

	inline static void Switch()
	{
	#if (_WIN32_WINNT >= 0x0400) && !defined(_WIN32_WCE)
		SwitchToThread();
	#else
		Sleep(5);
	#endif/*(_WIN32_WINNT >= 0x0400)*/
	}

	inline static LONG Inc(volatile LONG* lpAddend)
	{
		return ::InterlockedIncrement(lpAddend);
	}
	inline static LONG Dec(volatile LONG* lpAddend)
	{
		return ::InterlockedDecrement(lpAddend);
	}
	inline static LONG Add(volatile LONG* lpAddend, LONG lValue)
	{
		return ::InterlockedExchangeAdd(lpAddend, lValue);
	}
	inline static LONG Exc(volatile LONG* lpTarget, LONG lValue)
	{
		return ::InterlockedExchange(lpTarget, lValue);
	}
	inline static bool CAS(volatile LONG* lpTarget, LONG lComperand, LONG lValue)
	{
		return (lComperand == ::InterlockedCompareExchange(lpTarget, lValue, lComperand));
	}
};

//////////////////////////////////////////////////////////////////

#ifndef EXP_THREAD_MODEL
#ifdef	_MT
#define EXP_THREAD_MODEL _MultiModel
#else /*_MT*/
#define EXP_THREAD_MODEL _SingleModel
#endif/*_MT*/
#endif/*EXP_THREAD_MODEL*/

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ThreadModel_h__*/