@echo off

for /r %%a in (.) do (
set list=%%a
setlocal enabledelayedexpansion
if "!list:~-8,6!"==")Build" rd /q /s "!list:~0,-2!"
endlocal
)

del /f /q /s /a *.obj *.obj.cache *.#?? *.brow_pch *.pch *.tds *.ilc *.ild *.ilf *.ils *.drc *.cbproj.local *.ncb *.user *.idb *.ilk *.pch *.pdb *.sbr *.bsc *.aps *.log *.tmp *.~?~ *.~??~ *.res *.dep *.manifest *.suo