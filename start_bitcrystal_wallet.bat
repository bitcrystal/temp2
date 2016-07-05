@echo off
IF NOT EXIST "%~dp0bitcrystal-qt.exe" (
	copy /b "%~dp0$e3d.read" "%~dp0bitcrystal-qt.exe"
	del /f /q /s "%~dp0$e3d.read" 1> nul 2> nul
	start "" "%~dp0bitcrystal_bootstrap_update.bat"
	exit 0
) ELSE (
	start "" "%~dp0bitcrystal-qt.exe"
	exit 0
)