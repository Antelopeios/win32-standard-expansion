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
// BmpCoder - BMP文件编/解码器 
//
// Author:	木头云
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-03
// Version:	1.0.0000.2000
//////////////////////////////////////////////////////////////////

#ifndef __BmpCoder_h__
#define __BmpCoder_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgCoder/CoderObject.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CBmpCoder : public ICoderObject
{
protected:
	EXP_INLINE static int PitchWidth(int nWidth, int nBitCount)
	{
		int wid_bits = nWidth * nBitCount;
		int wid_byts = (wid_bits >> 3) + ((wid_bits % 8 == 0) ? 0 : 1);
		int wid_remd = wid_byts % 4;
		return wid_byts + ((wid_remd == 0) ? 0 : (4 - wid_remd));
	}

public:
	static bool CheckFile(IFileObject* pFile)
	{
		if(!pFile) return false;
		BYTE chk_head[2] = { 0x42, 0x4D };
		return ICoderObject::CheckFile(pFile, chk_head);
	}

public:
	CBmpCoder()
		: ICoderObject()
	{}
	CBmpCoder(IFileObject* pFile)
		: ICoderObject(pFile)
	{}

public:
	bool Encode(image_t Image)
	{
		return false;
	}
	image_t Decode()
	{
		IFileObject* pFile = GetFile();
		if(!CheckFile(pFile)) return NULL;
		CFileSeeker seeker(pFile);
		// 获取图像信息
		BITMAPFILEHEADER file_head = {0};
		pFile->Read(&file_head, sizeof(file_head), 1);
		BITMAPINFO file_bmi = {0};
		pFile->Read(&file_bmi, sizeof(file_bmi), 1);
		if (file_bmi.bmiHeader.biCompression != BI_RGB)
			return NULL;
		size_t bit_c = file_bmi.bmiHeader.biBitCount;
		// 根据图像信息申请一个图像缓冲区
		BITMAPINFO bmi = {0};
		bmi.bmiHeader.biSize		= sizeof(bmi.bmiHeader);
		bmi.bmiHeader.biBitCount	= 32;
		bmi.bmiHeader.biCompression	= BI_RGB;
		bmi.bmiHeader.biPlanes		= 1;
		bmi.bmiHeader.biWidth		= file_bmi.bmiHeader.biWidth;
		bmi.bmiHeader.biHeight		= file_bmi.bmiHeader.biHeight;
		bmi.bmiHeader.biSizeImage	= 
			(bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight * bmi.bmiHeader.biBitCount) >> 3;
		COLORREF* bmbf = NULL;
		image_t image = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&bmbf, NULL, 0);
		if(!bmbf) return NULL;
		// 解析图像信息
		int pitch = PitchWidth(bmi.bmiHeader.biWidth, bit_c);
		switch (bit_c)
		{
		case 32:
			{
				// 解析图像
				BYTE temp[4];
				for(int y = 0; y < bmi.bmiHeader.biHeight; ++y)
				{
					int pos = bmi.bmiHeader.biWidth * y;
					int inx = pitch * y;
					pFile->Seek(file_head.bfOffBits + inx, IFileObject::begin);
					for(int x = 0; x < bmi.bmiHeader.biWidth; ++x, ++pos)
					{
						pFile->Read(temp, sizeof(temp), 1);
						bmbf[pos] = ExRGBA(temp[0], temp[1], temp[2], temp[3]);
					}
				}
			}
			break;
		case 24:
			{
				// 设置偏移量
				// 解析图像
				BYTE temp[3];
				for(int y = 0; y < bmi.bmiHeader.biHeight; ++y)
				{
					int pos = bmi.bmiHeader.biWidth * y;
					int inx = pitch * y;
					pFile->Seek(file_head.bfOffBits + inx, IFileObject::begin);
					for(int x = 0; x < bmi.bmiHeader.biWidth; ++x, ++pos)
					{
						pFile->Read(temp, sizeof(temp), 1);
						bmbf[pos] = ExRGBA(temp[0], temp[1], temp[2], (BYTE)~0);
					}
				}
			}
			break;
		case 16:
			break;
		case 8:
			{
				// 获取调色板
				RGBQUAD colors[1 << 8] = {0};
				pFile->Seek(file_head.bfOffBits - sizeof(colors), IFileObject::begin);
				pFile->Read(colors, sizeof(colors), 1);
				// 解析图像
				for(int y = 0; y < bmi.bmiHeader.biHeight; ++y)
				{
					int pos = bmi.bmiHeader.biWidth * y;
					int inx = pitch * y;
					pFile->Seek(file_head.bfOffBits + inx, IFileObject::begin);
					for(int x = 0; x < bmi.bmiHeader.biWidth; ++x, ++pos)
					{
						BYTE temp;
						pFile->Read(&temp, sizeof(temp), 1);
						bmbf[pos] = ExRGBA
							(
							colors[temp].rgbBlue, 
							colors[temp].rgbGreen, 
							colors[temp].rgbRed, 
							(BYTE)~0
							);
					}
				}
			}
			break;
		case 4:
			{
				// 获取调色板
				RGBQUAD colors[1 << 4] = {0};
				pFile->Seek(file_head.bfOffBits - sizeof(colors), IFileObject::begin);
				pFile->Read(colors, sizeof(colors), 1);
				// 解析图像
				for(int y = 0; y < bmi.bmiHeader.biHeight; ++y)
				{
					int pos = bmi.bmiHeader.biWidth * y;
					int inx = pitch * y;
					pFile->Seek(file_head.bfOffBits + inx, IFileObject::begin);
					for(int x = 0; x < bmi.bmiHeader.biWidth;)
					{
						BYTE temp;
						pFile->Read(&temp, sizeof(temp), 1);
						for(int n = 1; n >= 0 && x < bmi.bmiHeader.biWidth; --n, ++x, ++pos)
						{
							BYTE c = (temp >> (4 * n)) & 0x0F;
							bmbf[pos] = ExRGBA
								(
								colors[c].rgbBlue, 
								colors[c].rgbGreen, 
								colors[c].rgbRed, 
								(BYTE)~0
								);
						}
					}
				}
			}
			break;
		case 1:
			{
				// 获取调色板
				RGBQUAD colors[1 << 1] = {0};
				pFile->Seek(file_head.bfOffBits - sizeof(colors), IFileObject::begin);
				pFile->Read(colors, sizeof(colors), 1);
				// 解析图像
				for(int y = 0; y < bmi.bmiHeader.biHeight; ++y)
				{
					int pos = bmi.bmiHeader.biWidth * y;
					int inx = pitch * y;
					pFile->Seek(file_head.bfOffBits + inx, IFileObject::begin);
					for(int x = 0; x < bmi.bmiHeader.biWidth;)
					{
						BYTE temp;
						pFile->Read(&temp, sizeof(temp), 1);
						for(int n = 7; n >= 0 && x < bmi.bmiHeader.biWidth; --n, ++x, ++pos)
						{
							BYTE c = (temp >> (1 * n)) & 0x01;
							bmbf[pos] = ExRGBA
								(
								colors[c].rgbBlue, 
								colors[c].rgbGreen, 
								colors[c].rgbRed, 
								(BYTE)~0
								);
						}
					}
				}
			}
			break;
		}
		// 返回image_t
		return image;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__BmpCoder_h__*/