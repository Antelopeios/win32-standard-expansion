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
// Date:	2011-02-15
// Version:	1.2.0017.1620
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
//////////////////////////////////////////////////////////////////

#ifndef __SmartPtr_h__
#define __SmartPtr_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread/ThreadModel.h"
#include "Memory/MemAlloc.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// ����ָ����ģ��
template <typename TypeT, typename AllocT = DefMemAlloc, typename ModelT = DefThreadModel>
class CReferPtrT
{
protected:
	typedef AllocT alloc_t;

	// ��Ա����
protected:
	TypeT*	p_ptr;
	volatile long n_ref;

	// ����/����
protected:
	CReferPtrT()
		: p_ptr(NULL)
		, n_ref(0)
	{}
	~CReferPtrT()
	{ alloc_t::Free(p_ptr); }

	// ����
protected:
	void InitPtr(TypeT* pt)
	{
		p_ptr = pt;
		n_ref = 1;
	}

	long GetRefCount()
	{ return n_ref; }
	TypeT* GetPtr()
	{ return p_ptr; }

	//////////////////////////////////

	bool operator==(TypeT* pt) const
	{ return (p_ptr == pt); }
	bool operator!=(TypeT* pt) const
	{ return (p_ptr != pt); }

	void operator++()
	{
		ModelT::Inc(&n_ref);
	}
	void operator--()
	{
		if (ModelT::Dec(&n_ref) <= 0)
			alloc_t::Free(this);
	}

	TypeT& operator*()
	{ return *p_ptr; }
	operator TypeT*()
	{ return p_ptr; }
};
// ����ָ����ģ�� ����

//////////////////////////////////////////////////////////////////

// ����ָ����ģ��
template <typename TypeT, typename RefPtrT = CReferPtrT<TypeT> >
class CSmartPtrT
{
public:
	typedef RefPtrT ref_ptr_t;
	typedef typename ref_ptr_t::alloc_t alloc_t;

	// ��Ա����
protected:
	ref_ptr_t* m_ptr;

	// ����/����
public:
	CSmartPtrT(void)
		: m_ptr(NULL)
	{}
	CSmartPtrT(TypeT* pt)
		: m_ptr(NULL)
	{
		if( !pt ) return ;
		m_ptr = alloc_t::Alloc<ref_ptr_t>();
		m_ptr->InitPtr(pt);
	}
	CSmartPtrT(const CSmartPtrT& ptr)
		: m_ptr(NULL)
	{ (*this) = ptr; }

	//////////////////////////////////

	template <typename Type2T>
	CSmartPtrT(Type2T* pt)
		: m_ptr(NULL)
	{
		if( !pt ) return ;
		m_ptr = alloc_t::Alloc<ref_ptr_t>();
		m_ptr->InitPtr((TypeT*)pt);
	}
	template <typename Type2T>
	CSmartPtrT(const CSmartPtrT<Type2T>& ptr)
		: m_ptr(NULL)
	{ (*this) = ptr; }

	//////////////////////////////////

	~CSmartPtrT(void)
	{ if( m_ptr ) -- (*m_ptr); }

	// ����
public:
	long GetRefCount()
	{
		if( m_ptr )
			return m_ptr->GetRefCount();
		else
			return 0;
	}

	void Release()
	{
		if( m_ptr ) delete m_ptr;
		m_ptr = NULL;
	}

	//////////////////////////////////

	CSmartPtrT& operator=(const CSmartPtrT& ptr)
	{
		if( (*this) == ptr ) return (*this);
		if( m_ptr ) -- (*m_ptr);
		m_ptr = ptr.m_ptr;
		if( m_ptr ) ++ (*m_ptr);
		return (*this);
	}
	bool operator==(TypeT* pt) const
	{
		if( m_ptr )
			return ((*m_ptr) == (TypeT*)pt);
		else if( !pt )
			return true;
		else
			return false;
	}
	bool operator==(const CSmartPtrT& ptr) const
	{ return (m_ptr == (ref_ptr_t*)ptr); }
	bool operator!=(TypeT* pt) const
	{
		if( m_ptr )
			return ((*m_ptr) != (TypeT*)pt);
		else if( pt )
			return true;
		else
			return false;
	}
	bool operator!=(const CSmartPtrT& ptr) const
	{ return (m_ptr != (ref_ptr_t*)ptr); }

	TypeT* operator+(DWORD offset) const
	{ return (m_ptr + offset); }
	TypeT* operator-(DWORD offset) const
	{ return (m_ptr - offset); }

	TypeT* operator->() const
	{ return m_ptr->GetPtr(); }
	TypeT& operator*()
	{ return *(*m_ptr); }
	TypeT& operator[](DWORD inx)
	{ return (m_ptr->GetPtr())[inx]; }
	operator TypeT*() const
	{ return m_ptr->GetPtr(); }

	//////////////////////////////////

	template <typename Type2T>
	CSmartPtrT& operator=(const CSmartPtrT<Type2T>& ptr)
	{
		if( (*this) == ptr ) return (*this);
		if( m_ptr ) -- (*m_ptr);
		m_ptr = (ref_ptr_t*)(CSmartPtrT<Type2T>::ref_ptr_t*)ptr;
		if( m_ptr ) ++ (*m_ptr);
		return (*this);
	}

	template <typename Type2T>
	bool operator==(Type2T* pt) const
	{ return ((*this) == (TypeT*)pt); }

	template <typename Type2T>
	bool operator==(const CSmartPtrT<Type2T>& ptr) const
	{ return (m_ptr == (ref_ptr_t*)(CSmartPtrT<Type2T>::ref_ptr_t*)ptr); }

	template <typename Type2T>
	bool operator!=(Type2T* pt) const
	{ return ((*this) != (TypeT*)pt); }

	template <typename Type2T>
	bool operator!=(const CSmartPtrT<Type2T>& ptr) const
	{ return (m_ptr != (ref_ptr_t*)(CSmartPtrT<Type2T>::ref_ptr_t*)ptr); }

	template <typename Type2T>
	operator Type2T*() const
	{ return (Type2T*)(m_ptr->GetPtr()); }

	//////////////////////////////////

	operator ref_ptr_t*() const
	{ return m_ptr; }
};
// ����ָ����ģ�� ����

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__SmartPtr_h__*/