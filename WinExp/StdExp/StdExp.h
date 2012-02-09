// Copyright 2010-2012, 木头云
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
// Date:	2012-02-09
// Version:	0.1.0021.1021
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
//	- 0.1.0013.1050(2011-06-13)	+ 添加Tree的容器算法;给一些模块添加一些方便外部调用的接口
//								# 修正若干个bug
//								^ 调整并统一static const类型变量的命名
//	- 0.1.0015.1750(2011-07-29)	# 修正若干个bug
//								+ 添加一些方便的接口
//	- 0.1.0017.1542(2011-12-10)	# 修正为了提升效率,使用函数内static变量做临时存储,而导致的多线程访问异常
//	- 0.1.0018.1750(2011-12-14)	+ 在Debugging模块中添加CrashHandler崩溃捕获机制
//								# 修正在不使用EXP默认域定义的情况下某些宏无法使用的问题
//	- 0.1.0019.2202(2012-01-14)	+ 为Memory部分的对外接口添加ReAlloc方法
//								^ 使用Traits及ReAlloc优化部分内存管理与容器类中的算法
//	- 0.1.0020.1952(2012-01-16)	+ 添加全局new操作符重载的可选编译选项
//								+ 添加托管非GC内存分配的可选编译选项
//								+ 添加关闭Debug下自动内存泄漏检测的可选编译选项
//	- 0.1.0021.1021(2012-02-09)	+ 线性容器添加Push...(),Pop...()系列接口,方便直接于头尾添加删除数据
//								+ 支持外部调试时自定义某次内存分配时中断(类似_CrtSetBreakAlloc())
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
