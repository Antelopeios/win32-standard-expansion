#pragma once

//////////////////////////////////////////////////////////////////

class CGuiLoader
{
protected:
	CGC gc;
	CSize m_DefSize;

protected:
	// ������Դ
	void LoadRes();
	// ���ؿؼ�
	void LoadCtl();
	// ���ش���
	void LoadWnd();
	// ��������
	void LinkGui();

public:
	CGuiLoader();

	static CGuiLoader* Instance();

public:
	void Init();
	void Term();

	CSize DefSize();
	int ScrWidth();
};

//////////////////////////////////////////////////////////////////

#define GUI() CGuiLoader::Instance()