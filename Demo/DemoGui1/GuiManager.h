#pragma once

//////////////////////////////////////////////////////////////////

#define REG_WND(name, inf) ExReg(_T(#name), inf)
#define GUI_WND(name) ExGet<IGuiWnd>(_T(#name))

#define REG_CTL(name, inf) ExReg(_T(#name), inf)
#define GUI_CTL(name) ExGet<IGuiCtl>(_T(#name))

#define REG_IMG(name, inf) ExReg(_T(#name), inf)
#define GUI_IMG(name) ExGet<CImage>(_T(#name))

#define REG_FNT(name, inf) ExReg(_T(#name), inf)
#define GUI_FNT(name) ExGet<CText>(_T(#name))