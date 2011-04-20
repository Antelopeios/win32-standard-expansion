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
// ImgCommon - 图像拓展库公用定义
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2010-04-20
// Version:	1.0.0004.2222
//
// History:
//	- 1.0.0001.1700(2010-04-07)	+ 添加pixel_t类型定义
//	- 1.0.0002.2120(2010-04-07)	+ 添加用于分离pixel_t通道分量的相关接口
//	- 1.0.0003.1700(2010-04-11)	+ 添加graph_t类型定义
//	- 1.0.0004.2222(2010-04-20)	+ 添加EXP_ZERO,ExIsZero()与ExIsEqual()定义,方便判断浮点数是否为0以及浮点数相等比较
//////////////////////////////////////////////////////////////////

#ifndef __ImgCommon_h__
#define __ImgCommon_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../StdExp/StdExp.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

typedef HDC			graph_t;
typedef HBITMAP		image_t;
typedef COLORREF	pixel_t;

#define ExRGB(r, g, b) RGB(r, g, b)
#define ExRGBA(r, g, b, a) \
	( (pixel_t) ((((BYTE)(r) | ((WORD)((BYTE)(g))<<8)) | (((DWORD)(BYTE)(b))<<16)) | (((DWORD)(BYTE)(a))<<24)) )

#define ExGetR(pix)	GetRValue(pix)
#define ExGetG(pix)	GetGValue(pix)
#define ExGetB(pix)	GetBValue(pix)
#define ExGetA(pix)	(LOBYTE((pix) >> 24))

#define EXP_ZERO	0.00000001
#define ExIsZero(num)	(((num) < EXP_ZERO) && ((num) > -EXP_ZERO))
#define ExIsEqual(a, b)	ExIsZero((a) - (b))

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgCommon_h__*/