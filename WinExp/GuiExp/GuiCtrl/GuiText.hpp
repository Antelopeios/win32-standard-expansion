// Copyright 2012, 木头云
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
// GuiText - 文字控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-26
// Version:	1.0.0001.1207
//
// History:
//	- 1.0.0000.0918(2012-03-13)	@ 开始构建GuiText
//	- 1.0.0001.1207(2012-03-26)	# 修正当CGuiText在控件显示的时候没有刷新控件内部文字区域,导致滚动条无法自动加载的问题
//////////////////////////////////////////////////////////////////

#ifndef __GuiText_hpp__
#define __GuiText_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG
	
//////////////////////////////////////////////////////////////////

class _txt_font : public _pic_font
{
	EXP_DECLARE_DYNCREATE_CLS(_pic_font, _pic_font)

protected:
	CImage m_imgClp;
	CText m_txtTmp;
	CRect m_rcImg;
	CString m_StrTmp;

public:
	BOOL FmtTxtRect(CRect rect, CText* text, CString* str, IGuiCtrlBase* ctl)
	{
		if (!text || !str)
		{
			ctl->SetFraRect(CSize());
			ctl->SetAllRect(CSize());
			return FALSE;
		}

		if (m_txtTmp == (*text) && 
			m_rcImg == rect && 
			m_StrTmp == *str) return TRUE;

		m_txtTmp = (*text);
		m_rcImg = rect;
		m_StrTmp = (*str);

		// 计算缓存图大小并预存分段位图
		CArrayT<CString> sa;
		ExStringToArray(*str, sa, _T('\n'), FALSE);
		CArrayT<CImage> ia;
		CArrayT<LONG> fa;
		LONG h = 0;
		for(DWORD i = 0; i < sa.GetCount(); ++i)
		{
			fa.PushLast(0);
			ia.PushLast(text->GetImage(sa[i], rect, 2, _T("..."), &(fa[i])));
			if (!ia[i].IsNull()) h += fa[i];
		}

		// 绘制缓存
		rect.MoveTo(CPoint());
		rect.Height(h);
		m_imgClp.Create(rect.Width(), rect.Height());
		for(DWORD i = 0; i < ia.GetCount(); ++i)
		{
			if (ia[i].IsNull()) continue;
			CImgDrawer::Draw(m_imgClp, ia[i], rect);
			rect.Top(rect.Top() + fa[i]);
		}

		ctl->SetFraRect(CSize(0, m_rcImg.Height()));
		ctl->SetAllRect(CSize(0, h));

		return TRUE;
	}
	void Msg(UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		if (nMessage == WM_SHOWWINDOW)
		{
			if (!wParam) return;
			if (!m_Val) return;
			CString* str = (CString*)Ctl()->GetState(_T("text"));
			if (!str) return;

			CRect rect;
			Ctl()->GetClientRect(rect);

			FmtTxtRect(rect, m_Val, str, (IGuiCtrlBase*)Ctl());
		}
		else
		if (nMessage == WM_PAINT)
		{
			if (!m_Val) return;
			CString* str = (CString*)Ctl()->GetState(_T("text"));
			if (!str) return;

			CRect rect;
			Ctl()->GetClientRect(rect);

			if (!FmtTxtRect(rect, m_Val, str, (IGuiCtrlBase*)Ctl())) return;

			CGraph* mem_img = (CGraph*)lParam;
			if (!mem_img || mem_img->IsNull()) return;

			CSize scr_sz;
			Ctl()->GetScrollSize(scr_sz);
			rect.Left(-scr_sz.cx);
			rect.Top(-scr_sz.cy);
			CImgDrawer::Draw(*mem_img, m_imgClp, rect);
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_txt_font, _pic_font)

//////////////////////////////////////////////////////////////////

class CGuiText : public IGuiCtrlBase
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiText, IGuiCtrlBase)
		
public:
	CGuiText()
	{
		// 添加逻辑对象
		AddSet(_T("_pic_style"));
		AddSet(_T("_pic_image"));
		AddSet(_T("_pic_color"));
		AddSet(_T("_pic_text"));
		AddSet(_T("_txt_font"));
		// 设置默认属性
		SetState(_T("color"), (void*)ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM));
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiText, IGuiCtrlBase)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiText_hpp__*/