// Tagger.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Tagger.h"

//////////////////////////////////////////////////////////////////

#ifdef	_CONSOLE
int _tmain(int argc, _TCHAR* argv[])
{
#else /*_CONSOLE*/
int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
#endif/*_CONSOLE*/

	// 初始化
	GLB()->Init();	// 全局初始化
	DAT()->Init();	// 数据层初始化
	TAG()->Init();	// 逻辑层初始化
	GUI()->Init();	// 界面层初始化

	// 释放资源
	GUI()->Term();
	TAG()->Term();
	DAT()->Term();
	GLB()->Term();

	return 0;
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
	task.rest.type = CData::file;
	task.call = RetProc;
	DAT()->PostTask(task);
}

void CTagger::GetFiles()
{
	CData::tsk_t task;
	task.oper = CData::get;
	task.rest.type = CData::tag;
	task.call = RetProc;
	DAT()->PostTask(task);
}

void CTagger::GetTag(const CString& sName)
{
	CData::tsk_t task;
	task.oper = CData::get;
	task.rest.type = CData::tag;
	task.rest.name = sName;
	task.call = RetProc;
	DAT()->PostTask(task);
}

void CTagger::GetFile(const CString& sName)
{
	CData::tsk_t task;
	task.oper = CData::get;
	task.rest.type = CData::file;
	task.rest.name = sName;
	task.call = RetProc;
	DAT()->PostTask(task);
}

void CTagger::Add(const CString& sFile, const CString& sTag)
{
	CData::tsk_t task;
	task.oper = CData::add;
	task.rest.type = CData::file;
	task.rest.name = sFile;
	task.rest.link.Add(sTag);
	task.call = RetProc;
	DAT()->PostTask(task);
}