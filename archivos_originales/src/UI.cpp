#include "UI.hpp"
#include <iostream>

UI::UI() : currentSelection(MenuSelection::Play) {
    loadFont();
    
    createText(titleText, "TOUHOU STYLE SHOOTER", 36, 0, 100);
    createText(playText, "Jugar", 24, 0, 250);
    createText(highScoresText, "Puntuaciones", 24, 0, 320);
    createText(exitText, "Salir", 24, 0, 390);
    createText(scoreText, "Puntuacion: 0", 18, 10, 10);
    createText(livesText, "Vidas: 3", 18, 10, 35);
    createText(ammoText, "Municion: 100", 18, 10, 60);
    createText(stageText, "Fase: 1", 18, 10, 85);
    createText(pauseText, "PAUSA - ESC para continuar", 28, 0, 300);
    createText(gameOverText, "GAME OVER", 48, 0, 200);
    createText(victoryText, "VICTORIA!", 48, 0, 200);
    createText(backText, "ESC para volver", 18, 0, 500);
    
    healthBarBg.setSize(sf::Vector2f(200, 20));
    healthBarBg.setFillColor(sf::Color(60, 60, 60));
    healthBarBg.setPosition(220, 10);
    
    healthBar.setSize(sf::Vector2f(200, 20));
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition(220, 10);
    
    ammoBarBg.setSize(sf::Vector2f(150, 15));
    ammoBarBg.setFillColor(sf::Color(60, 60, 60));
    ammoBarBg.setPosition(10, 85);
    
    ammoBar.setSize(sf::Vector2f(150, 15));
    ammoBar.setFillColor(sf::Color::Cyan);
    ammoBar.setPosition(10, 85);
}

void UI::loadFont() {
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "No se pudo cargar la fuente\n";
    }
}

void UI::createText(sf::Text& text, const std::string& str, unsigned int size, float x, float y) {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
    if (x == 0) {
        auto bounds = text.getLocalBounds();
        text.setPosition(320 - bounds.width / 2, y);
    } else {
        text.setPosition(x, y);
    }
}

void UI::updateHUD(const DataManager& dataManager, float bossHealthPercent) {
    scoreText.setString("Puntuacion: " + std::to_string(dataManager.getScore()));
    livesText.setString("Vidas: " + std::to_string(dataManager.getLives()));
    ammoText.setString("Municion: " + std::to_string(dataManager.getAmmo()));
    stageText.setString("Fase: " + std::to_string(dataManager.getStage()));
    
    float ammoPercent = static_cast<float>(dataManager.getAmmo()) / dataManager.getMaxAmmo();
    ammoBar.setSize(sf::Vector2f(150 * ammoPercent, 15));
    
    if (bossHealthPercent >= 0) {
        healthBar.setSize(sf::Vector2f(200 * bossHealthPercent, 20));
    }
}

void UI::handleMenuInput(bool up, bool down) {
    if (up) {
        if (currentSelection == MenuSelection::Play) currentSelection = MenuSelection::Exit;
        else if (currentSelection == MenuSelection::HighScores) currentSelection = MenuSelection::Play;
        else currentSelection = MenuSelection::HighScores;
    }
    if (down) {
        if (currentSelection == MenuSelection::Play) currentSelection = MenuSelection::HighScores;
        else if (currentSelection == MenuSelection::HighScores) currentSelection = MenuSelection::Exit;
        else currentSelection = MenuSelection::Play;
    }
}

MenuSelection UI::getSelection() const {
    return currentSelection;
}

void UI::renderMainMenu(sf::RenderWindow& window) {
    playText.setFillColor(currentSelection == MenuSelection::Play ? sf::Color::Yellow : sf::Color::White);
    highScoresText.setFillColor(currentSelection == MenuSelection::HighScores ? sf::Color::Yellow : sf::Color::White);
    exitText.setFillColor(currentSelection == MenuSelection::Exit ? sf::Color::Yellow : sf::Color::White);
    
    window.draw(titleText);
    window.draw(playText);
    window.draw(highScoresText);
    window.draw(exitText);
}

void UI::renderHUD(sf::RenderWindow& window) {
    window.draw(scoreText);
    window.draw(livesText);
    window.draw(ammoText);
    window.draw(stageText);
    window.draw(ammoBarBg);
    window.draw(ammoBar);
}

void UI::renderPauseMenu(sf::RenderWindow& window) {
    window.draw(pauseText);
}

void UI::renderGameOver(sf::RenderWindow& window, int score) {
    createText(gameOverText, "GAME OVER", 48, 0, 200);
    createText(scoreText, "Puntuacion: " + std::to_string(score), 28, 0, 280);
    window.draw(gameOverText);
    window.draw(scoreText);
    window.draw(backText);
}

void UI::renderVictory(sf::RenderWindow& window, int score) {
    createText(victoryText, "VICTORIA!", 48, 0, 200);
    createText(scoreText, "Puntuacion: " + std::to_string(score), 28, 0, 280);
    window.draw(victoryText);
    window.draw(scoreText);
    window.draw(backText);
}

void UI::renderHighScores(sf::RenderWindow& window, const std::vector<ScoreEntry>& scores) {
    createText(titleText, "MEJORES PUNTUACIONES", 32, 0, 80);
    window.draw(titleText);
    
    float yPos = 150;
    for (size_t i = 0; i < scores.size() && i < 8; ++i) {
        sf::Text entry;
        createText(entry, std::to_string(i + 1) + ". " + scores[i].name + " - " + 
                   std::to_string(scores[i].score) + " (Fase " + std::to_string(scores[i].stage) + ")",
                   20, 0, yPos);
        window.draw(entry);
        yPos += 40;
    }
    
    window.draw(backText);
}
