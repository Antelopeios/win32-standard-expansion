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
// ImageObject - 图像基类
//
// Author:	木头云
// Blog:	http://www.cnblogs.com/black-cat-ptr/
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-07
// Version:	1.0.0001.1730
//
// History:
//	- 1.0.0001.1730(2011-04-07)	+ 添加IImageObject::GetSize()接口
//////////////////////////////////////////////////////////////////

#ifndef __ImageObject_h__
#define __ImageObject_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

interface IImageObject : public INonCopyable
{
protected:
	image_t m_Image;

public:
	IImageObject()
		: m_Image(NULL)
	{}
	IImageObject(image_t Image)
		: m_Image(NULL)
	{ SetImage(Image); }
	virtual ~IImageObject()
	{}

public:
	virtual void SetImage(image_t Image)
	{ m_Image = Image; }
	virtual image_t GetImage()
	{ return m_Image; }

	image_t operator=(image_t Image)
	{
		SetImage(Image);
		return m_Image;
	}
	operator image_t()
	{ return GetImage(); }

	EXP_INLINE bool IsNull()
	{ return (m_Image == NULL); }

	virtual bool Delete() = 0;
	virtual image_t Create(DWORD nWidth, DWORD nHeight) = 0;

	virtual DWORD GetWidth() = 0;
	virtual DWORD GetHeight() = 0;
	virtual uint8_t GetChannel() = 0;
	virtual DWORD GetSize() = 0;
	virtual pixel_t* GetPixels() = 0;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ImageObject_h__*/