// Copyright 2011-2012, ľͷ��
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
// ImgCommon - ͼ����չ�⹫�ö���
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-16
// Version:	1.0.0009.2356
//
// History:
//	- 1.0.0001.1700(2011-04-07)	+ ���pixel_t���Ͷ���
//	- 1.0.0002.2120(2011-04-07)	+ ������ڷ���pixel_tͨ����������ؽӿ�
//	- 1.0.0003.1700(2011-04-11)	+ ���graph_t���Ͷ���
//	- 1.0.0004.2222(2011-04-20)	+ ���EXP_ZERO,ExIsZero()��ExIsEqual()����,�����жϸ������Ƿ�Ϊ0�Լ���������ȱȽ�
//	- 1.0.0005.1730(2011-04-25)	+ ���EXP_PI����
//	- 1.0.0006.0952(2011-05-25)	+ ���EXP_CM�������(BYTE)~0
//	- 1.0.0007.1413(2011-06-28)	+ ���ExRevColor(),���ڵߵ������е���ɫͨ��
//	- 1.0.0008.1020(2011-12-19)	+ ���chann_t�������BYTE��ʾͨ��
//								+ ���ExDivCM()�������ټ���((x) / EXP_CM)
//	- 1.0.0009.2356(2012-03-16)	= graph_t����Ϊdc_t
//////////////////////////////////////////////////////////////////

#ifndef __ImgCommon_h__
#define __ImgCommon_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdExp.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// ���Ͷ���

typedef HDC			dc_t;	
typedef HBITMAP		image_t;
typedef HFONT		font_t;
typedef COLORREF	pixel_t;
typedef BYTE		chann_t;

// ���ܶ���

#define ExRGB(r, g, b) RGB(r, g, b)
#define ExRGBA(r, g, b, a) \
	( (EXP::pixel_t) ((((EXP::chann_t)(r) | \
	((WORD)((EXP::chann_t)(g))<<8)) | \
	(((DWORD)(EXP::chann_t)(b))<<16)) | \
	(((DWORD)(EXP::chann_t)(a))<<24)) )

#define ExGetR(pix)	GetRValue(pix)
#define ExGetG(pix)	GetGValue(pix)
#define ExGetB(pix)	GetBValue(pix)
#define ExGetA(pix)	(LOBYTE((pix) >> 24))

#define ExRevColor(pix) \
	ExRGBA(ExGetB(pix), ExGetG(pix), ExGetR(pix), ExGetA(pix))

#define EXP_ZERO	0.00000001
#define ExIsZero(num)	(((num) < EXP_ZERO) && ((num) > -EXP_ZERO))
#define ExIsEqual(a, b)	ExIsZero((a) - (b))

#define EXP_PI		3.14159265
#define EXP_CM		((EXP::chann_t)~0)

#define ExDivCM(x) (((x) + (((x) >> 8) + 1)) >> 8)
#define ExMulCM(x) (((x) << 8) - (x))

//////////////////////////////////////////////////////////////////

EXP_END

#include "ImgCommon/ImgASM.hpp"

//////////////////////////////////////////////////////////////////

#endif/*__ImgCommon_h__*/