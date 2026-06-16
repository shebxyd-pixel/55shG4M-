#include "Boss.hpp"
#include "Bullet.hpp"
#include <cmath>
#include <cstdlib>

Boss::Boss(const std::string& texturePath, float startX, float startY)
    : x(startX), y(-100), speed(100.0f), health(500), maxHealth(500),
      phase(BossPhase::Phase1), phaseTimer(0.0f), attackTimer(0.0f),
      currentAttack(0), moveDirection(1.0f), entranceComplete(false) {
    
    if (!texture.loadFromFile(texturePath)) {
        sf::Image img;
        img.create(128, 128, sf::Color(180, 50, 120));
        for (int i = 0; i < 128; ++i) {
            for (int j = 0; j < 128; ++j) {
                if ((i + j) % 16 < 8) {
                    img.setPixel(i, j, sf::Color(200, 80, 150));
                }
            }
        }
        texture.loadFromImage(img);
    }
    
    sprite.setTexture(texture);
    sprite.setScale(0.25f, 0.25f);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setPosition(x, y);
}

void Boss::update(float deltaTime, std::vector<Bullet*>& bullets, float playerX) {
    phaseTimer += deltaTime;
    attackTimer += deltaTime;
    
    if (!entranceComplete) {
        y += speed * deltaTime;
        if (y >= 120) {
            entranceComplete = true;
            y = 120;
        }
    } else {
        x += moveDirection * speed * 0.5f * deltaTime;
        if (x < 150) {
            x = 150;
            moveDirection = 1.0f;
        }
        if (x > 490) {
            x = 490;
            moveDirection = -1.0f;
        }
        
        float healthPercent = static_cast<float>(health) / maxHealth;
        if (healthPercent < 0.33f && phase == BossPhase::Phase2) {
            phase = BossPhase::Phase3;
            phaseTimer = 0.0f;
        } else if (healthPercent < 0.66f && phase == BossPhase::Phase1) {
            phase = BossPhase::Phase2;
            phaseTimer = 0.0f;
        }
        
        float attackInterval = phase == BossPhase::Phase1 ? 1.2f : 
                              phase == BossPhase::Phase2 ? 0.9f : 0.6f;
        
        if (attackTimer >= attackInterval) {
            attackTimer = 0.0f;
            currentAttack = (currentAttack + 1) % 3;
            
            if (currentAttack == 0) {
                for (int i = -3; i <= 3; ++i) {
                    float angle = (90 + i * 15) * 3.14159f / 180.0f;
                    bullets.push_back(new Bullet(x, y + 50, 
                        cos(angle) * 200, sin(angle) * 200, BulletOwner::Enemy, 10));
                }
            } else if (currentAttack == 1) {
                float angle = atan2(playerX - x, 200) + 3.14159f / 2;
                bullets.push_back(new Bullet(x, y + 50,
                    cos(angle) * 250, sin(angle) * 250, BulletOwner::Enemy, 10));
            } else {
                for (int i = 0; i < 8; ++i) {
                    float angle = (i * 45) * 3.14159f / 180.0f + phaseTimer * 2;
                    bullets.push_back(new Bullet(x, y + 50,
                        cos(angle) * 150, sin(angle) * 150, BulletOwner::Enemy, 10));
                }
            }
        }
    }
    
    sprite.setPosition(x, y);
}

void Boss::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Boss::takeDamage(int dmg) {
    health -= dmg;
}

bool Boss::isDead() const {
    return health <= 0;
}

sf::FloatRect Boss::getBounds() const {
    return sprite.getGlobalBounds();
}

float Boss::getHealthPercent() const {
    return static_cast<float>(health) / maxHealth;
}

BossPhase Boss::getPhase() const {
    return phase;
}
