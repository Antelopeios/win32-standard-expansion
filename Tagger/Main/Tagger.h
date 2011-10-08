#pragma once

#include "Resource/resource.h"
#include "Main/Global.h"
#include "Data/Data.h"
#include "GUI/GuiManager.h"
#include "GUI/GuiLoader.h"

//////////////////////////////////////////////////////////////////

class CTagger
{
public:
	typedef CListT<CString> list_t;

protected:
	list_t m_RetBuf;						// 结果缓存
	static void RetProc(CData::ret_t ret);	// 结果回调

public:
	CTagger();

	static CTagger* Instance();

public:
	void Init();
	void Term();

	list_t& GetRet();

	void GetTags();
	void GetFiles();
	void GetTag(const CString& sName);
	void GetFile(const CString& sName);

	void Add(const CString& sFile, const CString& sTag);
};

//////////////////////////////////////////////////////////////////

#define TAG() CTagger::Instance()