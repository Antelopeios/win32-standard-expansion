// Copyright 2011-2012, 木头云
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
// Date:	2012-01-20
// Version:	1.0.0008.1712
//
// History:
//	- 1.0.0001.1135(2011-05-04)	+ 添加wnd_t类型定义
//	- 1.0.0002.1506(2011-05-11)	- 移除ATLThunk,采用GWL_USERDATA方式路由窗口过程
//	- 1.0.0003.1553(2011-05-19)	^ 置换掉默认的单例,内存管理等动作统一在本模块内完成
//	- 1.0.0004.1616(2011-06-08)	^ 将默认的Normal滤镜置换为CFilterOverlay,方便控件绘图
//	- 1.0.0005.1438(2011-07-06)	^ 简化结构,统一EXP_API相关的类/结构/接口声明
//	- 1.0.0006.0120(2011-07-07)	= 关闭由StdExp的RTTI改动引起的C4275警告
//	- 1.0.0007.1540(2011-07-20)	+ 添加ExGC()接口,供外部获取全局GC,防止由于DLL内外全局GC生存周期不一致导致的析构异常
//	- 1.0.0007.1540(2012-01-20)	- 移除ExGC()接口,使用对象相互托管方式解决GC析构顺序导致的异常
//////////////////////////////////////////////////////////////////

#ifndef __GuiCommon_h__
#define __GuiCommon_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

// 关闭一些警告
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

// Dll 导出宏定义
#ifdef EXP_EXPORTS
#define EXP_API __declspec(dllexport)
#else
#define EXP_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////

// 置换掉默认的全局单例
#include "Common/Common.h"
#include "Thread/Lock.h"
EXP_BEG
template <typename TypeT, typename ModelT = EXP_THREAD_MODEL>
EXP_INTERFACE IGuiSingletonT
{
public:
	EXP_INLINE static TypeT& Instance()
	{
		static TypeT* instance = NULL;
		if (instance == NULL)
		{
			ExLockThis(typename ModelT::_LockPolicy);
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
#define EXP_SINGLETON IGuiSingletonT

// 图像处理库
#include "ImgExp.h"

//////////////////////////////////////////////////////////////////

// 类型/功能定义
EXP_BEG
typedef HWND		wnd_t;
#define ExGetX(lp)	((int)(short)LOWORD(lp))
#define ExGetY(lp)	((int)(short)HIWORD(lp))
EXP_END

// 基本接口定义
#include "GuiCommon/GuiInterface.h"

EXP_BEG

// 通用对象创建接口
EXP_API IGuiObject* ExGui(LPCTSTR sGuiType, CGC* pGC = NULL);

EXP_END

// Gui 基础接口定义
#include "GuiCommon/GuiBase.h"

//////////////////////////////////////////////////////////////////

#endif/*__GuiCommon_h__*/