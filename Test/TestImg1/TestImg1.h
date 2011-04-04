#pragma once

#include "resource.h"

#include <shlobj.h>
#include <commdlg.h>

class CResGetter
{
public:
	static BOOL ReleaseBinary(HGLOBAL hData)
	{
		if (!hData) return TRUE;
		// 释放资源内存块
		UnlockResource(hData);
		return FreeResource(hData);
	}

	static HGLOBAL GetBinary(UINT nID, LPCTSTR szType, BYTE*& btBuff, DWORD& dwSize, HMODULE hInstance = NULL)
	{
		// 加载资源内存
		HRSRC info = ::FindResource(hInstance, MAKEINTRESOURCE(nID), szType);
		if( info == NULL ) return NULL;
		HGLOBAL data = ::LoadResource(hInstance, info);
		if( data == NULL ) return NULL;

		// 获得资源内存块
		dwSize = ::SizeofResource(hInstance, info);
		if( dwSize == NULL )
		{
			ReleaseBinary(data);
			return NULL;
		}
		btBuff = (BYTE*)::LockResource(data);
		if( btBuff == NULL )
		{
			ReleaseBinary(data);
			return NULL;
		}
		return data;
	}
};

class CSysDialog
{
	// 系统对话框方法
public:
	// 选择目录对话框
	static CString DirDialog(LPCTSTR lpszTitle = NULL, LPTSTR lpszPath = NULL)
	{
		TCHAR path[MAX_PATH] = {0};
		CComPtr<IMalloc> pMalloc = NULL;
		if(SHGetMalloc(&pMalloc) == S_OK && pMalloc)
		{
			LPITEMIDLIST root = NULL;
			if(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &root) == S_OK && root)
			{
				BROWSEINFO bi = {0};
				bi.hwndOwner = GetFocus();
				bi.pidlRoot = root;
				bi.lpszTitle = lpszTitle;
				bi.pszDisplayName = lpszPath;
				bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_DONTGOBELOWDOMAIN | BIF_EDITBOX;
				LPITEMIDLIST folder = SHBrowseForFolder(&bi);
				if(folder)
				{
					SHGetPathFromIDList(folder, path);
					pMalloc->Free(folder);
				}
				pMalloc->Free(root);
			}
		}
		return path;
	}

	// 打开文件对话框
	static BOOL FileDialog(
		CString& strRet, 
		BOOL bOpenFileDialog = TRUE, 
		LPCTSTR lpszFileName = NULL, 
		LPCTSTR lpszDefExt = NULL, 
		LPCTSTR lpszFilter = _T("所有文件(*.*)\0*.*\0"))
	{
		TCHAR szFile[MAX_PATH] = {0};
		OPENFILENAME ofn = {0};

		int structsize = 0;
		DWORD dwVersion, dwWindowsMajorVersion, dwWindowsMinorVersion;
		//检测目前的操作系统
		dwVersion = GetVersion();
		dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
		dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

		if (dwVersion < 0x80000000)
			// 如果运行的操作系统是Windows NT/2000
			structsize = 88; //显示新的文件对话框
		else
			//运行的操作系统Windows 95/98
			structsize = 76; //显示老的文件对话框

		ofn.lStructSize = structsize;
		ofn.hwndOwner = GetFocus();
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = _countof(szFile);
		ofn.lpstrDefExt = lpszDefExt;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.Flags |= OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK | OFN_EXPLORER;
		ofn.lpstrFilter = lpszFilter;

		if (lpszFileName)
			_tcscpy_s(szFile, lpszFileName);

		BOOL ret = bOpenFileDialog ? GetOpenFileName(&ofn) : GetSaveFileName(&ofn);
		if (ret) strRet = szFile;
		return ret;
	}
};