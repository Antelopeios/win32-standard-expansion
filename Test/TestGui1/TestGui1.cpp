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

	// 垃圾回收器
	CGC gc;

	// 加载资源
	CIOFile file(_T("../TestImg1/ground.png"));
	// 解码器
	ICoderObject* coder = CImgAnalyzer::GetCoder(&file, &gc);
	if (!coder) return 0;
	// 图片资源
	CImage img_pic(coder->Decode());
	CImage img_btn[9];
	file.Open(_T("btn_bg_cap_left.png"));
	img_btn[0].Set(coder->Decode());
	file.Open(_T("btn_bg_cap.png"));
	img_btn[1].Set(coder->Decode());
	file.Open(_T("btn_bg_cap_right.png"));
	img_btn[2].Set(coder->Decode());
	file.Open(_T("btn_bg_left.png"));
	img_btn[3].Set(coder->Decode());
	file.Open(_T("btn_bg.png"));
	img_btn[4].Set(coder->Decode());
	file.Open(_T("btn_bg_right.png"));
	img_btn[5].Set(coder->Decode());
	file.Open(_T("btn_bg_bottom_left.png"));
	img_btn[6].Set(coder->Decode());
	file.Open(_T("btn_bg_bottom.png"));
	img_btn[7].Set(coder->Decode());
	file.Open(_T("btn_bg_bottom_right.png"));
	img_btn[8].Set(coder->Decode());
	// 文字资源
	CText txt_btn[5];
	txt_btn[0].SetString(_T("Dark C.at"));
	txt_btn[0].SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
	txt_btn[0].SetColor(ExRGBA(0, 60, 116, 200));
	for(int i = 1; i < _countof(txt_btn); ++i)
		txt_btn[i] = txt_btn[0];

	// 创建效果对象并设置
	IGuiEffect* eff1 = ExDynCast<IGuiEffect>(ExGui(_T("CGuiFade"), &gc));
	IGuiEffect* eff2 = ExDynCast<IGuiEffect>(ExGui(_T("CGuiFade"), &gc));

	// 创建按钮控件对象并设置
	IGuiCtrl* btn = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiButton"), &gc));
	btn->SetState(_T("image"), img_btn);
	btn->SetState(_T("text"), txt_btn);
	btn->SetWindowRect(CRect(70, 100, 180, 140));
	//btn->SetEffect(eff1);

	// 创建图片控件对象并设置
	IGuiCtrl* pic = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc));
	pic->SetState(_T("image"), &img_pic);
	//CText text(_T("Dark C.at"), (font_t)::GetStockObject(DEFAULT_GUI_FONT), ExRGBA(255, 255, 0, 255));
	//pic->SetState(_T("text"), &text);
	CRect rect(0, 0, img_pic.GetWidth(), img_pic.GetHeight());
	pic->SetWindowRect(rect);
	//pic->SetEffect(eff2);

	// 创建窗口对象并设置
	IGuiBoard* wnd = ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc));
	wnd->Create(_T("Test"), /*CRect(0, 0, 500, 300)*/rect, SW_HIDE);
	wnd->CenterWindow();
	wnd->SetLayered();
	wnd->ShowWindow(SW_SHOW);

	// 创建事件对象并设置
	CCustomEvent cus_evt;
	CCusPicEvent pic_evt;
	CCusBtnEvent btn_evt;

	// 将窗口与控件及事件对象关联
	pic->AddEvent(&pic_evt);
	btn->AddEvent(&btn_evt);
	pic->AddComp(btn);
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
