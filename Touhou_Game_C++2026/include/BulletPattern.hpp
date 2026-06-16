#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "Bullet.hpp"

class BulletPatternGenerator {
public:
    BulletPatternGenerator() = default;
    ~BulletPatternGenerator() = default;
    
    // Patrones clásicos de Touhou
    std::vector<Bullet> generateSpiral(sf::Vector2f origin, int count, float speed, float startAngle, float angleStep, int bulletsPerStep = 1);
    std::vector<Bullet> generateRing(sf::Vector2f origin, int count, float speed, float startAngle = 0.0f);
    std::vector<Bullet> generateAimed(sf::Vector2f origin, sf::Vector2f target, int count, float spread, float speed);
    std::vector<Bullet> generateWave(sf::Vector2f origin, int count, float amplitude, float frequency, float speed);
    std::vector<Bullet> generateStar(sf::Vector2f origin, int arms, float speed);
    std::vector<Bullet> generateRandom(sf::Vector2f origin, int count, float speedMin, float speedMax);
};
