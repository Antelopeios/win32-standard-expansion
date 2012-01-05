// TestImg2.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestImg2.h"

#pragma comment(lib, "msimg32.lib")

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
CImage imgShow;

// 此代码模块中包含的函数的前向声明:
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

	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TESTIMG2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTIMG2));

	// 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	// 打开文件并获取解码器
	CIOFile file(_T("../TestImg1/ground.png"));
	//CIOFile file(_T("ground.png"));
	CGC gc;
	ICoderObject* coder = CImgAnalyzer::GetCoder(&file, &gc);
	// 解码文件
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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
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
		// 分析菜单选择:
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

// “关于”框的消息处理程序。
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
