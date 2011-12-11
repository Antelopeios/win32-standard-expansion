#include "stdafx.h"
#include "Main/Tagger.h"

//////////////////////////////////////////////////////////////////

// ��ȡ
BOOL CData::GetRet(tsk_t& task)
{
	if (task.rest.type == err) return FALSE;
	task.rest.link.Clear();
	BOOL retn = FALSE;
/*	ret_list_t::iterator_t dun = m_RestList.Tail();
	if(!m_RestList.Empty())
	{	// ����������ҽ��
		ret_list_t::iterator_t ite = m_RestList.Head();
		int dun_cnt = (*ite).cntr;
		for(; ite != m_RestList.Tail(); ++ite)
		{
			if ((*ite).type != task.rest.type || 
				(*ite).name != task.rest.name)
			{
				--((*ite).cntr);
				if (dun_cnt > (*ite).cntr)
				{	// �������д������ٵĽ��
					dun_cnt = (*ite).cntr;
					dun = ite;
				}
			}
			else
			{	// ����
				++((*ite).cntr);
				task.rest = *ite;
				retn = TRUE;
			}
		}
	}
	if(!retn)
	{	// δ���л���,�������ݿ�*/
		CGuiXML::iterator_t ite = m_Data.GetRoot();
		switch(task.rest.type)
		{
		case file:
			while(m_Data.GetNode(_T("link"), ite))
			{
				CString name = m_Data.GetAttr(_T("file"), ite);
				if (task.rest.name.Empty() || task.rest.name == name)
				{
					task.rest.link.Add(m_Data.GetAttr(_T("tag"), ite));
					retn = TRUE;
				}
			}
			break;
		case tag:
			while(m_Data.GetNode(_T("link"), ite))
			{
				CString name = m_Data.GetAttr(_T("tag"), ite);
				if (task.rest.name.Empty() || task.rest.name == name)
				{
					task.rest.link.Add(m_Data.GetAttr(_T("file"), ite));
					retn = TRUE;
				}
			}
			break;
		}
/*		if (retn)
		{	// ����
			task.rest.cntr = 1;
			// д�뻺��
			m_RestList.Add(task.rest);
			if (m_RestList.GetCount() > REST_MAX)
				m_RestList.Del(dun); // �ͷŵ����д������ٵĻ�����
		}
	}*/
	if (!retn) task.rest.type = err;
	if (task.call) task.call(task.rest);
	return retn;
}

// ���/�޸�
BOOL CData::AddRet(tsk_t& task)
{
	if (task.rest.type == err) return FALSE;
	if (task.rest.link.Empty()) return FALSE;
	ret_t ret = task.rest;
	ret_call_t call = task.call;
	task.call = NULL;
	GetRet(task);
	if (task.rest.type == err)
	{	// ���ݲ�����,���
		ret_t::lnk_list_t::iterator_t ret_it = ret.link.Head();
		for(; ret_it != ret.link.Tail(); ++ret_it)
		{
			CGuiXML::iterator_t ite = m_Data.GetRoot();
			m_Data.AddNode(_T("link"), ite);
			if (ret.type == file)
			{
				m_Data.AddAttr(_T("file"), ret.name, ite);
				m_Data.AddAttr(_T("tag"), *ret_it, ite);
			}
			else
			{
				m_Data.AddAttr(_T("tag"), ret.name, ite);
				m_Data.AddAttr(_T("file"), *ret_it, ite);
			}
		}
	}
	else
	{	// �����Ѵ���,�޸�
		ret_t::lnk_list_t::iterator_t ret_it = ret.link.Head();
		for(; ret_it != ret.link.Tail(); ++ret_it)
		{
			ret_t::lnk_list_t::iterator_t tsk_it = task.rest.link.Find(*ret_it);
			if (tsk_it != task.rest.link.Tail()) continue;
			// ����������,���һ�����
			CGuiXML::iterator_t ite = m_Data.GetRoot();
			m_Data.AddNode(_T("link"), ite);
			if (ret.type == file)
			{
				m_Data.AddAttr(_T("file"), ret.name, ite);
				m_Data.AddAttr(_T("tag"), *ret_it, ite);
			}
			else
			{
				m_Data.AddAttr(_T("tag"), ret.name, ite);
				m_Data.AddAttr(_T("file"), *ret_it, ite);
			}
		}
/*		ret_t::lnk_list_t::iterator_t tsk_it = task.rest.link.Head();
		for(; tsk_it != task.rest.link.Tail(); ++tsk_it)
		{
			ret_t::lnk_list_t::iterator_t ret_it = ret.link.Find(*tsk_it);
			if (ret_it != ret.link.Tail()) continue;
			// ����������,ɾ��һ�����
			CGuiXML::iterator_t ite = m_Data.GetRoot();
			while(m_Data.GetNode(_T("link"), ite))
			{
				CString name;
				if (ret.type == file)
				{
					name = m_Data.GetAttr(_T("file"), ite);
					if (task.rest.name == name)
					{
						name = m_Data.GetAttr(_T("tag"), ite);
						if ((*tsk_it) == name)
						{
							m_Data.DelNode(ite);
							break;
						}
					}
				}
				else
				{
					name = m_Data.GetAttr(_T("tag"), ite);
					if (task.rest.name == name)
					{
						name = m_Data.GetAttr(_T("file"), ite);
						if ((*tsk_it) == name)
						{
							m_Data.DelNode(ite);
							break;
						}
					}
				}
			}
		}*/
	}
	task.rest = ret;
	task.call = call;
	return GetRet(task);
}

// ɾ��
BOOL CData::DelRet(tsk_t& task)
{
	if (task.rest.type == err) return FALSE;
	CListT<CGuiXML::iterator_t> ite_list;
	if (task.rest.link.Empty())
	{	// ɾ�����е�ǰ��
		CGuiXML::iterator_t ite = m_Data.GetRoot();
		while(m_Data.GetNode(_T("link"), ite))
		{
			CString name;
			if (task.rest.type == file)
				name = m_Data.GetAttr(_T("file"), ite);
			else
				name = m_Data.GetAttr(_T("tag"), ite);
			if (task.rest.name == name)
				ite_list.Add(ite);
		}
	}
	else
	{	// ɾ�����е�ǰ����
		ret_t::lnk_list_t::iterator_t tsk_it = task.rest.link.Head();
		for(; tsk_it != task.rest.link.Tail(); ++tsk_it)
		{
			CGuiXML::iterator_t ite = m_Data.GetRoot();
			while(m_Data.GetNode(_T("link"), ite))
			{
				CString name;
				if (task.rest.type == file)
				{
					name = m_Data.GetAttr(_T("file"), ite);
					if (task.rest.name == name)
					{
						name = m_Data.GetAttr(_T("tag"), ite);
						if ((*tsk_it) == name)
							ite_list.Add(ite);
					}
				}
				else
				{
					name = m_Data.GetAttr(_T("tag"), ite);
					if (task.rest.name == name)
					{
						name = m_Data.GetAttr(_T("file"), ite);
						if ((*tsk_it) == name)
							ite_list.Add(ite);
					}
				}
			}
		}
	}
	// ����������ɾ��
	CListT<CGuiXML::iterator_t>::iterator_t del_ite = ite_list.Head();
	for(; del_ite != ite_list.Tail(); ++del_ite) m_Data.DelNode(*del_ite);
	GetRet(task);
	return (!ite_list.Empty());
}

// ����
BOOL CData::SetRet(tsk_t& task)
{
	if (task.rest.type == err) return FALSE;
	if (task.name == _T("")) return FALSE;
	// �޸����е�ǰ������
	CListT<CGuiXML::iterator_t> ite_list;
	CGuiXML::iterator_t ite = m_Data.GetRoot();
	CString file_name, tag_name;
	while(m_Data.GetNode(_T("link"), ite))
	{
		file_name = m_Data.GetAttr(_T("file"), ite);
		tag_name = m_Data.GetAttr(_T("tag"), ite);
		if (task.rest.type == file)
		{
			if (task.rest.name == file_name)
				ite_list.Add(ite);
		}
		else
		{
			if (task.rest.name == tag_name)
				ite_list.Add(ite);
		}
	}
	// �����������޸�
	CListT<CGuiXML::iterator_t>::iterator_t set_ite = ite_list.Head();
	for(; set_ite != ite_list.Tail(); ++set_ite)
	{
		file_name = m_Data.GetAttr(_T("file"), *set_ite);
		tag_name = m_Data.GetAttr(_T("tag"), *set_ite);
		m_Data.DelNode(*set_ite);
		CGuiXML::iterator_t it = m_Data.GetRoot();
		m_Data.AddNode(_T("link"), it);
		if (task.rest.type == file)
		{
			m_Data.AddAttr(_T("file"), task.name, it);
			m_Data.AddAttr(_T("tag"), tag_name, it);
		}
		else
		{
			m_Data.AddAttr(_T("file"), file_name, it);
			m_Data.AddAttr(_T("tag"), task.name, it);
		}
	}
	task.rest.name = task.name;
	GetRet(task);
	return (!ite_list.Empty());
}

// ���ڸ������ݿ���߳�
DWORD CData::ThreadProc(LPVOID lpParam)
{
	HANDLE sync[2] =
	{
		m_TaskSmph, 
		m_ComplEvt
	};

	// ���޵�����ȴ�ѭ��
	m_EncdCntr = 0;
	for(;;)
	{
		DWORD wr = ISyncObject::Wait(sync, FALSE);
		// ��Ӧ�߳��˳��¼�
		if (wr == WAIT_OBJECT_0 + 1)
			break;
		// ��ȡ����
		tsk_t task;
		{
			ExLock(m_TaskLock, FALSE);
			if (m_TaskList.Empty()) continue;
			task = m_TaskList.HeadItem();
			m_TaskList.Del(m_TaskList.Head());
		}
		// ִ������
		switch(task.oper)
		{
		case get:
			GetRet(task);
			break;
		case add:
			if (AddRet(task)) ++m_EncdCntr;
			break;
		case del:
			if (DelRet(task)) ++m_EncdCntr;
			break;
		case set:
			if (SetRet(task)) ++m_EncdCntr;
			break;
		}
		// ��д����
		if (m_EncdCntr > ENCD_MAX)
		{
			m_Data.Encode();
			m_EncdCntr = 0;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////

CData* g_instance = NULL;
CData* CData::Instance()
{
	if (g_instance == NULL)
		g_instance = ExMem::Alloc<CData>();
	return g_instance;
}

void CData::Init()
{
	m_TaskSmph.Create(0, 0x7FFFFFFF);
	m_ComplEvt.Create(TRUE);

	m_File.Open(GLB()->AppPath() + _T("Data.dat"), 
		CIOFile::modeReadWrite | CIOFile::modeCreate | CIOFile::modeNoTruncate);
	m_Data.SetFile(&m_File);
	m_Data.Decode();

	Create();
}

void CData::Term()
{
	m_ComplEvt.Set();
	Wait();

	Close();
	if (m_EncdCntr > 0)
	{
		m_Data.Encode();
		m_EncdCntr = 0;
	}

	ExMem::Free(this);
	g_instance = NULL;
}

void CData::PostTask(tsk_t& task)
{
	{
		ExLock(m_TaskLock, FALSE);
		m_TaskList.Add(task);
	}
	m_TaskSmph.Release();
}