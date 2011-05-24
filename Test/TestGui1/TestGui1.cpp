// TestGui1.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestGui1.h"

//////////////////////////////////////////////////////////////////

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR	 lpCmdLine,
					   int		 nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 加载资源
	CGC gc;
	CIOFile file(_T("../TestImg1/ground.png"));
	ICoderObject* coder = CImgAnalyzer::GetCoder(&file, &gc);
	if (!coder) return 0;
	CImage img_pic(coder->Decode());

	// 创建效果对象并设置
	IGuiEffect* eff = ExDynCast<IGuiEffect>(ExGui(_T("CGuiFade"), &gc));

	// 创建控件对象并设置
	IGuiCtrl* pic = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc));
	pic->SetState(_T("image"), &img_pic);
	CRect rect(0, 0, img_pic.GetWidth(), img_pic.GetHeight());
	pic->SetRect(rect);
	pic->SetEffect(eff);

	// 创建窗口对象并设置
	IGuiBoard* wnd = ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc));
	wnd->Create(_T("Test"), /*CRect(0, 0, 500, 300)*/rect, SW_HIDE);
	wnd->CenterWindow();
	wnd->SetLayered();
	wnd->ShowWindow(SW_SHOW);

	// 创建事件对象并设置
	CCustomEvent cus_evt;
	CCusPicEvent pic_evt;

	// 将窗口与控件及事件对象关联
	pic->AddEvent(&pic_evt);
	wnd->AddComp(pic);
	wnd->AddEvent(&cus_evt);

	// 主消息循环:
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
