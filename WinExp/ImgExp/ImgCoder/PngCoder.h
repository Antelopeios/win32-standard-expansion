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
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-07
// Version:	1.0.0001.2350
//
// History:
//	- 1.0.0001.2350(2011-04-07)	^ 优化CPngCoder::Decode()的实现,不再一次性解析图片的全部内容
//								+ 添加CPngCoder::Encode()实现
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
	// 重写Read回调

	static void PNGAPI png_coder_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		png_size_t check;
		if (png_ptr == NULL) return;
		check = (png_size_t)((IFileObject*)(png_ptr->io_ptr))->Read(data, length, 1);
		if (check != length) png_error(png_ptr, "Read Error");
	}

	// 重写Write回调

	static void PNGAPI png_coder_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		png_uint_32 check;
		if (png_ptr == NULL) return;
		check = (png_size_t)((IFileObject*)(png_ptr->io_ptr))->Write(data, length, 1);
		if (check != length) png_error(png_ptr, "Write Error");
	}

	static void PNGAPI png_coder_flush(png_structp png_ptr)
	{
		if(png_ptr == NULL) return;
		if (png_ptr->io_ptr) ((IFileObject*)(png_ptr->io_ptr))->Flush();
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
		IFileObject* file = GetFile();
		if(!file) return false;
		CImage exp_image;
		exp_image.SetTrust(false);
		exp_image = Image;
		if (exp_image.IsNull()) return false;
		// 声明并初始化压缩对象
		png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		png_infop info_ptr = png_create_info_struct(png_ptr);
		// 制定错误信息管理器
		setjmp(png_jmpbuf(png_ptr));
		// 将打开的压缩对象指定为图像文件的输出文件
		png_set_write_fn(png_ptr, (png_voidp)file, png_coder_write_data, png_coder_flush);
		// 设置压缩参数
		int img_w = exp_image.GetWidth();
		int img_h = exp_image.GetHeight();
		int img_c = 4;
		png_set_IHDR(png_ptr, info_ptr, img_w, img_h, 8, 
			PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		png_colorp palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
		png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
		// 写入图像信息
		png_write_info(png_ptr, info_ptr);
		png_set_packing(png_ptr);
		pixel_t* pixel = exp_image.GetPixels();
		png_bytep row_pointer = ExMem::Alloc<png_byte>(img_w * img_c);
		for(int y = 0; y < img_h; ++y)
		{
			// 读取image_t
			int pos = img_w * (img_h - y - 1);
			int inx = 0;
			for(int x = 0; x < img_w; ++x, ++pos, inx += img_c)
			{
				row_pointer[inx + 2] = ExGetR(pixel[pos]);
				row_pointer[inx + 1] = ExGetG(pixel[pos]);
				row_pointer[inx]	 = ExGetB(pixel[pos]);
				row_pointer[inx + 3] = ExGetA(pixel[pos]);
			}
			// 写入数据
			png_write_row(png_ptr, row_pointer);
		}
		ExMem::Free(row_pointer);
		png_write_end(png_ptr, info_ptr);
		// 释放资源
		png_free(png_ptr, palette);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return true;
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
		png_read_info(png_ptr, info_ptr);
		int img_w = png_get_image_width(png_ptr, info_ptr);
		int img_h = png_get_image_height(png_ptr, info_ptr);
		int img_c = png_get_channels(png_ptr, info_ptr);
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
		png_bytep row_pointer = ExMem::Alloc<png_byte>(img_w * img_c);
		for(int y = 0; y < img_h; ++y)
		{
			// 设置标记
			int pos = img_w * (img_h - y - 1);
			int inx = 0;
			// 解析数据
			png_read_row(png_ptr, row_pointer, NULL);
			// 写入image_t
			for(int x = 0; x < img_w; ++x, ++pos, inx += img_c)
			{
				bmbf[pos] = ExRGBA
					(
					row_pointer[inx + 2], 
					row_pointer[inx + 1], 
					row_pointer[inx], 
					(img_c == 4) ? row_pointer[inx + 3] : EXP_CM
					);
			}
		}
		ExMem::Free(row_pointer);
		png_read_end(png_ptr, info_ptr);
		// 释放资源
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		return image;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__PngCoder_h__*/