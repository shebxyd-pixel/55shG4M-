#include "Ally.hpp"

Ally::Ally(const std::string& texturePath, float startX, float startY)
    : x(startX), y(startY), fadeTimer(0.0f), active(true), collected(false) {
    
    if (!texture.loadFromFile(texturePath)) {
        sf::Image img;
        img.create(64, 64, sf::Color::Green);
        for (int i = 0; i < 64; ++i) {
            for (int j = 0; j < 64; ++j) {
                if ((i + j) % 8 < 4) {
                    img.setPixel(i, j, sf::Color(100, 255, 100));
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

void Ally::update(float deltaTime) {
    if (collected) {
        fadeTimer += deltaTime;
        float alpha = std::max(0.0f, 255.0f - fadeTimer * 500.0f);
        sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
        if (alpha <= 0) {
            active = false;
        }
    }
}

void Ally::render(sf::RenderWindow& window) {
    if (active) {
        window.draw(sprite);
    }
}

void Ally::collect() {
    collected = true;
}

bool Ally::isActive() const {
    return active;
}

bool Ally::isCollected() const {
    return collected;
}

sf::FloatRect Ally::getBounds() const {
    return sprite.getGlobalBounds();
}
