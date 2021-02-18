rem - RELEASE.BAT: LevelMod release
rem - --- use Notepad++ for nice syntax highlighting ---

rem - !!! variables !!! change, if needed !!!

	set sevenz="C:\Program Files\7-Zip\7z.exe"
	set target=LevelMod
	
rem - prepare paths

	cd /d "%~dp0"
	mkdir %target%

rem - copy debug files

	copy d3d8d.dll %target%\d3d8.dll
	%systemroot%\System32\xcopy "%~dp0"Data %target%\Data /E /I /H /Y

rem - pack to zip

	del levelmod_debug.zip
	%sevenz% a -tzip levelmod_debug.zip %target%

rem - remove temp dirs

	rmdir /s /q %target%


rem - prepare paths

	mkdir %target%

rem - copy debug files

	copy d3d8.dll %target%\d3d8.dll
	%systemroot%\System32\xcopy "%~dp0"Data  %target%\Data /E /I /H /Y

rem - pack to zip

	del levelmod.zip
	%sevenz% a -tzip levelmod.zip %target%


rem - remove temp dirs

	rmdir /s /q %target%

	echo "done!"