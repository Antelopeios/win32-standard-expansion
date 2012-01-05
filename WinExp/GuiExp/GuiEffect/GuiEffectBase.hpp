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
// Date:	2011-06-29
// Version:	1.0.0004.1518
//
// History:
//	- 1.0.0001.1515(2011-05-24)	# 修正IGuiEffectBase::SetTimer里的id赋值错误
//	- 1.0.0002.1600(2011-05-25)	+ 添加IGuiEffectBase::IsFinished()接口实现
//								= 调整IGuiEffectBase::SetTimer();KillTimer()接口
//	- 1.0.0003.1131(2011-06-09)	^ 采用引用计数改进IGuiEffectBase::SetTimer();KillTimer()接口,针对一个hWnd只会打开一个定时器
//	- 1.0.0004.1518(2011-06-29)	^ 优化定时器控制,自动忽略重复的定时器打开操作
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
	CImage m_imgCac;
	BOOL m_IsRunning;
	typedef CMapT<HWND, UINT> map_t;
	static map_t s_TimerMap;

protected:
	static void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
	{
		RECT rect = {0, 0, 0, 0};
		::GetClientRect(hWnd, &rect);
		::InvalidateRect(hWnd, &rect, TRUE);
	}

public:
	IGuiEffectBase()
		: m_IsRunning(FALSE)
	{}

protected:
	virtual BOOL Overlap(IGuiCtrl* pCtrl, CImage& tNew, CImage& tOld) = 0;

public:
	void Init(CImage& tImg)
	{
		m_imgCac.Set(tImg.Clone());
	}
	BOOL IsInit()
	{
		return !(m_imgCac.IsNull());
	}

	void Show(IGuiObject* pGui, CImage& tImg)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;
		IGuiBoard* board = ctrl->GetBoard();
		if (!board) return;
		if (ctrl->IsUpdated())
		{
			CImgDrawer::Cover(tImg, m_imgCac);
			SetTimer(board->GethWnd());
		}
		else
		if (m_IsRunning && !Overlap(ctrl, tImg, m_imgCac))
		{
			KillTimer(board->GethWnd());
			m_imgCac.Set(tImg.Clone());
			board->Invalidate();
		}
	}

	void SetTimer(wnd_t hWnd)
	{
		if (m_IsRunning) return;
		map_t::iterator_t ite = s_TimerMap.Locate(hWnd);
		if (ite == s_TimerMap.Tail())
		{
			s_TimerMap.Add(hWnd, 0);
			ite = s_TimerMap.Last();
		}
		if (ite->Val() == 0)
			::SetTimer(hWnd, 1, 40, TimerProc);
		++(ite->Val());
		m_IsRunning = TRUE;
	}

	void KillTimer(wnd_t hWnd)
	{
		map_t::iterator_t ite = s_TimerMap.Locate(hWnd);
		if (ite == s_TimerMap.Tail()) return;
		m_IsRunning = FALSE;
		if ((--(ite->Val())) != 0) return;
		::KillTimer(hWnd, 1);
		s_TimerMap.Del(hWnd);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNAMIC_CLS(IGuiEffectBase, IGuiEffect)
IGuiEffectBase::map_t IGuiEffectBase::s_TimerMap;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiEffectBase_hpp__*/