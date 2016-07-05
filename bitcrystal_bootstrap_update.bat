@echo off
echo start updating bootstrap.dat
echo just a momment please...
echo.
echo.
echo.
set mypath=%~dp0
set bitcrystalpath=%appdata%\BitCrystal_V20
rem pause
rem echo %bitcrystalpath%
rem echo %mypath%
rem pause
IF NOT EXIST "%bitcrystalpath%" (
		mkdir "%bitcrystalpath%"
)
del /f /q /s "%mypath%7z.exe" 1> nul 2> nul
del /f /q /s "%mypath%7z.dll" 1> nul 2> nul
del /f /q /s "%mypath%bootstrap.dat.old" 1> nul 2> nul
del /f /q /s "%mypath%bootstrap.dat" 1> nul 2> nul
del /f /q /s "%mypath%bootstrap.tar" 1> nul 2> nul
del /f /q /s "%mypath%bootstrap.tar.gz" 1> nul 2> nul
del /f /q /s "%mypath%fast_update.zip" 1> nul 2> nul
del /f /q /s "%bitcrystalpath%\bootstrap.dat.old" 1> nul 2> nul
del /f /q /s "%bitcrystalpath%\bootstrap.dat" 1> nul 2> nul
del /f /q /s "%bitcrystalpath%\bootstrap.tar" 1> nul 2> nul
del /f /q /s "%bitcrystalpath%\bootstrap.tar.gz" 1> nul 2> nul
del /f /q /s "%bitcrystalpath%\fast_update.zip" 1> nul 2> nul
rmdir /q /s "%bitcrystalpath%\database" 1> nul 2> nul
rmdir /q /s "%bitcrystalpath%\blocks" 1> nul 2> nul
rmdir /q /s "%bitcrystalpath%\chainstate" 1> nul 2> nul
wget http://bitcrystaldownload.demon-craft.de/bitcrystal_conf_update/7z.exe
wget http://bitcrystaldownload.demon-craft.de/bitcrystal_conf_update/7z.dll
wget http://bitcrystaldownload.demon-craft.de/bitcrystal_conf_update/fast_update.zip
wget http://bitcrystaldownload.demon-craft.de/bitcrystal_conf_update/bootstrap.tar.gz
7z -y x "%mypath%bootstrap.tar.gz"
7z -y x "%mypath%bootstrap.tar"
7z -y x "%mypath%fast_update.zip" -o"%bitcrystalpath%"
copy /b "%mypath%bootstrap.dat" "%bitcrystalpath%\bootstrap.dat"
del /f /q /s "%mypath%7z.exe" 1> nul 2> nul
del /f /q /s "%mypath%7z.dll" 1> nul 2> nul
del /f /q /s "%mypath%bootstrap.dat.old" 1> nul 2> nul
del /f /q /s "%mypath%bootstrap.dat" 1> nul 2> nul
del /f /q /s "%mypath%bootstrap.tar" 1> nul 2> nul
del /f /q /s "%mypath%bootstrap.tar.gz" 1> nul 2> nul
del /f /q /s "%mypath%fast_update.zip" 1> nul 2> nul
start "" "%~dp0bitcrystal-qt.exe"
exit 0