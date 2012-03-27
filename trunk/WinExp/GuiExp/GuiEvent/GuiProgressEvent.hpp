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
// GuiProgressEvent - 进度条控件事件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-20
// Version:	1.0.0000.1308
//
// History:
//	- 1.0.0000.1308(2012-03-20)	@ 开始构建GuiProgressEvent
//////////////////////////////////////////////////////////////////

#ifndef __GuiProgressEvent_hpp__
#define __GuiProgressEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiProgressEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiProgressEvent, IGuiEvent)

protected:
	IGuiCtl* m_Ctrl;

public:
	// 获得属性
	IGuiCtl* GetProg()
	{
		return (IGuiCtl*)m_Ctrl->GetState(_T("pg"));
	}
	uint32_t GetVal()
	{
		return (uint32_t)m_Ctrl->GetState(_T("val"));
	}
	uint32_t GetMax()
	{
		return (uint32_t)m_Ctrl->GetState(_T("max"));
	}

	// 格式化位置
	void Format()
	{
		ExAssert(m_Ctrl);
		IGuiCtl* prog = GetProg();
		if (!prog) return;
		uint32_t val = GetVal(), max = GetMax();
		if (val > max) val = max;
		CRect rect, rc_pg;
		m_Ctrl->GetClientRect(rect);
		rc_pg = rect;
		rc_pg.Width((rect.Width() * val) / max);
		prog->SetWindowRect(rc_pg);
	}

	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		m_Ctrl = ExDynCast<IGuiCtl>(pGui);
		if (!m_Ctrl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (!wParam) break;
		case WM_SIZE:
			Format();
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiProgressEvent, IGuiEvent)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiProgressEvent_hpp__*/
