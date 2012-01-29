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
// OverNew - 全局new操作符重载
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-29
// Version:	1.0.0002.0006
//
// History:
//	- 1.0.0000.1800(2012-01-15)	@ 开始构建全局new操作符重载
//	- 1.0.0001.1712(2012-01-20)	+ 添加dbnew及相关内存泄漏自动检测代码
//	- 1.0.0002.0006(2012-01-29)	= 不再通过new操作符重载重定义new的行为,直接通过宏定义一组新的分配接口
//								+ 定义realc,支持重新分配并当内存块变化时不释放原先的内存
//								+ 定义del,用于析构并销毁内存
//								+ 定义rel,用于直接销毁内存(不调用析构)
//								+ 定义cnst与dest,用于在已分配好的内存块上构造/析构对象
//////////////////////////////////////////////////////////////////

#ifndef __OverNew_h__
#define __OverNew_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/RegistAlloc.h"

//////////////////////////////////////////////////////////////////

#ifdef	dbnew
#undef	dbnew
#endif/*dbnew*/

#ifdef	renew
#undef	renew
#endif/*renew*/

#ifdef	realc
#undef	realc
#endif/*realc*/

#ifndef	EXP_DUMPING_MEMLEAKS

#define dbnew(type, count)		EXP::ExMem::Alloc<type>(count)
#define renew(ptr, type, count)	EXP::ExMem::ReAlloc<type>(ptr, count, TRUE)
#define realc(ptr, type, count)	EXP::ExMem::ReAlloc<type>(ptr, count, FALSE)

#else /*EXP_DUMPING_MEMLEAKS*/

#define dbnew(type, count)		(type*)EXP::ExMem::SetAlloc(EXP::ExMem::Alloc<type>(count), __FILE__, __LINE__)
#define renew(ptr, type, count)	EXP::ExMem::ReAlloc<type>(ptr, count, TRUE, __FILE__, __LINE__)
#define realc(ptr, type, count)	EXP::ExMem::ReAlloc<type>(ptr, count, FALSE, __FILE__, __LINE__)

#endif/*EXP_DUMPING_MEMLEAKS*/

//////////////////////////////////////////////////////////////////

#ifdef	gcnew
#undef	gcnew
#endif/*gcnew*/

#define gcnew(gc, type, count)	(gc).Alloc<type>(count)

//////////////////////////////////////////////////////////////////

#ifdef	del
#undef	del
#endif/*del*/

#ifdef	rel
#undef	rel
#endif/*rel*/

#define del(ptr)				EXP::ExMem::Free(ptr)
#define rel(ptr)				EXP::ExMem::FreeNoDest(ptr)

//////////////////////////////////////////////////////////////////

#ifdef	cnst
#undef	cnst
#endif/*cnst*/

#ifdef	dest
#undef	dest
#endif/*dest*/

#define cnst(ptr, type, count)	_Traits::Construct<type>(ptr, count)
#define dest(ptr, type, count)	_Traits::Destruct<type>(ptr, count)

//////////////////////////////////////////////////////////////////

#endif/*__OverNew_h__*/