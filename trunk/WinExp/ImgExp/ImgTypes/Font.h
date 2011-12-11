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
// Font - 字体对象类
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-10
// Version:	1.0.0001.1600
//
// History:
//	- 1.0.0001.1600(2011-08-10)	+ 添加CFont比较操作符重载
//////////////////////////////////////////////////////////////////

#ifndef __Font_h__
#define __Font_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/TypeObject.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CFont : public ITypeObjectT<font_t>
{
public:
	typedef ITypeObjectT<font_t> base_obj_t;

public:
	CFont(font_t tFont = NULL)
		: base_obj_t()
	{ Set(tFont); }
	virtual ~CFont()
	{}

public:
	font_t operator=(font_t tType)
	{
		Set(tType);
		return Get();
	}

	font_t Create(const LOGFONT* lpLogFont)
	{
		if (!lpLogFont) return Get();
		Set(::CreateFontIndirect(lpLogFont));
		return Get();
	}

	font_t Clone() const
	{
		LOGFONT lf = {0};
		GetLogFont(&lf);
		CFont font;
		return font.Create(&lf);
	}

	int GetLogFont(LOGFONT* pLogFont) const
	{
		return ::GetObject(Get(), sizeof(LOGFONT), pLogFont);
	}

	friend BOOL operator==(const CFont& fnt1, const CFont& fnt2)
	{
		LOGFONT lf1 = {0}, lf2 = {0};
		fnt1.GetLogFont(&lf1);
		fnt2.GetLogFont(&lf2);
		return (memcmp(&lf1, &lf2, sizeof(LOGFONT)) == 0);
	}
	friend BOOL operator!=(const CFont& fnt1, const CFont& fnt2)
	{
		return !(fnt1 == fnt2);
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Font_h__*/