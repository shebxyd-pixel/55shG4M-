#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    ~Player() = default;
    
    void update(sf::Time deltaTime, const sf::FloatRect& bounds);
    void render(sf::RenderWindow& window);
    void takeDamage();
    
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getShootPosition() const;
    
    bool isInvincible() const { return invincibleTime > 0.0f; }
    int getLives() const { return lives; }
    
private:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    int lives;
    float invincibleTime;
    float invincibleBlinkTime;
    float shootCooldown;
};
