# Maintenance Log - Project Overhaul
Date: June 16, 2026

## Overview
This log documents all changes made during the project overhaul, including:
- Creating an optimized C++ project compatible with Visual Studio 2026
- Porting the entire codebase to C# using SFML.Net
- Cleaning up the root directory
- Documenting all changes

---

## Changes Made

### 1. Created `Touhou_Game_C++2026`
- **Action**: Copied all C++ project files from `touhou_game` to a new directory
- **Purpose**: Maintain the existing C++ implementation for compatibility
- **Files Modified/Added**:
  - All files from `touhou_game/` copied to `Touhou_Game_C++2026/`
- **Status**: Complete

### 2. Ported to C# (Created `Touhou_Game_CSharp`)
- **Action**: Complete rewrite from C++/SFML to C#/SFML.Net
- **Files Created**:
  - `Touhou_Game_CSharp/src/Program.cs`: Full port containing:
    - All enumerations (GameState, BulletOwner, BulletType, MenuSelection)
    - Tile struct and all core classes:
      - ResourceManager (texture/font management)
      - Player (movement, shooting, invincibility)
      - Bullet (different types, rotation, movement)
      - BulletPatternGenerator (spiral, ring, aimed, star, wave, random patterns)
      - HUD (score, lives, bombs, FPS display)
      - Menu (main menu with selection)
      - World (open world generation and camera)
      - Game (main game loop, state management, collisions)
  - `Touhou_Game_CSharp/TouhouGameCS.csproj`: .NET 8.0 project file
  - `Touhou_Game_CSharp/assets/textures/`: Copied all textures

### 3. Cleaned Up Root Directory
- **Action**: Moved obsolete/redundant files to `Trash/` folder
- **Files Moved**:
  - CMake build artifacts (`CMakeFiles/`, `TouhouGame.dir/`, `_deps/`, `CMakeCache.txt`)
  - Visual Studio solution files (`TouhouGame.slnx`, `*.vcxproj`, `*.vcxproj.filters`)
  - Old Python/HTML files (`JUGAR.bat`, `JUEGO_HTML.html`, `setup_juego.bat`)
  - Original image assets
  - `.vs/` VS Code temp files
- **Purpose**: Keep root directory clean and organized

### 4. Project Structure
Final directory layout:
```
Pruebas Trae/
├── Touhou_Game_C++2026/    # Optimized C++ version (VS2026 compatible)
├── Touhou_Game_CSharp/     # Full C# port with SFML.Net
├── touhou_game/            # Original C++ project (kept for reference)
├── Trash/                  # Obsolete/redundant files
├── MAINTENANCE_LOG.md      # This file
└── .vscode/                # VS Code config (kept)
```

## Instructions for Use

### C++ Project
1. Navigate to `Touhou_Game_C++2026/`
2. Use CMake to configure and build:
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

### C# Project
1. Navigate to `Touhou_Game_CSharp/`
2. Restore NuGet packages and build:
   ```bash
   dotnet restore
   dotnet build
   dotnet run
   ```

---

## Notes
- The C# port includes all functionality from the C++ version
- Both projects use the same assets
- All obsolete files are preserved in the Trash folder for reference
