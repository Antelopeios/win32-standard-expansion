// Tagger.cpp : ����Ӧ�ó������ڵ㡣
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

	// ��ʼ��
	GBL()->Init();	// ȫ�ֳ�ʼ��
	DAT()->Init();	// ���ݲ��ʼ��
	TAG()->Init();	// �߼����ʼ��
	GUI()->Init();	// ������ʼ��

	// ����Ϣѭ��
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

	// �ͷ���Դ
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