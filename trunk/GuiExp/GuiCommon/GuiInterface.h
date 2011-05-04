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
// GuiInterface - 界面公用接口
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2010-05-03
// Version:	1.0.0000.1430
//////////////////////////////////////////////////////////////////

#ifndef __GuiInterface_h__
#define __GuiInterface_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI 对象接口
interface EXP_API IGuiObject : public IBaseObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiObject, IBaseObject)

public:
	IGuiObject() {}
	virtual ~IGuiObject() {}
};

//////////////////////////////////////////////////////////////////

// GUI 组合接口
interface EXP_API IGuiComp : public IGuiObject, public CListT<IGuiComp*>
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiComp, IGuiObject)

public:
	typedef CListT<IGuiComp*> base_list_t;

protected:
	bool		m_bTruCldr;	// 子容器链托管标记
	IGuiComp*	m_Pare;		// 父对象指针

public:
	IGuiComp(void)
		: m_bTruCldr(false)
		, m_Pare(NULL)
	{}
	virtual ~IGuiComp(void)
	{}

public:
	// 是否对子容器做托管
	void SetTrustChildren(bool bTruCldr = true) { m_bTruCldr = bTruCldr; }
	bool IsTrustChildren() { return m_bTruCldr; }
	// 获得父容器
	IGuiComp* GetParent() { return m_Pare; }

	// 组合接口
	virtual void Add(IGuiComp* pComp)
	{
		if (!pComp) return ;
		// 定位对象
		iterator_t ite = finder_t::Find(*this, pComp);
		if (ite != Tail()) return;
		// 添加新对象
		base_list_t::Add(pComp);
		if( pComp->m_Pare )
			pComp->m_Pare->Del(pComp);
		pComp->m_Pare = this;
	}
	virtual void Del(IGuiComp* pComp)
	{
		if (!pComp) return ;
		// 定位对象
		iterator_t ite = finder_t::Find(*this, pComp);
		if (ite == Tail()) return;
		// 删除对象
		base_list_t::Del(ite);
		pComp->m_Pare = NULL;
	}
};

// GUI 事件接口
interface EXP_API IGuiEvent : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiEvent, IGuiObject)

protected:
	IGuiComp* m_pComp;

public:
	IGuiEvent()
		: m_pComp(NULL)
	{}
	virtual ~IGuiEvent()
	{}

public:
	void SetComp(IGuiComp* pComp) { m_pComp = pComp; }
	IGuiComp* GetComp() { return m_pComp; }

	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;
};

//////////////////////////////////////////////////////////////////

// GUI 窗口对象接口
interface EXP_API IGuiWnd : public IGuiComp, public ITypeObjectT<wnd_t>
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiWnd, IGuiComp)

public:
	static const CString s_ClassName;
protected:
	HINSTANCE m_hIns;

public:
	IGuiWnd(void)
		: m_hIns(::GetModuleHandle(NULL))
	{}
	virtual ~IGuiWnd(void)
	{}

protected:
	ATOM RegisterWndClass(LPCTSTR sClassName)
	{
		WNDCLASSEX wcex		= {0};
		wcex.cbSize			= sizeof(WNDCLASSEX);
		wcex.style			= CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= DefWindowProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= m_hIns;
		wcex.hIcon			= NULL;
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= sClassName;
		return ::RegisterClassEx(&wcex);
	}

public:
	virtual bool Create(LPCTSTR sWndName, const CRect& rcWnd, 
						int nCmdShow = SW_SHOWNORMAL, DWORD dwStyle = WS_POPUP, DWORD dwExStyle = NULL, 
						wnd_t wndParent = NULL)
	{
		Destroy();

		RegisterWndClass(s_ClassName);
		Set(
			::CreateWindowEx
				(
				dwExStyle, sClassName, sWndName, dwStyle, 
				rcWnd.Left(), rcWnd.Top(), rcWnd.Width(), rcWnd.Height(), 
				wndParent, NULL, m_hIns, NULL
				)
			);
		if( IsNull() ) return false;

		::ShowWindow(nCmdShow);
		::UpdateWindow();

		return true;
	}
	virtual bool Destroy()
	{
		bool ret = true;
		if (!IsNull())
			ret = ::DestroyWindow(Get());
		Set(NULL);
		return ret;
	}

	// 窗口属性修改
	DWORD GetStyle() const
	{
		return (DWORD)GetWindowLong(GWL_STYLE);
	}
	DWORD GetExStyle() const
	{
		return (DWORD)GetWindowLong(GWL_EXSTYLE);
	}
	bool ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
	{
		return ModifyStyleEx(dwRemove, dwAdd, nFlags, GWL_STYLE);
	}
	bool ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0, int nStyleOffset = GWL_EXSTYLE)
	{
		ExAssert( !IsNull() );
		DWORD style = GetWindowLong(nStyleOffset);
		DWORD new_style = (style & ~dwRemove) | dwAdd;
		if (style == new_style)
			return false;
		SetWindowLong(nStyleOffset, new_style);
		if (nFlags != 0)
		{
			::SetWindowPos(Get(), NULL, 0, 0, 0, 0,
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}
		return true;
	}
	LONG SetWindowLong(int nIndex, LONG dwNewLong)
	{
		ExAssert( !IsNull() );
		return ::SetWindowLong(Get(), nIndex, dwNewLong);
	}
	LONG GetWindowLong(int nIndex) const
	{
		ExAssert( !IsNull() );
		return ::GetWindowLong(Get(), nIndex);
	}

	// 窗口刷新
	void Invalidate()
	{
		if (IsNull()) return ;
		RECT rect;
		::GetClientRect(Get(), &rect);
		::InvalidateRect(Get(), &rect, TRUE);
	}
	void InvalidateRect(const CRect& rcInv)
	{
		if (IsNull()) return ;
		RECT rect = 
		{
			rcInv.Left(), 
			rcInv.Top(), 
			rcInv.Right(), 
			rcInv.Bottom()
		};
		::InvalidateRect(Get(), &rect, TRUE);
	}
	void InvalidateRgn(HRGN hRgn)
	{
		if (IsNull()) return ;
		::InvalidateRgn(Get(), hRgn, TRUE);
	}
	bool ShowWindow(int nCmdShow)
	{
		if(IsNull()) return false;
		return (bool)::ShowWindow(Get(), nCmdShow);
	}
	bool UpdateWindow()
	{
		if (IsNull()) return false;
		return (bool)::UpdateWindow(Get());
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiInterface_h__*/