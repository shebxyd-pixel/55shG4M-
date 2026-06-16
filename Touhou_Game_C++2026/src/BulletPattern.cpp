#include "BulletPattern.hpp"

std::vector<Bullet> BulletPatternGenerator::generateSpiral(sf::Vector2f origin, int count, float speed, float startAngle, float angleStep, int bulletsPerStep) {
    std::vector<Bullet> bullets;
    
    for (int i = 0; i < count; i++) {
        float angle = startAngle + i * angleStep;
        sf::Vector2f vel(cos(angle) * speed, sin(angle) * speed);
        bullets.emplace_back(origin, vel, BulletOwner::Enemy, BulletType::Small);
    }
    
    return bullets;
}

std::vector<Bullet> BulletPatternGenerator::generateRing(sf::Vector2f origin, int count, float speed, float startAngle) {
    std::vector<Bullet> bullets;
    float angleStep = (2.0f * 3.14159f) / count;
    
    for (int i = 0; i < count; i++) {
        float angle = startAngle + i * angleStep;
        sf::Vector2f vel(cos(angle) * speed, sin(angle) * speed);
        bullets.emplace_back(origin, vel, BulletOwner::Enemy, BulletType::Medium);
    }
    
    return bullets;
}

std::vector<Bullet> BulletPatternGenerator::generateAimed(sf::Vector2f origin, sf::Vector2f target, int count, float spread, float speed) {
    std::vector<Bullet> bullets;
    float baseAngle = atan2(target.y - origin.y, target.x - origin.x);
    
    for (int i = 0; i < count; i++) {
        float angle = baseAngle + (i - count / 2.0f) * spread;
        sf::Vector2f vel(cos(angle) * speed, sin(angle) * speed);
        bullets.emplace_back(origin, vel, BulletOwner::Enemy, BulletType::Small);
    }
    
    return bullets;
}

std::vector<Bullet> BulletPatternGenerator::generateWave(sf::Vector2f origin, int count, float amplitude, float frequency, float speed) {
    std::vector<Bullet> bullets;
    
    for (int i = 0; i < count; i++) {
        float offset = static_cast<float>(i) * 0.3f;
        sf::Vector2f vel(50.0f * sin(frequency * offset), speed);
        bullets.emplace_back(origin + sf::Vector2f(amplitude * sin(offset), 0), vel, BulletOwner::Enemy, BulletType::Star);
    }
    
    return bullets;
}

std::vector<Bullet> BulletPatternGenerator::generateStar(sf::Vector2f origin, int arms, float speed) {
    std::vector<Bullet> bullets;
    float angleStep = (2.0f * 3.14159f) / arms;
    
    for (int i = 0; i < arms; i++) {
        float angle = i * angleStep;
        sf::Vector2f vel(cos(angle) * speed, sin(angle) * speed);
        bullets.emplace_back(origin, vel, BulletOwner::Enemy, BulletType::Star);
    }
    
    return bullets;
}

std::vector<Bullet> BulletPatternGenerator::generateRandom(sf::Vector2f origin, int count, float speedMin, float speedMax) {
    std::vector<Bullet> bullets;
    
    for (int i = 0; i < count; i++) {
        float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
        float speed = speedMin + static_cast<float>(rand()) / RAND_MAX * (speedMax - speedMin);
        sf::Vector2f vel(cos(angle) * speed, sin(angle) * speed);
        
        BulletType type = static_cast<BulletType>(rand() % 4);
        bullets.emplace_back(origin, vel, BulletOwner::Enemy, type);
    }
    
    return bullets;
}
