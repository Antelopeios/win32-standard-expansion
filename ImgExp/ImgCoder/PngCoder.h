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
// PngCoder - PNG文件编/解码器
//
// Author:	木头云
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-03
// Version:	1.0.0000.2200
//////////////////////////////////////////////////////////////////

#ifndef __PngCoder_h__
#define __PngCoder_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgCoder/CoderObject.h"
#include "ImgCoder/png/png.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CPngCoder : public ICoderObject
{
protected:
	static void PNGAPI png_coder_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		png_size_t check;
		if(png_ptr == NULL) return;
		check = (png_size_t)((IFileObject*)(png_ptr->io_ptr))->Read(data, length, 1);
		if (check != length) png_error(png_ptr, "Read Error");
	}

public:
	EXP_INLINE static bool CheckFile(IFileObject* pFile)
	{
		if(!pFile) return false;
		BYTE chk_head[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
		return ICoderObject::CheckFile(pFile, chk_head);
	}

public:
	CPngCoder()
		: ICoderObject()
	{}
	CPngCoder(IFileObject* pFile)
		: ICoderObject(pFile)
	{}

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
		// 声明并初始化解压缩对象
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		png_infop info_ptr = png_create_info_struct(png_ptr);
		// 制定错误信息管理器
		setjmp(png_jmpbuf(png_ptr));
		// 将打开的图像文件指定为解压缩对象的源文件
		png_set_read_fn(png_ptr, (png_voidp)file, png_coder_read_data);
		// 读取图像信息
		png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
		// 获得图像内存块
		int img_w = png_get_image_width(png_ptr, info_ptr);
		int img_h = png_get_image_height(png_ptr, info_ptr);
		int img_c = png_get_channels(png_ptr, info_ptr);
		png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);
		// 根据图像信息申请一个图像缓冲区
		pixel_t* bmbf = NULL;
		image_t image = GetImageBuff(img_w, img_h, (BYTE*&)bmbf);
		if(!image)
		{
			// 释放资源
			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
			return NULL;
		}
		// 解析图像信息
		for(int y = 0; y < img_h; ++y)
		{
			int pos = img_w * (img_h - y - 1);
			int inx = 0;
			for(int x = 0; x < img_w; ++x, ++pos, inx += img_c)
			{
				bmbf[pos] = ExRGBA
					(
					row_pointers[y][inx + 2], 
					row_pointers[y][inx + 1], 
					row_pointers[y][inx], 
					(img_c == 4) ? row_pointers[y][inx + 3] : (BYTE)~0
					);
			}
		}
		// 释放资源
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		return image;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__PngCoder_h__*/