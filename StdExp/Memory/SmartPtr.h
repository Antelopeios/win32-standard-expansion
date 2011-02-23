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
// SmartPtr - 智能指针
//
// Author:	木头云
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-02-15
// Version:	1.2.0017.1620
//
// History:
//	- 1.0.0001.1148(2009-08-13)	@ 完成基本的类模板构建
//	- 1.0.0002.1758(2009-08-14)	+ 添加TSmartPtr类相关操作符重载(==)
//								+ 添加CSmartPtrManager智能指针管理类及全局的智能指针管理链表
//	- 1.0.0003.1508(2009-08-17)	^ 优化CSmartPtrManager使之支持引用计数
//								+ 添加TSmartPtr类相关操作符重载(*)
//	- 1.0.0004.1600(2009-08-18)	^ 优化CSmartPtrManager指针管理链表的list为map
//								- 删除CSmartPtrManager类的Find()函数
//								= 调整TSmartPtr类模板参数,将is_ary改为DelFunction函数指针
//								= 调整CSmartPtrManager类,将其函数改为静态函数,并定义自动回收对象CAutoRecycle
//								+ 添加CDelFunc类,用于统一管理指针删除算法
//	- 1.0.0005.1720(2009-08-19)	+ 添加TSmartPtr类相关操作符重载([],+,-)
//								= 调整TSmartPtr类=操作符的返回值为TSmartPtr&
//								# 修正当指针自我复制时导致的指针被销毁的bug
//	- 1.1.0006.2300(2009-08-20)	+ 添加TSmartPtr类相关成员函数对不同型别指针的支持
//								- 删除CSmartPtrManager管理类,让TSmartPtr类使用TPtr指针计数类指针自动管理指针计数
//								= 调整_Ptr及_DelFun类型重定义到CDelFunc类内部
//								^ 优化TSmartPtr(TYPE*)构造函数的执行效率
//								= 调整类及其成员函数的声明格式
//	- 1.1.0007.1645(2009-08-21)	+ 添加TPtr类相关操作符重载(!=)
//								+ 添加TSmartPtr类相关操作符重载(!=)
//	- 1.1.0008.1255(2009-08-25)	# 修正TSmartPtr类bool operator==()与bool operator!=()对NULL指针的判断错误
//	- 1.1.0009.1353(2009-08-27)	= 调整TPtr与TSmartPtr类模板TYPE参数的默认值为CDelFunc::_Ptr
//	- 1.1.0010.1200(2009-08-31)	# 修正当使用类指针,智能指针析构时不会调用其析构函数的bug
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

// 计数指针类模板
template <typename TypeT, typename AllocT = DefMemAlloc, typename ModelT = DefThreadModel>
class CReferPtrT
{
protected:
	typedef AllocT alloc_t;

	// 成员变量
protected:
	TypeT*	p_ptr;
	volatile long n_ref;

	// 构造/析构
protected:
	CReferPtrT()
		: p_ptr(NULL)
		, n_ref(0)
	{}
	~CReferPtrT()
	{ alloc_t::Free(p_ptr); }

	// 操作
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
// 计数指针类模板 结束

//////////////////////////////////////////////////////////////////

// 智能指针类模板
template <typename TypeT, typename RefPtrT = CReferPtrT<TypeT> >
class CSmartPtrT
{
public:
	typedef RefPtrT ref_ptr_t;
	typedef typename ref_ptr_t::alloc_t alloc_t;

	// 成员变量
protected:
	ref_ptr_t* m_ptr;

	// 构造/析构
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

	// 操作
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
// 智能指针类模板 结束

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__SmartPtr_h__*/