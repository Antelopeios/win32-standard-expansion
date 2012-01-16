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
// GC - 垃圾回收器
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-16
// Version:	1.1.0019.1800
//
// History:
//	- 1.1.0013.2300(2011-02-24)	^ 优化CGCT::Free()的实现
//								+ 支持与SmartPtr智能指针同时使用
//	- 1.1.0014.2000(2011-03-01)	# 在GC内部也调用引用计数机制,而不是忽略计数;否则当SmartPtr释放指针后,GC将再次释放同一个指针
//	- 1.1.0015.0120(2011-04-04)	+ ExGC支持不使用GC进行内存分配,方便提供统一的内存分配接口
//	- 1.1.0015.0120(2011-04-04)	+ CGCAllocT支持与ExMem同样的方式直接分配内存
//								= 调整ExGC为ExMem,统一所有内存分配接口的调用方式
//	- 1.1.0017.1640(2011-05-19)	+ 添加CGCT::Regist接口,支持从外部直接注册指针
//								= CGCT::CheckAlloc()与CGCT::CheckFree()改为静态函数
//								= CGCAllocT在不使用外部GC构造对象时仍然向指针管理器注册指针(统一所有的指针管理)
//	- 1.1.0018.1742(2012-01-14)	+ 添加CGCT::ReAlloc接口
//	- 1.1.0019.1800(2012-01-16)	+ 支持通过EXP_MANAGED_ALLPTR关闭CGCAllocT的托管非GC内存分配
//////////////////////////////////////////////////////////////////

#ifndef __GC_h__
#define __GC_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/PtrManager.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = EXP_MEMORY_ALLOC, typename ModelT = EXP_THREAD_MODEL>
struct _GCPolicyT;

template <typename PolicyT = _GCPolicyT<> >
class CGCT : INonCopyable
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
	CGCT(DWORD nSize = PolicyT::DEF_SIZE)
		: m_BlockArray	(NULL)
		, m_nSize		(0)
		, m_nIndx		(0)
	{ SetGCSize(nSize); }
	~CGCT()
	{
		Clear();
		alloc_t::Free(m_BlockArray);
	}

public:
	template <typename TypeT>
	EXP_INLINE static TypeT* CheckAlloc(DWORD nCount = 1)
	{
		TypeT* ptr = alloc_t::Alloc<TypeT>(nCount);
		EXP_PTR_MANAGER.Add<alloc_t, model_t>(ptr);
		return ptr;
	}

	EXP_INLINE static void* CheckReAlloc(void* pPtr, DWORD nSize)
	{
		// 检查指针是否为空
		if (pPtr == NULL)
			return CheckAlloc<BYTE>(nSize);
		// 检查大小是否为零
		if (nSize == 0)
			return NULL;
		// 检查指针现有大小
		DWORD ptr_siz = alloc_t::Size(pPtr);
		// 若现有大小可以满足需求, 则直接返回
		if (ptr_siz >= nSize)
			return pPtr;
		// 若现有大小无法满足需求, 则分配新的空间并拷贝原有数据
		void* ptr_new = CheckAlloc<BYTE>(nSize);
		memcpy(ptr_new, pPtr, ptr_siz);
		return ptr_new;
	}
	template <typename TypeT>
	EXP_INLINE static TypeT* CheckReAlloc(void* pPtr, DWORD nCount, _true_type)
	{
		return (TypeT*)CheckReAlloc(pPtr, sizeof(TypeT) * nCount);
	}
	template <typename TypeT>
	EXP_INLINE static TypeT* CheckReAlloc(void* pPtr, DWORD nCount, _false_type)
	{
		// 检查指针是否为空
		if (pPtr == NULL)
			return CheckAlloc<TypeT>(nCount);
		// 检查大小是否为零
		if (nCount == 0)
			return NULL;
		// 检查指针现有对象数量
		_Regist* real = (_Regist*)_Regist::RealPtr(pPtr);
		// 若现有数量等于需求数量, 则直接返回
		if (real->count == nCount)
			return (TypeT*)pPtr;
		// 若现有数量大于需求数量, 则销毁多余对象, 并直接返回
		if (real->count > nCount)
		{
			if (real->destruct)
				real->destruct(((TypeT*)pPtr) + nCount, real->count - nCount);
			real->count = nCount;
			return (TypeT*)pPtr;
		}
		// 若现有数量小于需求数量, 则检查指针现有大小
		DWORD ptr_siz = alloc_t::Size(pPtr);
		DWORD size = sizeof(TypeT) * nCount;
		// 若现有大小可以满足需求, 则构造新对象, 并直接返回
		if (ptr_siz >= size)
		{
			_Traits::Construct<TypeT>(((TypeT*)pPtr) + real->count, nCount - real->count);
			real->count = nCount;
			return (TypeT*)pPtr;
		}
		// 若现有大小无法满足需求, 则分配新的空间, 拷贝原有对象数据, 并构造新对象
		void* ptr_new = alloc_t::Alloc(size);
		_Regist* real_new = (_Regist*)_Regist::RealPtr(ptr_new);
		real_new->count = nCount;
		real_new->destruct = (_Regist::traitor_t)(_Traits::Destruct<TypeT>);
		memcpy(ptr_new, pPtr, sizeof(TypeT) * real->count);
		_Traits::Construct<TypeT>(((TypeT*)ptr_new) + real->count, nCount - real->count);
		// 注册新空间
		EXP_PTR_MANAGER.Add<alloc_t, model_t>(ptr_new);
		return (TypeT*)ptr_new;
	}
	template <typename TypeT>
	EXP_INLINE static TypeT* CheckReAlloc(void* pPtr, DWORD nCount)
	{
		return CheckReAlloc<TypeT>(pPtr, nCount, _TraitsT<TypeT>::is_POD_type());
	}

	EXP_INLINE static void CheckFree(void* pPtr)
	{
		EXP_PTR_MANAGER.Del(pPtr);
	}

public:
	template <typename TypeT>
	EXP_INLINE TypeT* Construct(void* pPtr)
	{
		ExLock(m_Mutex, FALSE, mutex_t);
		return alloc_t::Construct<TypeT>(pPtr);
	}
	EXP_INLINE void* Destruct(void* pPtr)
	{
		ExLock(m_Mutex, FALSE, mutex_t);
		return alloc_t::Destruct(pPtr);
	}

public:
	DWORD GetGCSize()
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		return m_nSize;
	}
	void SetGCSize(DWORD nSize = PolicyT::DEF_SIZE)
	{
		ExLock(m_Mutex, FALSE, mutex_t);
		if( GetGCSize() >= nSize ) return;
		m_BlockArray = alloc_t::ReAlloc<void*>(m_BlockArray, nSize);
		m_nSize = nSize;
	}

	// 内存效验
	BOOL Valid(void* pPtr)
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		return alloc_t::Valid(pPtr);
	}
	// 内存大小
	DWORD Size(void* pPtr)
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		return alloc_t::Size(pPtr);
	}
	// 注册内存
	void* Regist(void* pPtr)
	{
		ExLock(m_Mutex, FALSE, mutex_t);
		if (GetGCSize() == m_nIndx)
			SetGCSize(PolicyT::Expan(GetGCSize()));
		m_BlockArray[m_nIndx] = pPtr;
		ExAssert(m_BlockArray[m_nIndx]);
		return m_BlockArray[m_nIndx++];
	}
	// 分配内存
	template <typename TypeT>
	TypeT* Alloc(DWORD nCount = 1)
	{
		if (nCount == 0) return NULL;
		ExLock(m_Mutex, FALSE, mutex_t);
		return (TypeT*)Regist(CheckAlloc<TypeT>(nCount));
	}
	void* Alloc(DWORD nSize)
	{
		if (nSize == 0) return NULL;
		return (void*)Alloc<BYTE>(nSize);
	}
	template <typename TypeT>
	TypeT* ReAlloc(void* pPtr, DWORD nCount)
	{
		if (nCount == 0) return NULL;
		TypeT* ptr_new = CheckReAlloc<TypeT>(pPtr, nCount);
		if (ptr_new == pPtr)
			return ptr_new;
		else
			return (TypeT*)Regist(ptr_new);
	}
	void* ReAlloc(void* pPtr, DWORD nSize)
	{
		if (nSize == 0) return NULL;
		return (void*)ReAlloc<BYTE>(pPtr, nSize);
	}
	// 回收内存
	void Free(void* pPtr)
	{
		if (!pPtr) return;
		ExLock(m_Mutex, FALSE, mutex_t);
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
		ExLock(m_Mutex, FALSE, mutex_t);
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
	typedef GCT gc_alloc_t;
	typedef typename gc_alloc_t::alloc_t alloc_t;

public:
	// alloc_t
	EXP_INLINE static BOOL Valid(void* pPtr)
	{ return alloc_t::Valid(pPtr); }
	EXP_INLINE static DWORD Size(void* pPtr)
	{ return alloc_t::Size(pPtr); }

#ifdef	EXP_MANAGED_ALLPTR
	template <typename TypeT>
	EXP_INLINE static TypeT* Alloc(DWORD nCount = 1)
	{ return gc_alloc_t::CheckAlloc<TypeT>(nCount); }
	EXP_INLINE static void* Alloc(DWORD nSize)
	{ return gc_alloc_t::CheckAlloc<BYTE>(nSize); }

	template <typename TypeT>
	EXP_INLINE static TypeT* ReAlloc(void* pPtr, DWORD nCount)
	{
		TypeT* ptr_new = gc_alloc_t::CheckReAlloc<TypeT>(pPtr, nCount);
		if (ptr_new != pPtr) Free(pPtr);
		return ptr_new;
	}
	EXP_INLINE static void* ReAlloc(void* pPtr, DWORD nSize)
	{ return ReAlloc<BYTE>(pPtr, nSize); }

	EXP_INLINE static void Free(void* pPtr)
	{ gc_alloc_t::CheckFree(pPtr); }
#else /*EXP_MANAGED_ALLPTR*/
	template <typename TypeT>
	EXP_INLINE static TypeT* Alloc(DWORD nCount = 1)
	{ return alloc_t::Alloc<TypeT>(nCount); }
	EXP_INLINE static void* Alloc(DWORD nSize)
	{ return alloc_t::Alloc(nSize); }

	template <typename TypeT>
	EXP_INLINE static TypeT* ReAlloc(void* pPtr, DWORD nCount)
	{ return alloc_t::ReAlloc<TypeT>(pPtr, nCount); }
	EXP_INLINE static void* ReAlloc(void* pPtr, DWORD nSize)
	{ return alloc_t::ReAlloc(pPtr, nSize); }

	EXP_INLINE static void Free(void* pPtr)
	{ alloc_t::Free(pPtr); }
#endif/*EXP_MANAGED_ALLPTR*/

	// gc_alloc_t
	EXP_INLINE static BOOL Valid(gc_alloc_t* alloc, void* pPtr)
	{ return alloc ? alloc->Valid(pPtr) : Valid(pPtr); }
	EXP_INLINE static DWORD Size(gc_alloc_t* alloc, void* pPtr)
	{ return alloc ? alloc->Size(pPtr) : Size(pPtr); }

	template <typename TypeT>
	EXP_INLINE static TypeT* Alloc(gc_alloc_t* alloc, DWORD nCount = 1)
	{ return alloc ? alloc->Alloc<TypeT>(nCount) : Alloc<TypeT>(nCount); }
	EXP_INLINE static void* Alloc(gc_alloc_t* alloc, DWORD nSize)
	{ return alloc ? alloc->Alloc(nSize) : Alloc(nSize); }

	template <typename TypeT>
	EXP_INLINE static TypeT* ReAlloc(gc_alloc_t* alloc, void* pPtr, DWORD nCount)
	{ return alloc ? alloc->ReAlloc<TypeT>(pPtr, nCount) : ReAlloc<TypeT>(pPtr, nCount); }
	EXP_INLINE static void* ReAlloc(gc_alloc_t* alloc, void* pPtr, DWORD nSize)
	{ return alloc ? alloc->ReAlloc(pPtr, nSize) : ReAlloc(pPtr, nSize); }

	EXP_INLINE static void Free(gc_alloc_t* alloc, void* pPtr)
	{ if (alloc) alloc->Free(pPtr); else Free(pPtr); }
};

typedef CGCAllocT<> CGCAlloc;
typedef CGCAlloc ExMem;

//////////////////////////////////////////////////////////////////

template <typename AllocT/* = EXP_MEMORY_ALLOC*/, typename ModelT/* = EXP_THREAD_MODEL*/>
struct _GCPolicyT
{
	typedef AllocT alloc_t;
	typedef ModelT model_t;
	typedef typename model_t::_LockPolicy mutex_policy_t;
	typedef CLockT<mutex_policy_t> mutex_t;

	static const DWORD DEF_SIZE = 100;
	static DWORD Expan(DWORD nSize)
	{ return nSize ? (nSize << 1) : 1; }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GC_h__*/