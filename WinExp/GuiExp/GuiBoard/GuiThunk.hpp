// Copyright 2011-2012, ľͷ��
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
// GuiThunk - ���ڹ����ض���
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-02
// Version:	1.0.0001.1802
//
// History:
//	- 1.0.0001.1100(2011-05-19)	= ��CGuiWnd���¼��������,������̷���CGuiWndEvent
//	- 1.0.0001.1802(2012-02-02)	# ������IGuiThunk����֮�������Ϣ�����³������������
//////////////////////////////////////////////////////////////////

#ifndef __GuiThunk_hpp__
#define __GuiThunk_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

interface IGuiThunk : public IGuiWnd
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiThunk, IGuiWnd)

protected:
	WNDPROC m_WndProc;	// ԭʼ�Ĵ��ڹ���
	BOOL	m_bHook;	// Hook ���

public:
	IGuiThunk(void)
		: m_WndProc(NULL)
		, m_bHook(FALSE)
	{}
	virtual ~IGuiThunk(void)
	{ Detach(); }

protected:
	// ��̬�ص�����,������ô��ڹ��̺���
	static LRESULT CALLBACK ThunkWndProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		ExAssert(hWnd != NULL);
		// ת��������Ϣ
		IGuiThunk* wnd_thk = (IGuiThunk*)::GetWindowLong(hWnd, GWL_USERDATA);
		ExAssert(wnd_thk != NULL);
		return wnd_thk->WndProc(nMessage, wParam, lParam);
	}
	// WndProc ���ڹ��̺���
	virtual LRESULT WndProc(UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		Send(ExDynCast<IGuiObject>(this), nMessage, wParam, lParam);
		return GetResult();
	}

public:
	LRESULT DefProc(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{ return m_WndProc ? (*m_WndProc)(Get(), nMessage, wParam, lParam) : 0; }

	// �������ھ��
	BOOL Attach(wnd_t hWnd)
	{
		ExAssert(hWnd != NULL);

		DWORD pid_cur = ::GetCurrentProcessId(), pid_wnd = NULL;
		::GetWindowThreadProcessId(hWnd, &pid_wnd);
		BOOL bHook = ( pid_cur != pid_wnd );
		if( bHook )
		{
			IGuiWnd::Attach(hWnd);
			m_bHook = bHook;
		}
		else
		{
			if (Get() || m_WndProc) Detach();
			IGuiWnd::Attach(hWnd);
			m_bHook = bHook;
			m_WndProc = (WNDPROC)SetWindowLong(GWL_WNDPROC, (LONG)ThunkWndProc);
			if (!m_WndProc) return FALSE;
			SetWindowLong(GWL_USERDATA, (LONG)this);
		}
		return TRUE;
	}
	// �ͷŴ��ھ��
	wnd_t Detach()
	{
		wnd_t ret = NULL;
		if (m_bHook)
			ret = IGuiWnd::Detach();
		else
		if (SetWindowLong(GWL_WNDPROC, (LONG)m_WndProc))
		{
			SetWindowLong(GWL_USERDATA, 0);
			ret = IGuiWnd::Detach();
		}
		m_bHook = FALSE;
		return ret;
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNAMIC_MULT(IGuiThunk, IGuiWnd)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiThunk_hpp__*/