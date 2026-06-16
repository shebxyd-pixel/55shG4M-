@echo off
chcp 65001 >nul
echo.
echo ================================================
echo   Touhou Style Bullet Hell - Build Script
echo ================================================
echo.

if not exist "build" mkdir build
cd build

echo Detectando compilador...
echo.

:: Probamos NMake (si tiene Build Tools)
echo [1/5] Probando NMake Makefiles...
cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilando con NMake...
    nmake
    if %ERRORLEVEL% EQU 0 goto SUCCESS
)

:: Probamos MinGW Makefiles
echo.
echo [2/5] Probando MinGW Makefiles...
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilando con MinGW...
    mingw32-make
    if %ERRORLEVEL% EQU 0 goto SUCCESS
)

:: Probamos Visual Studio 17 2022
echo.
echo [3/5] Probando Visual Studio 17 2022...
cmake .. -G "Visual Studio 17 2022" -A x64
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilando con VS 2022...
    cmake --build . --config Release
    if %ERRORLEVEL% EQU 0 goto SUCCESS
)

:: Probamos Visual Studio 16 2019
echo.
echo [4/5] Probando Visual Studio 16 2019...
cmake .. -G "Visual Studio 16 2019" -A x64
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilando con VS 2019...
    cmake --build . --config Release
    if %ERRORLEVEL% EQU 0 goto SUCCESS
)

:: Probamos Ninja
echo.
echo [5/5] Probando Ninja...
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilando con Ninja...
    ninja
    if %ERRORLEVEL% EQU 0 goto SUCCESS
)

:: Si nada funciona
echo.
echo.
echo ================================================
echo   FALLO EN LA COMPILACION!
echo ================================================
echo.
echo Necesitas uno de estos compiladores:
echo   1. Visual Studio Build Tools 2019/2022
echo   2. MinGW-w64
echo   3. Visual Studio Community 2019/2022
echo.
echo Descarga Visual Studio Build Tools desde:
echo https://visualstudio.microsoft.com/es/downloads/
echo.
echo Instala "Desktop development with C++"
echo.
pause
exit /b 1

:SUCCESS
echo.
echo.
echo ================================================
echo   COMPILACION EXITOSA!
echo ================================================
echo.
echo Ejecutable en: build/bin/TouhouGame.exe
echo.
pause
