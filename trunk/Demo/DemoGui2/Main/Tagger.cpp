// Tagger.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Tagger.h"

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
	DAT()->Init();	// 数据层初始化
	GUI()->Init();	// 界面层初始化
}

void CTagger::Term()
{
	GUI()->Term();	// 界面层卸载
	DAT()->Term();	// 数据层卸载

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

void CTagger::Del(const CString& sFile, const CString& sTag)
{
	CData::tsk_t task;
	task.oper = CData::del;
	task.rest.type = CData::file;
	task.rest.name = sFile;
	task.rest.link.Add(sTag);
	task.call = RetProc;
	DAT()->PostTask(task);
}

void CTagger::DelTag(const CString& sName)
{
	CData::tsk_t task;
	task.oper = CData::del;
	task.rest.type = CData::tag;
	task.rest.name = sName;
	task.call = RetProc;
	DAT()->PostTask(task);
}

void CTagger::DelFile(const CString& sName)
{
	CData::tsk_t task;
	task.oper = CData::del;
	task.rest.type = CData::file;
	task.rest.name = sName;
	task.call = RetProc;
	DAT()->PostTask(task);
}

void CTagger::SetTag(const CString& sOld, const CString& sNew)
{
	CData::tsk_t task;
	task.oper = CData::set;
	task.rest.type = CData::tag;
	task.rest.name = sOld;
	task.name = sNew;
	task.call = RetProc;
	DAT()->PostTask(task);
}

void CTagger::SetFile(const CString& sOld, const CString& sNew)
{
	CData::tsk_t task;
	task.oper = CData::set;
	task.rest.type = CData::file;
	task.rest.name = sOld;
	task.name = sNew;
	task.call = RetProc;
	DAT()->PostTask(task);
}