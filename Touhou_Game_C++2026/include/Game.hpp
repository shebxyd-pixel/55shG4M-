#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include "ResourceManager.hpp"
#include "Player.hpp"
#include "Bullet.hpp"
#include "World.hpp"
#include "HUD.hpp"
#include "Menu.hpp"
#include "BulletPattern.hpp"

enum class GameState {
    MainMenu,
    Playing,
    Paused,
    GameOver,
    Victory
};

class Game {
public:
    Game();
    ~Game();
    
    void run();
    
private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void handleCollisions();
    void spawnBullets();
    
    // Ventana principal
    std::unique_ptr<sf::RenderWindow> window;
    GameState state;
    
    // Recursos y entidades
    ResourceManager resources;
    Player player;
    std::vector<Bullet> enemyBullets;
    std::vector<Bullet> playerBullets;
    World world;
    HUD hud;
    Menu menu;
    
    // Puntuación y estado
    int score;
    int lives;
    int bombCount;
    float difficultyMultiplier;
    sf::Clock gameClock;
    
    // Patrones de balas
    BulletPatternGenerator patternGen;
    
    // Zonas de pantalla
    sf::FloatRect playerZone;
    sf::FloatRect combatZone;
};
