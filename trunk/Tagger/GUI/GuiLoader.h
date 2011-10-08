#pragma once

//////////////////////////////////////////////////////////////////

class CGuiLoader
{
protected:
	CGC gc;
	CSize m_DefSize;

#ifdef	_CONSOLE
	CEvent m_Msg;
#endif/*_CONSOLE*/

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

	// ���»�ȡ����
	void Refresh();
};

//////////////////////////////////////////////////////////////////

#define GUI() CGuiLoader::Instance()