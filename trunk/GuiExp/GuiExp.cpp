// GuiExp.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "GuiExp.h"

EXP_BEG

// ���ǵ���������һ��ʾ����
EXP_API IGuiObject* ExGui(LPCTSTR sGuiType, CGC* pGC/* = NULL*/)
{
	return ExDynCreate<IGuiObject>(sGuiType, pGC);
}

EXP_END