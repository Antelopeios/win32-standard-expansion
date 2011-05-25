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
// BmpCoder - JPG/JPEG文件编/解码器 
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-07
// Version:	1.0.0001.2126
//
// History:
//	- 1.0.0001.2126(2011-04-07)	^ 简化libjpeg的调用方式
//								+ 添加CJpgCoder::Encode()实现
//////////////////////////////////////////////////////////////////

#ifndef __JpgCoder_h__
#define __JpgCoder_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgCoder/CoderObject.h"
extern "C" {
#include "ImgCoder/jpeg/jinclude.h"
#include "ImgCoder/jpeg/jerror.h"
#include "ImgCoder/jpeg/jpeglib.h"
}

EXP_BEG

//////////////////////////////////////////////////////////////////

class CJpgCoder : public ICoderObject
{
protected:
	// 重写Read回调

	typedef struct {
		struct jpeg_source_mgr pub;
		IFileObject* infile;
		JOCTET * buffer;		/* start of buffer */
		boolean start_of_file;	/* have we gotten any data yet? */
	} my_source_mgr;
	typedef my_source_mgr * my_src_ptr;

	static const DWORD INPUT_BUF_SIZE = 4096;	/* choose an efficiently fread'able size */

	METHODDEF(boolean) fill_input_buffer (j_decompress_ptr cinfo)
	{
		my_src_ptr src = (my_src_ptr) cinfo->src;
		size_t nbytes;

		nbytes = src->infile->Read(src->buffer, INPUT_BUF_SIZE, 1);

		if (nbytes <= 0) {
			if (src->start_of_file)	/* Treat empty input file as fatal error */
				ERREXIT(cinfo, JERR_INPUT_EMPTY);
			WARNMS(cinfo, JWRN_JPEG_EOF);
			/* Insert a fake EOI marker */
			src->buffer[0] = (JOCTET) 0xFF;
			src->buffer[1] = (JOCTET) JPEG_EOI;
			nbytes = 2;
		}

		src->pub.next_input_byte = src->buffer;
		src->pub.bytes_in_buffer = nbytes;
		src->start_of_file = FALSE;

		return TRUE;
	}

	static void jpeg_stdio_src (j_decompress_ptr cinfo, IFileObject* infile)
	{
		::jpeg_stdio_src(cinfo, (FILE*)infile);
		my_src_ptr src = (my_src_ptr)(cinfo->src);
		src->pub.fill_input_buffer = fill_input_buffer;
	}

	// 重写Write回调

	typedef struct {
		struct jpeg_destination_mgr pub;
		IFileObject* outfile;
		JOCTET * buffer;		/* start of buffer */
	} my_destination_mgr;
	typedef my_destination_mgr * my_dest_ptr;

	static const DWORD OUTPUT_BUF_SIZE = 4096;	/* choose an efficiently fwrite'able size */

	METHODDEF(boolean) empty_output_buffer (j_compress_ptr cinfo)
	{
		my_dest_ptr dest = (my_dest_ptr) cinfo->dest;

		if (dest->outfile->Write(dest->buffer, OUTPUT_BUF_SIZE, 1) != (size_t) OUTPUT_BUF_SIZE)
			ERREXIT(cinfo, JERR_FILE_WRITE);

		dest->pub.next_output_byte = dest->buffer;
		dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

		return TRUE;
	}

	METHODDEF(void) term_destination (j_compress_ptr cinfo)
	{
		my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
		size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

		/* Write any data remaining in the buffer */
		if (datacount > 0) {
			if (dest->outfile->Write(dest->buffer, datacount, 1) != datacount)
				ERREXIT(cinfo, JERR_FILE_WRITE);
		}
		dest->outfile->Flush();
		/* Make sure we wrote the output file OK */
		if (dest->outfile->Error())
			ERREXIT(cinfo, JERR_FILE_WRITE);
	}

	static void jpeg_stdio_dest (j_compress_ptr cinfo, IFileObject* outfile)
	{
		::jpeg_stdio_dest(cinfo, (FILE*)outfile);
		my_dest_ptr dest = (my_dest_ptr)(cinfo->dest);
		dest->pub.empty_output_buffer = empty_output_buffer;
		dest->pub.term_destination = term_destination;
	}

public:
	EXP_INLINE static bool CheckFile(IFileObject* pFile)
	{
		if(!pFile) return false;
		BYTE chk_head[2] = { 0xFF, 0xD8 };
		return ICoderObject::CheckFile(pFile, chk_head);
	}

public:
	CJpgCoder()
		: ICoderObject()
	{}
	CJpgCoder(IFileObject* pFile)
		: ICoderObject(pFile)
	{}

public:
	bool Encode(image_t Image)
	{
		IFileObject* file = GetFile();
		if(!file) return false;
		CImage exp_image(Image);
		if (exp_image.IsNull()) return false;
		// 声明并初始化压缩对象
		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;
		// 制定错误信息管理器
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_compress(&cinfo);
		// 将打开的压缩对象指定为图像文件的输出文件
		jpeg_stdio_dest(&cinfo, file);
		// 设置压缩参数
		cinfo.image_width = exp_image.GetWidth();
		cinfo.image_height = exp_image.GetHeight();
		cinfo.input_components = 3;
		cinfo.in_color_space = JCS_RGB;
		jpeg_set_defaults(&cinfo);
		jpeg_set_quality(&cinfo, 80, true);
		// 开始压缩
		jpeg_start_compress(&cinfo, TRUE);
		JSAMPROW row_pointer[1];
		pixel_t* pixel = exp_image.GetPixels();
		DWORD size = (DWORD)(cinfo.image_width * cinfo.input_components);
		BYTE* data = ExMem::Alloc<BYTE>(size);
		while (cinfo.next_scanline < cinfo.image_height)
		{
			// 设置标记
			int pos = (cinfo.image_height - cinfo.next_scanline - 1) * cinfo.image_width;
			int inx = 0;
			// 读取image_t
			for(size_t x = 0; x < cinfo.image_width; ++x, ++pos, inx += cinfo.input_components)
			{
				data[inx + 2] = ExGetR(pixel[pos]);
				data[inx + 1] = ExGetG(pixel[pos]);
				data[inx]	  = ExGetB(pixel[pos]);
			}
			// 压缩数据
			row_pointer[0] = data;
			jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}
		ExMem::Free(data);
		jpeg_finish_compress(&cinfo);
		// 释放资源
		jpeg_destroy_compress(&cinfo);
		return true;
	}
	image_t Decode()
	{
		IFileObject* file = GetFile();
		if(!CheckFile(file)) return NULL;
		CFileSeeker seeker(file);
		// 声明并初始化解压缩对象
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr jerr;
		// 制定错误信息管理器
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);
		// 将打开的图像文件指定为解压缩对象的源文件
		jpeg_stdio_src(&cinfo, file);
		// 读取图像信息
		jpeg_read_header(&cinfo, TRUE);
		// 根据图像信息申请一个图像缓冲区
		pixel_t* bmbf = NULL;
		image_t image = GetImageBuff(cinfo.image_width, cinfo.image_height, (BYTE*&)bmbf);
		if(!image)
		{
			// 释放资源
			jpeg_destroy_decompress(&cinfo);
			return NULL;
		}
		// 开始解压缩
		jpeg_start_decompress(&cinfo);
		DWORD size = (DWORD)(cinfo.image_width * cinfo.num_components);
		BYTE* data = ExMem::Alloc<BYTE>(size);
		JSAMPROW row_pointer[1];
		while (cinfo.output_scanline < cinfo.output_height)
		{
			// 设置标记
			int pos = (cinfo.output_height - cinfo.output_scanline - 1) * cinfo.image_width;
			int inx = 0;
			// 解压数据
			ZeroMemory(data, size);
			row_pointer[0] = data;
			jpeg_read_scanlines(&cinfo, row_pointer, 1);
			// 写入image_t
			for(size_t x = 0; x < cinfo.image_width; ++x, ++pos, inx += cinfo.num_components)
				bmbf[pos] = ExRGBA(data[inx + 2], data[inx + 1], data[inx], EXP_CM);
		}
		ExMem::Free(data);
		jpeg_finish_decompress(&cinfo);
		// 释放资源
		jpeg_destroy_decompress(&cinfo);
		// 返回image_t
		return image;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__JpgCoder_h__*/