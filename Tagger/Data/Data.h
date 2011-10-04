#pragma once

//////////////////////////////////////////////////////////////////

class CData : public IThread
{
public:
	// ���ͱ��
	enum oper_t { get, add, del, set };
	enum type_t { err, file, tag };

	// �������
	struct ret_t
	{
		type_t type;
		CString name;
		int cntr; // ������

		typedef CListT<CString> lnk_list_t;
		lnk_list_t link;

		ret_t()
			: type(err)
			, cntr(0)
		{}
	};

	// ����ص�
	typedef void (*ret_call_t)(ret_t);

	// �������
	typedef struct tsk_t
	{
		oper_t oper;
		ret_t rest;
		CString name;	// �����õĸ�����
		ret_call_t call;

		tsk_t()
			: oper(get)
			, call(NULL)
		{}
	};

protected:
	CIOFile m_File;
	CGuiXML m_Data;

	CSemaphore m_TaskSmph;
	CEvent m_ComplEvt;

	typedef CListT<ret_t> ret_list_t;
	ret_list_t m_RestList;
	static const int REST_MAX = 10;		// �ڴ����ݻ�������������
	static const int ENCD_MAX = 100;	// 100��д���������д����һ��
	int m_EncdCntr;

	typedef CListT<tsk_t> tsk_list_t;
	tsk_list_t m_TaskList;
	CMutex m_TaskLock;

protected:
	bool GetRet(tsk_t& task);	// ��ȡ
	bool AddRet(tsk_t& task);	// ���/�޸�
	bool DelRet(tsk_t& task);	// ɾ��
	bool SetRet(tsk_t& task);	// ����
	DWORD ThreadProc(LPVOID lpParam);

public:
	static CData* Instance();

public:
	void Init();
	void Term();
	void PostTask(tsk_t& task);
};

//////////////////////////////////////////////////////////////////

#define DAT() CData::Instance()