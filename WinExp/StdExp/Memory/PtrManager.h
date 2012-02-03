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
// PtrManager - 指针管理器
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-29
// Version:	1.3.0031.0457
//
// History:
//	- 1.3.0028.1020(2011-05-30)	= 将CPtrManagerT独立作为一个单独的模块维护
//	- 1.3.0029.1116(2011-05-31)	# 添加一个bool标记,防止CPtrManagerT在析构后继续被调用导致Crash
//	- 1.3.0030.1540(2011-07-20)	= 将CPtrManager的单例独立到CPtrManager外部,定义EXP_PTR_MANAGER,可由外部按需要自行替换
//	- 1.3.0031.0457(2012-01-29)	^ 简化CPtrManagerT::CReferPtrT<>,不再指定线程模型
//////////////////////////////////////////////////////////////////

#ifndef __PtrManager_h__
#define __PtrManager_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/OverNew.h"
#include "Container/Map.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename ModelT = EXP_THREAD_MODEL>
class CPtrManagerT : INonCopyable
{
protected:
	// 计数指针接口
	interface IReferPtr
	{
	protected:
		void*		  p_ptr;
		volatile LONG n_ref;

	public:
		IReferPtr()
			: p_ptr(NULL)
			, n_ref(0)
		{}
		virtual ~IReferPtr()
		{}

	public:
		EXP_INLINE void InitPtr(void* pt)
		{
			p_ptr = pt;
			n_ref = 1;
		}

		EXP_INLINE long GetRefCount()
		{ return n_ref; }
		EXP_INLINE void* GetPtr()
		{ return p_ptr; }

		EXP_INLINE BOOL operator==(void* pt) const
		{ return (p_ptr == pt); }
		EXP_INLINE BOOL operator!=(void* pt) const
		{ return (p_ptr != pt); }

		virtual void Inc() = 0;
		virtual BOOL Dec() = 0;

		virtual void Free() = 0;
	};
	// 计数指针类
	template <typename AllocT = EXP_MEMORY_ALLOC>
	class CReferPtrT : public IReferPtr
	{
	public:
		CReferPtrT()
			: IReferPtr()
		{}
		~CReferPtrT()
		{ if (p_ptr) AllocT::Free(p_ptr); }

	public:
		void Inc()
		{
			ModelT::Inc(&n_ref);
		}
		BOOL Dec()
		{
			if (ModelT::Dec(&n_ref) == 0)
			{
				Free();
				return TRUE;
			}
			else
				return FALSE;
		}

		EXP_INLINE static CReferPtrT* Alloc()
		{ return dbnew(CReferPtrT); }
		void Free()
		{ del(this); }
	};

public:
	typedef ModelT model_t;
	typedef typename model_t::_LockPolicy mutex_policy_t;
	typedef CLockT<mutex_policy_t> mutex_t;
	typedef CMapT<void*, IReferPtr*, _MapPolicyT<1009, CHash> > ptr_map_t;

protected:
	mutex_t		m_Mutex;
	ptr_map_t	m_ReferPtrs;
	BOOL		m_IsDest;

public:
	CPtrManagerT()
		: m_IsDest(FALSE)
	{}
	~CPtrManagerT()
	{
		/*Clear(TRUE);*//*单例情况下,内部对象会被内存池自动回收*/
		m_IsDest = TRUE;
	}

public:
	// 获取指针引用计数
	EXP_INLINE long Get(void* pPtr)
	{
		if (!pPtr) return 0;
		if (m_IsDest) return 0;
		ExLock(m_Mutex, TRUE, mutex_t);
		ptr_map_t::iterator_t ite = m_ReferPtrs.Locate(pPtr);
		if (ite == m_ReferPtrs.Tail()) return 0;
		IReferPtr* ref_ptr = ite->Val();
		if (ref_ptr && ref_ptr->GetPtr())
			return ref_ptr->GetRefCount();
		else
			return 0;
	}
	// 添加指针引用计数
	template <typename AllocT>
	EXP_INLINE void Add(void* pPtr)
	{
		if (!pPtr) return;
		IReferPtr* ref_ptr = NULL;
		if (m_IsDest) return;
		ExLock(m_Mutex, FALSE, mutex_t);
		ptr_map_t::iterator_t ite = m_ReferPtrs.Locate(pPtr);
		if (ite == m_ReferPtrs.Tail())
		{
			ref_ptr = CReferPtrT<AllocT>::Alloc();
			ref_ptr->InitPtr(pPtr);
			m_ReferPtrs.Add(pPtr, ref_ptr);
		}
		else
		{
			ref_ptr = ite->Val();
			if (ref_ptr && ref_ptr->GetPtr())
				ref_ptr->Inc();
			else
			{
				if (ref_ptr) ref_ptr->Free();
				m_ReferPtrs.Del(ite);
			}
		}
	}
	EXP_INLINE void Add(void* pPtr)
	{
		Add<EXP_MEMORY_ALLOC>(pPtr);
	}
	// 删除指针引用计数
	EXP_INLINE void Del(void* pPtr, BOOL bRelease = FALSE)
	{
		if (!pPtr) return;
		if (m_IsDest) return;
		ExLock(m_Mutex, FALSE, mutex_t);
		ptr_map_t::iterator_t ite = m_ReferPtrs.Locate(pPtr);
		if (ite == m_ReferPtrs.Tail()) return;
		IReferPtr* ref_ptr = ite->Val();
		if (!bRelease && ref_ptr && ref_ptr->GetPtr())
		{
			if (ref_ptr->Dec())
				m_ReferPtrs.Del(ite);
		}
		else
		{
			if (ref_ptr) ref_ptr->Free();
			m_ReferPtrs.Del(ite);
		}
	}
	// 清空指针记录表
	EXP_INLINE void Clear(BOOL bNull = FALSE)
	{
		if (m_IsDest) return;
		ExLock(m_Mutex, FALSE, mutex_t);
		for(ptr_map_t::iterator_t ite = m_ReferPtrs.Head(); ite != m_ReferPtrs.Tail(); ++ite)
		{
			IReferPtr* ref_ptr = ite->Val();
			if (ref_ptr) ref_ptr->Free();
		}
		if (bNull)
			m_ReferPtrs.Null();
		else
			m_ReferPtrs.Clear();
	}
};

typedef CPtrManagerT<> CPtrManager;

#ifndef EXP_PTR_MANAGER
#define EXP_PTR_MANAGER ExSingleton<CPtrManager>()
#endif/*EXP_PTR_MANAGER*/

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__PtrManager_h__*/