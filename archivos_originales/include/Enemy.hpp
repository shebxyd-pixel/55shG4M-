#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>

class Enemy {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float x;
    float y;
    float speed;
    int health;
    int maxHealth;
    float shootTimer;
    float shootInterval;
    
public:
    Enemy(const std::string& texturePath, float startX, float startY);
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void takeDamage(int dmg);
    bool isDead() const;
    bool canShoot();
    void resetShootTimer();
    
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    float getHealthPercent() const;
};

#endif
