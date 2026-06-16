#include "DataManager.hpp"
#include <iostream>

DataManager::DataManager() : dataPath("data/scores.dat") {
    loadHighScores();
    resetGame();
}

void DataManager::loadHighScores() {
    std::ifstream file(dataPath);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            ScoreEntry entry;
            if (iss >> entry.name >> entry.score >> entry.stage) {
                highScores.push_back(entry);
            }
        }
        file.close();
    }
}

void DataManager::saveHighScores() {
    std::ofstream file(dataPath);
    if (file.is_open()) {
        for (const auto& entry : highScores) {
            file << entry.name << " " << entry.score << " " << entry.stage << "\n";
        }
        file.close();
    }
}

void DataManager::resetGame() {
    currentScore = 0;
    currentStage = 1;
    playerLives = maxLives;
    playerAmmo = maxAmmo;
}

void DataManager::addScore(int points) {
    currentScore += points;
}

void DataManager::nextStage() {
    currentStage++;
}

bool DataManager::useAmmo() {
    if (playerAmmo > 0) {
        playerAmmo--;
        return true;
    }
    return false;
}

void DataManager::refillAmmo() {
    playerAmmo = maxAmmo;
}

void DataManager::takeLife() {
    playerLives--;
}

bool DataManager::isGameOver() const {
    return playerLives <= 0;
}

int DataManager::getScore() const {
    return currentScore;
}

int DataManager::getStage() const {
    return currentStage;
}

int DataManager::getLives() const {
    return playerLives;
}

int DataManager::getAmmo() const {
    return playerAmmo;
}

int DataManager::getMaxAmmo() const {
    return maxAmmo;
}

void DataManager::addHighScore(const std::string& name) {
    ScoreEntry entry;
    entry.name = name;
    entry.score = currentScore;
    entry.stage = currentStage;
    highScores.push_back(entry);
    
    std::sort(highScores.begin(), highScores.end(),
        [](const ScoreEntry& a, const ScoreEntry& b) {
            return a.score > b.score;
        });
    
    if (highScores.size() > 10) {
        highScores.resize(10);
    }
    
    saveHighScores();
}

std::vector<ScoreEntry> DataManager::getHighScores() const {
    return highScores;
}
