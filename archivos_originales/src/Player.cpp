#include "Player.hpp"
#include <iostream>

Player::Player(const std::string& texturePath, float startX, float startY) 
    : speed(300.0f), x(startX), y(startY), shootCooldown(0.15f), currentShootCooldown(0.0f) {
    
    if (!texture.loadFromFile(texturePath)) {
        sf::Image img;
        img.create(64, 64, sf::Color(100, 200, 255));
        for (int i = 0; i < 64; ++i) {
            for (int j = 0; j < 64; ++j) {
                if ((i + j) % 8 < 4) {
                    img.setPixel(i, j, sf::Color(150, 220, 255));
                }
            }
        }
        texture.loadFromImage(img);
    }
    
    sprite.setTexture(texture);
    sprite.setScale(0.15f, 0.15f);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setPosition(x, y);
}

void Player::update(float deltaTime, const sf::RenderWindow& window) {
    if (currentShootCooldown > 0) {
        currentShootCooldown -= deltaTime;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        x -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        x += speed * deltaTime;
    }
    
    if (x < 40) x = 40;
    if (x > window.getSize().x - 40) x = window.getSize().x - 40;
    
    sprite.setPosition(x, y);
}

void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Player::canShoot() {
    return currentShootCooldown <= 0;
}

void Player::resetShootCooldown() {
    currentShootCooldown = shootCooldown;
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}
