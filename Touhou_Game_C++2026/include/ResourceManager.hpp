#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>

class ResourceManager {
public:
    ResourceManager() = default;
    ~ResourceManager() = default;
    
    bool loadAll();
    sf::Texture& getTexture(const std::string& name);
    sf::Font& getFont(const std::string& name);
    sf::SoundBuffer& getSoundBuffer(const std::string& name);
    
private:
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Font> fonts;
    std::map<std::string, sf::SoundBuffer> soundBuffers;
};
