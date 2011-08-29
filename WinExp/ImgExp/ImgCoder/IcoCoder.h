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
// Date:	2011-08-29
// Version:	1.0.0001.1447
//
// History:
//	- 1.0.0000.1421(2011-08-24)	@ 开始构建IcoCoder
//	- 1.0.0001.1447(2011-08-29)	@ IcoCoder对单图标ico的解析已基本完成,编码部分尚还有些问题
//////////////////////////////////////////////////////////////////

#ifndef __IcoCoder_h__
#define __IcoCoder_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgCoder/BmpCoder.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CIcoCoder : public CBmpCoder
{
public:

#pragma pack(1)

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

#pragma pack()

public:
	EXP_INLINE static bool CheckFile(IFileObject* pFile)
	{
		if(!pFile) return false;
		BYTE chk_head[6] = { 0, 0, 1, 0, 1, 0 };
		return ICoderObject::CheckFile(pFile, chk_head);
	}

public:
	CIcoCoder()
		: CBmpCoder()
	{}
	CIcoCoder(IFileObject* pFile)
		: CBmpCoder(pFile)
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
		ICONDIR file_head = {0};
		file_head.idType = 1;
		file_head.idCount = 1;
		file_head.idEntries->bWidth = (BYTE)exp_image.GetWidth();
		file_head.idEntries->bHeight = (BYTE)exp_image.GetHeight();
		file_head.idEntries->wPlanes = 1;
		file_head.idEntries->wBitCount = (exp_image.GetChannel() << 3);
		WORD off_clr = file_head.idEntries->wBitCount * file_head.idEntries->wPlanes;
		file_head.idEntries->bColorCount = off_clr < 8 ? 1 << off_clr : 0;
		file_head.idEntries->dwBytesInRes = exp_image.GetSize() + ((exp_image.GetWidth() * exp_image.GetHeight()) >> 8);
		file_head.idEntries->dwImageOffset = sizeof(file_head);
		BITMAPINFO file_info = {0};
		file_info.bmiHeader.biSize			= sizeof(file_info.bmiHeader);
		file_info.bmiHeader.biBitCount		= (exp_image.GetChannel() << 3);
		file_info.bmiHeader.biCompression	= BI_RGB;
		file_info.bmiHeader.biPlanes		= 1;
		file_info.bmiHeader.biWidth			= exp_image.GetWidth();
		file_info.bmiHeader.biHeight		= exp_image.GetHeight() << 1;
		// 写入文件
		file->Seek(0, IFileObject::begin);
		file->Write(&file_head, sizeof(file_head), 1);
		file->Write(&file_info, sizeof(file_info), 1);
		file->Write(exp_image.GetPixels(), exp_image.GetSize(), 1);
		pixel_t* tpbf = exp_image.GetPixels();
		long clr_size = (exp_image.GetWidth() * exp_image.GetHeight()) >> 8;
		BYTE* mkbf = (BYTE*)ZeroMemory(ExMem::Alloc<BYTE>(clr_size), clr_size * sizeof(BYTE));
		for(long i = 0; i < clr_size; ++i)
		{
			for(int n = 7; n >= 0; --n, ++tpbf)
				if (*tpbf == 0)
					mkbf[i] |= (0x01 << (1 * n));
		}
		file->Write(mkbf, clr_size, sizeof(BYTE));
		ExMem::Free(mkbf);
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
		file->Seek(file_head.idEntries->dwImageOffset, IFileObject::begin);
		BITMAPINFO file_info = {0};
		file->Read(&file_info, sizeof(file_info), 1);
		if (file_info.bmiHeader.biCompression != BI_RGB && 
			file_info.bmiHeader.biCompression != BI_BITFIELDS)
			return NULL;
		file_info.bmiHeader.biHeight >>= 1;
		// 填充BMP头
		BITMAPFILEHEADER bmp_head = {0};
		bmp_head.bfType = 0x4D42;
		bmp_head.bfSize = file_head.idEntries->dwImageOffset + file_head.idEntries->dwBytesInRes;
		long clr_size = 0;
		switch (file_info.bmiHeader.biBitCount)
		{
		case 16:
			clr_size = sizeof(uint32_t) * 3;
			break;
		case 8:
			clr_size = sizeof(RGBQUAD[1 << 8]);
			break;
		case 4:
			clr_size = sizeof(RGBQUAD[1 << 4]);
			break;
		case 1:
			clr_size = sizeof(RGBQUAD[1 << 1]);
			break;
		}
		bmp_head.bfOffBits = file_head.idEntries->dwImageOffset + sizeof(BITMAPINFOHEADER) + clr_size;
		// 根据图像信息申请一个图像缓冲区
		pixel_t* bmbf = NULL;
		image_t image = 
			GetImageBuff(file_info.bmiHeader.biWidth, file_info.bmiHeader.biHeight, (BYTE*&)bmbf);
		if(!image) return NULL;
		// 解析图像信息
		CBmpCoder::Decode(file, bmp_head, file_info, bmbf);
		// 掩码覆盖
		clr_size = (file_info.bmiHeader.biWidth * file_info.bmiHeader.biHeight) >> 8;
		BYTE* mkbf = ExMem::Alloc<BYTE>(clr_size);
		file->Read(mkbf, clr_size, sizeof(BYTE));
		pixel_t* tpbf = bmbf;
		for(long i = 0; i < clr_size; ++i)
		{
			for(int n = 7; n >= 0; --n, ++tpbf)
			{
				BYTE c = (mkbf[i] >> (1 * n)) & 0x01;
				if (c == 0)
				{
					if (ExGetA(*tpbf) == 0)
						*tpbf = ExRGBA
							(
							ExGetR(*tpbf), 
							ExGetG(*tpbf), 
							ExGetB(*tpbf), 
							EXP_CM
							);
				}
				else
				if (*tpbf != 0) *tpbf = 0;
			}
		}
		ExMem::Free(mkbf);
		// 返回image_t
		return image;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__IcoCoder_h__*/