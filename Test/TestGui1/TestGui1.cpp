// TestGui1.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestGui1.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR	 lpCmdLine,
					   int		 nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	IGuiBoard* wnd = (IGuiBoard*)ExGui(_T("CGuiWnd"));
	wnd->Create(_T("Test"), CRect(0, 0, 100, 100));

	// 主消息循环:
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
