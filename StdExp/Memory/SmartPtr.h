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
// SmartPtr - ����ָ��
//
// Author:	ľͷ��
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-03-04
// Version:	1.3.0023.2110
//
// History:
//	- 1.0.0001.1148(2009-08-13)	@ ��ɻ�������ģ�幹��
//	- 1.0.0002.1758(2009-08-14)	+ ���TSmartPtr����ز���������(==)
//								+ ���CSmartPtrManager����ָ������༰ȫ�ֵ�����ָ���������
//	- 1.0.0003.1508(2009-08-17)	^ �Ż�CSmartPtrManagerʹ֧֮�����ü���
//								+ ���TSmartPtr����ز���������(*)
//	- 1.0.0004.1600(2009-08-18)	^ �Ż�CSmartPtrManagerָ����������listΪmap
//								- ɾ��CSmartPtrManager���Find()����
//								= ����TSmartPtr��ģ�����,��is_ary��ΪDelFunction����ָ��
//								= ����CSmartPtrManager��,���亯����Ϊ��̬����,�������Զ����ն���CAutoRecycle
//								+ ���CDelFunc��,����ͳһ����ָ��ɾ���㷨
//	- 1.0.0005.1720(2009-08-19)	+ ���TSmartPtr����ز���������([],+,-)
//								= ����TSmartPtr��=�������ķ���ֵΪTSmartPtr&
//								# ������ָ�����Ҹ���ʱ���µ�ָ�뱻���ٵ�bug
//	- 1.1.0006.2300(2009-08-20)	+ ���TSmartPtr����س�Ա�����Բ�ͬ�ͱ�ָ���֧��
//								- ɾ��CSmartPtrManager������,��TSmartPtr��ʹ��TPtrָ�������ָ���Զ�����ָ�����
//								= ����_Ptr��_DelFun�����ض��嵽CDelFunc���ڲ�
//								^ �Ż�TSmartPtr(TYPE*)���캯����ִ��Ч��
//								= �����༰���Ա������������ʽ
//	- 1.1.0007.1645(2009-08-21)	+ ���TPtr����ز���������(!=)
//								+ ���TSmartPtr����ز���������(!=)
//	- 1.1.0008.1255(2009-08-25)	# ����TSmartPtr��bool operator==()��bool operator!=()��NULLָ����жϴ���
//	- 1.1.0009.1353(2009-08-27)	= ����TPtr��TSmartPtr��ģ��TYPE������Ĭ��ֵΪCDelFunc::_Ptr
//	- 1.1.0010.1200(2009-08-31)	# ������ʹ����ָ��,����ָ������ʱ�������������������bug
//	- 1.2.0018.1500(2011-02-24)	# ��SmartPtr���Ͻ�StdExp֮��,SmartPtr::Release()û�е���alloc_t�ͷ�ptr��Դ
//								# �����򻯸�дCReferPtrTʱû�н��ڲ���protected��Ա��Ϊpublic��Ա����ı������
//								+ ���SmartPtr::Inc()��SmartPtr::Dec()�ӿ�,����ѭ�����õ��������ʱ�ֶ��������ü���
//	- 1.3.0019.2100(2011-02-24)	= �ı����ü����Ĺ���ʽ,��ԭΪ��ʼ��Manager��ͳһ����(������GCЭͬ����)
//	- 1.3.0020.0400(2011-02-25)	# ��ʹ�ö��߳�ģ��ʱ,PtrManager��DefMemAlloc�ľ�̬��ʼ��������ͻ
//	- 1.3.0021.1723(2011-02-28)	+ ���SmartPtr::operator!()����������
//	- 1.3.0022.2000(2011-03-01)	# ����SmartPtr::Inc()�ڲ���ģ����ô���
//	- 1.3.0023.2110(2011-03-04)	= CPtrManagerT::CReferPtrT������ڴ���CPtrManagerT��AllocT����
//								+ CPtrManagerT::Clear()֧�ִ��ζ����Ƿ���ȫ����CPtrManagerT::m_ReferPtrs�������ڴ�
//////////////////////////////////////////////////////////////////

#ifndef __SmartPtr_h__
#define __SmartPtr_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/MemAlloc.h"
#include "Container/Map.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = EXP_MEMORY_ALLOC, typename ModelT = EXP_THREAD_MODEL>
class CPtrManagerT : CNonCopyable
{
protected:
	// ����ָ��ӿ�
	class IReferPtr
	{
	protected:
		void*		  p_ptr;
		volatile long n_ref;

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
			if (RefModelT::Dec(&n_ref) <= 0)
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

public:
	CPtrManagerT()
		: m_ReferPtrs(1021)
	{}
	~CPtrManagerT()
	{ Clear(true); }

public:
	EXP_INLINE static CPtrManagerT& Instance()
	{
		ExLockThis(model_t::_ExcPolicy);
		static CPtrManagerT instance;
		return instance;
	}

public:
	// ��ȡָ�����ü���
	EXP_INLINE long Get(void* pPtr)
	{
		if (!pPtr) return 0;
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

// ����ָ����ģ��
template <typename TypeT, typename AllocT = EXP_MEMORY_ALLOC, typename ModelT = EXP_THREAD_MODEL>
class CSmartPtrT
{
public:
	typedef AllocT alloc_t;
	typedef ModelT model_t;

	// ��Ա����
protected:
	TypeT* m_Ptr;

	// ����/����
public:
	CSmartPtrT(void)
		: m_Ptr(NULL)
	{}
	CSmartPtrT(TypeT* ptr)
		: m_Ptr(NULL)
	{ (*this) = ptr; }
	CSmartPtrT(const CSmartPtrT& ptr)
		: m_Ptr(NULL)
	{ (*this) = ptr; }

	//////////////////////////////////

	template <typename Type2T>
	CSmartPtrT(Type2T* ptr)
		: m_Ptr(NULL)
	{ (*this) = ptr; }
	template <typename Type2T>
	CSmartPtrT(const CSmartPtrT<Type2T>& ptr)
		: m_Ptr(NULL)
	{ (*this) = ptr; }

	//////////////////////////////////

	~CSmartPtrT(void)
	{ Dec(); }

	// ����
public:
	long GetRefCount()
	{ return CPtrManager::Instance().Get(m_Ptr); }

	void Inc()
	{ CPtrManager::Instance().Add<alloc_t, model_t>(m_Ptr); }
	void Dec()
	{ CPtrManager::Instance().Del(m_Ptr); }

	void Release()
	{
		CPtrManager::Instance().Del(m_Ptr, true);
		m_Ptr = NULL;
	}

	//////////////////////////////////

	CSmartPtrT& operator=(const CSmartPtrT& ptr)
	{
		if( (*this) == ptr ) return (*this);
		Dec();
		m_Ptr = ptr.m_Ptr;
		Inc();
		return (*this);
	}

	bool operator==(TypeT* ptr) const
	{
		if( m_Ptr )
			return (m_Ptr == ptr);
		else if( !ptr )
			return true;
		else
			return false;
	}
	bool operator==(const CSmartPtrT& ptr) const
	{ return (m_Ptr == ptr.m_Ptr); }
	bool operator!=(TypeT* ptr) const
	{
		if( m_Ptr )
			return (m_Ptr != ptr);
		else if( ptr )
			return true;
		else
			return false;
	}
	bool operator!=(const CSmartPtrT& ptr) const
	{ return (m_Ptr != ptr.m_Ptr); }

	TypeT* operator+(DWORD offset) const
	{ return (m_Ptr + offset); }
	TypeT* operator-(DWORD offset) const
	{ return (m_Ptr - offset); }

	bool operator!() const
	{ return !m_Ptr; }

	TypeT* operator->() const
	{ return m_Ptr; }
	operator TypeT*() const
	{ return m_Ptr; }

	//////////////////////////////////

	template <typename Type2T>
	CSmartPtrT& operator=(const CSmartPtrT<Type2T>& ptr)
	{
		if( (*this) == ptr ) return (*this);
		Dec();
		m_Ptr = (TypeT*)ptr.m_Ptr;
		Inc();
		return (*this);
	}

	template <typename Type2T>
	bool operator==(Type2T* ptr) const
	{ return ((*this) == (TypeT*)ptr); }
	template <typename Type2T>
	bool operator==(const CSmartPtrT<Type2T>& ptr) const
	{ return (m_Ptr == (TypeT*)ptr.m_Ptr); }
	template <typename Type2T>
	bool operator!=(Type2T* ptr) const
	{ return ((*this) != (TypeT*)ptr); }
	template <typename Type2T>
	bool operator!=(const CSmartPtrT<Type2T>& ptr) const
	{ return (m_Ptr != (TypeT*)ptr.m_Ptr); }

	template <typename Type2T>
	operator Type2T*() const
	{ return (Type2T*)m_Ptr; }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__SmartPtr_h__*/