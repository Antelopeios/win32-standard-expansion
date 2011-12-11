// Copyright 2010-2011, 木头云
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
// Common - 标准拓展库公用定义
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-12-11
// Version:	1.2.0009.2350
//
// History:
//	- 1.2.0006.1150(2011-03-02)	= 调整部分宏定义,定义并使用EXP_INLINE宏
//	- 1.2.0007.2000(2011-04-03)	+ 添加interface宏定义
//	- 1.2.0008.1000(2011-05-05)	= 将EXP宏定义由StdExp调整为win_exp
//	- 1.2.0009.2350(2011-12-11)	- 移除bool的宏定义,避免与stl发生冲突
//////////////////////////////////////////////////////////////////

#ifndef __Common_h__
#define __Common_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

// 公用库定义

#include "Common/TargetVer.h"
#include "Common/LibStd.h"

// 关闭一些警告消息
#pragma warning(disable:4003) // not enough actual parameters for macro 'identifier'

// namespace 定义

#define EXP					win_exp
#define EXP_BEG				namespace EXP {
#define EXP_END				}
#define EXP_USING_NAMESPACE	using namespace EXP;

EXP_BEG

//////////////////////////////////////////////////////////////////

// 数据类型定义

typedef unsigned __int8		uint8_t;
typedef unsigned __int16	uint16_t;
typedef unsigned __int32	uint32_t;
typedef unsigned __int64	uint64_t;
typedef __int8				int8_t;
typedef __int16				int16_t;
typedef __int32				int32_t;
typedef __int64				int64_t;

// 功能定义

#ifndef interface
#define interface struct
#endif

#ifndef _IN_
#define _IN_
#endif

#ifndef _OT_
#define _OT_
#endif

#ifndef _IN_OT_
#define _IN_OT_
#endif

#define EXP_INLINE			__forceinline

#define ExMakeWord(a, b)	MAKEWORD(a, b)
#define ExMakeLong(a, b)	MAKELONG(a, b)
#define ExMakeInt64(a, b)	((int64_t)(((int32_t)(a)) | ((int64_t)((int32_t)(b))) << 32))
#define ExLoWord(x)			LOWORD(x)
#define ExHiWord(x)			HIWORD(x)
#define ExLoByte(x)			LOBYTE(x)
#define ExHiByte(x)			HIBYTE(x)
#define ExLoLong(x)			((long)(((int64_t)(x)) & 0xffffffff))
#define ExHiLong(x)			((long)((((int64_t)(x)) >> 32) & 0xffffffff))

#define ExRandom(num)		((num) ? (rand() % (num)) : 0)
#define ExRandomize()		srand((unsigned)time(NULL))

// 导出定义

#ifndef EXP_API
#define EXP_API
#endif

#define EXP_CLASS class EXP_API
#define EXP_STRUCT struct EXP_API
#define EXP_INTERFACE interface EXP_API

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Common_h__*/