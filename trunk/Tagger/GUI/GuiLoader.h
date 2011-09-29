#pragma once

//////////////////////////////////////////////////////////////////

class CGuiLoader
{
protected:
	CGC gc;
	CSize m_DefSize;

protected:
	// 加载资源
	void LoadRes();
	// 加载控件
	void LoadCtl();
	// 加载窗口
	void LoadWnd();
	// 关联对象
	void LinkGui();

public:
	CGuiLoader();

	static CGuiLoader* Instance();

public:
	void Init();
	void Term();

	CSize DefSize();
	int ScrWidth();

	// 重新获取数据
	void Refresh();
};

//////////////////////////////////////////////////////////////////

#define GUI() CGuiLoader::Instance()