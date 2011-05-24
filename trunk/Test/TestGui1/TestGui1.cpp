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

	// ����Ч����������
	IGuiEffect* eff = ExDynCast<IGuiEffect>(ExGui(_T("CGuiFade"), &gc));

	// �����ؼ���������
	IGuiCtrl* pic = ExDynCast<IGuiCtrl>(ExGui(_T("CGuiPicture"), &gc));
	pic->SetState(_T("image"), &img_pic);
	CRect rect(0, 0, img_pic.GetWidth(), img_pic.GetHeight());
	pic->SetRect(rect);
	pic->SetEffect(eff);

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
	wnd->AddEvent(&cus_evt);

	// ����Ϣѭ��:
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
