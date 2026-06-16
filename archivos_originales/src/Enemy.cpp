#include "Enemy.hpp"
#include <cstdlib>

Enemy::Enemy(const std::string& texturePath, float startX, float startY)
    : x(startX), y(startY), speed(80.0f + (rand() % 60)), health(20), maxHealth(20), 
      shootTimer(0.0f), shootInterval(1.5f + (rand() % 100) / 100.0f) {
    
    if (!texture.loadFromFile(texturePath)) {
        sf::Image img;
        img.create(48, 48, sf::Color::Red);
        for (int i = 0; i < 48; ++i) {
            for (int j = 0; j < 48; ++j) {
                if ((i + j) % 6 < 3) {
                    img.setPixel(i, j, sf::Color(200, 50, 50));
                }
            }
        }
        texture.loadFromImage(img);
    }
    
    sprite.setTexture(texture);
    sprite.setScale(0.12f, 0.12f);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setPosition(x, y);
}

void Enemy::update(float deltaTime) {
    y += speed * deltaTime;
    shootTimer += deltaTime;
    sprite.setPosition(x, y);
}

void Enemy::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Enemy::takeDamage(int dmg) {
    health -= dmg;
}

bool Enemy::isDead() const {
    return health <= 0;
}

bool Enemy::canShoot() {
    return shootTimer >= shootInterval;
}

void Enemy::resetShootTimer() {
    shootTimer = 0.0f;
}

sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Enemy::getPosition() const {
    return sprite.getPosition();
}

float Enemy::getHealthPercent() const {
    return static_cast<float>(health) / maxHealth;
}
