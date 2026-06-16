@echo off
cls
echo ================================================================================
echo                     INICIANDO TOUHOU STYLE SHOOTER
echo ================================================================================
echo.

:: Detectar Python
set PYTHON_CMD=
python --version >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    set PYTHON_CMD=python
) else (
    py --version >nul 2>&1
    if %ERRORLEVEL% EQU 0 (
        set PYTHON_CMD=py
    ) else (
        echo [ERROR] No se encontro Python! Ejecuta setup_juego.bat primero.
        echo.
        pause
        exit /b 1
    )
)

:: Verificar que main.py exista
if not exist "main.py" (
    echo [ERROR] No se encontro el archivo main.py!
    echo.
    pause
    exit /b 2
)

:: Ejecutar el juego
echo [OK] Iniciando el juego...
echo.
%PYTHON_CMD% main.py

:: Si falla
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ================================================================================
    echo [ERROR] El juego se cerro con un error!
    echo ================================================================================
    echo.
    echo Asegurate de:
    echo 1. Haber ejecutado setup_juego.bat primero
    echo 2. Tener todas las dependencias instaladas
    echo.
    pause
)
