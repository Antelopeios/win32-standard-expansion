// TestImg3.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestImg3.h"

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
	LoadString(hInstance, IDC_TESTIMG3, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTIMG3));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	imgShow.Delete();

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTIMG3));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TESTIMG3);
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

	hWnd = CreateWindowEx(WS_EX_LAYERED, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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

	CRect rect;
	GetWindowRect(hWnd, (LPRECT)&rect);
	InvalidateRect(hWnd, (LPRECT)&rect, FALSE);

	return TRUE;
}

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
			CRect rect;
			GetWindowRect(hWnd, (LPRECT)&rect);

			CImage mem_img;
			mem_img.Create(rect.Width(), rect.Height());
			CImage rdr_img;
			rdr_img.Create(rect.Width(), rect.Height());

			{
				CImage ctl_img;
				ctl_img.Create(rect.Width(), rect.Height());

				pixel_t pixel = ExRGBA(128, 128, 128, 128);
				CImgASM::PixPreMul(&pixel, 1);

				CImgDrawer::Fill(ctl_img, pixel);
				CImgRenderer::Render(ctl_img, imgShow, CRect(), CPoint(), &CRenderNormal());
				//{
				//	CGraph mem_grp;
				//	mem_grp.Create();
				//	mem_grp.SetObject(ctl_img.Get());
				//	CGraph img_grp;
				//	img_grp.Create();
				//	img_grp.SetObject(imgShow.Get());
				//	BLENDFUNCTION bl = {0};
				//	bl.AlphaFormat = AC_SRC_ALPHA;
				//	bl.SourceConstantAlpha = 255;
				//	AlphaBlend(mem_grp, 0, 0, imgShow.GetWidth(), imgShow.GetHeight(), 
				//			   img_grp, 0, 0, imgShow.GetWidth(), imgShow.GetHeight(), bl);
				//	img_grp.Delete();
				//	mem_grp.Delete();
				//}

				CImgRenderer::Render(rdr_img, ctl_img, CRect(), CPoint(), &CRenderNormal());
			}

			CImgRenderer::Render(mem_img, rdr_img, CRect(), CPoint(), &CRenderCopy());

			CGraph mem_grp;
			mem_grp.Create(hdc);
			mem_grp.SetObject(mem_img.Get());

			POINT pt_wnd = {rect.Left(), rect.Top()};
			SIZE  sz_wnd = {rect.Width(), rect.Height()};
			POINT pt_src = {0, 0};

			BLENDFUNCTION blend		  = {0};
			blend.AlphaFormat		  = AC_SRC_ALPHA;
			blend.SourceConstantAlpha = EXP_CM;
			::UpdateLayeredWindow(hWnd, hdc, &pt_wnd, &sz_wnd, mem_grp, &pt_src, 0, &blend, ULW_ALPHA);

			mem_grp.Delete();
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
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
