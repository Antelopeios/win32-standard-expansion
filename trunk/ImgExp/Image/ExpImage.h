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
// ExpImage - 图像拓展类
//
// Author:	木头云
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-07
// Version:	1.0.0002.2100
//
// History:
//	- 1.0.0001.1730(2011-04-07)	+ 添加CExpImage::GetSize()接口
//	- 1.0.0002.2100(2011-04-07)	= 调整CExpImage::GetChannel()接口的定义,返回通道数量,而不是BitsCount
//////////////////////////////////////////////////////////////////

#ifndef __ExpImage_h__
#define __ExpImage_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Image/ImageObject.h"

//////////////////////////////////////////////////////////////////

class CExpImage : public IImageObject
{
protected:
	BITMAP m_Bitmap;

public:
	CExpImage()
		: IImageObject()
	{ ZeroMemory(&m_Bitmap, sizeof(m_Bitmap)); }
	CExpImage(image_t Image)
		: IImageObject()
	{ SetImage(Image); }
	virtual ~CExpImage()
	{}

public:
	virtual void SetImage(image_t Image)
	{
		m_Image = Image;
		if(!m_Image) return;
		ZeroMemory(&m_Bitmap, sizeof(m_Bitmap));
		GetObject(m_Image, sizeof(m_Bitmap), &m_Bitmap);
	}

	bool Delete()
	{
		bool ret = true;
		if (IsNull())
		{
			ret = ::DeleteObject(m_Image);
			m_Image = NULL;
		}
		ZeroMemory(&m_Bitmap, sizeof(m_Bitmap));
		return ret;
	}
	image_t Create(DWORD nWidth, DWORD nHeight)
	{
		if (nWidth <= 0 || nHeight <= 0) return GetImage();
		BITMAPINFO bmi = {0};
		bmi.bmiHeader.biSize		= sizeof(bmi.bmiHeader);
		bmi.bmiHeader.biBitCount	= 32;
		bmi.bmiHeader.biCompression	= BI_RGB;
		bmi.bmiHeader.biPlanes		= 1;
		bmi.bmiHeader.biWidth		= nWidth;
		bmi.bmiHeader.biHeight		= nHeight;
		BYTE* img_buf = NULL;
		SetImage(CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&img_buf, NULL, 0));
		if(!img_buf) Delete();
		return GetImage();
	}

	DWORD GetWidth()
	{ return (DWORD)m_Bitmap.bmWidth; }
	DWORD GetHeight()
	{ return (DWORD)m_Bitmap.bmHeight; }
	uint8_t GetChannel()
	{ return (uint8_t)(m_Bitmap.bmBitsPixel >> 3); }
	DWORD GetSize()
	{ return m_Bitmap.bmWidthBytes * m_Bitmap.bmHeight; }
	pixel_t* GetPixels()
	{ return (pixel_t*)m_Bitmap.bmBits; }
};

//////////////////////////////////////////////////////////////////

#endif/*__ExpImage_h__*/