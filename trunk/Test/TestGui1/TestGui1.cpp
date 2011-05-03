// TestGui1.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestGui1.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int i = ExGui();
	IGuiObject a;

	return 0;
}
