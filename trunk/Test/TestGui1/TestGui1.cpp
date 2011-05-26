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

	// ������Դ
	CGC gc;
	CIOFile file(_T("../TestImg1/ground.png"));
	ICoderObject* coder = CImgAnalyzer::GetCoder(&file, &gc);
	if (!coder) return 0;
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
	CText txt_btn[5];
	for(int i = 0; i < _countof(txt_btn); ++i)
	{
		txt_btn[i].SetString(_T("Dark C.at"));
		txt_btn[i].SetFont((font_t)::GetStockObject(DEFAULT_GUI_FONT));
		txt_btn[i].SetColor(ExRGBA(0, 60, 116, 200));
	}

	// ����Ч����������
	IGuiEffect* eff = ExDynCast<IGuiEffect>(ExGui(_T("CGuiFade"), &gc));

	// ����ͼƬ�ؼ���������
	IGuiCtrl* pic = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc));
	pic->SetState(_T("image"), &img_pic);
	//CText text(_T("Dark C.at"), (font_t)::GetStockObject(DEFAULT_GUI_FONT), ExRGBA(255, 255, 0, 255));
	//pic->SetState(_T("text"), &text);
	CRect rect(0, 0, img_pic.GetWidth(), img_pic.GetHeight());
	pic->SetRect(rect);
	pic->SetEffect(eff);

	// ������ť�ؼ���������
	IGuiCtrl* btn = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiButton"), &gc));
	btn->SetState(_T("image"), img_btn);
	btn->SetState(_T("text"), txt_btn);
	btn->SetRect(CRect(70, 100, 180, 140));

	// �������ڶ�������
	IGuiBoard* wnd = ExDynCast<IGuiBoard>(ExGui(_T("CGuiWnd"), &gc));
	wnd->Create(_T("Test"), /*CRect(0, 0, 500, 300)*/rect, SW_HIDE);
	wnd->CenterWindow();
	wnd->SetLayered();
	wnd->ShowWindow(SW_SHOW);

	// �����¼���������
	CCustomEvent cus_evt;
	CCusPicEvent pic_evt;

	// ��������ؼ����¼��������
	pic->AddEvent(&pic_evt);
	wnd->AddComp(pic);
	wnd->AddComp(btn);
	wnd->AddEvent(&cus_evt);

	// ����Ϣѭ��:
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// ������Դ
	for(int i = 0; i < _countof(img_btn); ++i)
		img_btn[i].Delete();
	img_pic.Delete();

	return 0;
}
