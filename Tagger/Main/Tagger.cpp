// Tagger.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Tagger.h"

//////////////////////////////////////////////////////////////////

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 初始化
	GBL()->Init();	// 全局初始化
	DAT()->Init();	// 数据层初始化
	TAG()->Init();	// 逻辑层初始化
	GUI()->Init();	// 界面层初始化

	// 主消息循环
	MSG msg = {0}; BOOL ret = FALSE;
	while ((ret = ::GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (ret == -1)
			break;
		else
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	// 释放资源
	GUI()->Term();
	TAG()->Term();
	DAT()->Term();
	GBL()->Term();

	return (int) msg.wParam;
}

//////////////////////////////////////////////////////////////////

CTagger::CTagger()
{
}

CTagger* g_instance = NULL;
CTagger* CTagger::Instance()
{
	if (g_instance == NULL)
		g_instance = ExMem::Alloc<CTagger>();
	return g_instance;
}

void CTagger::Init()
{
}

void CTagger::Term()
{
	ExMem::Free(this);
	g_instance = NULL;
}

//////////////////////////////////////////////////////////////////

void CTagger::RetProc(CData::ret_t ret)
{
	if (ret.type == CData::err) return;
	TAG()->m_RetBuf = ret.link;
	GUI()->Refresh();
}

CTagger::list_t& CTagger::GetRet()
{
	return m_RetBuf;
}

void CTagger::GetTags()
{
	CData::tsk_t task;
	task.oper = CData::get;
	task.rest.type = CData::tag;
	task.call = RetProc;
	DAT()->PostTask(task);
}

void CTagger::GetTags(const CString& sFile)
{
	CData::tsk_t task;
	task.oper = CData::get;
	task.rest.type = CData::file;
	task.rest.name = sFile;
	task.call = RetProc;
	DAT()->PostTask(task);
}

void CTagger::GetFiles(const CString& sTag)
{
	CData::tsk_t task;
	task.oper = CData::get;
	task.rest.type = CData::tag;
	task.rest.name = sTag;
	task.call = RetProc;
	DAT()->PostTask(task);
}