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
// ImageObject - 图像对象类
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-19
// Version:	1.0.0004.1630
//
// History:
//	- 1.0.0001.1730(2011-04-07)	+ 添加IImageObject::GetSize()接口
//	- 1.0.0002.1716(2011-04-12)	^ 移除IImageObject接口,通过ITypeObjectT接口模板统一通用的接口
//								= CExpImage更名为CImage
//								# 修正CImage::IsNull()判断不完全的问题
//								# 修正CImage::Delete()没有正确删除对象的问题
//								+ 添加CImage::Clone()方法
//	- 1.0.0004.1630(2011-04-19)	= 当对CImage::Clone()传入空区域时,此接口将拷贝整个图片
//
// History(CExpImage):
//	- 1.0.0001.1730(2011-04-07)	+ 添加CExpImage::GetSize()接口
//	- 1.0.0002.2100(2011-04-07)	= 调整CExpImage::GetChannel()接口的定义,返回通道数量,而不是BitsCount
//////////////////////////////////////////////////////////////////

#ifndef __ImageObject_h__
#define __ImageObject_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Types/Types.h"
#include "ImgTypes/Graph/Graph.h"

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
		if (IsNull()) return;
		GetObject(Get(), sizeof(m_Bitmap), &m_Bitmap);
	}
	bool IsNull()
	{
		if (base_obj_t::IsNull())
			return (bool)(GetPixels());
		else
			return false;
	}

	bool Delete()
	{
		bool ret = true;
		if (!IsNull())
			ret = ::DeleteObject(Get());
		Set(NULL);
		return ret;
	}
	image_t Create(DWORD nWidth, DWORD nHeight)
	{
		if (nWidth <= 0 || nHeight <= 0) return Get();
		Delete();
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
	image_t Clone(CRect& tRect)
	{
		CRect rc_tmp(tRect);
		if (rc_tmp.IsEmpty())
			rc_tmp.Set(CPoint(0, 0), CPoint(GetWidth(), GetHeight()));
		// 创建临时对象
		CImage exp_img;
		if(!exp_img.Create(rc_tmp.Width(), rc_tmp.Height()))
			return NULL;
		CGraph exp_gra;
		if(!exp_gra.Create())
		{
			exp_img.Delete();
			return NULL;
		}
		CGraph exp_mem;
		if(!exp_mem.Create())
		{
			exp_img.Delete();
			exp_gra.Delete();
			return NULL;
		}
		// 拷贝图像
		exp_gra.SetObject(exp_img.Get());
		exp_mem.SetObject(Get());
		::BitBlt(exp_gra, 0, 0, exp_img.GetWidth(), exp_img.GetHeight(), 
				 exp_mem, rc_tmp.Left(), rc_tmp.Top(), SRCCOPY);
		// 清理并返回对象
		exp_mem.Delete();
		exp_gra.Delete();
		return exp_img.Get();
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

EXP_END

#endif/*__ImageObject_h__*/