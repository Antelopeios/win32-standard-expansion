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
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-10-11
// Version:	1.0.0017.1715
//
// History:
//	- 1.0.0012.1202(2011-03-02)	# 修正CObjPoolT::Valid()与CObjPoolT::Size()的内部指针传递错误
//	- 1.0.0014.2140(2011-03-03)	+ CObjPoolT支持对任意大小进行分配
//	- 1.0.0015.1419(2011-05-18)	+ CPoolTypeT支持重写CPoolTypeT::Free()
//								= CPoolTypeT改名为IPoolTypeT
//	- 1.0.0016.1640(2011-06-14)	# 修正因IObjPool由于某些原因(如单例)被提前析构,从而导致IPoolTypeT::Free()的R6025错误
//	- 1.0.0017.1715(2011-10-11)	+ 添加CBlockPoolT,支持按照动态大小创建粒度对象
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

// ObjPool接口,方便上层直接操作统一接口
interface IObjPool
{
public:
	BOOL m_IsDest;

public:
	IObjPool() : m_IsDest(FALSE) {}
	virtual ~IObjPool() { m_IsDest = TRUE; }

public:
	virtual DWORD GetObjSize() = 0;
	virtual BOOL Valid(void* pPtr) = 0;
	virtual DWORD Size(void* pPtr) = 0;
	virtual void* Alloc(DWORD nSize) = 0;
	virtual void Free(void* pPtr) = 0;
	virtual void Clear() = 0;
};

//////////////////////////////////////////////////////////////////

template <typename AllocT = EXP_MEMHEAP_ALLOC, typename ModelT = EXP_THREAD_MODEL>
struct _ObjPoolPolicyT
{
	typedef AllocT alloc_t;
	typedef ModelT model_t;
	typedef typename model_t::_LockPolicy mutex_policy_t;
	typedef CLockT<mutex_policy_t> mutex_t;

	static const DWORD DEF_SIZE = 10;
	static const DWORD MAX_SIZE = 1000;
};

//////////////////////////////////////////////////////////////////

template <typename PolicyT = _ObjPoolPolicyT<> >
class CBlockPoolT : INonCopyable, public IObjPool
{
public:
	typedef typename PolicyT::alloc_t alloc_t;
	typedef typename PolicyT::mutex_t mutex_t;
	typedef typename PolicyT::model_t model_t;

protected:
	struct block_t
	{
		block_t* pNext;	// 下一个结点
		BOOL	 bFree;	// 是否已清理

		block_t()
			: pNext(NULL)
			, bFree(FALSE)
		{}
		~block_t()
		{
			bFree = TRUE;
			pNext = NULL;
		}
	};

	alloc_t		m_Alloc;
	mutex_t		m_Mutex;

	block_t*	m_FreeList;
	DWORD		m_nFreSize;	// 池大小
	DWORD		m_nMaxSize;	// 池大小上限
	DWORD		m_nObjSize;	// 粒度大小

public:
	CBlockPoolT(DWORD nObjSize, DWORD nSize = PolicyT::DEF_SIZE)
		: m_FreeList(NULL)
		, m_nFreSize(0)
		, m_nMaxSize(PolicyT::MAX_SIZE)
		, m_nObjSize(0)
	{
		SetObjSize(nObjSize);
		SetPoolSize(nSize);
	}
	virtual ~CBlockPoolT()
	{ Clear(); }

public:
	// 池大小设置
	DWORD GetPoolSize()
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		return m_nFreSize;
	}
	void SetPoolSize(DWORD nSize = PolicyT::DEF_SIZE)
	{
		DWORD diff_size = 0;
		{
			ExLock(m_Mutex, FALSE, mutex_t);
			if( nSize > m_nMaxSize )
				nSize = m_nMaxSize;
			if( m_nFreSize >= nSize ) return;
			diff_size = nSize - m_nFreSize;
		}
		for(DWORD i = 0; i < diff_size; ++i)
		{
			block_t* block = (block_t*)m_Alloc.Alloc(sizeof(block_t) + m_nObjSize);
			block->block_t::block_t();
			block->pNext = m_FreeList;
			m_FreeList = block;
			++m_nFreSize;
		}
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
	DWORD GetObjSize()
	{ return m_nObjSize; }
	void SetObjSize(DWORD nObjSize)
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		m_nObjSize = nObjSize;
	}

	// 内存效验
	BOOL Valid(void* pPtr)
	{
		block_t* block = (block_t*)((BYTE*)pPtr - sizeof(block_t));
		return m_Alloc.Valid(block);
	}
	// 内存大小
	DWORD Size(void* pPtr)
	{
		block_t* block = (block_t*)((BYTE*)pPtr - sizeof(block_t));
		return m_Alloc.Size(block) - sizeof(block_t);
	}
	// 分配内存
	void* Alloc(DWORD nSize)
	{
		if (nSize == 0) return NULL;
		ExLock(m_Mutex, FALSE, mutex_t);
		block_t* block = NULL;
		if (nSize <= m_nObjSize)
		{
			if (m_FreeList)
			{	// 提取内存块
				block = m_FreeList;
				m_FreeList = block->pNext;
				--m_nFreSize;
			} else // 分配新内存块
				block = (block_t*)m_Alloc.Alloc(sizeof(block_t) + m_nObjSize);
		} else // 分配任意大小内存块
			block = (block_t*)m_Alloc.Alloc(sizeof(block_t) + nSize);
		ExAssert(block);
		// 构造=>返回内存
		return (void*)(_Traits::Construct<block_t>(block) + 1);
	}
	void* Alloc() { return Alloc(m_nObjSize); }
	// 回收内存
	void Free(void* pPtr)
	{
		if (!pPtr) return;
		ExLock(m_Mutex, FALSE, mutex_t);
		block_t* block = (block_t*)((BYTE*)pPtr - sizeof(block_t));
		ExAssert(!block->bFree);
		if (block->bFree) return;
		// 析构=>归还/释放内存
		_Traits::Destruct<block_t>(block);
		if (m_nFreSize < m_nMaxSize)
		{
			block->pNext = m_FreeList;
			m_FreeList = block;
			++m_nFreSize;
		} else
			m_Alloc.Free(block);
	}
	// 清空内存
	void Clear()
	{
		ExLock(m_Mutex, FALSE, mutex_t);
		while (m_FreeList)
		{
			block_t* block = m_FreeList;
			m_FreeList = block->pNext;
			// 析构=>释放内存
			if(!block->bFree)
				_Traits::Destruct<block_t>(block);
			m_Alloc.Free(block);
		}
		m_FreeList = NULL;
		m_nFreSize = 0;
	}
};

typedef CBlockPoolT<> CBlockPool;

//////////////////////////////////////////////////////////////////

template <typename TypeT, typename PolicyT = _ObjPoolPolicyT<> >
class CObjPoolT : CBlockPoolT<PolicyT>
{
public:
	CObjPoolT(DWORD nSize = PolicyT::DEF_SIZE)
		: CBlockPoolT(sizeof(TypeT), nSize)
	{}
	virtual ~CObjPoolT()
	{}

public:
	// 分配内存
	void* Alloc(DWORD nSize = sizeof(TypeT))
	{
		if (nSize == 0) return NULL;
		if (nSize <= sizeof(TypeT)) nSize = sizeof(TypeT);
		// 构造=>返回内存
		return (void*)_Traits::Construct<TypeT>(CBlockPoolT::Alloc(nSize));
	}
	// 回收内存
	void Free(void* pPtr)
	{
		if (!pPtr) return;
		// 析构=>归还/释放内存
		_Traits::Destruct<TypeT>(pPtr);
		CBlockPoolT::Free(pPtr);
	}
	// 清空内存
	void Clear()
	{
		ExLock(m_Mutex, FALSE, mutex_t);
		while (m_FreeList)
		{
			block_t* block = m_FreeList;
			m_FreeList = block->pNext;
			// 析构=>释放内存
			if(!block->bFree)
			{
				_Traits::Destruct<TypeT>(block + 1);
				_Traits::Destruct<block_t>(block);
			}
			m_Alloc.Free(block);
		}
		m_FreeList = NULL;
		m_nFreSize = 0;
	}
};

//////////////////////////////////////////////////////////////////

template <typename TypeT, typename AllocT, typename ModelT = EXP_THREAD_MODEL>
interface IPoolTypeT
{
public:
	typedef CObjPoolT<TypeT, _ObjPoolPolicyT<AllocT, ModelT> > alloc_t;

	static alloc_t& GetAlloc()
	{ return ExSingleton<alloc_t>(); }

public:
	static TypeT* Alloc()
	{ return (TypeT*)GetAlloc().Alloc(); }
	virtual void Free()
	{
		if (GetAlloc().m_IsDest) return;
		GetAlloc().Free(static_cast<TypeT*>(this));
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ObjPool_h__*/