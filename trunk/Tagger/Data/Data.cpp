#include "stdafx.h"
#include "Main/Tagger.h"

//////////////////////////////////////////////////////////////////

// ���ڸ������ݿ���߳�
DWORD CData::ThreadProc()
{
	HANDLE sync[2] =
	{
		m_TaskSmph, 
		m_ComplEvt
	};

	// ���޵�����ȴ�ѭ��
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
		ret_t rest;
		switch(task.type)
		{
		case file:
			for(ret_list_t::iterator_t ite = m_RestList.Head(); ite != m_RestList.Tail(); ++ite)
			{
				if ((*ite).type != task.type) continue;
				//if ((*ite).name == task.name)
			}
			break;
		case tag:
			break;
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
}

void CData::Term()
{
	m_ComplEvt.Set();
	Wait();
	m_Data.Encode();
}