rem - --- RELEASE.BAT: LevelMod release
rem - --- use Notepad++ for nice syntax highlighting ---

rem - !!! variables !!! change, if needed !!!

	set sevenz="C:\Program Files\7-Zip\7z.exe"
	set target=release_temp

rem - prepare paths

	cd /d "%~dp0"
	mkdir %target%

rem - copy files

	copy "LevelMod\d3d8.dll" "%target%\d3d8.dll"
	copy "README.MD" "%target%\LevelMod_ReadMe.txt"
	copy "uninstall_levelmod.bat" "%target%\uninstall_levelmod.bat"
	%systemroot%\System32\xcopy "%~dp0\LevelMod\Data\*.qb" "%target%\Data" /E /I /H /Y
	%systemroot%\System32\xcopy "%~dp0\LevelMod\Data\*.txt" "%target%\Data" /E /I /H /Y

rem - pack to zip

	del levelmod_release.zip
	%sevenz% a -tzip "levelmod_release.zip" ".\%target%\*"

rem - remove temp dirs

	rmdir /s /q %target%

	echo "done!"