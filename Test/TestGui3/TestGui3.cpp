// TestGui3.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestGui3.h"

//////////////////////////////////////////////////////////////////

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 垃圾回收器
	CGC& gc = ExGC();

	// 相关资源定义
	RECT rc_dsk = {0};
	::GetClientRect(::GetDesktopWindow(), &rc_dsk);
	CRect rc_wnd(0, 0, 500, 300);
	HICON ic_wnd = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTGUI3));
	CText txt_lst;
	txt_lst.SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
	txt_lst.SetColor(ExRGBA(0, 60, 116, 200));
	// 解码器
	CIOFile file(_T("../TestGui3/icon.png"));
	ICoderObject* coder = CImgAnalyzer::GetCoder(&file, &gc);
	if (!coder) return 0;
	// 图片资源
	CImage img_pic(coder->Decode());
	CImage img_btn[9];
	file.Open(_T("../TestGui1/btn_bg_cap_left.png"));
	img_btn[0].Set(coder->Decode());
	file.Open(_T("../TestGui1/btn_bg_cap.png"));
	img_btn[1].Set(coder->Decode());
	file.Open(_T("../TestGui1/btn_bg_cap_right.png"));
	img_btn[2].Set(coder->Decode());
	file.Open(_T("../TestGui1/btn_bg_left.png"));
	img_btn[3].Set(coder->Decode());
	file.Open(_T("../TestGui1/btn_bg.png"));
	img_btn[4].Set(coder->Decode());
	file.Open(_T("../TestGui1/btn_bg_right.png"));
	img_btn[5].Set(coder->Decode());
	file.Open(_T("../TestGui1/btn_bg_bottom_left.png"));
	img_btn[6].Set(coder->Decode());
	file.Open(_T("../TestGui1/btn_bg_bottom.png"));
	img_btn[7].Set(coder->Decode());
	file.Open(_T("../TestGui1/btn_bg_bottom_right.png"));
	img_btn[8].Set(coder->Decode());
	// 文字资源
	CText txt_btn[8];
	txt_btn[0].SetString(_T("Dark C.at"));
	txt_btn[0].SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
	txt_btn[0].SetColor(ExRGBA(0, 60, 116, 200));
	for(int i = 1; i < _countof(txt_btn); ++i) txt_btn[i] = txt_btn[0];

	// 创建窗口对象并设置
	IGuiBoard* wnd = ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc));
	wnd->Create(_T("TestGui2"), rc_wnd, SW_HIDE, WS_OVERLAPPEDWINDOW);
	wnd->SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)ic_wnd);
	wnd->SendMessage(WM_SETICON, (WPARAM)FALSE, (LPARAM)ic_wnd);
	wnd->CenterWindow();
	//wnd->SetLayered(TRUE);
	wnd->ShowWindow(SW_SHOW);
	wnd->GetClientRect(rc_wnd);

	// 创建列表控件
	IGuiCtrl* list = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiListView"), &gc));
	CListT<IGuiCtrl*> items;
	for(int i = 0; i < 100; ++i)
	{
		IGuiCtrl* btn = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiLVItem"), &gc));
		btn->SetState(_T("icon"), &img_pic);
		//btn->SetState(_T("glow"), (void*)1);
		btn->SetState(_T("image"), img_btn);
		btn->SetState(_T("text"), txt_btn);
		CSize sz;
		//txt_btn->GetSize(sz);
		btn->SetState(_T("locate"), (void*)2);
		btn->SetState(_T("loc_off"), (void*)5);
		btn->SetWindowRect(CRect(0, 0, 70, 70/*sz.cx + 8, sz.cy*/));
		items.Add(btn);
	}
	list->SetState(_T("items"), &items);
	list->SetState(_T("space"), (void*)5);
	list->SetWindowRect(rc_wnd);

	// 创建事件对象并设置
	CCustomEvent cus_evt;
	CFillEvent	 fil_evt;

	// 关联对象
	wnd->AddComp(list);
	wnd->AddEvent(&cus_evt);
	wnd->AddEvent(&fil_evt);

	// 主消息循环:
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}