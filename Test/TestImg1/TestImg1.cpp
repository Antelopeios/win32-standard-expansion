// TestImg1.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestImg1.h"

#pragma comment(lib, "msimg32.lib")

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
HBITMAP hBitmap = NULL;

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
	LoadString(hInstance, IDC_TESTIMG1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTIMG1));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	ICoderObject::DeleteImage(hBitmap);

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTIMG1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TESTIMG1);
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

	// ����Դ
	BYTE* buff = NULL; DWORD size = 0;
	HGLOBAL hres = CResGetter::GetBinary(IDB_BMP1, _T("BMP"), buff, size);
	// ���ļ�����ȡ������
	CMemFile file(buff, size); CGC gc;
	ICoderObject* coder = CImgAnalyzer::GetCoder(&file, &gc);
	// �����ļ�
	hBitmap = coder->Decode();
	// �ر���Դ
	CResGetter::ReleaseBinary(hres);
	// ͼ���α�
	double matrix[4] = 
	{
		0, 1, 
		-1, 0, 
	};
	HBITMAP tmp = CImgDeformer::Deform(hBitmap, matrix);
	if (tmp)
	{
		ICoderObject::DeleteImage(hBitmap);
		hBitmap = tmp;
	}

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
		case IDM_OPEN:
			{
				CString ret;
				CSysDialog::FileDialog(ret, TRUE, NULL, NULL, 
					_T("ͼƬ�ļ�(*.bmp;*.jpg;*.png)\0*.bmp;*.jpg;*.png\0�����ļ�(*.*)\0*.*\0"));
				if (ret.Empty()) break;
				// ���ļ�����ȡ������
				CIOFile file(ret); CGC gc;
				ICoderObject* coder = CImgAnalyzer::GetCoder(&file, &gc);
				if (!coder) break;
				// �����ļ�
				ICoderObject::DeleteImage(hBitmap);
				hBitmap = coder->Decode();
				// ˢ�´���
				RECT rect = {0};
				GetClientRect(hWnd, &rect);
				InvalidateRect(hWnd, &rect, FALSE);
			}
			break;
		case IDM_SAVE:
			if (hBitmap)
			{
				CString ret; DWORD inx = 0;
				CSysDialog::FileDialog(ret, FALSE, NULL, NULL, 
					_T("BMP�ļ�(*.bmp)\0*.bmp\0JPG�ļ�(*.jpg)\0*.jpg\0PNG�ļ�(*.png)\0*.png\0"), &inx);
				if (ret.Empty()) break;
				// ��ȡ������
				CGC gc;
				ICoderObject* coder = CImgAnalyzer::GetCoder(inx, &gc);
				if (!coder) break;
				// �����ļ�
				CIOFile file(ret, CIOFile::modeCreate | CIOFile::modeReadWrite | CIOFile::shareExclusive);
				coder->SetFile(&file);
				coder->Encode(hBitmap);
			}
			break;
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

			HDC mem_hdc = ::CreateCompatibleDC(hdc);
			HBITMAP mem_hbm = ::CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
			HGDIOBJ mem_old = ::SelectObject(mem_hdc, mem_hbm);

			HBRUSH brh = (HBRUSH)GetStockObject(GRAY_BRUSH);
			FillRect(mem_hdc, &rect, brh);

			if (hBitmap)
			{
				HDC bmp_hdc = ::CreateCompatibleDC(hdc);
				HGDIOBJ bmp_old = ::SelectObject(bmp_hdc, hBitmap);
				BITMAP bmp_bm;
				::GetObject(hBitmap, sizeof(bmp_bm), &bmp_bm);
				BLENDFUNCTION bf = {0};
				bf.BlendOp				= AC_SRC_OVER;
				bf.BlendFlags			= 0;
				bf.AlphaFormat			= AC_SRC_ALPHA;
				bf.SourceConstantAlpha	= (BYTE)~0;
				::AlphaBlend(mem_hdc, 0, 0, bmp_bm.bmWidth, bmp_bm.bmHeight, 
							 bmp_hdc, 0, 0, bmp_bm.bmWidth, bmp_bm.bmHeight, bf);
				::SelectObject(bmp_hdc, bmp_old);
				::DeleteDC(bmp_hdc);
			}

			::BitBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, mem_hdc, 0, 0, SRCCOPY);

			::SelectObject(mem_hdc, mem_old);
			::DeleteObject(mem_hbm);
			::DeleteDC(mem_hdc);
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