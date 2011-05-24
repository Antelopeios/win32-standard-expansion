#pragma once

#include "resource.h"

//////////////////////////////////////////////////////////////////

class CCustomEvent : public IGuiEvent
{
public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiBoard* board = ExDynCast<IGuiBoard>(pGui);
		if (!board) return;

		ExTrace(_T("0x%04X\n"), nMessage);

		switch( nMessage )
		{
		case WM_NCHITTEST:
			{
				LRESULT hit_test = GetResult();
				SetResult((hit_test == HTCLIENT) ? HTCAPTION : hit_test);
			}
			break;
		case WM_KEYDOWN:
			{
				int nKey = wParam;
				if (nKey == 27) // ESC
					board->PostMessage(WM_CLOSE);
			}
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CCusPicEvent : public IGuiEvent
{
public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiCtrl* ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!ctrl) return;

		switch( nMessage )
		{
		case WM_NCLBUTTONDOWN:
			{
				CGC gc;
				IGuiCtrl::state_t* s = ctrl->GetState(_T("color"), &gc);
				if (!s) break;
				pixel_t* pix = (pixel_t*)(((void**)s->sta_arr)[0]);
				if (!pix) break;
				if (*pix)
					*pix = 0;
				else
					*pix = ExRGBA(128, 128, 128, 128);
				ctrl->SetState(_T("color"), pix);
			}
			break;
		}
	}
};