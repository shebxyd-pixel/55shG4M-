#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
    float x;
    float y;
    float shootCooldown;
    float currentShootCooldown;
    
public:
    Player(const std::string& texturePath, float startX, float startY);
    
    void update(float deltaTime, const sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    bool canShoot();
    void resetShootCooldown();
    
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    float getX() const { return x; }
};

#endif
