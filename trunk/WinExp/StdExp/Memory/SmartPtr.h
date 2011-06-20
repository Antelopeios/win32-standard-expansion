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
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-30
// Version:	1.3.0028.1804
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
//	- 1.2.0018.1500(2011-02-24)	# 将SmartPtr整合进StdExp之后,SmartPtr::Release()没有调用alloc_t释放ptr资源
//								# 修正简化改写CReferPtrT时没有将内部的protected成员改为public成员引起的编译错误
//								+ 添加SmartPtr::Inc()与SmartPtr::Dec()接口,用于循环引用等特殊情况时手动调整引用计数
//	- 1.3.0019.2100(2011-02-24)	= 改变引用计数的管理方式,还原为开始的Manager类统一管理(便于与GC协同工作)
//	- 1.3.0020.0400(2011-02-25)	# 当使用多线程模型时,PtrManager与DefMemAlloc的静态初始化发生冲突
//	- 1.3.0021.1723(2011-02-28)	+ 添加SmartPtr::operator!()操作符重载
//	- 1.3.0022.2000(2011-03-01)	# 修正SmartPtr::Inc()内部的模板调用错误
//	- 1.3.0023.2110(2011-03-04)	= CPtrManagerT::CReferPtrT本身的内存由CPtrManagerT的AllocT构建
//								+ CPtrManagerT::Clear()支持传参定义是否完全清理CPtrManagerT::m_ReferPtrs的所有内存
//	- 1.3.0024.1130(2011-04-02)	# 修正CPtrManagerT::CReferPtrT::Dec()可能出现多次Free自身的问题
//	- 1.3.0025.1858(2011-05-11)	^ 优化CPtrManagerT的单例实现方式
//	- 1.3.0026.1724(2011-05-12)	# 修正CSmartPtrT对裸指针的构造函数在实现上存在无限递归的问题
//	- 1.3.0027.1619(2011-05-19)	= CPtrManagerT在单例情况下,内部对象会被内存池自动回收,因此析构函数不做任何工作
//	- 1.3.0028.1804(2011-05-30)	= 将CPtrManagerT独立作为一个单独的模块维护
//								= 调整CSmartPtrT模板内部定义,参数TypeT直接作为内部变量的类型,而不是TypeT*
//								+ CSmartPtrT支持不托管内部对象
//////////////////////////////////////////////////////////////////

#ifndef __SmartPtr_h__
#define __SmartPtr_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/MemAlloc.h"
#include "Memory/PtrManager.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// 智能指针类模板
template <typename TypeT, typename AllocT = EXP_MEMORY_ALLOC, typename ModelT = EXP_THREAD_MODEL>
class CSmartPtrT
{
public:
	typedef TypeT type_t;
	typedef AllocT alloc_t;
	typedef ModelT model_t;

	// 成员变量
protected:
	type_t m_Ptr;
	bool m_bTru;

	// 构造/析构
public:
	CSmartPtrT(void)
		: m_Ptr(NULL)
		, m_bTru(true)
	{}
	CSmartPtrT(type_t ptr)
		: m_Ptr(NULL)
		, m_bTru(true)
	{ (*this) = ptr; }
	CSmartPtrT(const CSmartPtrT& ptr)
		: m_Ptr(NULL)
		, m_bTru(true)
	{ (*this) = ptr; }

	//////////////////////////////////

	template <typename Type2T>
	CSmartPtrT(Type2T ptr)
		: m_Ptr(NULL)
		, m_bTru(true)
	{ (*this) = ptr; }
	template <typename Type2T>
	CSmartPtrT(const CSmartPtrT<Type2T>& ptr)
		: m_Ptr(NULL)
		, m_bTru(true)
	{ (*this) = ptr; }

	//////////////////////////////////

	~CSmartPtrT(void)
	{ Dec(); }

	// 操作
public:
	long GetRefCount()
	{ return CPtrManager::Instance().Get(m_Ptr); }

	// 是否做托管
	void SetTrust(bool bTru = true) { m_bTru = bTru; }
	bool IsTrust() { return m_bTru; }

	void Inc()
	{ if (m_bTru) CPtrManager::Instance().Add<alloc_t, model_t>(m_Ptr); }
	void Dec()
	{ if (m_bTru) CPtrManager::Instance().Del(m_Ptr); }

	void Release()
	{
		CPtrManager::Instance().Del(m_Ptr, true);
		m_Ptr = NULL;
	}

	//////////////////////////////////

	CSmartPtrT& operator=(type_t ptr)
	{
		if ((*this) == ptr) return (*this);
		Dec();
		m_Ptr = ptr;
		Inc();
		return (*this);
	}
	CSmartPtrT& operator=(const CSmartPtrT& ptr)
	{ return ((*this) = ptr.m_Ptr); }

	bool operator==(type_t ptr) const
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
	bool operator!=(type_t ptr) const
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

	type_t operator+(DWORD offset) const
	{ return (m_Ptr + offset); }
	type_t operator-(DWORD offset) const
	{ return (m_Ptr - offset); }

	bool operator!() const
	{ return !m_Ptr; }

	type_t operator->() const
	{ return m_Ptr; }
	operator type_t() const
	{ return m_Ptr; }

	//////////////////////////////////

	template <typename Type2T>
	CSmartPtrT& operator=(Type2T ptr)
	{
		if ((*this) == ptr) return (*this);
		Dec();
		m_Ptr = (type_t)ptr;
		Inc();
		return (*this);
	}
	template <typename Type2T>
	CSmartPtrT& operator=(const CSmartPtrT<Type2T>& ptr)
	{ return ((*this) = ptr.m_Ptr); }

	template <typename Type2T>
	bool operator==(Type2T ptr) const
	{ return ((*this) == (type_t)ptr); }
	template <typename Type2T>
	bool operator==(const CSmartPtrT<Type2T>& ptr) const
	{ return (m_Ptr == (type_t)ptr.m_Ptr); }
	template <typename Type2T>
	bool operator!=(Type2T ptr) const
	{ return ((*this) != (type_t)ptr); }
	template <typename Type2T>
	bool operator!=(const CSmartPtrT<Type2T>& ptr) const
	{ return (m_Ptr != (type_t)ptr.m_Ptr); }

	template <typename Type2T>
	operator Type2T() const
	{ return (Type2T)m_Ptr; }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__SmartPtr_h__*/