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
// LibStd - 标准库定义
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-26
// Version:	1.2.0008.0050
//
// History:
//	- 1.2.0005.1714(2011-08-11)	+ #include <conio.h>,避免一些在ANSI字符集下的编译错误
//	- 1.2.0006.1325(2011-11-27)	+ #include <stdlib.h>(_countof宏)
//	- 1.2.0007.2310(2011-12-11)	- 移除strsafe.h,避免同时使用stl时出现的编译警告
//	- 1.2.0008.0050(2012-01-26)	+ 添加EXP_UNUSED_WINH宏,支持外部关闭windows.h的引用
//////////////////////////////////////////////////////////////////

#ifndef __LibStd_h__
#define __LibStd_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

// 标准库支持

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <tchar.h>
#include <time.h>
#include <process.h>
#ifndef	EXP_UNUSED_WINH
#include <windows.h>
#endif/*EXP_UNUSED_WINH*/

//////////////////////////////////////////////////////////////////

#endif/*__LibStd_h__*/
