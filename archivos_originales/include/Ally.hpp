#ifndef ALLY_HPP
#define ALLY_HPP

#include <SFML/Graphics.hpp>

class Ally {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float x;
    float y;
    float fadeTimer;
    bool active;
    bool collected;
    
public:
    Ally(const std::string& texturePath, float startX, float startY);
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void collect();
    bool isActive() const;
    bool isCollected() const;
    
    sf::FloatRect getBounds() const;
};

#endif
