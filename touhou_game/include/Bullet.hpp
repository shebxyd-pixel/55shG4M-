#pragma once
#include <SFML/Graphics.hpp>

enum class BulletOwner {
    Player,
    Enemy,
    Boss
};

enum class BulletType {
    Small,
    Medium,
    Large,
    Star,
    Heart,
    Ring,
    Arrow,
    Butterfly
};

class Bullet {
public:
    Bullet(sf::Vector2f pos, sf::Vector2f vel, BulletOwner owner, BulletType type);
    ~Bullet() = default;
    
    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);
    
    sf::FloatRect getBounds() const;
    bool isOffScreen(const sf::RenderWindow& window) const;
    
    BulletOwner getOwner() const { return owner; }
    
private:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    BulletOwner owner;
    BulletType type;
    float rotation;
    float rotationSpeed;
};
