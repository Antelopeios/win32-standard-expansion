// Copyright 2011, ľͷ��
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
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2010-05-11
// Version:	1.0.0000.1100
//////////////////////////////////////////////////////////////////

#ifndef __GuiThunk_hpp__
#define __GuiThunk_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

interface IGuiThunk : public IGuiBoardBase
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiThunk, IGuiBoardBase)

protected:
	WNDPROC m_WndProc;	// ԭʼ�Ĵ��ڹ���
	bool	m_bHook;	// Hook ���

public:
	IGuiThunk(void)
		: m_WndProc(NULL)
		, m_bHook(false)
	{}
	virtual ~IGuiThunk(void)
	{}

protected:
	// ��̬�ص�����,������ô��ڹ��̺���
	static LRESULT CALLBACK ThunkWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		ExAssert(hWnd != NULL);
		// ת��������Ϣ
		IGuiThunk* wnd_thk = (IGuiThunk*)::GetWindowLong(hWnd, GWL_USERDATA);
		ExAssert(wnd_thk != NULL);
		return wnd_thk->WndProc(message, wParam, lParam);
	}
	// WndProc ���ڹ��̺���
	virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch( message )
		{
		case WM_DESTROY:
		//	::PostQuitMessage(0);
			break;
		}
		return m_WndProc ? (*m_WndProc)(Get(), message, wParam, lParam) : NULL;
	}

public:
	// �������ھ��
	bool Attach(wnd_t hWnd)
	{
		ExAssert(hWnd != NULL);

		DWORD pid_cur = ::GetCurrentProcessId(), pid_wnd = NULL;
		::GetWindowThreadProcessId(hWnd, &pid_wnd);
		bool bHook = ( pid_cur != pid_wnd );
		if( bHook )
		{
			IGuiBoardBase::Attach(hWnd);
			m_bHook = bHook;
		}
		else
		{
			if (Get() || m_WndProc) Detach();
			IGuiBoardBase::Attach(hWnd);
			m_bHook = bHook;
			m_WndProc = (WNDPROC)SetWindowLong(GWL_WNDPROC, (LONG)ThunkWndProc);
			if (!m_WndProc) return false;
			SetWindowLong(GWL_USERDATA, (LONG)this);
		}
		return true;
	}
	// �ͷŴ��ھ��
	wnd_t Detach()
	{
		wnd_t ret = NULL;
		if (m_bHook)
			ret = IGuiBoardBase::Detach();
		else
		if (SetWindowLong(GWL_WNDPROC, (LONG)m_WndProc))
		{
			SetWindowLong(GWL_USERDATA, 0);
			ret = IGuiBoardBase::Detach();
		}
		m_bHook = false;
		return ret;
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNAMIC_CLS(IGuiThunk, IGuiBoardBase)

//////////////////////////////////////////////////////////////////

#endif/*__GuiThunk_hpp__*/