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
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-03
// Version:	1.0.0000.2123
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
	typedef struct {
		struct jpeg_source_mgr pub;
		IFileObject* infile;
		JOCTET * buffer;		/* start of buffer */
		boolean start_of_file;	/* have we gotten any data yet? */
	} my_source_mgr;
	typedef my_source_mgr * my_src_ptr;

	static const DWORD INPUT_BUF_SIZE = 4096;	/* choose an efficiently fread'able size */

	METHODDEF(void) init_source (j_decompress_ptr cinfo)
	{
		my_src_ptr src = (my_src_ptr) cinfo->src;

		/* We reset the empty-input-file flag for each image,
		* but we don't clear the input buffer.
		* This is correct behavior for reading a series of images from one source.
		*/
		src->start_of_file = TRUE;
	}

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

	METHODDEF(void) skip_input_data (j_decompress_ptr cinfo, long num_bytes)
	{
		my_src_ptr src = (my_src_ptr) cinfo->src;

		/* Just a dumb implementation for now.  Could use fseek() except
		* it doesn't work on pipes.  Not clear that being smart is worth
		* any trouble anyway --- large skips are infrequent.
		*/
		if (num_bytes > 0) {
			while (num_bytes > (long) src->pub.bytes_in_buffer) {
				num_bytes -= (long) src->pub.bytes_in_buffer;
				(void) src->pub.fill_input_buffer(cinfo);
				/* note we assume that fill_input_buffer will never return FALSE,
				* so suspension need not be handled.
				*/
			}
			src->pub.next_input_byte += (size_t) num_bytes;
			src->pub.bytes_in_buffer -= (size_t) num_bytes;
		}
	}

	METHODDEF(void) term_source (j_decompress_ptr cinfo)
	{
		/* no work necessary here */
	}

	static void jpeg_stdio_src (j_decompress_ptr cinfo, IFileObject* infile)
	{
		my_src_ptr src;
		if (cinfo->src == NULL) {	/* first time for this JPEG object? */
			cinfo->src = (struct jpeg_source_mgr *)
				(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, SIZEOF(my_source_mgr));
			src = (my_src_ptr) cinfo->src;
			src->buffer = (JOCTET *)
				(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, INPUT_BUF_SIZE * SIZEOF(JOCTET));
		}
		src = (my_src_ptr) cinfo->src;
		src->pub.init_source = init_source;
		src->pub.fill_input_buffer = fill_input_buffer;
		src->pub.skip_input_data = skip_input_data;
		src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
		src->pub.term_source = term_source;
		src->infile = infile;
		src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
		src->pub.next_input_byte = NULL; /* until buffer loaded */
	}

public:
	static bool CheckFile(IFileObject* pFile)
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
		return false;
	}
	image_t Decode()
	{
		IFileObject* pFile = GetFile();
		if(!CheckFile(pFile)) return NULL;
		CFileSeeker seeker(pFile);
		CGC gc;
		// 声明并初始化解压缩对象
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr jerr;
		// 制定错误信息管理器
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);
		// 将打开的图像文件指定为解压缩对象的源文件
		jpeg_stdio_src(&cinfo, pFile);
		// 读取图像信息
		jpeg_read_header(&cinfo, TRUE);
		// 根据图像信息申请一个图像缓冲区
		BITMAPINFO bmi = {0};
		bmi.bmiHeader.biSize		= sizeof(bmi.bmiHeader);
		bmi.bmiHeader.biBitCount	= 32;
		bmi.bmiHeader.biCompression	= BI_RGB;
		bmi.bmiHeader.biPlanes		= 1;
		bmi.bmiHeader.biWidth		= cinfo.image_width;
		bmi.bmiHeader.biHeight		= cinfo.image_height;
		bmi.bmiHeader.biSizeImage	= 
			(bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight * bmi.bmiHeader.biBitCount) >> 3;
		COLORREF* bmbf = NULL;
		image_t image = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&bmbf, NULL, 0);
		if(!bmbf)
		{
			// 释放资源
			jpeg_destroy_decompress(&cinfo);
			return NULL;
		}
		// 开始解压缩
		jpeg_start_decompress(&cinfo);
		DWORD size = (DWORD)(cinfo.image_width * cinfo.num_components);
		BYTE* data = (BYTE*)ExGC::Alloc<BYTE>(&gc, size);
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
				bmbf[pos] = ExRGBA(data[inx + 2], data[inx + 1], data[inx], (BYTE)~0);
		}
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