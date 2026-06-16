#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "DataManager.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Boss.hpp"
#include "Ally.hpp"
#include "NPC.hpp"
#include "Bullet.hpp"
#include "UI.hpp"

enum class GameState {
    MainMenu,
    Playing,
    Paused,
    GameOver,
    Victory,
    HighScores
};

class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    DataManager dataManager;
    
    GameState currentState;
    UI ui;
    
    Player* player;
    Boss* boss;
    Ally* ally;
    NPC* npc;
    
    std::vector<Enemy*> enemies;
    std::vector<Bullet*> playerBullets;
    std::vector<Bullet*> enemyBullets;
    
    sf::Texture playerTexture;
    sf::Texture bossTexture;
    sf::Texture allyTexture;
    sf::Texture npcTexture;
    sf::Texture enemyTexture;
    
    sf::SoundBuffer shootBuffer;
    sf::SoundBuffer hitBuffer;
    sf::SoundBuffer explosionBuffer;
    sf::Sound shootSound;
    sf::Sound hitSound;
    sf::Sound explosionSound;
    sf::Music bgMusic;
    sf::Font font;
    
    float spawnTimer;
    float enemySpawnRate;
    bool allySpawned;
    bool npcSpawned;
    bool bossSpawned;
    int npcMessageX;
    int npcMessageY;
    
    void loadAssets();
    void initGame();
    void handleEvents();
    void update(float deltaTime);
    void render();
    void spawnEnemy();
    void spawnBoss();
    void checkCollisions();
    void cleanup();
    
public:
    Game();
    ~Game();
    void run();
};

#endif
