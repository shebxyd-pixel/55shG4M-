@echo off
setlocal enabledelayedexpansion
cls
echo ================================================================================
echo                   TOUHOU STYLE SHOOTER - INSTALADOR AVANZADO
echo ================================================================================
echo.

:: Variables
set PYTHON_CMD=
set PIP_CMD=
set ERROR=0
set STEP=1

:: ==============================================
:: PASO 1: Detectar Python
:: ==============================================
echo [PASO %STEP%] Buscando Python en tu sistema...
set /a STEP+=1

:: Probamos "python" primero
python --version >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Encontrado Python con el comando 'python'
    set PYTHON_CMD=python
    goto CHECK_PIP
)

:: Probamos "py" (launcher de Python para Windows)
py --version >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Encontrado Python con el comando 'py'
    set PYTHON_CMD=py
    goto CHECK_PIP
)

:: Si no encontramos Python
echo.
echo [ERROR] No se encontro Python instalado en tu sistema!
echo.
echo Por favor sigue estos pasos:
echo 1. Abre https://www.python.org/downloads/
echo 2. Descarga la version mas reciente de Python (3.10 o superior)
echo 3. Durante la instalacion, **Muy Importante**: marca la casilla que dice:
echo    "Add Python to PATH"
echo 4. Vuelve a ejecutar este instalador
echo.
pause
exit /b 1

:CHECK_PIP
:: ==============================================
:: PASO 2: Verificar PIP
:: ==============================================
echo.
echo [PASO %STEP%] Verificando PIP...
set /a STEP+=1

%PYTHON_CMD% -m pip --version >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] PIP esta funcionando correctamente
    set PIP_CMD=%PYTHON_CMD% -m pip
    goto INSTALL_PYGAME
)

echo [ERROR] PIP no esta disponible o no funciona correctamente
echo.
pause
exit /b 2

:INSTALL_PYGAME
:: ==============================================
:: PASO 3: Instalar Pygame (metodos multiples)
:: ==============================================
echo.
echo [PASO %STEP%] Instalando Pygame...
set /a STEP+=1

:: METODO 1: Instalar wheel precompilado oficial (mas facil)
echo.
echo [INTENTO 1] Instalando version precompilada de Pygame...
%PIP_CMD% install pygame --only-binary :all: --user
if %ERRORLEVEL% EQU 0 (
    goto VERIFY_PYGAME
)

:: METODO 2: Intentar version anterior estable
echo.
echo [INTENTO 2] Fallo. Intentando version estable anterior...
%PIP_CMD% install pygame==2.5.2 --only-binary :all: --user
if %ERRORLEVEL% EQU 0 (
    goto VERIFY_PYGAME
)

:: METODO 3: Actualizar pip y setuptools y volver a intentar
echo.
echo [INTENTO 3] Actualizando herramientas de instalacion...
%PIP_CMD% install --upgrade pip setuptools wheel --user
if %ERRORLEVEL% EQU 0 (
    echo [OK] Herramientas actualizadas. Reintentando...
    %PIP_CMD% install pygame --only-binary :all: --user
    if %ERRORLEVEL% EQU 0 (
        goto VERIFY_PYGAME
    )
)

:: Si todos los metodos fallan, dar instrucciones detalladas
echo.
echo ================================================================================
echo [ERROR] No se pudo instalar Pygame automaticamente!
echo ================================================================================
echo.
echo OPCION 1: Usa el JUEGO EN HTML (sin necesidad de instalar NADA!)
echo   Abre el archivo "JUEGO_HTML.html" con cualquier navegador web (Chrome, Edge, etc.)
echo.
echo OPCION 2: Instalar manualmente:
echo.
echo PASOS PARA OPCION 2:
echo 1. Abre "Visual Studio Installer" (o descargalo desde:
echo    https://visualstudio.microsoft.com/es/visual-cpp-build-tools/^)
echo 2. Instala "Desktop development with C++" (marca esa casilla)
echo 3. Vuelve a ejecutar este instalador
echo.
echo O bien, ejecuta esto en una ventana CMD como Administrador:
echo   %PIP_CMD% install pygame
echo.
pause
exit /b 3

:VERIFY_PYGAME
:: ==============================================
:: PASO 4: Verificar que Pygame se instalo bien
:: ==============================================
echo.
echo [PASO %STEP%] Verificando instalacion de Pygame...
set /a STEP+=1

%PYTHON_CMD% -c "import pygame; print(f'[OK] Pygame {pygame.version.ver} instalado correctamente!')" >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    %PYTHON_CMD% -c "import pygame; print(f'[OK] Pygame {pygame.version.ver} instalado correctamente!')"
    goto FIN_OK
)

echo [ERROR] No se pudo verificar la instalacion de Pygame!
echo.
echo Pero puedes usar el juego en HTML: abre JUEGO_HTML.html con tu navegador!
echo.
pause
exit /b 4

:FIN_OK
:: ==============================================
:: TODO LISTO!
:: ==============================================
echo.
echo ================================================================================
echo                          TODO INSTALADO CORRECTAMENTE!
echo ================================================================================
echo.
echo Ahora puedes ejecutar JUGAR.bat para empezar a jugar!
echo.
echo Tambien puedes usar el juego en HTML: abre JUEGO_HTML.html con tu navegador!
echo.
echo Presiona cualquier tecla para cerrar este instalador...
pause >nul
exit /b 0
