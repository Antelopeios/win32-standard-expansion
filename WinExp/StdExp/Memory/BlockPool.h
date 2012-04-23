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
// BlockPool - 对象池
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-29
// Version:	1.0.0020.0450
//
// History:
//	- 1.0.0012.1202(2011-03-02)	# 修正CObjPoolT::Valid()与CObjPoolT::Size()的内部指针传递错误
//	- 1.0.0014.2140(2011-03-03)	+ CObjPoolT支持对任意大小进行分配
//	- 1.0.0015.1419(2011-05-18)	+ CPoolTypeT支持重写CPoolTypeT::Free()
//								= CPoolTypeT改名为IPoolTypeT
//	- 1.0.0016.1640(2011-06-14)	# 修正因IObjPool由于某些原因(如单例)被提前析构,从而导致IPoolTypeT::Free()的R6025错误
//	- 1.0.0017.1715(2011-10-11)	+ 添加CBlockPoolT,支持按照动态大小创建粒度对象
//	- 1.0.0018.1915(2012-01-16)	# 修正CObjPoolT在析构时自动调用的是CBlockPoolT::Clear(),从而导致的内存释放错误
//	- 1.0.0019.1725(2012-01-27)	^ _ObjPoolPolicyT::MAX_SIZE调整至内存极限大小,默认不限制CObjPoolT的存储数量上限
//								+ _ObjPoolPolicyT支持通过模板参数调整默认粒度对象大小
//	- 1.0.0020.0450(2012-01-29)	= _ObjPoolPolicyT改名为_PoolPolicyT
//								- 彻底移除IObjPool,CObjPoolT与IPoolTypeT
//								+ 定义EXP_BLKPOOL_ALLOC作为上层的静态对象池分配器策略
//////////////////////////////////////////////////////////////////

#ifndef __BlockPool_h__
#define __BlockPool_h__

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

template <DWORD ObjSizeT = 0, typename AllocT = EXP_MEMHEAP_ALLOC, typename ModelT = EXP_THREAD_MODEL>
struct _PoolPolicyT
{
	typedef AllocT alloc_t;
	typedef ModelT model_t;
	typedef typename model_t::_LockPolicy mutex_policy_t;
	typedef CLockT<mutex_policy_t> mutex_t;

	static const DWORD OBJ_SIZE = ObjSizeT;
	static const DWORD DEF_SIZE = 0;
	static const DWORD MAX_SIZE = (DWORD)~0;
};

//////////////////////////////////////////////////////////////////

template <typename PolicyT = _PoolPolicyT<> >
class CBlockPoolT : INonCopyable
{
public:
	typedef typename PolicyT::alloc_t alloc_t;
	typedef typename PolicyT::mutex_t mutex_t;
	typedef typename PolicyT::model_t model_t;

protected:
#pragma pack(1)
	struct block_t
	{
		block_t* pNext;	// 下一个结点
	};
#pragma pack()

	mutex_t		m_Mutex;

	block_t*	m_FreeList;
	DWORD		m_nFreSize;	// 池大小
	DWORD		m_nMaxSize;	// 池大小上限
	DWORD		m_nObjSize;	// 粒度大小

public:
	CBlockPoolT(DWORD nObjSize = PolicyT::OBJ_SIZE, 
				DWORD nDefSize = PolicyT::DEF_SIZE, 
				DWORD nMaxSize = PolicyT::MAX_SIZE)
		: m_FreeList(NULL)
		, m_nFreSize(0)
		, m_nMaxSize(0)
		, m_nObjSize(0)
	{
		SetMaxSize(nMaxSize);
		SetObjSize(nObjSize);
		SetPoolSize(nDefSize);
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
			if (m_nObjSize == 0) return;
			if (nSize > m_nMaxSize)
				nSize = m_nMaxSize;
			if (m_nFreSize >= nSize) return;
			diff_size = nSize - m_nFreSize;
		}
		for(DWORD i = 0; i < diff_size; ++i)
		{
			block_t* block = (block_t*)alloc_t::Alloc(sizeof(block_t) + m_nObjSize);
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
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		return m_nObjSize;
	}
	void SetObjSize(DWORD nObjSize)
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		m_nObjSize = nObjSize;
	}

	// 内存效验
	BOOL Valid(void* pPtr)
	{
		block_t* block = (block_t*)((BYTE*)pPtr - sizeof(block_t));
		return alloc_t::Valid(block);
	}
	// 内存大小
	DWORD Size(void* pPtr)
	{
		block_t* block = (block_t*)((BYTE*)pPtr - sizeof(block_t));
		return alloc_t::Size(block) - sizeof(block_t);
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
				block = (block_t*)alloc_t::Alloc(sizeof(block_t) + m_nObjSize);
		} else // 分配任意大小内存块
			block = (block_t*)alloc_t::Alloc(sizeof(block_t) + nSize);
		ExAssert(block);
		// 返回内存
		return (void*)(block + 1);
	}
	void* Alloc() { return Alloc(m_nObjSize); }
	// 回收内存
	void Free(void* pPtr)
	{
		if (!pPtr) return;
		ExLock(m_Mutex, FALSE, mutex_t);
		block_t* block = (block_t*)((BYTE*)pPtr - sizeof(block_t));
		// 归还/释放内存
		if (alloc_t::Size(block) > (sizeof(block_t) + m_nObjSize))
			alloc_t::Free(block);
		else
		if (m_nFreSize < m_nMaxSize)
		{
			block->pNext = m_FreeList;
			m_FreeList = block;
			++m_nFreSize;
		} else
			alloc_t::Free(block);
	}
	// 清空内存
	void Clear()
	{
		ExLock(m_Mutex, FALSE, mutex_t);
		while (m_FreeList)
		{
			block_t* block = m_FreeList;
			m_FreeList = block->pNext;
			// 释放内存
			alloc_t::Free(block);
		}
		m_FreeList = NULL;
		m_nFreSize = 0;
	}
};

typedef CBlockPoolT<> CBlockPool;

#ifndef EXP_BLKPOOL_ALLOC
#define EXP_BLKPOOL_ALLOC CBlockPoolT<_PoolPolicyT<0, EXP_MEMHEAP_ALLOC, _SingleModel> >
#endif/*EXP_BLKPOOL_ALLOC*/

//////////////////////////////////////////////////////////////////

template <DWORD ObjSizeT, typename AllocT = EXP_MEMHEAP_ALLOC, typename ModelT = EXP_THREAD_MODEL>
class CPoolAllocT
{
public:
	typedef CPoolAllocT alloc_t;
	typedef CBlockPoolT<_PoolPolicyT<ObjSizeT, AllocT, ModelT> > mem_alloc_t;

	EXP_INLINE static mem_alloc_t& GetPool()
	{
		return EXP_SINGLETON<mem_alloc_t, ModelT>::Instance();
	}

public:
	EXP_INLINE static BOOL Valid(void* pPtr)
	{ return GetPool().Valid(pPtr); }
	EXP_INLINE static DWORD Size(void* pPtr)
	{ return GetPool().Size(pPtr); }

	EXP_INLINE static void* Alloc(DWORD nSize)
	{ return GetPool().Alloc(nSize); }
	EXP_INLINE static void Free(void* pPtr)
	{ GetPool().Free(pPtr); }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__BlockPool_h__*/