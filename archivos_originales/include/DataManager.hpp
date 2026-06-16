#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

struct ScoreEntry {
    std::string name;
    int score;
    int stage;
};

class DataManager {
private:
    std::vector<ScoreEntry> highScores;
    int currentScore;
    int currentStage;
    int playerLives;
    int playerAmmo;
    const int maxAmmo = 100;
    const int maxLives = 3;
    std::string dataPath;

    void loadHighScores();
    void saveHighScores();

public:
    DataManager();
    
    void resetGame();
    void addScore(int points);
    void nextStage();
    bool useAmmo();
    void refillAmmo();
    void takeLife();
    bool isGameOver() const;
    
    int getScore() const;
    int getStage() const;
    int getLives() const;
    int getAmmo() const;
    int getMaxAmmo() const;
    
    void addHighScore(const std::string& name);
    std::vector<ScoreEntry> getHighScores() const;
};

#endif
