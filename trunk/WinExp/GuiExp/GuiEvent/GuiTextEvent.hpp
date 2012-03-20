// Copyright 2012, ľͷ��
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
// GuiTextEvent - ���ֿؼ��¼�
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-13
// Version:	1.0.0000.0918
//
// History:
//	- 1.0.0000.0918(2012-03-13)	@ ��ʼ����GuiTextEvent
//////////////////////////////////////////////////////////////////

#ifndef __GuiTextEvent_hpp__
#define __GuiTextEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiTextEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiTextEvent, IGuiEvent)

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

		// ���㻺��ͼ��С��Ԥ��ֶ�λͼ
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

		// ���ƻ���
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

	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrlBase* ctl = ExDynCast<IGuiCtrlBase>(pGui);
		if (!ctl) return;

		// ������Ϣ
		switch( nMessage )
		{
		case WM_PAINT:
			if (lParam)
			{
				CGraph* mem_img = (CGraph*)lParam;
				if (!mem_img || mem_img->IsNull()) break;

				CRect rect;
				ctl->GetClientRect(rect);

				// �������
				CImage* image = (CImage*)ctl->GetState(_T("image"));
				pixel_t pixel = (pixel_t)ctl->GetState(_T("color"));
				CText* text = (CText*)ctl->GetState(_T("font"));
				CString* str = (CString*)ctl->GetState(_T("text"));

				// ��ͼ
				CImgDrawer::Fill(*mem_img, rect, pixel);
				if (image && !image->IsNull())
					CImgDrawer::Draw(*mem_img, image->Get(), rect, 
						CPoint(), CSize(rect.Width(), rect.Height()));

				// ������
				if (FmtTxtRect(rect, text, str, ctl))
				{
					CSize scr_sz;
					ctl->GetScrollSize(scr_sz);
					rect.Left(-scr_sz.cx);
					rect.Top(-scr_sz.cy);
					CImgDrawer::Draw(*mem_img, m_imgClp, rect);
				}
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiTextEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiTextEvent_hpp__*/
