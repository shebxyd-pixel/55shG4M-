@echo off
echo ========================================
echo  TOUHOU STYLE SHOOTER - BUILD SCRIPT
echo ========================================
echo.

if not exist "build" mkdir build
cd build

echo Configurando CMake...
cmake .. -G "Visual Studio 17 2022" -A x64 -DSFML_DIR="C:/SFML-2.6.1"

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilando...
    cmake --build . --config Release
    
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo ========================================
        echo  COMPILACION EXITOSA!
        echo ========================================
        echo.
        echo Ejecutable en: build\bin\Release\TouhouStyleShooter.exe
        echo.
        pause
    ) else (
        echo.
        echo ERROR en la compilacion
        pause
    )
) else (
    echo.
    echo ERROR en la configuracion de CMake
    echo Asegurate de tener SFML en C:/SFML-2.6.1 o modifica este script
    pause
)
