// TestImg2.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestImg2.h"

#pragma comment(lib, "msimg32.lib")

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
CImage imgShow;

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TESTIMG2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTIMG2));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTIMG2));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TESTIMG2);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	// ���ļ�����ȡ������
	CIOFile file(_T("../TestImg1/ground.png"));
	//CIOFile file(_T("ground.png"));
	CGC gc;
	ICoderObject* coder = CImgAnalyzer::GetCoder(&file, &gc);
	// �����ļ�
	imgShow = coder->Decode();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

volatile long g_Frames = 0, g_FramesCounter = 0;
volatile BOOL g_EndTest = FALSE;
CImage g_MemImg;
CMutex g_MemLck;

class CSpeedTest : public IThread
{
protected:
	DWORD OnThread(LPVOID lpParam)
	{
		g_MemImg.Create(imgShow.GetWidth(), imgShow.GetHeight());

		while(!g_EndTest)
		{
			{
				ExLock(g_MemLck, false);

				//CImgASM::PixZero(g_MemImg.GetPixels(), g_MemImg.GetSize() >> 2);
				ZeroMemory(g_MemImg.GetPixels(), g_MemImg.GetSize());
				//{
				//	CGraph mem_grp;
				//	mem_grp.Create();
				//	mem_grp.SetObject(g_MemImg.Get());
				//	HBRUSH brh = (HBRUSH)GetStockObject(GRAY_BRUSH);
				//	FillRect(mem_grp, &(RECT)CRect(0, 0, imgShow.GetWidth(), imgShow.GetHeight()), brh);
				//	mem_grp.Delete();
				//}
				{
					//CGraph mem_grp;
					//mem_grp.Create();
					//mem_grp.SetObject(g_MemImg.Get());
					//CGraph img_grp;
					//img_grp.Create();
					//img_grp.SetObject(imgShow.Get());
					//BLENDFUNCTION bl = {0};
					//bl.AlphaFormat = AC_SRC_ALPHA;
					//bl.SourceConstantAlpha = 255;
					//AlphaBlend(mem_grp, 0, 0, imgShow.GetWidth(), imgShow.GetHeight(), 
					//		   img_grp, 0, 0, imgShow.GetWidth(), imgShow.GetHeight(), bl);
					////BitBlt(mem_grp, 0, 0, imgShow.GetWidth(), imgShow.GetHeight(), img_grp, 0, 0, SRCCOPY);
					////StretchBlt(mem_grp, 0, 0, imgShow.GetWidth() * 2, imgShow.GetHeight() / 2, 
					////		   img_grp, 0, 0, imgShow.GetWidth(), imgShow.GetHeight(), SRCCOPY);
					//img_grp.Delete();
					//mem_grp.Delete();

					//Render(g_MemImg, imgShow, CRect(), CPoint());

					CImgDrawer::Draw(g_MemImg, imgShow/*, 200*/);
					//CImgDrawer::Cover(g_MemImg, imgShow/*, 200*/);

					//CImgRenderer::Render(g_MemImg, imgShow, CRect(), CPoint(), &CRenderCopy(/*200*/));
					//CImgRenderer::Render(g_MemImg, imgShow, CRect(), CPoint(), &CRenderNormal());
					//CImgRenderer::Render(g_MemImg, imgShow, CRect(), CPoint(), &CRenderOverlay(/*200*/));

					//CImgDrawer::Draw(g_MemImg, imgShow, CRect(0, 0, imgShow.GetWidth() * 2, imgShow.GetHeight() / 2), CRect()/*, 200*/);

					//CImage img_tmp(CImgDeformer::ZomDeform(imgShow, imgShow.GetWidth() * 2, imgShow.GetHeight() / 2));
					//CImgRenderer::Render(g_MemImg, img_tmp, CRect(), CPoint(), &CRenderCopy(/*200*/));
				}
			}
			++g_FramesCounter;
		}

		return 0;
	}
} g_SpeedTest;

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_SHOWWINDOW:
		if (wParam)
		{
			g_SpeedTest.Create();
			::SetTimer(hWnd, 1, 1000, NULL);
		}
		break;
	case WM_TIMER:
		if (wParam == 1)
		{
			g_Frames = g_FramesCounter;
			g_FramesCounter = 0;
			RECT rect = {0};
			GetClientRect(hWnd, &rect);
			::InvalidateRect(hWnd, &rect, FALSE);
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		{
			RECT rect = {0};
			GetClientRect(hWnd, &rect);

			CImage mem_img;
			mem_img.Create(rect.right - rect.left, rect.bottom - rect.top);

			CGraph mem_grp;
			mem_grp.Create(hdc);
			mem_grp.SetObject(mem_img.Get());

			HBRUSH brh = (HBRUSH)GetStockObject(/*BLACK_BRUSH*/GRAY_BRUSH);
			FillRect(mem_grp, &rect, brh);

			{
				ExLock(g_MemLck, true);
				Render(mem_img, g_MemImg, CRect(), CPoint());
			}
			Render(mem_img, imgShow, 
				CRect(imgShow.GetWidth(), 0, imgShow.GetWidth() << 1, imgShow.GetHeight()), 
				CPoint());

			CText text(_T(""), (font_t)::GetStockObject(DEFAULT_GUI_FONT), ExRGBA(255, 255, 255, 128));
			text.Format(_T("%d fps, %d mspkf"), g_Frames, g_Frames ? 1000000 / g_Frames : 0);
			CImage bmp_img(text.GetImage());
			if(!bmp_img.IsNull())
				Render(mem_img, bmp_img, CRect(
						rect.right - bmp_img.GetWidth() - 5, 
						rect.bottom - bmp_img.GetHeight() - 5, 
						rect.right, rect.bottom), CPoint());

			::BitBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, mem_grp, 0, 0, SRCCOPY);
			mem_grp.Delete();
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_ERASEBKGND:
		break;
	case WM_DESTROY:
		g_EndTest = TRUE;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
