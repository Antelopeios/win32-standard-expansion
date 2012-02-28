#pragma once

#include "resource.h"

//////////////////////////////////////////////////////////////////

class CCustomEvent : public IGuiEvent
{
public:
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiWnd* wnd = ExDynCast<IGuiWnd>(pGui);
		if (!wnd) return;

		switch( nMessage )
		{
		case WM_KEYDOWN:
			{
				if (wParam == VK_ESCAPE) // ESC
					wnd->PostMessage(WM_CLOSE);
			}
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		}
	}
};
