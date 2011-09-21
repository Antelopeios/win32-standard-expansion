#pragma once

//////////////////////////////////////////////////////////////////

class CData : public IThread
{
protected:
	CIOFile m_File;
	CGuiXML m_Data;

	CSemaphore m_TaskSmph;
	CEvent m_ComplEvt;

	// ���ͱ��
	enum type_t { file, tag };

	// �������
	typedef struct _Tsk
	{
		type_t type;
		CString name;

		_Tsk()
			: type(file)
		{}
	} tsk_t;
	typedef CListT<tsk_t> tsk_list_t;
	tsk_list_t m_TaskList;
	CMutex m_TaskLock;

	// �������
	typedef struct _Ret
	{
		type_t type;
		CString name;
		int cntr; // ������

		typedef CListT<CString> lnk_list_t;
		lnk_list_t link;

		_Ret()
			: type(file)
			, cntr(0)
		{}
	} ret_t;
	typedef CListT<ret_t> ret_list_t;
	ret_list_t m_RestList;
	static const DWORD REST_MAX = 10;

protected:
	DWORD ThreadProc();

public:
	static CData* Instance();

public:
	void Init();
	void Term();
};

//////////////////////////////////////////////////////////////////

#define GET_DAT() CData::Instance()