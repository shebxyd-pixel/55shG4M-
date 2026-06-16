#include "HUD.hpp"

HUD::HUD() {
    hudBackground.setFillColor(sf::Color(20, 20, 50, 200));
    hudBackground.setSize(sf::Vector2f(300, 120));
    hudBackground.setPosition(10, 10);
    hudBackground.setOutlineColor(sf::Color::Magenta);
    hudBackground.setOutlineThickness(2);
    
    bossHealthBg.setSize(sf::Vector2f(400, 30));
    bossHealthBg.setFillColor(sf::Color(50, 30, 50, 200));
    bossHealthBg.setPosition(600, 750);
    
    bossHealthBar.setSize(sf::Vector2f(400, 30));
    bossHealthBar.setFillColor(sf::Color(200, 50, 100));
    bossHealthBar.setPosition(600, 750);
}

void HUD::update(int score, int lives, int bombs, float fps) {
    scoreText.setString("Score: " + std::to_string(score));
    livesText.setString("Lives: " + std::to_string(lives));
    bombsText.setString("Bombs: " + std::to_string(bombs));
    fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
}

void HUD::render(sf::RenderWindow& window) {
    window.draw(hudBackground);
    window.draw(scoreText);
    window.draw(livesText);
    window.draw(bombsText);
    window.draw(fpsText);
    window.draw(bossHealthBg);
    window.draw(bossHealthBar);
}

void HUD::setFont(sf::Font& font) {
    scoreText.setFont(font);
    livesText.setFont(font);
    bombsText.setFont(font);
    fpsText.setFont(font);
    
    scoreText.setCharacterSize(24);
    livesText.setCharacterSize(24);
    bombsText.setCharacterSize(24);
    fpsText.setCharacterSize(20);
    
    scoreText.setFillColor(sf::Color::Cyan);
    livesText.setFillColor(sf::Color::Green);
    bombsText.setFillColor(sf::Color(255, 150, 255));
    fpsText.setFillColor(sf::Color::Yellow);
    
    scoreText.setPosition(20, 20);
    livesText.setPosition(20, 50);
    bombsText.setPosition(20, 80);
    fpsText.setPosition(20, 110);
}
