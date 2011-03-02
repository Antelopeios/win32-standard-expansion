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
// ObjPool - 对象池
//
// Author:	木头云
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-03-02
// Version:	1.0.0012.1202
//
// History:
//	- 1.2.0012.1202(2011-03-02)	# 修正CObjPoolT::Valid()与CObjPoolT::Size()的内部指针传递错误
//////////////////////////////////////////////////////////////////

#ifndef __ObjPool_h__
#define __ObjPool_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Algorithm/NonCopyable.h"
#include "Debugging/Assertion.h"
#include "Pattern/Singleton.h"
#include "Thread/Lock.h"
#include "Memory/MemHeap.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = _MemHeap, typename ModelT = EXP_THREAD_MODEL>
struct _ObjPoolPolicyT;

template <typename TypeT, typename PolicyT = _ObjPoolPolicyT<> >
class CObjPoolT : CNonCopyable
{
public:
	typedef typename PolicyT::alloc_t alloc_t;
	typedef typename PolicyT::mutex_t mutex_t;
	typedef typename PolicyT::model_t model_t;

protected:
	template <typename Type2T>
	struct block_t
	{
		enum { HeadSize = (sizeof(block_t*) + sizeof(bool)) };

		block_t* pNext;	// 下一个结点
		bool	 bFree;	// 是否已清理
		Type2T	 Buff;	// 内存块

		block_t() : pNext(NULL), bFree(false) {}
		~block_t() { bFree = true; pNext = NULL; }
	};

	alloc_t			m_Alloc;
	mutex_t			m_Mutex;

	block_t<TypeT>*	m_FreeList;
	DWORD			m_nFreSize;	// 池大小
	DWORD			m_nMaxSize;	// 池大小上限

public:
	CObjPoolT(DWORD nSize = PolicyT::s_nDefSize)
		: m_FreeList(NULL)
		, m_nFreSize(0)
		, m_nMaxSize(PolicyT::s_nMaxSize)
	{ SetPoolSize(nSize); }
	virtual ~CObjPoolT()
	{ Clear(); }

public:
	// 池大小设置
	DWORD GetPoolSize()
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_nFreSize;
	}
	void SetPoolSize(DWORD nSize = PolicyT::s_nDefSize)
	{
		DWORD diff_size = 0;
		{
			ExLock(m_Mutex, false, mutex_t);
			if( nSize > m_nMaxSize )
				nSize = m_nMaxSize;
			if( m_nFreSize >= nSize ) return;
			diff_size = nSize - m_nFreSize;
		}
		for(DWORD i = 0; i < diff_size; ++i)
		{
			block_t<TypeT>* pBlock = (block_t<TypeT>*)m_Alloc.Alloc<block_t<BYTE[sizeof(TypeT)]> >();
			pBlock->pNext = m_FreeList;
			m_FreeList = pBlock;
			++m_nFreSize;
		}
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

	// 内存效验
	bool Valid(void* pPtr)
	{
		block_t<TypeT>* pBlock = (block_t<TypeT>*)((BYTE*)pPtr - block_t<TypeT>::HeadSize);
		return m_Alloc.Valid(pBlock);
	}
	// 内存大小
	DWORD Size(void* pPtr)
	{
		block_t<TypeT>* pBlock = (block_t<TypeT>*)((BYTE*)pPtr - block_t<TypeT>::HeadSize);
		return m_Alloc.Size(pBlock) - block_t<TypeT>::HeadSize;
	}
	// 分配内存
	template <typename Type2T>
	Type2T* Alloc()
	{
		ExLock(m_Mutex, false, mutex_t);
		if (sizeof(Type2T) > sizeof(TypeT))
			return (Type2T*)&(m_Alloc.Alloc<block_t<Type2T> >()->Buff);
		else
		{
			if (m_FreeList)
			{
				block_t<TypeT>* pBlock = m_FreeList;
				m_FreeList = pBlock->pNext;
				--m_nFreSize;
				return (Type2T*)&(m_Alloc.Construct<block_t<Type2T> >(pBlock)->Buff);
			}
			else
				return (Type2T*)&(m_Alloc.Construct<block_t<Type2T> >
								 (m_Alloc.Alloc<block_t<BYTE[sizeof(TypeT)]> >())->Buff);
		}
	}
	TypeT* Alloc()
	{
		ExLock(m_Mutex, false, mutex_t);
		if (m_FreeList)
		{
			block_t<TypeT>* pBlock = m_FreeList;
			m_FreeList = pBlock->pNext;
			--m_nFreSize;
			return (TypeT*)&(m_Alloc.Construct<block_t<TypeT> >(pBlock)->Buff);
		}
		return (TypeT*)&(m_Alloc.Alloc<block_t<TypeT> >()->Buff);
	}
	// 回收内存
	void Free(void* pPtr)
	{
		if (!pPtr) return;
		ExLock(m_Mutex, false, mutex_t);
		block_t<TypeT>* pBlock = (block_t<TypeT>*)((BYTE*)pPtr - block_t<TypeT>::HeadSize);
		ExAssert(!pBlock->bFree);
		if (pBlock->bFree) return;
		if (m_nFreSize >= m_nMaxSize)
			m_Alloc.Free(pBlock);
		else
		{
			m_Alloc.Destruct(pBlock);
			pBlock->pNext = m_FreeList;
			m_FreeList = pBlock;
			++m_nFreSize;
		}
	}
	// 清空内存
	void Clear()
	{
		ExLock(m_Mutex, false, mutex_t);
		while (m_FreeList)
		{
			block_t<TypeT>* pBlock = m_FreeList;
			m_FreeList = pBlock->pNext;
			m_Alloc.Free(pBlock);
		}
		m_FreeList = NULL;
		m_nFreSize = 0;
	}
};

//////////////////////////////////////////////////////////////////

template <typename TypeT, typename AllocT, typename ModelT = EXP_THREAD_MODEL>
class CPoolTypeT
{
public:
	typedef CObjPoolT<TypeT, _ObjPoolPolicyT<AllocT, ModelT> > alloc_t;

	static alloc_t& GetAlloc()
	{ return ExSingleton<alloc_t>(); }

public:
	static TypeT* Alloc()
	{ return GetAlloc().Alloc(); }
	void Free()
	{ GetAlloc().Free(static_cast<TypeT*>(this)); }
};

//////////////////////////////////////////////////////////////////

template <typename AllocT/* = _MemHeap*/, typename ModelT/* = EXP_THREAD_MODEL*/>
struct _ObjPoolPolicyT
{
	typedef AllocT alloc_t;
	typedef ModelT model_t;
	typedef typename model_t::_LockPolicy mutex_policy_t;
	typedef CLockT<mutex_policy_t> mutex_t;

	static const DWORD	s_nDefSize = 100;
	static const DWORD	s_nMaxSize = 10000;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ObjPool_h__*/