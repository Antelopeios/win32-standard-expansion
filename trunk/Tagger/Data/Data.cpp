#include "stdafx.h"
#include "Main/Tagger.h"

//////////////////////////////////////////////////////////////////

// ��ȡ
bool CData::GetRet(tsk_t& task)
{
	bool retn = false;
	ret_list_t::iterator_t dun = m_RestList.Tail();
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
				retn = true;
			}
		}
	}
	if(!retn)
	{	// δ���л���,�������ݿ�
		CGuiXML::iterator_t ite = m_Data.GetRoot();
		switch(task.rest.type)
		{
		case file:
			while(m_Data.GetNode(_T("link"), ite))
			{
				CString name = m_Data.GetAttr(_T("file"), ite);
				if (task.rest.name == name)
				{
					task.rest.link.Add(m_Data.GetAttr(_T("tag"), ite));
					retn = true;
				}
			}
			break;
		case tag:
			while(m_Data.GetNode(_T("link"), ite))
			{
				CString name = m_Data.GetAttr(_T("tag"), ite);
				if (task.rest.name == name)
				{
					task.rest.link.Add(m_Data.GetAttr(_T("file"), ite));
					retn = true;
				}
			}
			break;
		}
		if (retn)
		{	// ����
			task.rest.cntr = 1;
			// д�뻺��
			m_RestList.Add(task.rest);
			if (m_RestList.GetCount() > REST_MAX)
				m_RestList.Del(dun); // �ͷŵ����д������ٵĻ�����
		}
	}
	if (task.call) task.call(task.rest);
	return false;
}

// ����/�޸�
bool CData::AddRet(tsk_t& task)
{
	if (task.rest.type == err) return false;
	if (task.rest.link.Empty()) return false;
	ret_t ret = task.rest;
	task.rest.type = err;
	GetRet(task);
	bool retn = false;
	if (task.rest.type == err)
	{	// ���ݲ�����,����
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
		retn = true;
	}
	else
	{	// �����Ѵ���,�޸�
		ret_t::lnk_list_t::iterator_t ret_it = ret.link.Head();
		for(; ret_it != ret.link.Tail(); ++ret_it)
		{
			ret_t::lnk_list_t::iterator_t tsk_it = task.rest.link.Find(*ret_it);
			if (tsk_it != task.rest.link.Tail()) continue;
			// ����������,����һ�����
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
			retn = true;
		}
		ret_t::lnk_list_t::iterator_t tsk_it = task.rest.link.Head();
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
			retn = true;
		}
	}
	return retn;
}

// ɾ��
bool CData::DelRet(tsk_t& task)
{
	if (task.rest.type == err) return false;
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
	for(; del_ite != ite_list.Tail(); ++del_ite)
		m_Data.DelNode(*del_ite);
	return (!ite_list.Empty());
}

// ����
bool CData::SetRet(tsk_t& task)
{
	if (task.rest.type == err) return false;
	if (task.name == _T("")) return false;
	// �޸����е�ǰ������
	bool retn = false;
	CGuiXML::iterator_t ite = m_Data.GetRoot();
	while(m_Data.GetNode(_T("link"), ite))
	{
		CString file_name, tag_name;
		file_name = m_Data.GetAttr(_T("file"), ite);
		tag_name = m_Data.GetAttr(_T("tag"), ite);
		if (task.rest.type == file)
		{
			if (task.rest.name == file_name)
			{
				m_Data.DelNode(ite);
				CGuiXML::iterator_t it = m_Data.GetRoot();
				m_Data.AddNode(_T("link"), it);
				m_Data.AddAttr(_T("file"), task.name, it);
				m_Data.AddAttr(_T("tag"), tag_name, it);
				retn = true;
			}
		}
		else
		{
			if (task.rest.name == tag_name)
			{
				m_Data.DelNode(ite);
				CGuiXML::iterator_t it = m_Data.GetRoot();
				m_Data.AddNode(_T("link"), it);
				m_Data.AddAttr(_T("file"), file_name, it);
				m_Data.AddAttr(_T("tag"), task.name, it);
				retn = true;
			}
		}
	}
	return retn;
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
		DWORD wr = ISyncObject::Wait(sync, false);
		// ��Ӧ�߳��˳��¼�
		if (wr == WAIT_OBJECT_0 + 1)
			break;
		// ��ȡ����
		tsk_t task;
		{
			ExLock(m_TaskLock, false);
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

CData* CData::Instance()
{
	static CData data;
	return &data;
}

void CData::Init()
{
	m_TaskSmph.Create(0, 0x7FFFFFFF);
	m_ComplEvt.Create(true);

	m_File.Open(GET_GBL()->GetAppPath() + _T("Data.dat"), CIOFile::modeReadWrite | CIOFile::modeCreate);
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
}

void CData::PostTask(tsk_t& task)
{
	{
		ExLock(m_TaskLock, false);
		m_TaskList.Add(task);
	}
	m_TaskSmph.Release();
}