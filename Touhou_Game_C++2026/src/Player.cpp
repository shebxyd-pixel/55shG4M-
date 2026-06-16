#include "Player.hpp"

Player::Player() : speed(300.0f), lives(3), invincibleTime(0.0f), invincibleBlinkTime(0.0f), shootCooldown(0.0f) {
    sf::Image img;
    img.create(48, 48, sf::Color::Cyan);
    for (int x = 10; x < 38; x++) {
        for (int y = 10; y < 38; y++) {
            img.setPixel(x, y, sf::Color(0, 150, 200));
        }
    }
    
    static sf::Texture tex;
    tex.loadFromImage(img);
    sprite.setTexture(tex);
    sprite.setOrigin(24, 24);
    
    position = sf::Vector2f(200, 500);
    velocity = sf::Vector2f(0, 0);
}

void Player::update(sf::Time deltaTime, const sf::FloatRect& bounds) {
    float dt = deltaTime.asSeconds();
    
    if (invincibleTime > 0.0f) {
        invincibleTime -= dt;
        invincibleBlinkTime += dt;
    }
    
    velocity = sf::Vector2f(0, 0);
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x = -speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x = speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        velocity.y = -speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        velocity.y = speed;
    }
    
    position += velocity * dt;
    
    if (position.x < bounds.left + 24) position.x = bounds.left + 24;
    if (position.x > bounds.left + bounds.width - 24) position.x = bounds.left + bounds.width - 24;
    if (position.y < bounds.top + 24) position.y = bounds.top + 24;
    if (position.y > bounds.top + bounds.height - 24) position.y = bounds.top + bounds.height - 24;
    
    sprite.setPosition(position);
}

void Player::render(sf::RenderWindow& window) {
    if (invincibleTime > 0.0f) {
        if (static_cast<int>(invincibleBlinkTime * 10) % 2 == 0) {
            window.draw(sprite);
        }
    } else {
        window.draw(sprite);
    }
}

void Player::takeDamage() {
    if (invincibleTime <= 0.0f) {
        lives--;
        invincibleTime = 2.0f;
        invincibleBlinkTime = 0.0f;
    }
}

sf::FloatRect Player::getBounds() const {
    return sf::FloatRect(position.x - 12, position.y - 12, 24, 24);
}

sf::Vector2f Player::getPosition() const {
    return position;
}

sf::Vector2f Player::getShootPosition() const {
    return sf::Vector2f(position.x, position.y - 20);
}
