#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <random>
#include "Grid.h"
#include "Castle.h"
#include "Tower.h"
#include "Enemy.h"
#include "AIController.h"
#include "Player.h"

class Game {
public:
    Game();
    void run();

private:
    static constexpr std::size_t ROWS = 20;
    static constexpr std::size_t COLS = 20;
    static constexpr int STARTING_CASTLE_HP = 100;
    static constexpr int TOWERS_TO_PLACE = 5;
    static constexpr int SCORE_PER_ENEMY = 10;

    Grid grid_;
    Castle castle_;
    std::vector<Tower> towers_;
    std::vector<Enemy> enemies_;
    AIController ai_;
    Player player_;

    int score_{0};
    int turn_{0};
    bool ascii_{false};
    int enemiesDestroyed_{0};
    bool waitingForWave_{true};
    int currentWaveScore_{0};
    int currentWaveMaxScore_{0};

    std::mt19937 rng_;

    void rebuildGrid();
    void printGrid() const;
    std::string cellSymbol(std::size_t r, std::size_t c) const;
    void ensureWaveReady();
    void performUpgrades();
    void moveEnemies();
    void waveCompleted(bool excellent);
    void showGameOver(bool playerWon) const;
};

#endif // GAME_H
