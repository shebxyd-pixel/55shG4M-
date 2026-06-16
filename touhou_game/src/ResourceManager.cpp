#include "ResourceManager.hpp"
#include <iostream>
#include <cstring>

bool ResourceManager::loadAll() {
    bool success = true;
    
    // Cargar texturas de recursos proporcionados
    if (!textures["bullet_sprites"].loadFromFile("assets/textures/bullet_sprites.jpg")) {
        sf::Image img;
        img.create(512, 512, sf::Color::Transparent);
        textures["bullet_sprites"].loadFromImage(img);
        std::cerr << "Warning: Could not load bullet_sprites.jpg" << std::endl;
    }
    
    if (!textures["window_layout"].loadFromFile("assets/textures/window_layout.jpg")) {
        sf::Image img;
        img.create(256, 256, sf::Color::Blue);
        textures["window_layout"].loadFromImage(img);
        std::cerr << "Warning: Could not load window_layout.jpg" << std::endl;
    }
    
    // Crear texturas placeholder si no están disponibles
    sf::Image playerImg;
    playerImg.create(64, 64, sf::Color::Cyan);
    textures["player"].loadFromImage(playerImg);
    
    sf::Image terrainImg;
    terrainImg.create(128, 128, sf::Color(40, 40, 70));
    textures["terrain"].loadFromImage(terrainImg);
    
    // Cargar fuentes (usar fuentes del sistema)
    #ifdef _WIN32
    if (!fonts["default"].loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Warning: Could not load system font" << std::endl;
    }
    #elif defined(__linux__)
    if (!fonts["default"].loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Warning: Could not load system font" << std::endl;
    }
    #elif defined(__APPLE__)
    if (!fonts["default"].loadFromFile("/System/Library/Fonts/Arial.ttf")) {
        std::cerr << "Warning: Could not load system font" << std::endl;
    }
    #endif
    
    return success;
}

sf::Texture& ResourceManager::getTexture(const std::string& name) {
    return textures.at(name);
}

sf::Font& ResourceManager::getFont(const std::string& name) {
    return fonts.at(name);
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(const std::string& name) {
    return soundBuffers.at(name);
}
