cmake_minimum_required(VERSION 3.16)
project(TouhouGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opciones de compilación
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(USE_SYSTEM_SFML "Use system SFML installation" OFF)

# Directorios de salida
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Incluir FetchContent para descargar dependencias
include(FetchContent)

# SFML 2.6.x - Descarga automática si no está en el sistema
if(NOT USE_SYSTEM_SFML)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
        GIT_SHALLOW TRUE
    )
    set(SFML_BUILD_AUDIO TRUE)
    set(SFML_BUILD_GRAPHICS TRUE)
    set(SFML_BUILD_WINDOW TRUE)
    set(SFML_BUILD_SYSTEM TRUE)
    set(SFML_BUILD_NETWORK FALSE)
    FetchContent_MakeAvailable(SFML)
endif()

# Encontrar SFML
if(USE_SYSTEM_SFML)
    find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)
endif()

# Archivos de código fuente
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

file(GLOB_RECURSE HEADERS
    "include/*.hpp"
)

# Ejecutable principal
add_executable(TouhouGame
    ${SOURCES}
    ${HEADERS}
)

# Directorios de inclusión
target_include_directories(TouhouGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Enlazar bibliotecas
target_link_libraries(TouhouGame PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

# Copiar activos a la carpeta de compilación
add_custom_command(TARGET TouhouGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/
        ${CMAKE_BINARY_DIR}/bin/assets/
)

# Configuración específica de Windows
if(WIN32)
    set_target_properties(TouhouGame PROPERTIES
        WIN32_EXECUTABLE $<CONFIG:Release>
    )
    # Copiar DLLs de SFML en Windows
    if(NOT USE_SYSTEM_SFML)
        add_custom_command(TARGET TouhouGame POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                $<TARGET_FILE_DIR:sfml-graphics>/../
                $<TARGET_FILE_DIR:TouhouGame>/
        )
    endif()
endif()

# Optimización para Release
target_compile_options(TouhouGame PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/O2 /Oi /Ot /GL>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-O3 -march=native>
)

# Avisos
if(MSVC)
    target_compile_options(TouhouGame PRIVATE /W4)
else()
    target_compile_options(TouhouGame PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Mensaje de finalización
message(STATUS "Touhou Project Style Game - CMake Build Configuration Complete!")
message(STATUS "Build Directory: ${CMAKE_BINARY_DIR}")
