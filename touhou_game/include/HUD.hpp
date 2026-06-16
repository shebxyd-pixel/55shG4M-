#pragma once
#include <SFML/Graphics.hpp>

class HUD {
public:
    HUD();
    ~HUD() = default;
    
    void update(int score, int lives, int bombs, float fps);
    void render(sf::RenderWindow& window);
    void setFont(sf::Font& font);
    
private:
    sf::Text scoreText;
    sf::Text livesText;
    sf::Text bombsText;
    sf::Text fpsText;
    sf::RectangleShape hudBackground;
    sf::RectangleShape bossHealthBar;
    sf::RectangleShape bossHealthBg;
};
