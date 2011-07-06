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
// RTTI - 运行时类型识别
//
// Author:	木头云
// Home:	http://hi.baidu.com/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-07-07
// Version:	1.1.0013.0058
//
// History:
//	- 1.1.0008.1730(2011-05-05)	^ 规范化基类重定义,并添加统一的基类获取宏
//	- 1.1.0009.1908(2011-05-11)	^ 规范化CTypeInfoFactory的单例实现方式
//	- 1.1.0010.1704(2011-05-13)	# 修正RTTI在多继承时的编译错误
//	- 1.1.0011.1511(2011-05-16)	= 调整一些内部接口的名称定义
//	- 1.1.0012.1400(2011-05-19)	^ ExDynCreate()支持默认传入空指针GC构造对象
//	- 1.1.0013.0058(2011-07-07)	# 由于在Dll中使用RTTI,会导致工程中的RTTI编译失败,因此将IBaseObjectT模板改为类
//////////////////////////////////////////////////////////////////

#ifndef __RTTI_h__
#define __RTTI_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/GC.h"
#include "Pattern/Singleton.h"
#include "Container/Map.h"
#include "Container/String.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// RTTI 起始类声明
interface IBaseObject;

//////////////////////////////////////////////////////////////////

// 类型信息结构
struct _TypeInfo
{
	typedef IBaseObject* (*creator_t)(CGC*);

	LPTSTR		className;
	int			type_id;
	_TypeInfo*	pBaseClass[3];
	creator_t	m_Creator;	// NULL => abstract class

	EXP_INLINE IBaseObject* CreateObject(CGC* gc = NULL)
	{
		if (!m_Creator) return NULL;
		return (*m_Creator)(gc);
	}
	EXP_INLINE bool IsKindOf(_TypeInfo& cls)
	{
		if( type_id == cls.type_id )
			return true;
		for(int i = 0; i < 3; i++)
		{
			if(!pBaseClass[i]) break;
			if( pBaseClass[i]->IsKindOf(cls))
				return true;
		}
		return false;
	}
	EXP_INLINE bool operator==(const _TypeInfo& info)
	{ return this == &info; }
	EXP_INLINE bool operator!=(const _TypeInfo& info)
	{ return this != &info; }
};

//////////////////////////////////////////////////////////////////

// _TypeInfo 指针单例工厂
class CTypeInfoFactory : INonCopyable, public EXP_SINGLETON<CTypeInfoFactory>
{
	friend interface EXP_SINGLETON<CTypeInfoFactory>;

private:
	typedef CMapT<CString, _TypeInfo*> key_map;
	key_map dc_funcs;

private:
	CTypeInfoFactory() : dc_funcs(1021) {}

public:
	// 向工厂注册一个类名
	bool RegTypeInfo(LPCTSTR c_key, _TypeInfo* inf)
	{
		if (!c_key) return false;
		CString key(c_key);
		if (dc_funcs.Locate(key) == dc_funcs.Tail())
		{
			dc_funcs.Add(key, inf);
			return true;
		}
		else
			return false;
	}
	// 从工厂获得一个 _TypeInfo
	_TypeInfo* GetTypeInfo(LPCTSTR c_key)
	{
		if (!c_key) return false;
		CString key(c_key);
		key_map::iterator_t ite = dc_funcs.Locate(key);
		if (ite == dc_funcs.Tail())
			return NULL;
		else
			return ite->Val();
	}
};

// 向工厂注册 _TypeInfo 指针
#define ExRegTypeInfo(key, inf)	CTypeInfoFactory::Instance().RegTypeInfo(key, inf)
// 从工厂得到 _TypeInfo 指针
#define ExGetTypeInfo(key)		CTypeInfoFactory::Instance().GetTypeInfo(key)

//////////////////////////////////////////////////////////////////

// Base Typedef 宏定义

#define EXP_BASE base_t
#define EXP_DEF_BASETYPE(base_name)															\
public:																						\
	typedef base_name EXP_BASE;

#define EXP_BASE2 base2_t
#define EXP_DEF_BASETYPE2(base_name)														\
public:																						\
	typedef base_name EXP_BASE2;

#define EXP_BASE3 base3_t
#define EXP_DEF_BASETYPE3(base_name)														\
public:																						\
	typedef base_name base3_t;

#define EXP_MULT mult_t
#define EXP_DEF_MULTTYPE(mult_name)															\
public:																						\
	typedef mult_name mult_t;

//////////////////////////////////////////////////////////////////

// TYPEINFO 类型信息宏定义

#define ExTypeInfoCls(cls_name)		(cls_name::GetTypeInfoClass())
#define ExTypeInfoObj(obj_name)		(obj_name.GetTypeInfo())
#define ExTypeInfoPtr(ptr_name)		(ptr_name->GetTypeInfo())

#define EXP_TYPEINFO_MEMBER			rttiTypeInfo

//////////////////////////////////////////////////////////////////

// 类的 RTTI 宏定义

#define EXP_DECLARE_TYPEINFO(cls_name)														\
public:																						\
	virtual int GetTypeID()				{ return EXP_TYPEINFO_MEMBER.type_id; }				\
	virtual LPCTSTR GetTypeName()		{ return EXP_TYPEINFO_MEMBER.className; }			\
	virtual _TypeInfo& GetTypeInfo()		{ return EXP_TYPEINFO_MEMBER; }					\
	static _TypeInfo& GetTypeInfoClass()	{ return EXP_TYPEINFO_MEMBER; }					\
private:																					\
	static _TypeInfo EXP_TYPEINFO_MEMBER;													\

#define EXP_DECLARE_TYPEINFO_CLS(cls_name, base_name)										\
	EXP_DEF_MULTTYPE(cls_name)																\
	EXP_DEF_BASETYPE(base_name)																\
	EXP_DECLARE_TYPEINFO(cls_name)

#define EXP_DECLARE_TYPEINFO_MULT(cls_name, base_name)										\
	EXP_DEF_MULTTYPE(base_name::EXP_MULT)													\
	EXP_DEF_BASETYPE(base_name)																\
	EXP_DECLARE_TYPEINFO(cls_name)

#define EXP_DECLARE_TYPEINFO_MULT2(cls_name, base_name, base_name2)							\
	EXP_DEF_MULTTYPE(base_name::EXP_MULT)													\
	EXP_DEF_BASETYPE(base_name)																\
	EXP_DEF_BASETYPE2(base_name2)															\
	EXP_DECLARE_TYPEINFO(cls_name)

#define EXP_DECLARE_TYPEINFO_MULT3(cls_name, base_name, base_name2, base_name3)				\
	EXP_DEF_MULTTYPE(base_name::EXP_MULT)													\
	EXP_DEF_BASETYPE(base_name)																\
	EXP_DEF_BASETYPE2(base_name2)															\
	EXP_DEF_BASETYPE3(base_name3)															\
	EXP_DECLARE_TYPEINFO(cls_name)

#define EXP_DECLARE_TYPEINFO_NULL(cls_name)													\
	EXP_DEF_MULTTYPE(cls_name)																\
	EXP_DECLARE_TYPEINFO(cls_name)															\
public:																						\
	bool IsKindOf(_TypeInfo& cls);

// dynamically typeinfo

#define EXP_DECLARE_DYNAMIC_CLS(cls_name, base_name)										\
	EXP_DECLARE_TYPEINFO_CLS(cls_name, base_name)

#define EXP_DECLARE_DYNAMIC_MULT(cls_name, base_name)										\
	EXP_DECLARE_TYPEINFO_MULT(cls_name, base_name)

#define EXP_DECLARE_DYNAMIC_MULT2(cls_name, base_name, base_name2)							\
	EXP_DECLARE_TYPEINFO_MULT2(cls_name, base_name, base_name2)

#define EXP_DECLARE_DYNAMIC_MULT3(cls_name, base_name, base_name2, base_name3)				\
	EXP_DECLARE_TYPEINFO_MULT3(cls_name, base_name, base_name2, base_name3)

#define EXP_DECLARE_DYNAMIC_NULL(cls_name)													\
	EXP_DECLARE_TYPEINFO_NULL(cls_name)

// dynamically constructable

#define EXP_DECLARE_DYNCREATE_C(cls_name)													\
public:																						\
	static IBaseObject* CreateObject(CGC* gc = NULL);										\
private:																					\
	static bool m_bRegSuccess;

#define EXP_DECLARE_DYNCREATE_CLS(cls_name, base_name)										\
	EXP_DECLARE_DYNAMIC_CLS(cls_name, base_name)											\
	EXP_DECLARE_DYNCREATE_C(cls_name)

#define EXP_DECLARE_DYNCREATE_MULT(cls_name, base_name)										\
	EXP_DECLARE_DYNAMIC_MULT(cls_name, base_name)											\
	EXP_DECLARE_DYNCREATE_C(cls_name)

#define EXP_DECLARE_DYNCREATE_MULT2(cls_name, base_name, base_name2)						\
	EXP_DECLARE_DYNAMIC_MULT2(cls_name, base_name, base_name2)								\
	EXP_DECLARE_DYNCREATE_C(cls_name)

#define EXP_DECLARE_DYNCREATE_MULT3(cls_name, base_name, base_name2, base_name3)			\
	EXP_DECLARE_DYNAMIC_MULT3(cls_name, base_name, base_name2, base_name3)					\
	EXP_DECLARE_DYNCREATE_C(cls_name)

#define EXP_DECLARE_DYNCREATE_NULL(cls_name)												\
	EXP_DECLARE_DYNAMIC_NULL(cls_name)														\
	EXP_DECLARE_DYNCREATE_C(cls_name)

	/////////////////////////////////

#define EXP_IMPLEMENT_TYPEINFO_CLS(cls_name, base_name, pfn_new, tmp)						\
	tmp																						\
	_TypeInfo cls_name::EXP_TYPEINFO_MEMBER =												\
	{																						\
		_T(#cls_name), 																		\
		IBaseObject::TypeInfoOrder++, 														\
		{&ExTypeInfoCls(base_name), NULL, NULL}, 											\
		pfn_new																				\
	};

#define EXP_IMPLEMENT_TYPEINFO_MULT(cls_name, base_name, pfn_new, tmp)						\
	EXP_IMPLEMENT_TYPEINFO_CLS(cls_name, base_name, pfn_new, tmp)

#define EXP_IMPLEMENT_TYPEINFO_MULT2(cls_name, base_name, base_name2, pfn_new, tmp)			\
	tmp																						\
	_TypeInfo cls_name::EXP_TYPEINFO_MEMBER =												\
	{																						\
		_T(#cls_name), 																		\
		IBaseObject::TypeInfoOrder++, 														\
		{&ExTypeInfoCls(base_name), &ExTypeInfoCls(base_name2), NULL}, 						\
		pfn_new																				\
	};

#define EXP_IMPLEMENT_TYPEINFO_MULT3(cls_name, base_name, base_name2, base_name3, pfn_new, tmp)	\
	tmp																						\
	_TypeInfo cls_name::EXP_TYPEINFO_MEMBER =												\
	{																						\
		_T(#cls_name), 																		\
		IBaseObject::TypeInfoOrder++, 														\
		{&ExTypeInfoCls(base_name), &ExTypeInfoCls(base_name2), &ExTypeInfoCls(base_name3)}, \
		pfn_new																				\
	};

#define EXP_IMPLEMENT_TYPEINFO_NULL(cls_name, pfn_new, tmp)									\
	tmp																						\
	_TypeInfo cls_name::EXP_TYPEINFO_MEMBER =												\
	{																						\
		_T(#cls_name), 																		\
		IBaseObject::TypeInfoOrder++, 														\
		{NULL, NULL, NULL}, 																\
		pfn_new																				\
	};																						\
	tmp																						\
	bool cls_name::IsKindOf(_TypeInfo& cls)													\
	{																						\
		_TypeInfo* p = &(this->GetTypeInfo());												\
		return (p ? p->IsKindOf(cls) : false);												\
	}

// dynamically typeinfo

#define EXP_IMPLEMENT_DYNAMIC_CLS(cls_name, base_name, tmp)									\
	EXP_IMPLEMENT_TYPEINFO_CLS(cls_name, base_name, NULL, tmp)

#define EXP_IMPLEMENT_DYNAMIC_MULT(cls_name, base_name, tmp)								\
	EXP_IMPLEMENT_TYPEINFO_MULT(cls_name, base_name, NULL, tmp)

#define EXP_IMPLEMENT_DYNAMIC_MULT2(cls_name, base_name, base_name2, tmp)					\
	EXP_IMPLEMENT_TYPEINFO_MULT2(cls_name, base_name, base_name2, NULL, tmp)

#define EXP_IMPLEMENT_DYNAMIC_MULT3(cls_name, base_name, base_name2, base_name3, tmp)		\
	EXP_IMPLEMENT_TYPEINFO_MULT3(cls_name, base_name, base_name2, base_name3, NULL, tmp)

#define EXP_IMPLEMENT_DYNAMIC_NULL(cls_name, tmp)											\
	EXP_IMPLEMENT_TYPEINFO_NULL(cls_name, NULL, tmp)

// dynamically constructable

#define EXP_IMPLEMENT_DYNCREATE_C(cls_name, base_name, tmp)									\
	tmp																						\
	IBaseObject* cls_name::CreateObject(CGC* gc/* = NULL*/)									\
	{ return (base_name*)ExMem::Alloc<cls_name>(gc); }										\
	tmp																						\
	bool cls_name::m_bRegSuccess =															\
		ExRegTypeInfo( _T(#cls_name), &(cls_name::EXP_TYPEINFO_MEMBER) );

#define EXP_IMPLEMENT_DYNCREATE_CLS(cls_name, base_name, tmp)								\
	EXP_IMPLEMENT_TYPEINFO_CLS(cls_name, base_name, cls_name::CreateObject, tmp)			\
	EXP_IMPLEMENT_DYNCREATE_C(cls_name, base_name, tmp)

#define EXP_IMPLEMENT_DYNCREATE_MULT(cls_name, base_name, tmp)								\
	EXP_IMPLEMENT_TYPEINFO_MULT(cls_name, base_name, cls_name::CreateObject, tmp)			\
	EXP_IMPLEMENT_DYNCREATE_C(cls_name, EXP_MULT, tmp)

#define EXP_IMPLEMENT_DYNCREATE_MULT2(cls_name, base_name, base_name2, tmp)					\
	EXP_IMPLEMENT_TYPEINFO_MULT2(cls_name, base_name, base_name2, cls_name::CreateObject, tmp) \
	EXP_IMPLEMENT_DYNCREATE_C(cls_name, EXP_MULT, tmp)

#define EXP_IMPLEMENT_DYNCREATE_MULT3(cls_name, base_name, base_name2, base_name3, tmp)		\
	EXP_IMPLEMENT_TYPEINFO_MULT3(cls_name, base_name, base_name2, base_name3, cls_name::CreateObject, tmp) \
	EXP_IMPLEMENT_DYNCREATE_C(cls_name, EXP_MULT, tmp)

#define EXP_IMPLEMENT_DYNCREATE_NULL(cls_name, tmp)											\
	EXP_IMPLEMENT_TYPEINFO_NULL(cls_name, cls_name::CreateObject, tmp)						\
	EXP_IMPLEMENT_DYNCREATE_C(cls_name, IBaseObject, tmp)

//////////////////////////////////////////////////////////////////

// RTTI 起始类
interface IBaseObject
{
	EXP_DECLARE_DYNCREATE_NULL(IBaseObject)

public:
	// type_id 自增量
	static int TypeInfoOrder;
};

//////////////////////////////////////////////////////////////////

// 动态指针效验函数
EXP_INLINE bool ExDynCheck(LPCTSTR c_key, IBaseObject* ptr)
{
	if( ptr )
	{
		_TypeInfo* inf = ExGetTypeInfo(c_key);
		if( inf )
			return ptr->IsKindOf(*inf);
		else
			return false;
	}
	else
		return false;
}

// 动态指针转换函数模板
template <typename TypeT>
EXP_INLINE TypeT* ExDynCast(void* ptr)
{
	if( ptr )
		return ((TypeT::EXP_MULT*)ptr)->IsKindOf(ExTypeInfoCls(TypeT)) ? (TypeT*)(TypeT::EXP_MULT*)ptr : NULL;
	else
		return NULL;
}

// 动态对象创建函数
EXP_INLINE void* ExDynCreate(LPCTSTR c_key, CGC* gc = NULL)
{
	if( c_key == NULL ) return NULL;
	_TypeInfo* inf = ExGetTypeInfo(c_key);
	if( inf )
		return inf->CreateObject(gc);
	else
		return NULL;
}

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__RTTI_h__*/
