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
// Image - 图像对象类实现
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-10
// Version:	1.0.0000.1530
//
// History:
//	- 1.0.0000.1530(2011-08-10)	= 将Image的部分实现移到cpp中完成
//////////////////////////////////////////////////////////////////

#include "Image.h"
#include "ImgPainter/ImgRenderer.h"

//////////////////////////////////////////////////////////////////

image_t CImage::Clone(const CRect& tRect/* = CRect()*/) const
{
	CRect rc_tmp(tRect), rect(0, 0, GetWidth(), GetHeight());
	if (rc_tmp.IsEmpty()) rc_tmp = rect;
	// 创建临时对象
	CImage exp_img;
	exp_img.SetTrust(false);
	if(!exp_img.Create(rc_tmp.Width(), rc_tmp.Height()))
		return NULL;
	// 拷贝图像
	if(!CImgRenderer::Render(exp_img, Get(), CRect(), rc_tmp.pt1, &CRenderCopy()))
	{
		exp_img.Delete();
		return NULL;
	}
	return exp_img;
}