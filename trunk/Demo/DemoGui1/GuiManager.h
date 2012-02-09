#pragma once

//////////////////////////////////////////////////////////////////

#define REG_WND(name, inf) ExReg(IGuiBoard, _T(#name), inf)
#define GUI_WND(name) ExGet(IGuiBoard, _T(#name))

#define REG_CTL(name, inf) ExReg(IGuiCtrl, _T(#name), inf)
#define GUI_CTL(name) ExGet(IGuiCtrl, _T(#name))

#define REG_IMG(name, inf) ExReg(CImage, _T(#name), inf)
#define GUI_IMG(name) ExGet(CImage, _T(#name))

#define REG_FNT(name, inf) ExReg(CText, _T(#name), inf)
#define GUI_FNT(name) ExGet(CText, _T(#name))