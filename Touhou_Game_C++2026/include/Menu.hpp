#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class MenuSelection {
    Play,
    Settings,
    HighScores,
    Exit
};

class Menu {
public:
    Menu();
    ~Menu() = default;
    
    void update();
    void render(sf::RenderWindow& window);
    
    void setFont(sf::Font& font);
    MenuSelection getSelection() const;
    void moveUp();
    void moveDown();
    
private:
    std::vector<sf::Text> menuItems;
    MenuSelection currentSelection;
    sf::Text title;
    sf::RectangleShape background;
};
