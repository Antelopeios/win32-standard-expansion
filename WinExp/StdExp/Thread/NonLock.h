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
// NonLock - 无锁(非阻塞型同步(Non-blocking Synchronization))
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-11
// Version:	1.0.0001.2354
//
// History:
//	- 1.0.0001.2354(2011-05-11)	= NonBlocking改名为NonLock
//////////////////////////////////////////////////////////////////

#ifndef __NonLock_h__
#define __NonLock_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread/ThreadModel.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename TypeT, typename PolicyT = EXP_THREAD_MODEL::_LockPolicy>
class CNonLockT
{
public:
	typedef TypeT type_t;
	typedef PolicyT policy_t;
	typedef typename CLockT<policy_t>::mutex_t mutex_t;

	struct ret_t
	{
		type_t type;
		volatile LONG ret;
		ret_t() : ret(0) {}
	};

protected:
	ret_t m_Ret;
	type_t m_New;
	BOOL m_Has;
	mutex_t m_LckRef;

public:
	CNonLockT()
		: m_Has(FALSE)
	{}
	virtual ~CNonLockT()
	{}

public:
	void Set(const type_t& tNew)
	{
		ExLock(m_LckRef, FALSE, mutex_t);
		m_New = tNew;
		m_Has = TRUE;
	}
	type_t& Get()
	{
		ExLock(m_LckRef, FALSE, mutex_t);
		if (++(m_Ret.ret) <= 1 && m_Has)
		{
			m_Ret.type = m_New;
			m_Has = FALSE;
		}
		return m_Ret.type;
	}
	void Release()
	{
		ExLock(m_LckRef, FALSE, mutex_t);
		--(m_Ret.ret);
	}
};

//////////////////////////////////////////////////////////////////

// 自动化的数据访问类
template <typename NonLockT>
class CNonLockerT : INonCopyable
{
public:
	typedef NonLockT non_t;
	typedef typename non_t::type_t type_t;

protected:
	non_t& m_Non;
	type_t m_Dat;

public:
	CNonLockerT(non_t& tNon)
		: m_Non(tNon)
	{ m_Dat = m_Non.Get(); }
	~CNonLockerT()
	{ m_Non.Release(); }

public:
	type_t* operator->()
	{ return &m_Dat; }
	operator type_t*()
	{ return &m_Dat; }
	type_t& operator*()
	{ return m_Dat; }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__NonLock_h__*/