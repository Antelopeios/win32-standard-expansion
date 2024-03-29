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
// ImgAnalyzer - 图像分析器
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-19
// Version:	1.0.0002.1620
//
// History:
//	- 1.0.0001.1730(2011-04-07)	+ 支持直接通过ImageFormat返回对应的ICoderObject
//	- 1.0.0002.1620(2011-05-19)	+ CImgAnalyzer::GetCoder()支持默认传入空指针GC构造对象
//////////////////////////////////////////////////////////////////

#ifndef __ImgAnalyzer_h__
#define __ImgAnalyzer_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgCoder/BmpCoder.h"
#include "ImgCoder/JpgCoder.h"
#include "ImgCoder/PngCoder.h"
#include "ImgCoder/IcoCoder.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CImgAnalyzer
{
public:
	enum ImageFormat { raw, bmp, jpg, png, ico };

public:
	static int GetImageFormat(IFileObject* pFile)
	{
		if (!pFile) return raw;
		if (CBmpCoder::CheckFile(pFile))
			return bmp;
		else
		if (CJpgCoder::CheckFile(pFile))
			return jpg;
		else
		if (CPngCoder::CheckFile(pFile))
			return png;
		else
		if (CIcoCoder::CheckFile(pFile))
			return ico;
		return raw;
	}
	static ICoderObject* GetCoder(int nFormat, CGC* pGC = NULL)
	{
		ICoderObject* coder = NULL;
		switch (nFormat)
		{
		case bmp:
			coder = pGC ? gcnew(*pGC, CBmpCoder) : dbnew(CBmpCoder);
			break;
		case jpg:
			coder = pGC ? gcnew(*pGC, CJpgCoder) : dbnew(CJpgCoder);
			break;
		case png:
			coder = pGC ? gcnew(*pGC, CPngCoder) : dbnew(CPngCoder);
			break;
		case ico:
			coder = pGC ? gcnew(*pGC, CIcoCoder) : dbnew(CIcoCoder);
			break;
		}
		return coder;
	}
	static ICoderObject* GetCoder(IFileObject* pFile, CGC* pGC = NULL)
	{
		if (!pFile) return NULL;
		ICoderObject* coder = GetCoder(GetImageFormat(pFile), pGC);
		if (coder) coder->SetFile(pFile);
		return coder;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImgAnalyzer_h__*/