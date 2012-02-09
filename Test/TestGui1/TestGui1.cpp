// TestGui1.cpp : ����Ӧ�ó������ڵ㡣
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

	// ����������
	CGC gc;

	// ������Դ
	CIOFile file(_T("TestImg1/ground.png"));
	// ������
	ICoderObject* coder = CImgAnalyzer::GetCoder(&file, &gc);
	if (!coder) return 0;
	// ͼƬ��Դ
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
	// ������Դ
	CText txt_btn[8];
	txt_btn[0].SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
	txt_btn[0].SetColor(ExRGBA(0, 60, 116, 200));
	for(int i = 1; i < _countof(txt_btn); ++i) txt_btn[i] = txt_btn[0];

	// ������ť�ؼ���������
	IGuiCtrl* btn = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiButton")));
	btn->SetState(_T("image"), img_btn);
	btn->SetState(_T("font"), txt_btn);
	btn->SetState(_T("text"), &CString(_T("Dark C.at")));
	btn->SetState(_T("locate"), (void*)4);
	btn->SetState(_T("loc_off"), (void*)10);
	btn->SetWindowRect(CRect(70, 100, 180, 140));
	//btn->SetEffect(ExDynCast<IGuiEffect>(ExGui(_T("CGuiFade"))));

	// ����ͼƬ�ؼ���������
	IGuiCtrl* pic = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture")));
	pic->SetState(_T("image"), &img_pic);
	CRect rect(0, 0, img_pic.GetWidth(), img_pic.GetHeight());
	pic->SetWindowRect(rect);
	pic->SetEffect(ExDynCast<IGuiEffect>(ExGui(_T("CGuiFade"))));

	// �������ڶ�������
	IGuiBoard* wnd = ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc));
	wnd->Create(_T("Test"), /*CRect(0, 0, 500, 300)*/rect, SW_HIDE);
	wnd->CenterWindow();
	wnd->SetLayered();
	wnd->ShowWindow(SW_SHOW);

	// ��������ؼ����¼��������
	pic->AddEvent(dbnew(CCusPicEvent));
	btn->AddEvent(dbnew(CCusBtnEvent));
	wnd->AddComp(pic);
	pic->AddComp(btn);
	wnd->AddEvent(dbnew(CCustomEvent));

	IGuiCtrl* itm = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPushBtn")));
	itm->Free();

	// ����Ϣѭ��:
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
