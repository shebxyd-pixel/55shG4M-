#include "Game.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game() : window(sf::VideoMode(640, 800), "Touhou Style Shooter"), 
               currentState(GameState::MainMenu), player(nullptr), boss(nullptr), 
               ally(nullptr), npc(nullptr), spawnTimer(0.0f), enemySpawnRate(2.0f),
               allySpawned(false), npcSpawned(false), bossSpawned(false) {
    
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    loadAssets();
}

Game::~Game() {
    cleanup();
}

void Game::loadAssets() {
    playerTexture.loadFromFile("assets/sprites/UtaUtane.jpg");
    bossTexture.loadFromFile("assets/sprites/MomoneMomo.jpg");
    allyTexture.loadFromFile("assets/sprites/AkitaNeru.jpg");
    npcTexture.loadFromFile("assets/sprites/MegurineLuka.jpg");
    font.loadFromFile("C:/Windows/Fonts/arial.ttf");
}

void Game::initGame() {
    cleanup();
    dataManager.resetGame();
    player = new Player("assets/sprites/UtaUtane.jpg", 320, 700);
    spawnTimer = 0.0f;
    enemySpawnRate = 2.0f;
    allySpawned = false;
    npcSpawned = false;
    bossSpawned = false;
    npcMessageX = 100 + (rand() % 440);
    npcMessageY = 300 + (rand() % 200);
}

void Game::cleanup() {
    delete player;
    delete boss;
    delete ally;
    delete npc;
    player = nullptr;
    boss = nullptr;
    ally = nullptr;
    npc = nullptr;
    
    for (auto e : enemies) delete e;
    for (auto b : playerBullets) delete b;
    for (auto b : enemyBullets) delete b;
    enemies.clear();
    playerBullets.clear();
    enemyBullets.clear();
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (currentState == GameState::MainMenu || currentState == GameState::HighScores) {
                if (event.key.code == sf::Keyboard::Up) ui.handleMenuInput(true, false);
                if (event.key.code == sf::Keyboard::Down) ui.handleMenuInput(false, true);
                if (event.key.code == sf::Keyboard::Return) {
                    if (currentState == GameState::MainMenu) {
                        if (ui.getSelection() == MenuSelection::Play) {
                            initGame();
                            currentState = GameState::Playing;
                        } else if (ui.getSelection() == MenuSelection::HighScores) {
                            currentState = GameState::HighScores;
                        } else if (ui.getSelection() == MenuSelection::Exit) {
                            window.close();
                        }
                    }
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    currentState = GameState::MainMenu;
                }
            } else if (currentState == GameState::Playing) {
                if (event.key.code == sf::Keyboard::Escape) {
                    currentState = GameState::Paused;
                }
            } else if (currentState == GameState::Paused) {
                if (event.key.code == sf::Keyboard::Escape) {
                    currentState = GameState::Playing;
                }
            } else if (currentState == GameState::GameOver || currentState == GameState::Victory) {
                if (event.key.code == sf::Keyboard::Escape) {
                    currentState = GameState::MainMenu;
                }
            }
        }
    }
}

void Game::spawnEnemy() {
    float x = 50 + (rand() % 540);
    enemies.push_back(new Enemy("assets/sprites/UtaUtane.jpg", x, -50));
}

void Game::spawnBoss() {
    boss = new Boss("assets/sprites/MomoneMomo.jpg", 320, -100);
    bossSpawned = true;
}

void Game::checkCollisions() {
    for (auto bullet : playerBullets) {
        if (!bullet->isActive()) continue;
        
        for (auto enemy : enemies) {
            if (bullet->getBounds().intersects(enemy->getBounds())) {
                enemy->takeDamage(bullet->getDamage());
                bullet->deactivate();
                if (enemy->isDead()) {
                    dataManager.addScore(100);
                }
            }
        }
        
        if (boss && bullet->getBounds().intersects(boss->getBounds())) {
            boss->takeDamage(bullet->getDamage());
            bullet->deactivate();
            dataManager.addScore(10);
        }
    }
    
    for (auto bullet : enemyBullets) {
        if (!bullet->isActive()) continue;
        
        if (bullet->getBounds().intersects(player->getBounds())) {
            bullet->deactivate();
            dataManager.takeLife();
            if (dataManager.isGameOver()) {
                currentState = GameState::GameOver;
                dataManager.addHighScore("Player");
            }
        }
    }
    
    for (auto enemy : enemies) {
        if (enemy->getBounds().intersects(player->getBounds())) {
            dataManager.takeLife();
            enemy->takeDamage(100);
            if (dataManager.isGameOver()) {
                currentState = GameState::GameOver;
                dataManager.addHighScore("Player");
            }
        }
    }
    
    if (ally && ally->isActive() && !ally->isCollected() && 
        player->getBounds().intersects(ally->getBounds())) {
        ally->collect();
        dataManager.refillAmmo();
    }
    
    if (npc && player->getBounds().intersects(npc->getBounds())) {
        npc->showMessage();
    }
}

void Game::update(float deltaTime) {
    if (currentState != GameState::Playing) return;
    
    player->update(deltaTime, window);
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && player->canShoot() && dataManager.useAmmo()) {
        player->resetShootCooldown();
        auto pos = player->getPosition();
        playerBullets.push_back(new Bullet(pos.x, pos.y - 30, 0, -500, BulletOwner::Player, 20));
    }
    
    spawnTimer += deltaTime;
    int stage = dataManager.getStage();
    
    if (!npcSpawned) {
        npc = new NPC("assets/sprites/MegurineLuka.jpg", npcMessageX, npcMessageY, "Ya tu sabe, continua por aqui");
        npcSpawned = true;
    }
    
    if (stage >= 3 && !allySpawned) {
        ally = new Ally("assets/sprites/AkitaNeru.jpg", 320, 400);
        allySpawned = true;
    }
    
    float spawnRate = 2.5f - (stage * 0.3f);
    if (spawnRate < 0.5f) spawnRate = 0.5f;
    
    if (spawnTimer >= spawnRate && !bossSpawned && stage <= 5) {
        spawnTimer = 0.0f;
        int enemiesToSpawn = 1 + (stage / 2);
        for (int i = 0; i < enemiesToSpawn; ++i) {
            spawnEnemy();
        }
    }
    
    if (stage == 5 && !bossSpawned && enemies.empty()) {
        spawnBoss();
    }
    
    if (stage < 5 && enemies.empty() && spawnTimer > 3.0f && !bossSpawned) {
        dataManager.nextStage();
        dataManager.addScore(500);
    }
    
    for (auto enemy : enemies) {
        enemy->update(deltaTime);
        if (enemy->canShoot()) {
            enemy->resetShootTimer();
            auto pos = enemy->getPosition();
            enemyBullets.push_back(new Bullet(pos.x, pos.y + 20, 0, 300, BulletOwner::Enemy, 10));
        }
    }
    
    if (boss) {
        boss->update(deltaTime, enemyBullets, player->getX());
        if (boss->isDead()) {
            dataManager.addScore(5000);
            currentState = GameState::Victory;
            dataManager.addHighScore("Player");
        }
    }
    
    if (ally) ally->update(deltaTime);
    if (npc) npc->update(deltaTime);
    
    for (auto bullet : playerBullets) bullet->update(deltaTime);
    for (auto bullet : enemyBullets) bullet->update(deltaTime);
    
    checkCollisions();
    
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](Enemy* e) { 
            if (e->isDead() || e->getPosition().y > 850) {
                delete e;
                return true;
            }
            return false;
        }), enemies.end());
    
    playerBullets.erase(std::remove_if(playerBullets.begin(), playerBullets.end(),
        [this](Bullet* b) {
            if (!b->isActive() || b->isOffScreen(window)) {
                delete b;
                return true;
            }
            return false;
        }), playerBullets.end());
    
    enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(),
        [this](Bullet* b) {
            if (!b->isActive() || b->isOffScreen(window)) {
                delete b;
                return true;
            }
            return false;
        }), enemyBullets.end());
    
    ui.updateHUD(dataManager, boss ? boss->getHealthPercent() : -1.0f);
}

void Game::render() {
    window.clear(sf::Color(20, 20, 40));
    
    if (currentState == GameState::MainMenu) {
        ui.renderMainMenu(window);
    } else if (currentState == GameState::HighScores) {
        ui.renderHighScores(window, dataManager.getHighScores());
    } else if (currentState == GameState::Playing || currentState == GameState::Paused) {
        for (auto enemy : enemies) enemy->render(window);
        if (boss) boss->render(window);
        if (ally) ally->render(window);
        if (npc) npc->render(window);
        player->render(window);
        for (auto bullet : playerBullets) bullet->render(window);
        for (auto bullet : enemyBullets) bullet->render(window);
        ui.renderHUD(window);
        
        if (boss) {
            sf::RectangleShape bossHpBg(sf::Vector2f(400, 25));
            bossHpBg.setFillColor(sf::Color(50, 50, 50));
            bossHpBg.setPosition(120, 750);
            window.draw(bossHpBg);
            
            sf::RectangleShape bossHp(sf::Vector2f(400 * boss->getHealthPercent(), 25));
            bossHp.setFillColor(sf::Color(255, 50, 50));
            bossHp.setPosition(120, 750);
            window.draw(bossHp);
            
            sf::Text bossText;
            bossText.setFont(font);
            bossText.setString("JEFE FINAL");
            bossText.setCharacterSize(14);
            bossText.setFillColor(sf::Color::White);
            bossText.setPosition(120, 725);
            window.draw(bossText);
        }
        
        if (currentState == GameState::Paused) {
            ui.renderPauseMenu(window);
        }
    } else if (currentState == GameState::GameOver) {
        ui.renderGameOver(window, dataManager.getScore());
    } else if (currentState == GameState::Victory) {
        ui.renderVictory(window, dataManager.getScore());
    }
    
    window.display();
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        handleEvents();
        update(deltaTime);
        render();
    }
}
