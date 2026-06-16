#include "Game.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game() : state(GameState::MainMenu), score(0), lives(3), bombCount(3), difficultyMultiplier(1.0f) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Obtener la resolución del escritorio
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window = std::make_unique<sf::RenderWindow>(
        desktop,
        "Touhou Style Bullet Hell",
        sf::Style::Fullscreen
    );
    window->setFramerateLimit(60);
    window->setVerticalSyncEnabled(true);
    
    // Configurar zonas de pantalla (estilo Touhou: jugador a la izquierda, combate a la derecha)
    float windowWidth = static_cast<float>(window->getSize().x);
    float windowHeight = static_cast<float>(window->getSize().y);
    
    playerZone = sf::FloatRect(
        windowWidth * 0.05f,
        windowHeight * 0.1f,
        windowWidth * 0.3f,
        windowHeight * 0.8f
    );
    
    combatZone = sf::FloatRect(
        windowWidth * 0.38f,
        windowHeight * 0.05f,
        windowWidth * 0.57f,
        windowHeight * 0.9f
    );
    
    // Cargar todos los recursos
    if (!resources.loadAll()) {
        std::cerr << "Failed to load resources!" << std::endl;
    }
    
    menu.setFont(resources.getFont("default"));
    hud.setFont(resources.getFont("default"));
    
    world.generateWorld();
}

Game::~Game() = default;

void Game::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    
    while (window->isOpen()) {
        timeSinceLastUpdate += clock.restart();
        
        while (timeSinceLastUpdate >= timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            processEvents();
            update(timePerFrame);
        }
        
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }
        
        if (state == GameState::MainMenu) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    menu.moveUp();
                }
                if (event.key.code == sf::Keyboard::Down) {
                    menu.moveDown();
                }
                if (event.key.code == sf::Keyboard::Return) {
                    if (menu.getSelection() == MenuSelection::Play) {
                        state = GameState::Playing;
                        score = 0;
                        lives = 3;
                        bombCount = 3;
                        player = Player();
                        enemyBullets.clear();
                        playerBullets.clear();
                    }
                    if (menu.getSelection() == MenuSelection::Exit) {
                        window->close();
                    }
                }
            }
        } else if (state == GameState::Playing) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                state = GameState::Paused;
            }
        } else if (state == GameState::Paused) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                state = GameState::Playing;
            }
        }
    }
}

void Game::update(sf::Time deltaTime) {
    if (state == GameState::Playing) {
        player.update(deltaTime, playerZone);
        
        // Disparar
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            static float shootTimer = 0.0f;
            shootTimer += deltaTime.asSeconds();
            if (shootTimer > 0.1f) {
                shootTimer = 0.0f;
                sf::Vector2f shootPos = player.getShootPosition();
                playerBullets.emplace_back(shootPos, sf::Vector2f(0, -700.0f), BulletOwner::Player, BulletType::Small);
                playerBullets.emplace_back(shootPos + sf::Vector2f(-15, 0), sf::Vector2f(-30, -600.0f), BulletOwner::Player, BulletType::Small);
                playerBullets.emplace_back(shootPos + sf::Vector2f(15, 0), sf::Vector2f(30, -600.0f), BulletOwner::Player, BulletType::Small);
            }
        }
        
        // Generar balas enemigas
        static float spawnTimer = 0.0f;
        spawnTimer += deltaTime.asSeconds();
        
        if (spawnTimer > 0.5f) {
            spawnTimer = 0.0f;
            sf::Vector2f enemyPos(combatZone.left + combatZone.width / 2, combatZone.top + 100);
            
            // Patrón aleatorio
            int pattern = rand() % 4;
            switch (pattern) {
                case 0: {
                    auto bullets = patternGen.generateRing(enemyPos, 12, 200.0f);
                    enemyBullets.insert(enemyBullets.end(), bullets.begin(), bullets.end());
                    break;
                }
                case 1: {
                    auto bullets = patternGen.generateSpiral(enemyPos, 5, 250.0f, 0.0f, 0.3f);
                    enemyBullets.insert(enemyBullets.end(), bullets.begin(), bullets.end());
                    break;
                }
                case 2: {
                    auto bullets = patternGen.generateStar(enemyPos, 8, 180.0f);
                    enemyBullets.insert(enemyBullets.end(), bullets.begin(), bullets.end());
                    break;
                }
                case 3: {
                    auto bullets = patternGen.generateAimed(enemyPos, player.getPosition(), 5, 0.2f, 300.0f);
                    enemyBullets.insert(enemyBullets.end(), bullets.begin(), bullets.end());
                    break;
                }
            }
        }
        
        // Actualizar balas
        for (auto& bullet : enemyBullets) bullet.update(deltaTime);
        for (auto& bullet : playerBullets) bullet.update(deltaTime);
        
        // Eliminar balas fuera de pantalla
        enemyBullets.erase(
            std::remove_if(enemyBullets.begin(), enemyBullets.end(),
                [this](const Bullet& b) { return b.isOffScreen(*window); }),
            enemyBullets.end()
        );
        playerBullets.erase(
            std::remove_if(playerBullets.begin(), playerBullets.end(),
                [this](const Bullet& b) { return b.isOffScreen(*window); }),
            playerBullets.end()
        );
        
        handleCollisions();
        
        world.setCameraCenter(player.getPosition());
        world.update(deltaTime);
        
        hud.update(score, lives, bombCount, 60.0f);
    }
}

void Game::handleCollisions() {
    if (player.isInvincible()) return;
    
    for (auto& bullet : enemyBullets) {
        if (bullet.getBounds().intersects(player.getBounds())) {
            player.takeDamage();
            score = std::max(0, score - 100);
            lives = player.getLives();
            if (lives <= 0) {
                state = GameState::GameOver;
            }
        }
    }
    
    for (auto& bullet : playerBullets) {
        if (bullet.getBounds().intersects(combatZone)) {
            score += 10;
        }
    }
}

void Game::render() {
    window->clear(sf::Color(20, 20, 50));
    
    world.render(*window, world.getCamera());
    
    // Dibujar zonas
    sf::RectangleShape pZoneRect(sf::Vector2f(playerZone.width, playerZone.height));
    pZoneRect.setPosition(playerZone.left, playerZone.top);
    pZoneRect.setFillColor(sf::Color(30, 30, 70, 100));
    pZoneRect.setOutlineColor(sf::Color::Cyan);
    pZoneRect.setOutlineThickness(2);
    window->draw(pZoneRect);
    
    sf::RectangleShape cZoneRect(sf::Vector2f(combatZone.width, combatZone.height));
    cZoneRect.setPosition(combatZone.left, combatZone.top);
    cZoneRect.setFillColor(sf::Color(30, 20, 60, 150));
    cZoneRect.setOutlineColor(sf::Color::Magenta);
    cZoneRect.setOutlineThickness(2);
    window->draw(cZoneRect);
    
    for (auto& bullet : enemyBullets) bullet.render(*window);
    for (auto& bullet : playerBullets) bullet.render(*window);
    
    player.render(*window);
    hud.render(*window);
    
    if (state == GameState::MainMenu) {
        menu.render(*window);
    } else if (state == GameState::Paused) {
        sf::Text pauseText;
        pauseText.setFont(resources.getFont("default"));
        pauseText.setString("PAUSED - Press ESC to Continue");
        pauseText.setCharacterSize(40);
        pauseText.setFillColor(sf::Color::White);
        pauseText.setStyle(sf::Text::Bold);
        
        auto bounds = pauseText.getLocalBounds();
        pauseText.setOrigin(bounds.width / 2, bounds.height / 2);
        pauseText.setPosition(window->getSize().x / 2, window->getSize().y / 2);
        
        sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        
        window->draw(overlay);
        window->draw(pauseText);
    } else if (state == GameState::GameOver) {
        sf::Text goText;
        goText.setFont(resources.getFont("default"));
        goText.setString("GAME OVER!");
        goText.setCharacterSize(60);
        goText.setFillColor(sf::Color(255, 80, 80));
        goText.setStyle(sf::Text::Bold);
        
        auto bounds = goText.getLocalBounds();
        goText.setOrigin(bounds.width / 2, bounds.height / 2);
        goText.setPosition(window->getSize().x / 2, window->getSize().y / 2 - 50);
        
        sf::Text scoreText;
        scoreText.setFont(resources.getFont("default"));
        scoreText.setString("Final Score: " + std::to_string(score));
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::White);
        
        bounds = scoreText.getLocalBounds();
        scoreText.setOrigin(bounds.width / 2, bounds.height / 2);
        scoreText.setPosition(window->getSize().x / 2, window->getSize().y / 2 + 50);
        
        sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
        overlay.setFillColor(sf::Color(0, 0, 0, 200));
        
        window->draw(overlay);
        window->draw(goText);
        window->draw(scoreText);
    }
    
    window->display();
}
