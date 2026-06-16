#ifndef BOSS_HPP
#define BOSS_HPP

#include <SFML/Graphics.hpp>
#include <vector>

enum class BossPhase {
    Phase1,
    Phase2,
    Phase3
};

class Boss {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float x;
    float y;
    float speed;
    int health;
    int maxHealth;
    BossPhase phase;
    float phaseTimer;
    float attackTimer;
    int currentAttack;
    float moveDirection;
    float entranceComplete;
    
public:
    Boss(const std::string& texturePath, float startX, float startY);
    
    void update(float deltaTime, std::vector<class Bullet*>& bullets, float playerX);
    void render(sf::RenderWindow& window);
    void takeDamage(int dmg);
    bool isDead() const;
    
    sf::FloatRect getBounds() const;
    float getHealthPercent() const;
    BossPhase getPhase() const;
};

#endif
