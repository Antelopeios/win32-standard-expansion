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
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-19
// Version:	1.0.0003.1553
//
// History:
//	- 1.0.0001.1135(2011-05-04)	+ 添加wnd_t类型定义
//	- 1.0.0002.1506(2011-05-11)	- 移除ATLThunk,采用GWL_USERDATA方式路由窗口过程
//	- 1.0.0003.1553(2011-05-19)	^ 置换掉默认的单例,内存管理等动作统一在本模块内完成
//////////////////////////////////////////////////////////////////

#ifndef __GuiCommon_h__
#define __GuiCommon_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

// Dll 导出宏定义
#ifdef EXP_EXPORTS
#define EXP_API __declspec(dllexport)
#else
#define EXP_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////

#include "../StdExp/Common/Common.h"
#include "../StdExp/Thread/Lock.h"

EXP_BEG

// 模块内部单例定义
template <typename TypeT>
class EXP_API IGuiSingletonT
{
public:
	EXP_INLINE static TypeT& Instance()
	{
		static TypeT* instance = NULL;
		if (instance == NULL)
		{
			ExLockThis();
			if (instance == NULL)
			{
				static TypeT type;
				instance = &type;
			}
		}
		return (*instance);
	}
};

EXP_END

//////////////////////////////////////////////////////////////////

// 图像处理库
#define EXP_SINGLETON IGuiSingletonT // 置换掉默认的单例,内存管理等动作统一在本模块内完成
#include "../ImgExp/ImgExp.h"

//////////////////////////////////////////////////////////////////

EXP_BEG

// 类型定义

typedef HWND		wnd_t;

// 功能定义

#define ExGetX(lp)	((int)(short)LOWORD(lp))
#define ExGetY(lp)	((int)(short)HIWORD(lp))

EXP_END

//////////////////////////////////////////////////////////////////

// 基本接口定义
#include "GuiCommon/GuiInterface.h"

//////////////////////////////////////////////////////////////////

EXP_BEG

// 通用对象创建接口
EXP_API IGuiObject* ExGui(LPCTSTR sGuiType, CGC* pGC = NULL);

EXP_END

//////////////////////////////////////////////////////////////////

#endif/*__GuiCommon_h__*/