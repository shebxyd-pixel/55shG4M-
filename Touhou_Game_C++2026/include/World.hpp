#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

struct Tile {
    sf::Sprite sprite;
    sf::Vector2f worldPosition;
    bool solid;
};

class World {
public:
    World();
    ~World() = default;
    
    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window, sf::View& camera);
    
    void generateWorld();
    bool checkCollision(const sf::FloatRect& bounds) const;
    
    sf::View getCamera() const { return camera; }
    void setCameraCenter(sf::Vector2f center);
    
private:
    std::vector<Tile> tiles;
    sf::Texture terrainTexture;
    sf::View camera;
    
    float worldWidth;
    float worldHeight;
};
