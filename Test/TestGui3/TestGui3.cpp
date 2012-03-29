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
	CGC gc;

	// 相关资源定义
	RECT rc_dsk = {0};
	::GetClientRect(::GetDesktopWindow(), &rc_dsk);
	CRect rc_wnd(0, 0, 500, 300);
	CText txt_lst;
	txt_lst.SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
	txt_lst.SetColor(ExRGBA(0, 60, 116, 200));
	// 解码器
	CIOFile file(_T("TestGui3/icon.png"));
	ICoderObject* coder = CImgAnalyzer::GetCoder(&file, &gc);
	if (!coder) return 0;
	// 图片资源
	CImage img_pic(coder->Decode());
	CImage img_btn[9];
	file.Open(_T("TestGui1/btn_bg_cap_left.png"));
	img_btn[0].Set(coder->Decode());
	file.Open(_T("TestGui1/btn_bg_cap.png"));
	img_btn[1].Set(coder->Decode());
	file.Open(_T("TestGui1/btn_bg_cap_right.png"));
	img_btn[2].Set(coder->Decode());
	file.Open(_T("TestGui1/btn_bg_left.png"));
	img_btn[3].Set(coder->Decode());
	file.Open(_T("TestGui1/btn_bg.png"));
	img_btn[4].Set(coder->Decode());
	file.Open(_T("TestGui1/btn_bg_right.png"));
	img_btn[5].Set(coder->Decode());
	file.Open(_T("TestGui1/btn_bg_bottom_left.png"));
	img_btn[6].Set(coder->Decode());
	file.Open(_T("TestGui1/btn_bg_bottom.png"));
	img_btn[7].Set(coder->Decode());
	file.Open(_T("TestGui1/btn_bg_bottom_right.png"));
	img_btn[8].Set(coder->Decode());
	// 文字资源
	CText txt_btn[8];
	txt_btn[0].SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
	txt_btn[0].SetColor(ExRGBA(0, 60, 116, 200));
	for(int i = 1; i < _countof(txt_btn); ++i) txt_btn[i] = txt_btn[0];

	// 创建窗口对象并设置
	IGuiWnd* wnd = ExGui<IGuiWnd>(_T("CGuiWnd"), &gc);
	wnd->Create(_T("TestGui2"), rc_wnd, SW_HIDE, WS_OVERLAPPEDWINDOW);
	wnd->CenterWindow();
	//wnd->SetLayered(TRUE);
	wnd->ShowWindow(SW_SHOW);
	wnd->GetClientRect(rc_wnd);

	// 创建滚动条
	IGuiCtl* scrl_v = ExGui<IGuiCtl>(_T("CGuiScroll"));
	scrl_v->SetVisible(FALSE);
	ExDynCast<IGuiCtl>(scrl_v->GetState(_T("up")))->SetWindowRect(CRect());
	ExDynCast<IGuiCtl>(scrl_v->GetState(_T("down")))->SetWindowRect(CRect());
	scrl_v->SetState(_T("sli_blk_thr_sta"), (void*)TRUE);
	scrl_v->SetState(_T("sli_ori"), (void*)TRUE);
	scrl_v->AddEvent(dbnew(CScrEventV));

	// 创建列表控件
	IGuiCtl* list = ExGui<IGuiCtl>(_T("CGuiListView"));
	IGuiCtl::items_t items;
	for(int i = 0; i < 100; ++i)
	{
		IGuiCtl* btn = ExGui<IGuiCtl>(_T("CGuiLVItem"));
		btn->SetState(_T("icon"), &img_pic);
		//btn->SetState(_T("glow"), (void*)1);
		btn->SetState(_T("image"), img_btn);
		btn->SetState(_T("font"), txt_btn);
		btn->SetState(_T("text"), &CString(_T("Dark C.at")));
		CSize sz;
		btn->SetState(_T("loc_off"), (void*)2);
		btn->SetState(_T("ico_off"), (void*)2);
		btn->SetWindowRect(CRect(0, 0, 70, 70));
		items.Add(btn);
	}
	list->SetState(_T("items"), &items);
	list->SetState(_T("space"), (void*)5);
	list->SetScroll(scrl_v, TRUE);
	list->AddEvent(ExGui(_T("CScrollEvent")));

	// 关联对象
	wnd->AddComp(scrl_v);
	wnd->AddComp(list);
	wnd->AddEvent(ExMem::Alloc<CCustomEvent>());
	wnd->AddEvent(ExGui(_T("CSizeEvent")));

	// 主消息循环:
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}