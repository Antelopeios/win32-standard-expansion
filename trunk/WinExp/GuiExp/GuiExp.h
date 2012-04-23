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
// GuiExp - 界面拓展库(GUI Expansion)
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-04-22
// Version:	1.0.0012.1900
//
// History:
//	- 1.0.0001.1539(2011-05-31)	@ 基本完成GuiBoard;GuiCtrl;GuiEffect;GuiEvent等模块的架构构建
//	- 1.0.0002.1138(2011-06-01)	# 修改一些注释文字中的错误
//								^ 去掉预编译;调整结构;精简文件
//	- 1.0.0003.1208(2011-09-07)	# 修正若干bug
//								^ 调整结构并优化算法,简化调用接口
//	- 1.0.0004.1258(2011-10-10)	# 修正若干bug
//								@ 基本完成XML解析模块的构建
//	- 1.0.0007.1925(2012-01-03)	^ 配合ImgExp的改动调整窗口与控件的绘图逻辑
//	- 1.0.0008.1712(2012-01-20)	# 基本解决使用界面库导致关闭程序时出现内存泄漏报告的问题
//								# 采用父对象托管所有其下子对象(组合对象与事件对象)的方式解决因对象析构时间不一致引起的崩溃
//	- 1.0.0009.1730(2012-02-09)	+ 支持通过皮肤脚本加载UI
//								= 改进各个控件的资源管理方式
//	- 1.0.0010.1657(2012-02-28)	^ 优化并简化对外接口的调用方式
//								^ 优化改进部分内部实现方式
//	- 1.0.0011.2356(2012-03-16)	^ 控件支持额外的属性参数,方便复杂属性设置的实现
//								^ 优化控件绘图机制,由每个控件一张独立位图改为独立的绘图剪切区与坐标,优化绘图算法与绘图时的内存消耗
//////////////////////////////////////////////////////////////////

#ifndef __GuiExp_h__
#define __GuiExp_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

#include "GuiCommon/GuiCommon.h"
#include "GuiBoard/GuiBoard.h"
#include "GuiCtrl/GuiCtrl.h"
#include "GuiManager/GuiManager.h"

//////////////////////////////////////////////////////////////////

#pragma comment(lib, "GuiExp.lib")

//////////////////////////////////////////////////////////////////

#endif/*__GuiExp_h__*/
