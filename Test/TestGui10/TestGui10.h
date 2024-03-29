#pragma once

//////////////////////////////////////////////////////////////////

class CScrEventV : public IGuiEvent
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
				IGuiWnd* wnd = ctl->GetWnd();
				if (wnd)
				{
					CRect rc_wnd;
					wnd->GetClientRect(rc_wnd);
					rc_wnd.Left(rc_wnd.Right() - 10);
					ctl->SetWindowRect(rc_wnd);
				}
			}
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CScrEventH : public IGuiEvent
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
				IGuiWnd* wnd = ctl->GetWnd();
				if (wnd)
				{
					CRect rc_wnd;
					wnd->GetClientRect(rc_wnd);
					rc_wnd.Right(rc_wnd.Right() - 10);
					rc_wnd.Top(rc_wnd.Bottom() - 10);
					ctl->SetWindowRect(rc_wnd);
				}
			}
			break;
		}
	}
};

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
		case BM_CLICK:
			{
				IGuiCtl::itree_t::iterator_t* pite = 
					(IGuiCtl::itree_t::iterator_t*)ctl->GetState(_T("iter"));
				if (!pite) break;
				typedef IGuiCtl::itree_t::ite_list_t ite_list_t;
				ite_list_t list = (*pite)->Children();
				if (list.Empty()) break;
				for(ite_list_t::iterator_t ite = list.Head(); ite != list.Tail(); ++ite)
				{
					IGuiCtl* cld = (*(*ite));
					cld->SendMessage(BM_CLICK, 1);
					cld->SetVisible(!cld->IsVisible());
				}
				if (wParam == 0)
				{
					IGuiWnd* wnd = ctl->GetWnd();
					wnd->SendMessage(WM_SIZE, SIZE_RESTORED);
				}
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