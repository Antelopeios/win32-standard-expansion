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
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-06-09
// Version:	1.0.0001.1616
//
// History:
//	- 1.0.0000.0935(2011-05-24)	@ 构建完CGuiFade的第一个版本
//	- 1.0.0001.1616(2011-06-09)	# 修正当多个渐变对象同时执行时会因静态的alpha值而产生冲突的问题
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
	int m_Alpha;

public:
	CGuiFade()
		: m_Alpha(0)
	{}

protected:
	bool Overlap(IGuiCtrl* pCtrl, CImage& tNew, CImage& tOld)
	{
		if (!pCtrl) return false;
		if (m_Alpha >= EXP_CM) m_Alpha = 0;
		if (m_Alpha > EXP_CM - 25) m_Alpha = EXP_CM;
		//ExTrace(_T("0x%08X fade alpha: %d\n"), pCtrl, m_Alpha);

		CImage tmp_old(tOld.Clone());
		CImgRenderer::Render(tmp_old, tNew, CRect(), CPoint(), &CRenderCopy(m_Alpha));
		CImgRenderer::Render(tNew, tmp_old, CRect(), CPoint(), &CRenderCopy());

		m_Alpha += 25;
		return (m_Alpha < EXP_CM);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiFade, IGuiEffectBase)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiFade_hpp__*/