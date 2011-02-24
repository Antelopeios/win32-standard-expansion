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
// GC - 垃圾回收器
//
// Author:	木头云
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-02-24
// Version:	1.1.0013.2300
//
// History:
//	- 1.1.0013.2300(2011-02-24)	^ 优化CGCT::Free()的实现
//								+ 支持与SmartPtr智能指针同时使用
//////////////////////////////////////////////////////////////////

#ifndef __GC_h__
#define __GC_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/SmartPtr.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = DefMemAlloc, typename ModelT = DefThreadModel>
struct _GCPolicyT;

template <typename PolicyT = _GCPolicyT<> >
class CGCT : CNonCopyable
{
public:
	typedef typename PolicyT::alloc_t alloc_t;
	typedef typename PolicyT::model_t model_t;
	typedef typename PolicyT::mutex_t mutex_t;

protected:
	mutex_t	m_Mutex;

	void**	m_BlockArray;	// 内存块数组
	DWORD	m_nSize;
	DWORD	m_nIndx;

public:
	CGCT(DWORD nSize = PolicyT::s_nDefSize)
		: m_BlockArray	(NULL)
		, m_nSize		(0)
		, m_nIndx		(0)
	{ SetGCSize(nSize); }
	~CGCT()
	{
		Clear();
		alloc_t::Free(m_BlockArray);
	}

protected:
	void CheckFree(void* pPtr)
	{
		// 有引用计数时不自动释放指针
		if (CPtrManager::Instance().Get(pPtr) <= 0)
			alloc_t::Free(pPtr);
	}

public:
	template <typename TypeT>
	inline TypeT* Construct(void* pPtr)
	{
		ExLock(m_Mutex, false, mutex_t);
		return alloc_t::Construct<TypeT>(pPtr);
	}
	inline void* Destruct(void* pPtr)
	{
		ExLock(m_Mutex, false, mutex_t);
		return alloc_t::Destruct(pPtr);
	}

public:
	DWORD GetGCSize()
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_nSize;
	}
	void SetGCSize(DWORD nSize = PolicyT::s_nDefSize)
	{
		ExLock(m_Mutex, false, mutex_t);
		if( GetGCSize() >= nSize ) return;
		void** pArray = alloc_t::Alloc<void*>(nSize);
		if (m_BlockArray)
		{
			if (m_nIndx)
				memcpy(pArray, m_BlockArray, sizeof(void*) * m_nIndx);
			alloc_t::Free(m_BlockArray);
		}
		m_BlockArray = pArray;
		m_nSize = nSize;
	}

	// 内存效验
	bool Valid(void* pPtr)
	{
		ExLock(m_Mutex, true, mutex_t);
		return alloc_t::Valid(pPtr);
	}
	// 内存大小
	DWORD Size(void* pPtr)
	{
		ExLock(m_Mutex, true, mutex_t);
		return alloc_t::Size(pPtr);
	}
	// 分配内存
	template <typename TypeT>
	TypeT* Alloc(DWORD nCount = 1)
	{
		if (nCount == 0) return NULL;
		ExLock(m_Mutex, false, mutex_t);
		if (GetGCSize() == m_nIndx)
			SetGCSize(PolicyT::Expan(GetGCSize()));
		m_BlockArray[m_nIndx] = alloc_t::Alloc<TypeT>(nCount);
		ExAssert(m_BlockArray[m_nIndx]);
		return ((TypeT*)(m_BlockArray[m_nIndx++]));
	}
	void* Alloc(DWORD nSize)
	{
		if (nSize == 0) return NULL;
		return (void*)Alloc<BYTE>(nSize);
	}
	// 回收内存
	void Free(void* pPtr)
	{
		if (!pPtr) return;
		ExLock(m_Mutex, false, mutex_t);
		DWORD i = 0;
		for(; i < m_nIndx; ++i)
		{
			if (pPtr = m_BlockArray[i])
			{
				CheckFree(pPtr);
				memmove((void*)(m_BlockArray + i), (void*)(m_BlockArray + i + 1), sizeof(void*) * (m_nIndx - i - 1));
				break;
			}
		}
		if (i < m_nIndx) --m_nIndx;
	}
	// 清空GC
	void Clear()
	{
		ExLock(m_Mutex, false, mutex_t);
		if (!m_nIndx) return;
		// 后进先出
		for(DWORD i = (m_nIndx - 1); i > 0; --i)
			CheckFree(m_BlockArray[i]);
		CheckFree(m_BlockArray[0]);
		m_nIndx = 0;
	}
};

typedef CGCT<> CGC;

//////////////////////////////////////////////////////////////////

template <typename GCT = CGC>
class CGCAllocT
{
public:
	typedef GCT alloc_t;

public:
	static bool Valid(alloc_t& alloc, void* pPtr)
	{ return alloc.Valid(pPtr); }
	static DWORD Size(alloc_t& alloc, void* pPtr)
	{ return alloc.Size(pPtr); }

	template <typename TypeT>
	static TypeT* Alloc(alloc_t& alloc, DWORD nCount = 1)
	{ return alloc.Alloc<TypeT>(nCount); }
	static void* Alloc(alloc_t& alloc, DWORD nSize)
	{ return alloc.Alloc(nSize); }

	static void Free(alloc_t& alloc, void* pPtr)
	{ alloc.Free(pPtr); }
};

typedef CGCAllocT<> CGCAlloc;
typedef CGCAlloc ExGC;

//////////////////////////////////////////////////////////////////

template <typename AllocT/* = DefMemAlloc*/, typename ModelT/* = DefThreadModel*/>
struct _GCPolicyT
{
	typedef AllocT alloc_t;
	typedef ModelT model_t;
	typedef typename model_t::_LockPolicy mutex_policy_t;
	typedef CLockT<mutex_policy_t> mutex_t;

	static const DWORD s_nDefSize = 100;
	static DWORD Expan(DWORD nSize)
	{ return nSize ? (nSize << 1) : 1; }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GC_h__*/