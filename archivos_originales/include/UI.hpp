#ifndef UI_HPP
#define UI_HPP

#include <SFML/Graphics.hpp>
#include "DataManager.hpp"

enum class MenuSelection {
    Play,
    HighScores,
    Exit
};

class UI {
private:
    sf::Font font;
    sf::Text titleText;
    sf::Text playText;
    sf::Text highScoresText;
    sf::Text exitText;
    sf::Text scoreText;
    sf::Text livesText;
    sf::Text ammoText;
    sf::Text stageText;
    sf::Text pauseText;
    sf::Text gameOverText;
    sf::Text victoryText;
    sf::Text backText;
    
    sf::RectangleShape healthBarBg;
    sf::RectangleShape healthBar;
    sf::RectangleShape ammoBarBg;
    sf::RectangleShape ammoBar;
    
    MenuSelection currentSelection;
    
    void createText(sf::Text& text, const std::string& str, unsigned int size, float x, float y);
    
public:
    UI();
    
    void loadFont();
    void updateHUD(const DataManager& dataManager, float bossHealthPercent = -1.0f);
    void handleMenuInput(bool up, bool down);
    MenuSelection getSelection() const;
    
    void renderMainMenu(sf::RenderWindow& window);
    void renderHUD(sf::RenderWindow& window);
    void renderPauseMenu(sf::RenderWindow& window);
    void renderGameOver(sf::RenderWindow& window, int score);
    void renderVictory(sf::RenderWindow& window, int score);
    void renderHighScores(sf::RenderWindow& window, const std::vector<ScoreEntry>& scores);
};

#endif
