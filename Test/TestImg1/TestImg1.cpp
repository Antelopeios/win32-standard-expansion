// TestImg1.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestImg1.h"

#pragma comment(lib, "msimg32.lib")

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
CImage imgShow, imgOrig;

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
	LoadString(hInstance, IDC_TESTIMG1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTIMG1));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTIMG1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TESTIMG1);
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

	// 打开资源
	BYTE* buff = NULL; DWORD size = 0;
	HGLOBAL hres = CResGetter::GetBinary(IDB_BMP1, _T("BMP"), buff, size);
	// 打开文件并获取解码器
	CMemFile file(buff, size); CGC gc;
	ICoderObject* coder = CImgAnalyzer::GetCoder(&file, &gc);
	// 解码文件
	imgOrig = coder->Decode();
	imgShow = imgOrig.Clone();
	// 关闭资源
	CResGetter::ReleaseBinary(hres);

	// 允许拖放
	DragAcceptFiles(hWnd, TRUE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

BOOL Invalidate(HWND hWnd)
{
	// 刷新窗口
	RECT rect = {0};
	GetClientRect(hWnd, &rect);
	return InvalidateRect(hWnd, &rect, FALSE);
}

BOOL OpenFile(HWND hWnd, CString& sPath)
{
	if (sPath.Empty()) return FALSE;
	// 打开文件并获取解码器
	CIOFile file(sPath); CGC gc;
	ICoderObject* coder = CImgAnalyzer::GetCoder(&file, &gc);
	if (!coder) return FALSE;
	// 解码文件
	imgOrig = coder->Decode();
	imgShow = imgOrig.Clone();
	return TRUE;
} 

BOOL SaveFile(DWORD nInx, CString& sPath)
{
	if (imgShow.IsNull()) return FALSE;
	if (sPath.Empty()) return FALSE;
	// 获取解码器
	CGC gc;
	ICoderObject* coder = CImgAnalyzer::GetCoder(nInx, &gc);
	if (!coder) return FALSE;
	// 编码文件
	CIOFile file(sPath, CIOFile::modeCreate | CIOFile::modeReadWrite | CIOFile::shareExclusive);
	coder->SetFile(&file);
	coder->Encode(imgShow);
	return TRUE;
}

void Render(HWND hWnd, IFilterObject* pFilter = NULL)
{
	if (imgShow.IsNull()) return;
	CImgRenderer::Render(imgShow, imgShow, CRect(), CPoint(), pFilter);
	// 刷新窗口
	Invalidate(hWnd);
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
		// 文件
		case IDM_OPEN:
			{
				CString ret;
				CSysDialog::FileDialog(ret, TRUE, NULL, NULL, 
					_T("图片文件(*.bmp;*.jpg;*.png)\0*.bmp;*.jpg;*.png\0所有文件(*.*)\0*.*\0"));
				OpenFile(hWnd, ret);
				// 刷新窗口
				Invalidate(hWnd);
			}
			break;
		case IDM_SAVE:
			if (imgShow)
			{
				CString ret; DWORD inx = 0;
				CSysDialog::FileDialog(ret, FALSE, NULL, NULL, 
					_T("BMP文件(*.bmp)\0*.bmp\0JPG文件(*.jpg)\0*.jpg\0PNG文件(*.png)\0*.png\0"), &inx);
				SaveFile(inx, ret);
			}
			break;
		// 原图
		case IDM_ORI:
			if (imgOrig)
			{
				imgShow = imgOrig.Clone();
				// 刷新窗口
				Invalidate(hWnd);
			}
			break;
		// 形变
		case IDM_PLG:
			if (imgShow)
			{
				CPoint var[2] = 
				{
					CPoint(500, 200), 
					CPoint(600, 400)
				};
				image_t tmp = CImgDeformer::PlgDeform(imgShow, var);
				if(!tmp) break;
				imgShow = tmp;
				// 刷新窗口
				Invalidate(hWnd);
			}
			break;
		case IDM_WHL:
			if (imgShow)
			{
				image_t tmp = CImgDeformer::WhlDeform(imgShow, -45);
				if(!tmp) break;
				imgShow = tmp;
				// 刷新窗口
				Invalidate(hWnd);
			}
			break;
		case IDM_ZOM:
			if (imgShow)
			{
				CImage bmp_img(imgShow);
				image_t tmp = CImgDeformer::ZomDeform(imgShow, (LONG)(bmp_img.GetWidth() * -1.2), (LONG)(bmp_img.GetHeight() * 1.5));
				if(!tmp) break;
				imgShow = tmp;
				// 刷新窗口
				Invalidate(hWnd);
			}
			break;
		// 渲染
		case IDM_GRAY:
			Render(hWnd, &CFilterGrayT<CFilterCopy>());
			break;
		case IDM_INVE:
			Render(hWnd, &CFilterInverseT<CFilterCopy>());
			break;
		case IDM_RELI:
			Render(hWnd, &CFilterReliefT<CFilterCopy>());
			break;
		case IDM_DIFF:
			Render(hWnd, &CFilterDiffuseT<CFilterCopy>());
			break;
		case IDM_GAUS:
			Render(hWnd, &CFilterGaussT<CFilterCopy>());
			break;
		case IDM_OUTG:
			if (imgShow)
			{
				CFilterOuterGlowT<CFilterCopy> filter;
				CPoint pt_flt(filter.GetRadius(), filter.GetRadius());
				// 将图片扩大
				CRect rc(0, 0, imgShow.GetWidth(), imgShow.GetHeight());
				image_t tmp = imgShow.Clone(rc + pt_flt);
				if (!tmp) break;
				imgShow = tmp;
				// 渲染图片
				Render(hWnd, &filter);
			}
			break;
		case IDM_GRAD:
			Render(hWnd, &CFilterGradientT<CFilterCopy>(
				ExRGBA(255, 255, 255, 128), ExRGBA(0, 0, 0, 128), 0x9, false));
			break;
		// 关于
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		// 退出
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

			if(!imgShow.IsNull())
			{
				CImgRenderer::Render
					(
					mem_img, imgShow, 
					CRect
						(
						(rect.right - imgShow.GetWidth()) / 2, 
						(rect.bottom - imgShow.GetHeight()) / 2, 
						rect.right, 
						rect.bottom
						), 
					CPoint()
					);
			}

			CText text(_T("Dark C.at"), (font_t)::GetStockObject(DEFAULT_GUI_FONT), ExRGBA(255, 255, 255, 128));
			CImage bmp_img(text.GetImage());
			if(!bmp_img.IsNull())
			{
				CImgRenderer::Render
					(
					mem_img, bmp_img, 
					CRect
						(
						rect.right - bmp_img.GetWidth() - 5, 
						rect.bottom - bmp_img.GetHeight() - 5, 
						rect.right, 
						rect.bottom
						), 
					CPoint()
					);
			}

			::BitBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, mem_grp, 0, 0, SRCCOPY);

			mem_grp.Delete();
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_ERASEBKGND:
		break;
	case WM_DROPFILES:
		{
			HDROP drop = (HDROP)wParam;
			CString path;
			DragQueryFile(drop, 0, path.GetCStr(MAX_PATH), MAX_PATH);
			DragFinish(drop);
			if (path.Empty()) break;
			WIN32_FIND_DATA fd;
			HANDLE find = ::FindFirstFile(path, &fd);
			if (find == INVALID_HANDLE_VALUE)
				break;
			::FindClose(find);
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				break;
			OpenFile(hWnd, path);
			// 刷新窗口
			Invalidate(hWnd);
		}
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
