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
// Lock - 锁/临界区
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-12
// Version:	1.2.0012.1500
//
// History:
//	- 1.2.0011.0400(2011-02-25)	# 移除_LockPolicy宏,避免在使用ThreadModel::_LockPolicy时出现C3083错误
//	- 1.2.0012.1500(2012-02-12)	^ 调整CLockerT的调用方式,让ExLock使用起来更加方便
//////////////////////////////////////////////////////////////////

#ifndef __Lock_h__
#define __Lock_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread/ThreadModel.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename PolicyT = EXP_THREAD_MODEL::_LockPolicy>
class CLockT : INonCopyable
{
public:
	typedef PolicyT policy_t;
	typedef typename policy_t::lock_t lock_t;
	typedef CLockT mutex_t;

private:
	lock_t	m_lc;
	lock_t&	m_Lock;

public:
	CLockT() : m_Lock(m_lc)
	{ PolicyT::IntLock(m_Lock); }
	CLockT(lock_t& lc) : m_Lock(lc)
	{ PolicyT::IntLock(m_Lock); }
	~CLockT()
	{ PolicyT::DelLock(m_Lock); }

	EXP_INLINE void Lock(BOOL bRead = FALSE)
	{ PolicyT::Lock(m_Lock, bRead); }
	EXP_INLINE void Unlock(BOOL bRead = FALSE)
	{ PolicyT::Unlock(m_Lock, bRead); }
};

typedef CLockT<> CMutex;
typedef CLockT<EXP_THREAD_MODEL::_ExcPolicy> CExcMutex;
typedef CLockT<EXP_THREAD_MODEL::_CriPolicy> CCriMutex;
typedef CLockT<EXP_THREAD_MODEL::_ShrPolicy> CShrMutex;

//////////////////////////////////////////////////////////////////

// 自动化的加锁/解锁类
template <typename MutexT = CMutex>
class CLockerT : INonCopyable
{
public:
	typedef typename MutexT::mutex_t mutex_t;

private:
	mutex_t& m_Lock;
	BOOL m_bRead;

public:
	CLockerT(mutex_t& lock)
		: m_Lock(lock), m_bRead(FALSE)
	{}
	~CLockerT()
	{ m_Lock.Unlock(m_bRead); }

public:
	void Lock(BOOL bRead = FALSE)
	{ m_Lock.Lock(m_bRead = bRead); }
};

typedef CLockerT<> CLocker;

//////////////////////////////////////////////////////////////////

#define ExLock(mutex, is_read, mutex_t)	\
	CLockerT<mutex_t> locker(mutex); \
	locker.Lock(is_read)

#define ExLockThis(policy_t)			\
	static CLockT<policy_t>::lock_t lc;	\
	static CLockT<policy_t> mutex(lc);	\
	ExLock(mutex, FALSE, CLockT<policy_t>)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Lock_h__*/