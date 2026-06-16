#include "Bullet.hpp"

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f vel, BulletOwner owner, BulletType type)
    : position(pos), velocity(vel), owner(owner), type(type), rotation(0.0f), rotationSpeed(0.0f) {
    
    sf::Color bulletColor;
    float radius;
    
    switch (type) {
        case BulletType::Small:
            radius = 6.0f;
            bulletColor = owner == BulletOwner::Player ? sf::Color::Cyan : sf::Color(255, 100, 100);
            break;
        case BulletType::Medium:
            radius = 10.0f;
            bulletColor = owner == BulletOwner::Player ? sf::Color(0, 200, 255) : sf::Color(255, 150, 50);
            break;
        case BulletType::Large:
            radius = 15.0f;
            bulletColor = owner == BulletOwner::Player ? sf::Color(0, 150, 255) : sf::Color(200, 50, 200);
            break;
        case BulletType::Star:
            radius = 12.0f;
            bulletColor = sf::Color::Yellow;
            break;
        case BulletType::Heart:
            radius = 14.0f;
            bulletColor = sf::Color(255, 100, 150);
            break;
        case BulletType::Ring:
            radius = 18.0f;
            bulletColor = sf::Color(150, 255, 100);
            break;
        case BulletType::Arrow:
            radius = 10.0f;
            bulletColor = sf::Color(100, 100, 255);
            break;
        case BulletType::Butterfly:
            radius = 14.0f;
            bulletColor = sf::Color(150, 100, 255);
            break;
    }
    
    static sf::CircleShape shape;
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setFillColor(bulletColor);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(1);
    
    sprite = sf::Sprite();
    sf::RenderTexture rt;
    rt.create(static_cast<unsigned int>(radius * 2 + 4), static_cast<unsigned int>(radius * 2 + 4));
    rt.clear(sf::Color::Transparent);
    shape.setPosition(radius + 2, radius + 2);
    rt.draw(shape);
    rt.display();
    
    static sf::Texture tex;
    tex = rt.getTexture();
    sprite.setTexture(tex);
    sprite.setOrigin(radius + 2, radius + 2);
    sprite.setPosition(position);
    
    rotationSpeed = owner == BulletOwner::Player ? 0.0f : static_cast<float>((rand() % 200) - 100);
}

void Bullet::update(sf::Time deltaTime) {
    float dt = deltaTime.asSeconds();
    position += velocity * dt;
    rotation += rotationSpeed * dt;
    sprite.setPosition(position);
    sprite.setRotation(rotation);
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Bullet::getBounds() const {
    return sprite.getGlobalBounds();
}

bool Bullet::isOffScreen(const sf::RenderWindow& window) const {
    auto size = window.getSize();
    return position.x < -50 || position.x > size.x + 50 ||
           position.y < -50 || position.y > size.y + 50;
}
