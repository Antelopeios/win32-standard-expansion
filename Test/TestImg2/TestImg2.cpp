// TestImg2.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestImg2.h"

#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
CImage imgShow, imgOrig;

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
//    �˴��������ӵ� Windows 95 �еġ�RegisterClassEx��
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
	CIOFile file(_T("../TestImg1/ground.png")); CGC gc;
	ICoderObject* coder = CImgAnalyzer::GetCoder(&file, &gc);
	// �����ļ�
	imgOrig = coder->Decode();
	imgShow = imgOrig.Clone();

	__m128 m_1, m_2, m_r;

	timeBeginPeriod(1);
	DWORD t_s = timeGetTime();

	for(int i = 0; i < 10000000; ++i)
	{
		ExSetSSE(m_1, 123, 321, 231, 312);
		ExSetSSE(m_2, 312, 213, 231, 123);
		{
			uint32_t* sr = (uint32_t*)(&m_r);
			uint32_t* s1 = (uint32_t*)(&m_1);
			uint32_t* s2 = (uint32_t*)(&m_2);
			__asm
			{
				mov eax, [s1]
				mov esi, [s2]
				mov ecx, [sr]
				movups xmm0, [eax]
				movups xmm1, [esi]
				addps xmm0, xmm1
				movups [ecx], xmm0
			}
		}
		{
			uint32_t* sr = (uint32_t*)(&m_r);
			uint32_t* s1 = (uint32_t*)(&m_1);
			uint32_t* s2 = (uint32_t*)(&m_2);
			__asm
			{
				mov eax, [s1]
				mov esi, [s2]
				mov ecx, [sr]
				movups xmm0, [eax]
				movups xmm1, [esi]
				subps xmm0, xmm1
				movups [ecx], xmm0
			}
		}
		{
			uint32_t* sr = (uint32_t*)(&m_r);
			uint32_t* s1 = (uint32_t*)(&m_1);
			uint32_t* s2 = (uint32_t*)(&m_2);
			__asm
			{
				mov eax, [s1]
				mov esi, [s2]
				mov ecx, [sr]
				movups xmm0, [eax]
				movups xmm1, [esi]
				mulps xmm0, xmm1
				movups [ecx], xmm0
			}
		}
		{
			uint32_t* sr = (uint32_t*)(&m_r);
			uint32_t* s1 = (uint32_t*)(&m_1);
			uint32_t* s2 = (uint32_t*)(&m_2);
			__asm
			{
				mov eax, [s1]
				mov esi, [s2]
				mov ecx, [sr]
				movups xmm0, [eax]
				movups xmm1, [esi]
				divps xmm0, xmm1
				movups [ecx], xmm0
			}
		}
		//ExAddSSE(&m_r, &m_1, &m_2);
		//ExSubSSE(&m_r, &m_1, &m_2);
		//ExMulSSE(&m_r, &m_1, &m_2);
		//ExDivSSE(&m_r, &m_1, &m_2);
	}
	//for(int i = 0; i < 10000000; ++i)
	//{
	//	CSSE::SetNor(m_1, (float)123, (float)321, (float)231, (float)312);
	//	CSSE::SetNor(m_2, (float)312, (float)213, (float)231, (float)123);

	//	m_r.m128_f32[0] = (m_1.m128_f32[0] + m_2.m128_f32[0]);
	//	m_r.m128_f32[1] = (m_1.m128_f32[1] + m_2.m128_f32[1]);
	//	m_r.m128_f32[2] = (m_1.m128_f32[2] + m_2.m128_f32[2]);
	//	m_r.m128_f32[3] = (m_1.m128_f32[3] + m_2.m128_f32[3]);

	//	m_r.m128_f32[0] = (m_1.m128_f32[0] - m_2.m128_f32[0]);
	//	m_r.m128_f32[1] = (m_1.m128_f32[1] - m_2.m128_f32[1]);
	//	m_r.m128_f32[2] = (m_1.m128_f32[2] - m_2.m128_f32[2]);
	//	m_r.m128_f32[3] = (m_1.m128_f32[3] - m_2.m128_f32[3]);

	//	m_r.m128_f32[0] = (m_1.m128_f32[0] * m_2.m128_f32[0]);
	//	m_r.m128_f32[1] = (m_1.m128_f32[1] * m_2.m128_f32[1]);
	//	m_r.m128_f32[2] = (m_1.m128_f32[2] * m_2.m128_f32[2]);
	//	m_r.m128_f32[3] = (m_1.m128_f32[3] * m_2.m128_f32[3]);

	//	m_r.m128_f32[0] = (m_1.m128_f32[0] / m_2.m128_f32[0]);
	//	m_r.m128_f32[1] = (m_1.m128_f32[1] / m_2.m128_f32[1]);
	//	m_r.m128_f32[2] = (m_1.m128_f32[2] / m_2.m128_f32[2]);
	//	m_r.m128_f32[3] = (m_1.m128_f32[3] / m_2.m128_f32[3]);
	//}

	DWORD t_e = timeGetTime();
	timeEndPeriod(1);
	t_e -= t_s;
	CString msg;
	msg.Format(_T("%d ms (%f, %f, %f, %f)"), t_e, 
		m_r.m128_f32[0], m_r.m128_f32[1], m_r.m128_f32[2], m_r.m128_f32[3]);
	::MessageBox(NULL, (LPCTSTR)msg, NULL, 0);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

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

			HBRUSH brh = (HBRUSH)GetStockObject(GRAY_BRUSH);
			FillRect(mem_grp, &rect, brh);

			mem_grp.Delete();

			timeBeginPeriod(1);
			DWORD t_s = timeGetTime();
			for(int i = 0; i < 1000; ++i)
			{
				//CGraph mem_grp;
				//mem_grp.Create();
				//mem_grp.SetObject(mem_img.Get());
				//CGraph img_grp;
				//img_grp.Create();
				//img_grp.SetObject(imgShow.Get());

				//BLENDFUNCTION bl = {0};
				//bl.AlphaFormat = AC_SRC_OVER;
				//bl.SourceConstantAlpha = 255;
				//AlphaBlend(mem_grp, 0, 0, imgShow.GetWidth(), imgShow.GetHeight(), 
				//		   img_grp, 0, 0, imgShow.GetWidth(), imgShow.GetHeight(), bl);

				//img_grp.Delete();
				//mem_grp.Delete();

				Render(mem_img, imgShow, CRect(), CPoint());

				//CImgRenderer::Render(mem_img, imgShow, CRect(), CPoint()/*, &CRenderOverlay()*/);
			}
			DWORD t_e = timeGetTime();
			timeEndPeriod(1);
			t_e -= t_s;
			CString msg;
			msg.Format(_T("%d ms"), t_e);
			::MessageBox(NULL, (LPCTSTR)msg, NULL, 0);

			mem_grp.Create(hdc);
			mem_grp.SetObject(mem_img.Get());
			::BitBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, mem_grp, 0, 0, SRCCOPY);
			mem_grp.Delete();
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_ERASEBKGND:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ��������
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