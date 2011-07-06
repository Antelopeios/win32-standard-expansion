// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "GuiExp.h"

#ifdef _DEBUG
#pragma comment(lib, "../../_Output/Debug/ImgExp.lib")
#else
#pragma comment(lib, "../../_Output/Release/ImgExp.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "../../_Output/Debug/GuiExp.lib")
#else
#pragma comment(lib, "../../_Output/Release/GuiExp.lib")
#endif