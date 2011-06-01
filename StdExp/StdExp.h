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
// StdExp - 标准拓展库(Standard Expansion)
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-12
// Version:	0.1.0012.1730
//
// History:
//	- 0.1.0005.2000(2010-12-11)	@ 完成Memory部分所有类的构建
//	- 0.1.0006.2200(2010-12-29)	@ 完成Container部分Array与String的构建
//	- 0.1.0007.2220(2011-01-19)	@ 完成File部分IOFile与MemFile的构建
//	- 0.1.0008.1200(2011-02-23)	@ 完成Thread部分的构建
//	- 0.1.0009.2359(2011-03-01)	^ 统一所有对外功能宏的命名方式
//	- 0.1.0010.2110(2011-03-04)	^ 大幅优化内存池随机分配/释放的效率
//	- 0.1.0011.1605(2011-04-07)	^ 统一所有Object基类的类型(interface)及命名前缀(I)
//	- 0.1.0012.1730(2011-05-12)	+ 添加无锁数据访问的通用算法
//								^ 优化各个模块的细节处理与对外接口
//								# 修正一些接口的内部实现错误
//////////////////////////////////////////////////////////////////

#ifndef __StdExp_h__
#define __StdExp_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

#include "Common/Common.h"
#include "Debugging/Debugging.h"
#include "Memory/Memory.h"
#include "Pattern/Pattern.h"
#include "Thread/Thread.h"
#include "Container/Container.h"
#include "File/File.h"
#include "Algorithm/Algorithm.h"

//////////////////////////////////////////////////////////////////

#ifndef EXP_UNUSED_NAMESPACE
EXP_USING_NAMESPACE
#endif/*EXP_UNUSED_NAMESPACE*/

#endif/*__StdExp_h__*/
