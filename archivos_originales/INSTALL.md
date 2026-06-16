# Guía de Instalación - Touhou Style Shooter

## Requisitos

- CMake 3.16 o superior
- Compilador C++17 compatible (Visual Studio 2019+, MinGW, GCC 8+, Clang 8+)
- SFML 2.6.x

## Paso 1: Descargar SFML

1. Ve a https://www.sfml-dev.org/download/sfml/2.6.1/
2. Descarga la versión correspondiente a tu sistema:
   - Windows: SFML 2.6.1 - 32-bit o 64-bit (GCC o Visual Studio)
   - Linux: Instala via paquete: `sudo apt-get install libsfml-dev`
   - macOS: `brew install sfml`

## Paso 2: Configurar y Compilar

### Windows (Visual Studio)

1. Extrae SFML en una carpeta, por ejemplo: `C:\SFML-2.6.1`
2. Abre PowerShell en la carpeta del proyecto
3. Ejecuta:
   ```powershell
   mkdir build
   cd build
   cmake .. -DSFML_DIR="C:/SFML-2.6.1"
   cmake --build . --config Release
   ```
4. El ejecutable estará en `build/bin/Release/TouhouStyleShooter.exe`

### Windows (MinGW)

1. Extrae SFML en una carpeta
2. Abre PowerShell en la carpeta del proyecto
3. Ejecuta:
   ```powershell
   mkdir build
   cd build
   cmake .. -G "MinGW Makefiles" -DSFML_DIR="C:/SFML-2.6.1"
   mingw32-make
   ```

### Linux/macOS

1. Abre terminal en la carpeta del proyecto
2. Ejecuta:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

## Paso 3: Ejecutar

- Navega a la carpeta `build/bin/`
- Ejecuta `TouhouStyleShooter` (o el .exe en Windows)

## Controles

- **Flechas Izquierda/Derecha** o **A/D**: Mover jugador
- **Espacio**: Disparar
- **ESC**: Pausar / Volver al menú
- **Arriba/Abajo**: Navegar por menús
- **Enter**: Seleccionar opción del menú

## Funcionalidades del Juego

### Personajes

1. **Uta Utane (Jugador)**: Controlable horizontalmente, dispara proyectiles, tiene 3 vidas y munición limitada
2. **Momone Momo (Jefe Final)**: Aparece en fase 5 con 3 patrones de ataque únicos
3. **Akita Neru (Aliado)**: Aparece en fase 3, recarga toda la munición al pasar por ella
4. **Megurine Luka (NPC)**: Mensaje "Ya tu sabe, continua por aqui" en posición aleatoria

### Fases

- **Fase 1-4**: Oleadas de enemigos con dificultad creciente
- **Fase 5**: Batalla final contra el jefe

### Puntuación

- Enemigo normal: 100 pts
- Jefe (por impacto): 10 pts
- Siguiente fase: 500 pts
- Victoria final: 5000 pts

## Estructura del Proyecto

```
Pruebas Trae/
├── CMakeLists.txt
├── include/            # Archivos de cabecera
│   ├── Game.hpp
│   ├── Player.hpp
│   ├── Enemy.hpp
│   ├── Boss.hpp
│   ├── Ally.hpp
│   ├── NPC.hpp
│   ├── Bullet.hpp
│   ├── UI.hpp
│   └── DataManager.hpp
├── src/               # Código fuente
│   ├── main.cpp
│   ├── Game.cpp
│   ├── Player.cpp
│   ├── Enemy.cpp
│   ├── Boss.cpp
│   ├── Ally.cpp
│   ├── NPC.cpp
│   ├── UI.cpp
│   └── DataManager.cpp
├── assets/
│   └── sprites/       # Imágenes de personajes
├── data/              # Archivos de puntuaciones (generados automáticamente)
└── INSTALL.md         # Esta guía
```
