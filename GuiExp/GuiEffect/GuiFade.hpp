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
// GuiFade - 淡入淡出效果
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2010-05-24
// Version:	1.0.0000.0935
//////////////////////////////////////////////////////////////////

#ifndef __GuiFade_hpp__
#define __GuiFade_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiFade : public IGuiEffectBase
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiFade, IGuiEffectBase)

protected:
	bool Overlap(IGuiCtrl* pCtrl, CImage& tNew, CImage& tOld)
	{
		if (!pCtrl) return false;
		static int alpha = 0;
		if (alpha >= EXP_CM) alpha = 0;
		if (alpha > EXP_CM - 25) alpha = EXP_CM;
	//	ExTrace(_T("fade alpha: %d\n"), alpha);

		CImage tmp_old(tOld.Clone());
		CImgRenderer::Render(tmp_old, tNew, CRect(), CPoint(), &CFilterCopy(alpha));
		CImgRenderer::Render(tNew, tmp_old, CRect(), CPoint(), &CFilterCopy());

		alpha += 25;
		return (alpha < EXP_CM);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiFade, IGuiEffectBase)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiFade_hpp__*/