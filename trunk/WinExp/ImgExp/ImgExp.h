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
// ImgExp - 图像拓展库(Image Expansion)
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-16
// Version:	0.1.0006.2356
//
// History:
//	- 0.1.0001.2300(2011-04-19)	@ 完成ImgCoder部分所有类的构建
//								@ 完成ImgTypes部分所有类的构建
//	- 0.1.0002.1400(2011-05-02)	@ 完成ImgDrawer部分所有类的构建
//	- 0.1.0003.1750(2011-05-30)	^ ImgTypes部分基于引用计数重新构建,支持自动回收机制
//	- 0.1.0004.1630(2011-06-20)	@ ImgDrawer重命名为ImgPainter,并添加基本绘图模块
//	- 0.1.0005.1925(2012-01-03)	^ 所有图片加载,生成与处理均面向预乘图片进行
//								^ 简化图片的绘制过程与算法
//	- 0.1.0006.2356(2012-03-16)	+ 添加带有独立剪切区与坐标的绘图机制
//////////////////////////////////////////////////////////////////

#ifndef __ImgExp_h__
#define __ImgExp_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

#include "ImgCommon/ImgCommon.h"
#include "ImgTypes/ImgTypes.h"
#include "ImgCoder/ImgCoder.h"
#include "ImgPainter/ImgPainter.h"

//////////////////////////////////////////////////////////////////

#pragma comment(lib, "ImgExp.lib")

//////////////////////////////////////////////////////////////////

#endif/*__ImgExp_h__*/