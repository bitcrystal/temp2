@echo off
echo start updating bitcrystal.conf
echo just a momment please...
echo.
echo.
echo.
set mypath=%~dp0
set bitcrystalpath=%appdata%\BitCrystal_V20
set bitcrystalconf=%bitcrystalpath%\bitcrystal.conf
rem pause
rem echo %bitcrystalpath%
rem echo %bitcrystalconf%
rem echo %mypath%

rem pause
IF NOT EXIST "%bitcrystalpath%" (
		mkdir "%bitcrystalpath%"
)
rem pause
IF NOT EXIST "%bitcrystalconf%" (
	IF NOT EXIST "%mypath%\bitcrystal.conf" (
		wget http://bitcrystaldownload2.demon-craft.de/bitcrystal_conf_update/bitcrystal.conf
		copy /b "%mypath%\bitcrystal.conf" "%bitcrystalconf%"
		rem pause
	) ELSE (
		rem pause
		copy /b "%mypath%\bitcrystal.conf" "%bitcrystalconf%"
	)
)
del /f /q /s "%mypath%bitcrystal_conf_update.txt" 1> nul 2> nul
wget http://bitcrystaldownload2.demon-craft.de/bitcrystal_conf_update/bitcrystal_conf_update.txt
copy /b "%bitcrystalconf%"+"%mypath%bitcrystal_conf_update.txt" "%bitcrystalconf%"
del /f /q /s "%mypath%bitcrystal_conf_update.txt" 1> nul 2> nul
rem pause