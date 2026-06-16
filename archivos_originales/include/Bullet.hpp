#ifndef BULLET_HPP
#define BULLET_HPP

#include <SFML/Graphics.hpp>

enum class BulletOwner {
    Player,
    Enemy
};

class Bullet {
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    BulletOwner owner;
    int damage;
    bool active;
    
public:
    Bullet(float x, float y, float vx, float vy, BulletOwner owner, int dmg);
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    bool isOffScreen(const sf::RenderWindow& window) const;
    
    sf::FloatRect getBounds() const;
    BulletOwner getOwner() const;
    int getDamage() const;
    bool isActive() const;
    void deactivate();
};

#endif
