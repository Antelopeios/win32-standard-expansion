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
// IcoCoder - ICO文件编/解码器 
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-24
// Version:	1.0.0000.1421
//
// History:
//	- 1.0.0000.1421(2011-08-24)	@ 开始构建IcoCoder
//////////////////////////////////////////////////////////////////

#ifndef __IcoCoder_h__
#define __IcoCoder_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgCoder/CoderObject.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CIcoCoder : public ICoderObject
{
public:
	typedef struct
	{
		BYTE	bWidth;			// Width, in pixels, of the image
		BYTE	bHeight;		// Height, in pixels, of the image
		BYTE	bColorCount;	// Number of colors in image (0 if >=8bpp)
		BYTE	bReserved;		// Reserved ( must be 0)
		WORD	wPlanes;		// Color Planes
		WORD	wBitCount;		// Bits per pixel
		DWORD	dwBytesInRes;	// How many bytes in this resource?
		DWORD	dwImageOffset;	// Where in the file is this image?
	} ICONDIRENTRY, *LPICONDIRENTRY;

	typedef struct
	{
		WORD			idReserved;		// Reserved (must be 0)
		WORD			idType;			// Resource Type (1 for icons)
		WORD			idCount;		// How many images?
		ICONDIRENTRY	idEntries[1];	// An entry for each image
	} ICONDIR, *LPICONDIR;

public:
	EXP_INLINE static bool CheckFile(IFileObject* pFile)
	{
		if(!pFile) return false;
		BYTE chk_head[6] = { 0, 0, 1, 0, 1, 0 };
		return ICoderObject::CheckFile(pFile, chk_head);
	}

public:
	CIcoCoder()
		: ICoderObject()
	{}
	CIcoCoder(IFileObject* pFile)
		: ICoderObject(pFile)
	{}

public:
	bool Encode(image_t Image)
	{
		IFileObject* file = GetFile();
		if(!file) return false;
		CImage exp_image;
		exp_image.SetTrust(false);
		exp_image = Image;
		if (exp_image.IsNull()) return false;
		// 填充图像信息
		BITMAPFILEHEADER file_head = {0};
		file_head.bfType = 0x4D42;
		file_head.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + exp_image.GetSize();
		file_head.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		BITMAPINFO file_info = {0};
		file_info.bmiHeader.biSize			= sizeof(file_info.bmiHeader);
		file_info.bmiHeader.biBitCount		= (exp_image.GetChannel() << 3);
		file_info.bmiHeader.biCompression	= BI_RGB;
		file_info.bmiHeader.biPlanes		= 1;
		file_info.bmiHeader.biWidth			= exp_image.GetWidth();
		file_info.bmiHeader.biHeight		= exp_image.GetHeight();
		// 写入文件
		file->Seek(0, IFileObject::begin);
		file->Write(&file_head, sizeof(file_head), 1);
		file->Write(&file_info, sizeof(file_info), 1);
		file->Write(exp_image.GetPixels(), exp_image.GetSize(), 1);
		file->Seek(0, IFileObject::begin);
		return true;
	}
	image_t Decode()
	{
		IFileObject* file = GetFile();
		if(!CheckFile(file)) return NULL;
		CFileSeeker seeker(file);
		// 获取图像信息
		ICONDIR file_head = {0};
		file->Read(&file_head, sizeof(file_head), 1);
		BITMAPINFO file_info = {0};
		file->Read(&file_info, sizeof(file_info), 1);
		if (file_info.bmiHeader.biCompression != BI_RGB && 
			file_info.bmiHeader.biCompression != BI_BITFIELDS)
			return NULL;

		return NULL;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__IcoCoder_h__*/