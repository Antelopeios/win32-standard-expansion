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
// Image - 图像对象类
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-16
// Version:	1.0.0011.2313
//
// History:
//	- 1.0.0001.1730(2011-04-07)	+ 添加IImageObject::GetSize()接口
//	- 1.0.0002.1716(2011-04-12)	^ 移除IImageObject接口,通过ITypeObjectT接口模板统一通用的接口
//								= CExpImage更名为CImage
//								# 修正CImage::IsNull()判断不完全的问题
//								# 修正CImage::Delete()没有正确删除对象的问题
//								+ 添加CImage::Clone()方法
//	- 1.0.0005.2350(2011-04-19)	= 当对CImage::Clone()传入空区域时,此接口将拷贝整个图片
//								# 修正CImage::IsNull()中的一个逻辑错误
//	- 1.0.0006.1730(2011-04-26)	= 将部分接口的返回值由DWORD改为LONG
//	- 1.0.0007.1343(2011-04-28)	= CImage::Clone()具有默认参数
//	- 1.0.0008.2319(2011-05-24)	+ 为CImage的属性获取接口及一些常量接口添加const类型
//	- 1.0.0009.0154(2011-07-13)	^ 提高CImage::Clone()在默认参数下的执行效率
//	- 1.0.0010.1530(2011-08-10)	^ 再次优化CImage::Clone()的执行效率
//	- 1.0.0011.2313(2012-03-16)	^ 优化CImage赋值接口,当对象赋值时无需重新获取计算图像的各种属性
//
// History(CExpImage):
//	- 1.0.0001.1730(2011-04-07)	+ 添加CExpImage::GetSize()接口
//	- 1.0.0002.2100(2011-04-07)	= 调整CExpImage::GetChannel()接口的定义,返回通道数量,而不是BitsCount
//////////////////////////////////////////////////////////////////

#ifndef __Image_h__
#define __Image_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Types/Types.h"
#include "ImgTypes/TypeObject.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CImage : public ITypeObjectT<image_t>
{
public:
	typedef ITypeObjectT<image_t> base_obj_t;

protected:
	BITMAP m_Bitmap;

public:
	CImage(image_t tImage = NULL)
		: base_obj_t()
	{ Set(tImage); }
	virtual ~CImage()
	{}

public:
	void Set(image_t tImage)
	{
		base_obj_t::Set(tImage);
		ZeroMemory(&m_Bitmap, sizeof(m_Bitmap));
		if (base_obj_t::IsNull()) return;
		::GetObject(Get(), sizeof(m_Bitmap), &m_Bitmap);
	}
	BOOL IsNull() const
	{
		if (base_obj_t::IsNull())
			return TRUE;
		else
			return (!(BOOL)(GetPixels()));
	}

	image_t operator=(image_t tType)
	{
		return base_obj_t::operator=(tType);
	}
	CImage& operator=(CImage& tType)
	{
		base_obj_t::Set(tType);
		memcpy(&m_Bitmap, &(tType.m_Bitmap), sizeof(m_Bitmap));
		return (*this);
	}

	image_t Create(DWORD nWidth, DWORD nHeight)
	{
		if (nWidth <= 0 || nHeight <= 0) return Get();
		BITMAPINFO bmi = {0};
		bmi.bmiHeader.biSize		= sizeof(bmi.bmiHeader);
		bmi.bmiHeader.biBitCount	= 32;
		bmi.bmiHeader.biCompression	= BI_RGB;
		bmi.bmiHeader.biPlanes		= 1;
		bmi.bmiHeader.biWidth		= nWidth;
		bmi.bmiHeader.biHeight		= nHeight;
		BYTE* img_buf = NULL;
		Set(CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&img_buf, NULL, 0));
		if(!img_buf) Delete();
		return Get();
	}

	image_t Clone(const CRect& tRect = CRect()) const;

	LONG GetWidth() const
	{ return m_Bitmap.bmWidth; }
	LONG GetHeight() const
	{ return m_Bitmap.bmHeight; }
	uint8_t GetChannel() const
	{ return (uint8_t)(m_Bitmap.bmBitsPixel >> 3); }
	LONG GetSize() const
	{ return m_Bitmap.bmWidthBytes * m_Bitmap.bmHeight; }
	pixel_t* GetPixels() const
	{ return (pixel_t*)m_Bitmap.bmBits; }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Image_h__*/