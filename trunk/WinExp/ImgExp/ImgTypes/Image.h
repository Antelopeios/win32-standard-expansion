// Copyright 2011, ľͷ��
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
// Image - ͼ�������
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-07-13
// Version:	1.0.0009.0154
//
// History:
//	- 1.0.0001.1730(2011-04-07)	+ ����IImageObject::GetSize()�ӿ�
//	- 1.0.0002.1716(2011-04-12)	^ �Ƴ�IImageObject�ӿ�,ͨ��ITypeObjectT�ӿ�ģ��ͳһͨ�õĽӿ�
//								= CExpImage����ΪCImage
//								# ����CImage::IsNull()�жϲ���ȫ������
//								# ����CImage::Delete()û����ȷɾ�����������
//								+ ����CImage::Clone()����
//	- 1.0.0005.2350(2011-04-19)	= ����CImage::Clone()���������ʱ,�˽ӿڽ���������ͼƬ
//								# ����CImage::IsNull()�е�һ���߼�����
//	- 1.0.0006.1730(2011-04-26)	= �����ֽӿڵķ���ֵ��DWORD��ΪLONG
//	- 1.0.0007.1343(2011-04-28)	= CImage::Clone()����Ĭ�ϲ���
//	- 1.0.0008.2319(2011-05-24)	+ ΪCImage�����Ի�ȡ�ӿڼ�һЩ�����ӿ�����const����
//	- 1.0.0009.0154(2011-07-13)	^ ���CImage::Clone()��Ĭ�ϲ����µ�ִ��Ч��
//
// History(CExpImage):
//	- 1.0.0001.1730(2011-04-07)	+ ����CExpImage::GetSize()�ӿ�
//	- 1.0.0002.2100(2011-04-07)	= ����CExpImage::GetChannel()�ӿڵĶ���,����ͨ������,������BitsCount
//////////////////////////////////////////////////////////////////

#ifndef __Image_h__
#define __Image_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/Types/Types.h"
#include "ImgTypes/Graph.h"

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
	bool IsNull()
	{
		if (base_obj_t::IsNull())
			return true;
		else
			return (!(bool)(GetPixels()));
	}

	image_t operator=(image_t tType)
	{
		Set(tType);
		return Get();
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

	image_t Clone(CRect& tRect = CRect()) const
	{
		CRect rc_tmp(tRect), rect(0, 0, GetWidth(), GetHeight());
		if (rc_tmp.IsEmpty()) rc_tmp = rect;
		// ������ʱ����
		CImage exp_img;
		exp_img.SetTrust(false);
		if(!exp_img.Create(rc_tmp.Width(), rc_tmp.Height()))
			return NULL;
		if (rc_tmp == rect)
			memcpy(exp_img.GetPixels(), GetPixels(), GetSize());
		else
		{
			CGraph exp_gra;
			if(!exp_gra.Create())
				return NULL;
			CGraph exp_mem;
			if(!exp_mem.Create())
				return NULL;
			// ����ͼ��
			exp_gra.SetObject(exp_img.Get());
			exp_mem.SetObject(Get());
			::BitBlt(exp_gra, 0, 0, exp_img.GetWidth(), exp_img.GetHeight(), 
					 exp_mem, rc_tmp.Left(), rc_tmp.Top(), SRCCOPY);
			// ���������ض���
			exp_mem.Delete();
			exp_gra.Delete();
		}
		return exp_img;
	}

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