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
// GuiCommon - 界面拓展库公用定义
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2010-05-04
// Version:	1.0.0001.1135
//
// History:
//	- 1.0.0001.1135(2010-05-04)	+ 添加wnd_t类型定义
//////////////////////////////////////////////////////////////////

#ifndef __GuiCommon_h__
#define __GuiCommon_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 图像处理库
#include "../ImgExp/ImgExp.h"

// ATL Thunk
#include <atlstdthunk.h>

//////////////////////////////////////////////////////////////////

// Dll 导出宏定义
#ifdef EXP_EXPORTS
#define EXP_API __declspec(dllexport)
#else
#define EXP_API __declspec(dllimport)
#endif

EXP_BEG

//////////////////////////////////////////////////////////////////

// 类型定义

typedef HWND		wnd_t;

// 功能定义

#define ExGetX(lp)	((int)(short)LOWORD(lp))
#define ExGetY(lp)	((int)(short)HIWORD(lp))

//////////////////////////////////////////////////////////////////

EXP_END

#include "GuiCommon/GuiInterface.h"

//////////////////////////////////////////////////////////////////

#endif/*__GuiCommon_h__*/