#include "Menu.hpp"

Menu::Menu() : currentSelection(MenuSelection::Play) {
    background.setFillColor(sf::Color(10, 10, 30, 240));
    
    title.setString("Touhou Style\n  Bullet Hell");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::Magenta);
    title.setStyle(sf::Text::Bold);
    
    menuItems.resize(4);
    
    menuItems[0].setString("Play Game");
    menuItems[1].setString("Settings");
    menuItems[2].setString("High Scores");
    menuItems[3].setString("Exit");
    
    for (size_t i = 0; i < menuItems.size(); i++) {
        menuItems[i].setCharacterSize(30);
        menuItems[i].setStyle(sf::Text::Bold);
        menuItems[i].setFillColor(i == 0 ? sf::Color::Yellow : sf::Color::White);
    }
}

void Menu::update() {
    for (size_t i = 0; i < menuItems.size(); i++) {
        if (static_cast<MenuSelection>(i) == currentSelection) {
            menuItems[i].setFillColor(sf::Color::Yellow);
        } else {
            menuItems[i].setFillColor(sf::Color::White);
        }
    }
}

void Menu::render(sf::RenderWindow& window) {
    auto size = window.getSize();
    
    background.setSize(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
    window.draw(background);
    
    auto bounds = title.getLocalBounds();
    title.setOrigin(bounds.width / 2, bounds.height / 2);
    title.setPosition(size.x / 2, 150);
    window.draw(title);
    
    for (size_t i = 0; i < menuItems.size(); i++) {
        bounds = menuItems[i].getLocalBounds();
        menuItems[i].setOrigin(bounds.width / 2, bounds.height / 2);
        menuItems[i].setPosition(size.x / 2, 300 + i * 70);
        window.draw(menuItems[i]);
    }
}

void Menu::setFont(sf::Font& font) {
    title.setFont(font);
    for (auto& item : menuItems) {
        item.setFont(font);
    }
}

MenuSelection Menu::getSelection() const {
    return currentSelection;
}

void Menu::moveUp() {
    if (currentSelection == MenuSelection::Play) {
        currentSelection = MenuSelection::Exit;
    } else {
        int idx = static_cast<int>(currentSelection) - 1;
        currentSelection = static_cast<MenuSelection>(idx);
    }
}

void Menu::moveDown() {
    if (currentSelection == MenuSelection::Exit) {
        currentSelection = MenuSelection::Play;
    } else {
        int idx = static_cast<int>(currentSelection) + 1;
        currentSelection = static_cast<MenuSelection>(idx);
    }
}
