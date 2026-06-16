#ifndef NPC_HPP
#define NPC_HPP

#include <SFML/Graphics.hpp>

class NPC {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Text message;
    sf::Font font;
    float x;
    float y;
    bool messageVisible;
    float messageTimer;
    
public:
    NPC(const std::string& texturePath, float startX, float startY, const std::string& msg);
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void showMessage();
    
    sf::FloatRect getBounds() const;
};

#endif
