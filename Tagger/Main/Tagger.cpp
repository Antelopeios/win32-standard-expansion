// Tagger.cpp : ����Ӧ�ó������ڵ㡣
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

	// ��ʼ��
	TAG()->Init();

	// ж��
	TAG()->Term();

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
	GLB()->Init();	// ȫ�ֳ�ʼ��
	DAT()->Init();	// ���ݲ��ʼ��
	GUI()->Init();	// ������ʼ��
}

void CTagger::Term()
{
	GUI()->Term();	// �����ж��
	DAT()->Term();	// ���ݲ�ж��
	GLB()->Term();	// ȫ��ж��

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