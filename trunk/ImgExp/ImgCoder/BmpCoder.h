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
// Date:	2011-04-06
// Version:	1.0.0002.1326
//
// History:
//	- 1.0.0001.2350(2011-04-05)	^ 优化BmpCoder的结构
//								+ 添加对16位位图的解析处理
//	- 1.0.0002.1326(2011-04-06)	= 32位位图不再解析Alpha通道
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
	EXP_INLINE static int PitchBytes(int nWidth, int nBitCount)
	{
		int wid_bits = nWidth * nBitCount;
		return (wid_bits >> 3) + ((wid_bits % 8 == 0) ? 0 : 1);
	}
	// 4 * ( (biWidth * biBitCount + 31) / 32 );
	EXP_INLINE static int PitchWidth(int nPitByts)
	{
		int wid_remd = nPitByts % 4;
		return nPitByts + ((wid_remd == 0) ? 0 : (4 - wid_remd));
	}
	EXP_INLINE static int PitchWidth(int nWidth, int nBitCount)
	{ return PitchWidth(PitchBytes(nWidth, nBitCount)); }

public:
	EXP_INLINE static bool CheckFile(IFileObject* pFile)
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

protected:
#pragma push_macro("PreDecode")
#undef PreDecode
#define PreDecode() \
	int img_w = bmiInfo.bmiHeader.biWidth; \
	int img_h = bmiInfo.bmiHeader.biHeight; \
	int bit_c = bmiInfo.bmiHeader.biBitCount; \
	int bit_s = bmhHead.bfOffBits; \
	int pit_b = PitchBytes(img_w, bit_c); \
	int pit_w = PitchWidth(pit_b); \
	CGC gc; \
	BYTE* temp = ExMem::Alloc<BYTE>(&gc, pit_b)
//#define PreDecode

	EXP_INLINE static void Decode32(IFileObject* pFile, BITMAPFILEHEADER& bmhHead, BITMAPINFO& bmiInfo, COLORREF* bmBuff)
	{
		if (!pFile || !bmBuff) return;
		PreDecode();
		// 解析图像
		for(int y = 0; y < img_h; ++y)
		{
			int pos = img_w * y;
			int inx = pit_w * y;
			pFile->Seek(bit_s + inx, IFileObject::begin);
			pFile->Read(temp, pit_b, 1);
			inx = 0;
			for(int x = 0; x < img_w; ++x, ++pos, inx += 4)
			{
				bmBuff[pos] = ExRGBA
					(
					temp[inx], 
					temp[inx + 1], 
					temp[inx + 2], 
					(BYTE)~0/*temp[inx + 3]*/
				);
			}
		}
	}

	EXP_INLINE static void Decode24(IFileObject* pFile, BITMAPFILEHEADER& bmhHead, BITMAPINFO& bmiInfo, COLORREF* bmBuff)
	{
		if (!pFile || !bmBuff) return;
		PreDecode();
		// 解析图像
		for(int y = 0; y < img_h; ++y)
		{
			int pos = img_w * y;
			int inx = pit_w * y;
			pFile->Seek(bit_s + inx, IFileObject::begin);
			pFile->Read(temp, pit_b, 1);
			inx = 0;
			for(int x = 0; x < img_w; ++x, ++pos, inx += 3)
			{
				bmBuff[pos] = ExRGBA
					(
					temp[inx], 
					temp[inx + 1], 
					temp[inx + 2], 
					(BYTE)~0
					);
			}
		}
	}

	EXP_INLINE static void Decode555(IFileObject* pFile, BITMAPFILEHEADER& bmhHead, BITMAPINFO& bmiInfo, COLORREF* bmBuff)
	{
		if (!pFile || !bmBuff) return;
		PreDecode();
		// 解析图像
		for(int y = 0; y < img_h; ++y)
		{
			int pos = img_w * y;
			int inx = pit_w * y;
			pFile->Seek(bit_s + inx, IFileObject::begin);
			pFile->Read(temp, pit_b, 1);
			inx = 0;
			for(int x = 0; x < img_w; ++x, ++pos, inx += 2)
			{
				bmBuff[pos] = ExRGBA
					(
					(temp[inx] & 0x1F) << 3, 
					(((temp[inx + 1] << 6) >> 3) + (temp[inx] >> 5)) << 3, 
					((temp[inx + 1] << 1) >> 3) << 3, 
					(BYTE)~0
					);
			}
		}
	}

	EXP_INLINE static void Decode565(IFileObject* pFile, BITMAPFILEHEADER& bmhHead, BITMAPINFO& bmiInfo, COLORREF* bmBuff)
	{
		if (!pFile || !bmBuff) return;
		PreDecode();
		// 解析图像
		for(int y = 0; y < img_h; ++y)
		{
			int pos = img_w * y;
			int inx = pit_w * y;
			pFile->Seek(bit_s + inx, IFileObject::begin);
			pFile->Read(temp, pit_b, 1);
			inx = 0;
			for(int x = 0; x < img_w; ++x, ++pos, inx += 2)
			{
				bmBuff[pos] = ExRGBA
					(
					(temp[inx] & 0x1F) << 3, 
					(((temp[inx + 1] << 5) >> 2) + (temp[inx] >> 5)) << 2, 
					(temp[inx + 1] >> 3) << 3, 
					(BYTE)~0
					);
			}
		}
	}

	EXP_INLINE static void Decode16(IFileObject* pFile, BITMAPFILEHEADER& bmhHead, BITMAPINFO& bmiInfo, COLORREF* bmBuff)
	{
		if (!pFile || !bmBuff) return;
		// 判断位图类型
		if (bmiInfo.bmiHeader.biCompression == BI_RGB)
			Decode555(pFile, bmhHead, bmiInfo, bmBuff);
		else // BI_BITFIELDS
		{
			// 获取掩码
			DWORD mask[3] = {0};
			pFile->Seek(bmhHead.bfOffBits - sizeof(mask), IFileObject::begin);
			pFile->Read(mask, sizeof(mask), 1);
			if (mask[0] == 0x7C00) // 555
				Decode555(pFile, bmhHead, bmiInfo, bmBuff);
			else // 565
				Decode565(pFile, bmhHead, bmiInfo, bmBuff);
		}
	}

	EXP_INLINE static void Decode8(IFileObject* pFile, BITMAPFILEHEADER& bmhHead, BITMAPINFO& bmiInfo, COLORREF* bmBuff)
	{
		if (!pFile || !bmBuff) return;
		PreDecode();
		// 获取调色板
		RGBQUAD colors[1 << 8] = {0};
		pFile->Seek(bit_s - sizeof(colors), IFileObject::begin);
		pFile->Read(colors, sizeof(colors), 1);
		// 解析图像
		for(int y = 0; y < img_h; ++y)
		{
			int pos = img_w * y;
			int inx = pit_w * y;
			pFile->Seek(bit_s + inx, IFileObject::begin);
			pFile->Read(temp, pit_b, 1);
			for(int x = 0; x < img_w; ++x, ++pos)
			{
				bmBuff[pos] = ExRGBA
					(
					colors[temp[x]].rgbBlue, 
					colors[temp[x]].rgbGreen, 
					colors[temp[x]].rgbRed, 
					(BYTE)~0
					);
			}
		}
	}

	EXP_INLINE static void Decode4(IFileObject* pFile, BITMAPFILEHEADER& bmhHead, BITMAPINFO& bmiInfo, COLORREF* bmBuff)
	{
		if (!pFile || !bmBuff) return;
		PreDecode();
		// 获取调色板
		RGBQUAD colors[1 << 4] = {0};
		pFile->Seek(bit_s - sizeof(colors), IFileObject::begin);
		pFile->Read(colors, sizeof(colors), 1);
		// 解析图像
		for(int y = 0; y < img_h; ++y)
		{
			int pos = img_w * y;
			int inx = pit_w * y;
			pFile->Seek(bit_s + inx, IFileObject::begin);
			pFile->Read(temp, pit_b, 1);
			inx = 0;
			for(int x = 0; x < img_w; ++inx)
			{
				for(int n = 1; n >= 0 && x < img_w; --n, ++x, ++pos)
				{
					BYTE c = (temp[inx] >> (4 * n)) & 0x0F;
					bmBuff[pos] = ExRGBA
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

	EXP_INLINE static void Decode1(IFileObject* pFile, BITMAPFILEHEADER& bmhHead, BITMAPINFO& bmiInfo, COLORREF* bmBuff)
	{
		if (!pFile || !bmBuff) return;
		PreDecode();
		// 获取调色板
		RGBQUAD colors[1 << 1] = {0};
		pFile->Seek(bit_s - sizeof(colors), IFileObject::begin);
		pFile->Read(colors, sizeof(colors), 1);
		// 解析图像
		for(int y = 0; y < img_h; ++y)
		{
			int pos = img_w * y;
			int inx = pit_w * y;
			pFile->Seek(bit_s + inx, IFileObject::begin);
			pFile->Read(temp, pit_b, 1);
			inx = 0;
			for(int x = 0; x < img_w; ++inx)
			{
				for(int n = 7; n >= 0 && x < img_w; --n, ++x, ++pos)
				{
					BYTE c = (temp[inx] >> (1 * n)) & 0x01;
					bmBuff[pos] = ExRGBA
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

#pragma pop_macro("PreDecode")

public:
	bool Encode(image_t Image)
	{
		return false;
	}
	image_t Decode()
	{
		IFileObject* file = GetFile();
		if(!CheckFile(file)) return NULL;
		CFileSeeker seeker(file);
		// 获取图像信息
		BITMAPFILEHEADER file_head = {0};
		file->Read(&file_head, sizeof(file_head), 1);
		BITMAPINFO file_info = {0};
		file->Read(&file_info, sizeof(file_info), 1);
		if (file_info.bmiHeader.biCompression != BI_RGB && 
			file_info.bmiHeader.biCompression != BI_BITFIELDS)
			return NULL;
		// 根据图像信息申请一个图像缓冲区
		COLORREF* bmbf = NULL;
		image_t image = 
			GetImageBuff(file_info.bmiHeader.biWidth, file_info.bmiHeader.biHeight, (BYTE*&)bmbf);
		if(!image) return NULL;
		// 解析图像信息
		switch (file_info.bmiHeader.biBitCount)
		{
		case 32:
			Decode32(file, file_head, file_info, bmbf);
			break;
		case 24:
			Decode24(file, file_head, file_info, bmbf);
			break;
		case 16:
			Decode16(file, file_head, file_info, bmbf);
			break;
		case 8:
			Decode8(file, file_head, file_info, bmbf);
			break;
		case 4:
			Decode4(file, file_head, file_info, bmbf);
			break;
		case 1:
			Decode1(file, file_head, file_info, bmbf);
			break;
		}
		// 返回image_t
		return image;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__BmpCoder_h__*/