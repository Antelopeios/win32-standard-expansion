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
// GuiEffectBase - 效果基础实现
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-25
// Version:	1.0.0002.1600
//
// History:
//	- 1.0.0001.1515(2011-05-24)	# 修正IGuiEffectBase::SetTimer里的id赋值错误
//	- 1.0.0002.1600(2011-05-25)	+ 添加IGuiEffectBase::IsFinished()接口实现
//								= 调整IGuiEffectBase::SetTimer();KillTimer()接口
//////////////////////////////////////////////////////////////////

#ifndef __GuiEffectBase_hpp__
#define __GuiEffectBase_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCtrl/GuiCtrl.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI 效果对象接口
interface IGuiEffectBase : public IGuiEffect
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiEffectBase, IGuiEffect)

protected:
	UINT_PTR m_idEvent;
	CImage m_imgCac;

protected:
	static void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
	{
		RECT rect = {0, 0, 0, 0};
		::GetClientRect(hWnd, &rect);
		::InvalidateRect(hWnd, &rect, TRUE);
	}

public:
	IGuiEffectBase()
		: m_idEvent(0)
	{}

protected:
	virtual bool Overlap(IGuiCtrl* pCtrl, CImage& tNew, CImage& tOld) = 0;

public:
	void Init(CImage& tImg)
	{
		m_imgCac.Set(tImg.Clone());
	}
	bool IsInit()
	{
		return !(m_imgCac.IsNull());
	}

	bool IsFinished()
	{
		return (m_idEvent == 0);
	}

	void Show(IGuiObject* pGui, CImage& tImg)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;
		IGuiBoard* board = ctrl->GetBoard();
		if (!board) return;
		if (ctrl->IsUpdated())
		{
			CImgRenderer::Render(tImg, m_imgCac, CRect(), CPoint(), &CFilterCopy());
			SetTimer(board->GethWnd());
		}
		else
		if (m_idEvent && !Overlap(ctrl, tImg, m_imgCac))
		{
			KillTimer(board->GethWnd());
			m_imgCac.Set(tImg.Clone());
			board->Invalidate();
		}
	}

	void SetTimer(wnd_t hWnd)
	{
		static UINT_PTR id = 1;
		::SetTimer(hWnd, id, 40, TimerProc);
		m_idEvent = id++;
	}

	void KillTimer(wnd_t hWnd)
	{
		::KillTimer(hWnd, m_idEvent);
		m_idEvent = 0;
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNAMIC_CLS(IGuiEffectBase, IGuiEffect)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiEffectBase_hpp__*/