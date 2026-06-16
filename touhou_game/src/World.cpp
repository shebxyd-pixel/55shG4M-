#include "World.hpp"

World::World() : worldWidth(8000.0f), worldHeight(6000.0f) {
    camera.setSize(1920.0f, 1080.0f);
    camera.setCenter(4000.0f, 3000.0f);
    
    sf::Image img;
    img.create(128, 128, sf::Color(40, 40, 70));
    terrainTexture.loadFromImage(img);
}

void World::generateWorld() {
    tiles.clear();
    
    const int gridSize = 64;
    for (float x = 0; x < worldWidth; x += gridSize) {
        for (float y = 0; y < worldHeight; y += gridSize) {
            Tile tile;
            tile.worldPosition = sf::Vector2f(x, y);
            
            sf::Sprite tSprite;
            tSprite.setTexture(terrainTexture);
            tSprite.setPosition(tile.worldPosition);
            
            if (rand() % 10 < 2) {
                tSprite.setColor(sf::Color(60, 60, 90));
            } else if (rand() % 10 < 4) {
                tSprite.setColor(sf::Color(35, 35, 65));
            }
            
            tile.sprite = tSprite;
            tile.solid = false;
            
            tiles.push_back(tile);
        }
    }
}

void World::update(sf::Time deltaTime) {
}

void World::render(sf::RenderWindow& window, sf::View& cam) {
    window.setView(cam);
    
    for (const auto& tile : tiles) {
        if (cam.getViewport().contains(tile.worldPosition)) {
            window.draw(tile.sprite);
        }
    }
    
    window.setView(window.getDefaultView());
}

bool World::checkCollision(const sf::FloatRect& bounds) const {
    for (const auto& tile : tiles) {
        if (tile.solid && tile.sprite.getGlobalBounds().intersects(bounds)) {
            return true;
        }
    }
    return false;
}

void World::setCameraCenter(sf::Vector2f center) {
    center.x = std::max(camera.getSize().x / 2, std::min(worldWidth - camera.getSize().x / 2, center.x));
    center.y = std::max(camera.getSize().y / 2, std::min(worldHeight - camera.getSize().y / 2, center.y));
    camera.setCenter(center);
}
