// GuiExp.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "GuiExp.h"

EXP_BEG

// 这是导出函数的一个示例。
EXP_API IGuiObject* ExGui(LPCTSTR sGuiType, CGC* pGC/* = NULL*/)
{
	return ExDynCreate<IGuiObject>(sGuiType, pGC);
}

EXP_END