// Copyright 2011, ľͷ��
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
// PtrManager - ָ�������
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-31
// Version:	1.3.0029.1116
//
// History:
//	- 1.3.0028.1020(2011-05-30)	= ��CPtrManagerT������Ϊһ��������ģ��ά��
//	- 1.3.0029.1116(2011-05-31)	# ���һ��bool���,��ֹCPtrManagerT����������������õ���Crash
//////////////////////////////////////////////////////////////////

#ifndef __PtrManager_h__
#define __PtrManager_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Container/Map.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = EXP_MEMORY_ALLOC, typename ModelT = EXP_THREAD_MODEL>
class CPtrManagerT : INonCopyable, public EXP_SINGLETON<CPtrManagerT<AllocT, ModelT> >
{
protected:
	// ����ָ��ӿ�
	class IReferPtr
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

		EXP_INLINE bool operator==(void* pt) const
		{ return (p_ptr == pt); }
		EXP_INLINE bool operator!=(void* pt) const
		{ return (p_ptr != pt); }

		virtual void Inc() = 0;
		virtual bool Dec() = 0;

		virtual void Free() = 0;
	};
	// ����ָ����
	template <typename RefAllocT = EXP_MEMORY_ALLOC, typename RefModelT = EXP_THREAD_MODEL>
	class CReferPtrT : public IReferPtr
	{
	public:
		CReferPtrT()
			: IReferPtr()
		{}
		~CReferPtrT()
		{ if (p_ptr) RefAllocT::Free(p_ptr); }

	public:
		void Inc()
		{
			RefModelT::Inc(&n_ref);
		}
		bool Dec()
		{
			if (RefModelT::Dec(&n_ref) == 0)
			{
				Free();
				return true;
			}
			else
				return false;
		}

		EXP_INLINE static CReferPtrT* Alloc()
		{ return AllocT::Alloc<CReferPtrT>(); }
		void Free()
		{ AllocT::Free(this); }
	};

public:
	typedef AllocT alloc_t;
	typedef ModelT model_t;
	typedef typename model_t::_LockPolicy mutex_policy_t;
	typedef CLockT<mutex_policy_t> mutex_t;
	typedef CMapT<void*, IReferPtr*, _MapPolicyT<CHash, alloc_t> > ptr_map_t;

protected:
	mutex_t		m_Mutex;
	ptr_map_t	m_ReferPtrs;
	bool		m_IsDest;

public:
	CPtrManagerT()
		: m_ReferPtrs(1021)
		, m_IsDest(false)
	{}
	~CPtrManagerT()
	{
		/*Clear(true);*//*���������,�ڲ�����ᱻ�ڴ���Զ�����*/
		m_IsDest = true;
	}

public:
	// ��ȡָ�����ü���
	EXP_INLINE long Get(void* pPtr)
	{
		if (!pPtr) return 0;
		if (m_IsDest) return 0;
		ExLock(m_Mutex, true, mutex_t);
		ptr_map_t::iterator_t ite = m_ReferPtrs.Locate(pPtr);
		if (ite == m_ReferPtrs.Tail()) return 0;
		IReferPtr* ref_ptr = ite->Val();
		if (ref_ptr && ref_ptr->GetPtr())
			return ref_ptr->GetRefCount();
		else
			return 0;
	}
	// ���ָ�����ü���
	template <typename RefAllocT, typename RefModelT>
	EXP_INLINE void Add(void* pPtr)
	{
		if (!pPtr) return;
		IReferPtr* ref_ptr = NULL;
		if (m_IsDest) return;
		ExLock(m_Mutex, false, mutex_t);
		ptr_map_t::iterator_t ite = m_ReferPtrs.Locate(pPtr);
		if (ite == m_ReferPtrs.Tail())
		{
			ref_ptr = CReferPtrT<RefAllocT, RefModelT>::Alloc();
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
	// ɾ��ָ�����ü���
	EXP_INLINE void Del(void* pPtr, bool bRelease = false)
	{
		if (!pPtr) return;
		if (m_IsDest) return;
		ExLock(m_Mutex, false, mutex_t);
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
	// ���ָ���¼��
	EXP_INLINE void Clear(bool bNull = false)
	{
		if (m_IsDest) return;
		ExLock(m_Mutex, false, mutex_t);
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

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__PtrManager_h__*/