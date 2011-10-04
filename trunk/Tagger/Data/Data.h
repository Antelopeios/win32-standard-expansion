#pragma once

//////////////////////////////////////////////////////////////////

class CData : public IThread
{
public:
	// 类型标记
	enum oper_t { get, add, del, set };
	enum type_t { err, file, tag };

	// 结果缓存
	struct ret_t
	{
		type_t type;
		CString name;
		int cntr; // 计数器

		typedef CListT<CString> lnk_list_t;
		lnk_list_t link;

		ret_t()
			: type(err)
			, cntr(0)
		{}
	};

	// 结果回调
	typedef void (*ret_call_t)(ret_t);

	// 任务队列
	typedef struct tsk_t
	{
		oper_t oper;
		ret_t rest;
		CString name;	// 改名用的副名称
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
	static const int REST_MAX = 10;		// 内存数据缓存粒度最大个数
	static const int ENCD_MAX = 100;	// 100次写入操作将回写磁盘一次
	int m_EncdCntr;

	typedef CListT<tsk_t> tsk_list_t;
	tsk_list_t m_TaskList;
	CMutex m_TaskLock;

protected:
	bool GetRet(tsk_t& task);	// 获取
	bool AddRet(tsk_t& task);	// 添加/修改
	bool DelRet(tsk_t& task);	// 删除
	bool SetRet(tsk_t& task);	// 改名
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