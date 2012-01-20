// Copyright 2012, 木头云
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
// Traits - 萃取器
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-19
// Version:	1.0.0003.1530
//
// History:
//	- 1.0.0001.2102(2012-01-14)	= 将萃取器部分从RegistAlloc中独立出来
//	- 1.0.0002.1916(2012-01-16)	^ 简化类型萃取器的特化定义列表
//	- 1.0.0003.1530(2012-01-19)	+ 添加wchar_t的特化定义
//////////////////////////////////////////////////////////////////

#ifndef __Traits_h__
#define __Traits_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Debugging/Assertion.h"
#include <new>

EXP_BEG

//////////////////////////////////////////////////////////////////
// 类型萃取器

struct _true_type {};
struct _false_type {};

template <typename TypeT>
struct _TraitsT
{
	typedef _false_type is_POD_type;
};

// 特化

#undef ExSplTraits
#define ExSplTraits(type, name) \
	template <name> \
	struct _TraitsT<type> \
	{ \
		typedef _true_type is_POD_type; \
	}
//#define SplTraitsType(type)

ExSplTraits(bool);
ExSplTraits(float);
ExSplTraits(double);
ExSplTraits(long double);

ExSplTraits(int);
ExSplTraits(unsigned int);

ExSplTraits(char);
ExSplTraits(unsigned char);

ExSplTraits(wchar_t);

ExSplTraits(short);
ExSplTraits(unsigned short);

ExSplTraits(long);
ExSplTraits(unsigned long);

ExSplTraits(TypeT*, typename TypeT);

//////////////////////////////////////////////////////////////////
// 构造/析构函数萃取器

template <typename TypeT>
EXP_INLINE static TypeT* ExConstruct(void* pPtr, DWORD nCount, _true_type)
{
	ExAssert(nCount);
	return (TypeT*)pPtr;
}
template <typename TypeT>
EXP_INLINE static TypeT* ExConstruct(void* pPtr, DWORD nCount, _false_type)
{
	ExAssert(nCount);
#pragma push_macro("new")
#undef new
	for (DWORD i = 0; i < nCount; ++i)
		::new(((TypeT*)pPtr) + i) TypeT;
#pragma pop_macro("new")
	return (TypeT*)pPtr;
}

template <typename TypeT>
EXP_INLINE static TypeT* ExDestruct(void* pPtr, DWORD nCount, _true_type)
{
	ExAssert(nCount);
	return (TypeT*)pPtr;
}
template <typename TypeT>
EXP_INLINE static TypeT* ExDestruct(void* pPtr, DWORD nCount, _false_type)
{
	ExAssert(nCount);
	for (DWORD i = 0; i < nCount; ++i)
		((TypeT*)pPtr)[i].TypeT::~TypeT();
	return (TypeT*)pPtr;
}

struct _Traits
{
	typedef void (*traitor_t)(void*, DWORD);

	template <typename TypeT>
	EXP_INLINE static TypeT* Construct(void* pPtr, DWORD nCount = 1)
	{
		return ExConstruct<TypeT>(pPtr, nCount, _TraitsT<TypeT>::is_POD_type());
	}
	template <typename TypeT>
	EXP_INLINE static TypeT* Destruct(void* pPtr, DWORD nCount = 1)
	{
		return ExDestruct<TypeT>(pPtr, nCount, _TraitsT<TypeT>::is_POD_type());
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Traits_h__*/