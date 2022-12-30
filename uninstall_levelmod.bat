echo "THPS3 LevelMod Uninstall"

@ECHO OFF

:choice
set /P c=Are you sure you want to uninstall LevelMod [Y/N]?
if /I "%c%" EQU "Y" goto :unins
goto :choice

:unins

rmdir /s /q "%~dp0\data\scripts\levelmod"
del "%~dp0\data\scripts\qdir_lm.txt"
del "%~dp0\LevelMod.ini"
del "%~dp0\d3d8.dll"
del "%~dp0\levelmod_readme.txt"
del "%~dp0\uninstall_levelmod.bat"

:choice
pause