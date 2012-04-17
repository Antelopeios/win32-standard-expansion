#pragma once

//////////////////////////////////////////////////////////////////

class CCusButton : public IGuiCtl
{
	EXP_DECLARE_DYNCREATE_MULT(CCusButton, IGuiCtl)
		
protected:
	IGuiCtl* m_Grd;
	IGuiCtl* m_Txt1;
	IGuiCtl* m_Txt2;
	IGuiCtl* m_Txt3;

public:
	CCusButton()
	{
		// 创建用于组合的基本控件
		m_Grd = ExGui<IGuiCtl>(_T("CGuiButton"));
		m_Txt1 = ExGui<IGuiCtl>(_T("CGuiStatic"));
		m_Txt2 = ExGui<IGuiCtl>(_T("CGuiStatic"));
		m_Txt3 = ExGui<IGuiCtl>(_T("CGuiStatic"));
		// 设置基本控件的默认属性
		m_Txt1->SetState(_T("comf"), (void*)TRUE);	// 设置控件在调整文字时自适应文字大小
		m_Txt2->SetState(_T("comf"), (void*)TRUE);
		m_Txt3->SetState(_T("comf"), (void*)TRUE);
		m_Txt1->SetThrough(TRUE);					// 设置内部显示用控件透过
		m_Txt2->SetThrough(TRUE);
		m_Txt3->SetThrough(TRUE);
		// 关联基本控件
		m_Grd->AddComp(m_Txt1);
		m_Grd->AddComp(m_Txt2);
		m_Grd->AddComp(m_Txt3);
		AddComp(m_Grd);
	}
	~CCusButton()
	{
		// 删除基本控件
		m_Grd->DelComp(m_Txt1);
		m_Grd->DelComp(m_Txt2);
		m_Grd->DelComp(m_Txt3);
		DelComp(m_Grd);
	}

public:
	void SendMsg(void* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		switch(nMessage)
		{
		case WM_SIZE:
			{
				CRect rect;
				GetClientRect(rect);
				m_Grd->SetWindowRect(rect);
			}
			break;
		default:
			IGuiCtl::SendMsg(pGui, nMessage, wParam, lParam);
		}
	}
	BOOL Execute(const CString& key, const CString& val)
	{
		return m_Grd->Execute(key, val);
	}
	void* GetState(const CString& sType, void* pParam = NULL)
	{
		if (sType == _T("text"))
			return m_Txt1->GetState(_T("text"), pParam);
		else
		if (sType == _T("text2"))
			return m_Txt2->GetState(_T("text"), pParam);
		else
		if (sType == _T("text3"))
			return m_Txt3->GetState(_T("text"), pParam);
		else
			return m_Grd->GetState(sType, pParam);
	}
	BOOL SetState(const CString& sType, void* pState, void* pParam = NULL)
	{
		if (sType == _T("text"))
			return m_Txt1->SetState(_T("text"), pState, pParam);
		else
		if (sType == _T("text2"))
			return m_Txt2->SetState(_T("text"), pState, pParam);
		else
		if (sType == _T("text3"))
			return m_Txt3->SetState(_T("text"), pState, pParam);
		else
			return m_Grd->SetState(sType, pState, pParam);
	}
};

EXP_IMPLEMENT_DYNCREATE_MULT(CCusButton, IGuiCtl)

//////////////////////////////////////////////////////////////////

class CBtnEvent : public IGuiEvent
{
public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtl* ctl = ExDynCast<IGuiCtl>(pGui);
		if (!ctl) return;

		switch( nMessage )
		{
		case WM_SHOWWINDOW:
			if (wParam)
			{
				IGuiBase* wnd = ctl->GetParent();
				if (!wnd) break;
				CRect rc_wnd;
				wnd->GetClientRect(rc_wnd);
				rc_wnd.Deflate(CPoint(10, 10));
				ctl->SetWindowRect(rc_wnd);
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CWndEvent : public IGuiEvent
{
public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiWnd* wnd = ExDynCast<IGuiWnd>(pGui);
		if (!wnd) return;

		switch( nMessage )
		{
		case WM_SIZE:
			for(IGuiBase::list_t::iterator_t ite = wnd->GetComp().Head(); ite != wnd->GetComp().Tail(); ++ite)
			{
				IGuiCtl* ctl = ExDynCast<IGuiCtl>(*ite);
				if (!ctl) continue;
				ctl->SendMessage(WM_SHOWWINDOW, TRUE);
			}
			break;
		}
	}
};