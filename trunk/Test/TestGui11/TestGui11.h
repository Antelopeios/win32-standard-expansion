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
				// 调整背景位置
				CRect rect;
				GetClientRect(rect);
				m_Grd->SetRect(rect);
				// 设置文字内容区域自适应文字大小
				m_Txt1->SetState(_T("comf"), (void*)TRUE);
				m_Txt2->SetState(_T("comf"), (void*)TRUE);
				m_Txt3->SetState(_T("comf"), (void*)TRUE);
				// 获得文字内容区域
				CRect rc1, rc2, rc3;
				m_Txt1->GetRect(rc1);
				m_Txt2->GetRect(rc2);
				m_Txt3->GetRect(rc3);
				// 调整文字内容位置
				rc2.MoveTo((rect.Width() - rc1.Width()) >> 1, (rect.Height() - rc1.Height()) >> 1);
				rc1.MoveTo(0, rc2.Top() - rc1.Height() - 2);
				rc3.MoveTo(rect.Width() - rc3.Width(), rc2.Bottom() + 2);
				if (rc2.Left() < 0) rc2.Left(0);
				if (rc3.Left() < 0) rc3.Left(0);
				if (rc1.Width() > rect.Width()) rc1.Width(rect.Width());
				if (rc2.Width() > rect.Width()) rc2.Width(rect.Width());
				if (rc3.Width() > rect.Width()) rc3.Width(rect.Width());
				m_Txt1->SetRect(rc1);
				m_Txt2->SetRect(rc2);
				m_Txt3->SetRect(rc3);
			}
			break;
		default:
			IGuiCtl::SendMsg(pGui, nMessage, wParam, lParam);
		}
	}
	BOOL Execute(const CString& key, const CString& val)
	{
		if (key == _T("font"))
		{
			m_Txt1->Execute(key, val);
			m_Txt2->Execute(key, val);
			m_Txt3->Execute(key, val);
			return TRUE;
		}
		else
		if (key == _T("text"))
			return m_Txt1->Execute(key, val);
		else
		if (key == _T("text2"))
			return m_Txt2->Execute(key, val);
		else
		if (key == _T("text3"))
			return m_Txt3->Execute(key, val);
		else
			return m_Grd->Execute(key, val);
	}
	void* GetState(const CString& sType, void* pParam = NULL)
	{
		if (sType == _T("font"))
			return m_Txt1->GetState(_T("font"), pParam);
		else
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
		if (sType == _T("font"))
		{
			m_Txt1->SetState(_T("font"), pState, pParam);
			m_Txt2->SetState(_T("font"), pState, pParam);
			m_Txt3->SetState(_T("font"), pState, pParam);
			return TRUE;
		}
		else
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