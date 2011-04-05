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
// CoderObject - 编/解码器基类 
//
// Author:	木头云
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-05
// Version:	1.0.0001.2350
//
// History:
//	- 1.0.0001.2350(2011-04-05)	= 将具体的image_t内存块申请工作统一放在ICoderObject中处理
//								= ICoderObject::DeleteImage()不再断言Image参数
//////////////////////////////////////////////////////////////////

#ifndef __CoderObject_h__
#define __CoderObject_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

interface ICoderObject
{
protected:
	IFileObject* m_pFile;

	template <DWORD SizeT>
	EXP_INLINE static bool CheckFile(IFileObject* pFile, const BYTE (&chkHead)[SizeT])
	{
		if(!pFile) return false;
		CFileSeeker seeker(pFile);
		BYTE tmp_buff[SizeT] = {0};
		// 判断头部
		if(!pFile->Seek(0, IFileObject::begin))
			return false;
		if (pFile->Read(tmp_buff, _countof(tmp_buff), sizeof(BYTE)) != _countof(chkHead))
			return false;
		if (memcmp(tmp_buff, chkHead, sizeof(chkHead)) != 0)
			return false;
		return true;
	}

	EXP_INLINE static image_t GetImageBuff(LONG nWidth, LONG nHeight, BYTE*& pBuff)
	{
		if (nWidth <= 0 || nHeight <= 0) return NULL;
		pBuff = NULL;
		BITMAPINFO bmi = {0};
		bmi.bmiHeader.biSize		= sizeof(bmi.bmiHeader);
		bmi.bmiHeader.biBitCount	= 32;
		bmi.bmiHeader.biCompression	= BI_RGB;
		bmi.bmiHeader.biPlanes		= 1;
		bmi.bmiHeader.biWidth		= nWidth;
		bmi.bmiHeader.biHeight		= nHeight;
		image_t image = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pBuff, NULL, 0);
		return pBuff ? image : NULL;
	}

public:
	ICoderObject()
		: m_pFile(NULL)
	{}
	ICoderObject(IFileObject* pFile)
		: m_pFile(NULL)
	{ SetFile(pFile); }
	virtual ~ICoderObject()
	{}

public:
	virtual void SetFile(IFileObject* pFile)
	{ m_pFile = pFile; }
	virtual IFileObject* GetFile()
	{ return m_pFile; }

	virtual bool Encode(image_t Image) = 0;
	virtual image_t Decode() = 0;

	EXP_INLINE static bool DeleteImage(image_t Image)
	{ return Image ? ::DeleteObject(Image) : true; }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__CoderObject_h__*/