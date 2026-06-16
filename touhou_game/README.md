# Touhou Style Bullet Hell - C++ / SFML

Un juego de disparos vertical estilo Touhou Project con:
- Pantalla completa
- Mundo abierto a gran escala
- Patrones de balas estilo danmaku
- Arquitectura modular
- Build system con CMake

## Características

### Visuales
- Zonas separadas: jugador a la izquierda, combate a la derecha
- Estilo Touhou clásico
- Sprites y recursos incluidos
- 60 FPS garantizados

### Patrones de Balas
- Espirales
- Anillos
- Estrellas
- Disparos dirigidos
- Ondas

### Controles
- **WASD o flechas**: Mover jugador
- **Espacio**: Disparar
- **ESC**: Pausar / Menú
- **Flechas arriba/abajo**: Navegar menús

## Compilación y Ejecución

### Windows
Ejecutar el archivo `build.bat` o manualmente:
```bash
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### Linux
```bash
cd build
cmake ..
make
```

### macOS
```bash
cd build
cmake ..
make
```

## Estructura del Proyecto

```
touhou_game/
├── CMakeLists.txt
├── README.md
├── include/            # Archivos de cabecera
│   ├── Game.hpp
│   ├── ResourceManager.hpp
│   ├── Player.hpp
│   ├── Bullet.hpp
│   ├── World.hpp
│   ├── HUD.hpp
│   ├── Menu.hpp
│   └── BulletPattern.hpp
├── src/                # Código fuente
│   ├── main.cpp
│   ├── Game.cpp
│   ├── ResourceManager.cpp
│   ├── Player.cpp
│   ├── Bullet.cpp
│   ├── World.cpp
│   ├── HUD.cpp
│   ├── Menu.cpp
│   └── BulletPattern.cpp
├── assets/
│   └── textures/       # Recursos gráficos
│       ├── bullet_sprites.jpg
│       └── window_layout.jpg
├── data/               # Datos guardados
└── build/              # Archivos de compilación
```

## Dependencias
- **SFML 2.6.x**: Automáticamente descargado por CMake
- **C++20**: Compilador compatible
- **CMake 3.16+**: Build system

## Créditos
Inspirado en la franquicia Touhou Project de Team Shanghai Alice.
