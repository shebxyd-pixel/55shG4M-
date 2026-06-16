#include "Bullet.hpp"

Bullet::Bullet(float x, float y, float vx, float vy, BulletOwner owner, int dmg)
    : velocity(vx, vy), owner(owner), damage(dmg), active(true) {
    
    shape.setRadius(owner == BulletOwner::Player ? 6.0f : 8.0f);
    shape.setFillColor(owner == BulletOwner::Player ? sf::Color::Cyan : sf::Color::Magenta);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(x, y);
}

void Bullet::update(float deltaTime) {
    shape.move(velocity * deltaTime);
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(shape);
}

bool Bullet::isOffScreen(const sf::RenderWindow& window) const {
    auto pos = shape.getPosition();
    return pos.y < -50 || pos.y > window.getSize().y + 50 || 
           pos.x < -50 || pos.x > window.getSize().x + 50;
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();
}

BulletOwner Bullet::getOwner() const {
    return owner;
}

int Bullet::getDamage() const {
    return damage;
}

bool Bullet::isActive() const {
    return active;
}

void Bullet::deactivate() {
    active = false;
}
