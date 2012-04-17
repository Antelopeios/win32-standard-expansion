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
// Date:	2012-03-27
// Version:	1.0.0002.1227
//
// History:
//	- 1.0.0000.0918(2012-03-13)	@ 开始构建GuiText
//	- 1.0.0001.1207(2012-03-26)	# 修正当CGuiText在控件显示的时候没有刷新控件内部文字区域,导致滚动条无法自动加载的问题
//	- 1.0.0002.1227(2012-03-27)	# 修正当CGuiText改变大小时可能导致滚动条刷新不正常的问题
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
	BOOL FmtTxtRect(CRect rect, CText* text, CString* str, IGuiCtl* ctl)
	{
		if (!text || !str)
		{
			ctl->SetFraRect(CSize());
			ctl->SetAllRect(CSize());
			return FALSE;
		}

		if (m_rcImg != rect)
		{
			m_rcImg = rect;
			ctl->SetFraRect(CSize(rect.Width(), rect.Height()));
		}

		if (m_txtTmp == (*text) && 
			m_rcImg == rect && 
			m_StrTmp == *str) return TRUE;

		m_txtTmp = (*text);
		m_StrTmp = (*str);

		// 计算缓存图大小并预存分段位图
		CArrayT<CString> sa;
		ExStringToArray(*str, sa, _T('\n'), FALSE);
		CArrayT<CImage> ia;
		CSize sz_all;
		for(DWORD i = 0; i < sa.GetCount(); ++i)
		{
			ia.PushLast(text->GetImage(sa[i]));
			if(!ia[i].IsNull())
			{
				sz_all.cx = max(sz_all.cx, ia[i].GetWidth());
				sz_all.cy += (ia[i].GetHeight() + 2);
			}
		}

		// 绘制缓存
		CRect rc(0, 0, sz_all.cx, sz_all.cy);
		m_imgClp.Create(rc.Width(), rc.Height());
		for(DWORD i = 0; i < ia.GetCount(); ++i)
		{
			if (ia[i].IsNull()) continue;
			CImgDrawer::Draw(m_imgClp, ia[i], rc);
			rc.Top(rc.Top() + ia[i].GetHeight() + 2);
		}

		ctl->SetAllRect(sz_all);

		return TRUE;
	}
	void Msg(UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		if (nMessage == WM_SIZE)
		{
			if (!m_Val) return;
			CString* str = (CString*)Ctl()->GetState(_T("text"));
			if (!str) return;

			CRect rect;
			Ctl()->GetClientRect(rect);

			FmtTxtRect(rect, m_Val, str, (IGuiCtl*)Ctl());
		}
		else
		if (nMessage == WM_PAINT)
		{
			if (!m_Val) return;
			CString* str = (CString*)Ctl()->GetState(_T("text"));
			if (!str) return;

			CRect rect;
			Ctl()->GetClientRect(rect);

			if (!FmtTxtRect(rect, m_Val, str, (IGuiCtl*)Ctl())) return;

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

class CGuiText : public CGuiPicture
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiText, CGuiPicture)
		
public:
	CGuiText()
	{
		// 添加逻辑对象
		AddSet(_T("_txt_font"));
		// 设置默认属性
		SetState(_T("color"), (void*)ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM));
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiText, CGuiPicture)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiText_hpp__*/