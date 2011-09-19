#pragma once

//////////////////////////////////////////////////////////////////

typedef CGuiManagerT<IGuiBoard> CWndManager;
typedef CGuiManagerT<IGuiCtrl> CCtlManager;
typedef CGuiManagerT<CImage> CImgManager;
typedef CGuiManagerT<CText> CTxtManager;

#define REG_WND(name, inf) CWndManager::Reg(_T(#name), inf)
#define GET_WND(name) CWndManager::Get(_T(#name))

#define REG_CTL(name, inf) CCtlManager::Reg(_T(#name), inf)
#define GET_CTL(name) CCtlManager::Get(_T(#name))

#define REG_IMG(name, inf) CImgManager::Reg(_T(#name), inf)
#define GET_IMG(name) CImgManager::Get(_T(#name))

#define REG_TXT(name, inf) CTxtManager::Reg(_T(#name), inf)
#define GET_TXT(name) CTxtManager::Get(_T(#name))
