#include "NPC.hpp"
#include <SFML/Graphics.hpp>

NPC::NPC(const std::string& texturePath, float startX, float startY, const std::string& msg)
    : x(startX), y(startY), messageVisible(false), messageTimer(0.0f) {
    
    if (!texture.loadFromFile(texturePath)) {
        sf::Image img;
        img.create(64, 64, sf::Color(255, 180, 200));
        for (int i = 0; i < 64; ++i) {
            for (int j = 0; j < 64; ++j) {
                if ((i + j) % 8 < 4) {
                    img.setPixel(i, j, sf::Color(255, 200, 220));
                }
            }
        }
        texture.loadFromImage(img);
    }
    
    sprite.setTexture(texture);
    sprite.setScale(0.15f, 0.15f);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setPosition(x, y);
    
    message.setString(msg);
    message.setCharacterSize(16);
    message.setFillColor(sf::Color::White);
    message.setOutlineColor(sf::Color::Black);
    message.setOutlineThickness(2);
    message.setPosition(x - 100, y - 60);
}

void NPC::update(float deltaTime) {
    if (messageVisible) {
        messageTimer += deltaTime;
        if (messageTimer > 4.0f) {
            messageVisible = false;
        }
    }
}

void NPC::render(sf::RenderWindow& window) {
    window.draw(sprite);
    if (messageVisible) {
        window.draw(message);
    }
}

void NPC::showMessage() {
    messageVisible = true;
    messageTimer = 0.0f;
}

sf::FloatRect NPC::getBounds() const {
    return sprite.getGlobalBounds();
}
