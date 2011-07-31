#pragma once

//////////////////////////////////////////////////////////////////

typedef CGuiManagerT<IGuiBoard> CWndManager;
typedef CGuiManagerT<IGuiCtrl> CCtlManager;
typedef CGuiManagerT<CImage> CImgManager;

#define REG_WND(name, inf) CWndManager::Reg(_T(#name), inf)
#define GUI_WND(name) CWndManager::Get(_T(#name))

#define REG_CTL(name, inf) CCtlManager::Reg(_T(#name), inf)
#define GUI_CTL(name) CCtlManager::Get(_T(#name))

#define REG_IMG(name, inf) CImgManager::Reg(_T(#name), inf)
#define GUI_IMG(name) CImgManager::Get(_T(#name))
