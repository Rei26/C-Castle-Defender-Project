#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include <vector>
#include <random>
#include <algorithm>
#include "Grid.h"
#include "Enemy.h"
#include "Tower.h"

class AIController {
public:
    AIController(std::size_t topRow, std::size_t cols);

    void startNextWave();
    void advanceTurn(Grid& grid, const std::vector<Tower>& towers, std::vector<Enemy>& enemies);

    void notifyDestroyed(int count);
    void notifyCastleDamage(int dmg);

    bool waveInProgress() const { return waveInProgress_; }
    bool spawnQuotaMet() const { return spawnedThisWave_ >= enemiesPerWave_; }
    bool waitingForWaveStart() const { return !waveInProgress_ && currentWave_ < totalWaves_; }
    bool allWavesFinished() const { return currentWave_ >= totalWaves_ && !waveInProgress_; }
    bool hasMoreWaves() const { return currentWave_ < totalWaves_; }
    int currentWaveNumber() const { return waveInProgress_ ? currentWave_ : std::min(currentWave_ + 1, totalWaves_); }
    int totalWaves() const { return totalWaves_; }
    int enemiesPerWave() const { return enemiesPerWave_; }
    int spawnedTotal() const { return spawnedTotal_; }
    int destroyedTotal() const { return destroyedTotal_; }

    bool finalizeWave(int waveScoreEarned, int waveScoreMax);

private:
    std::size_t topRow_{0};
    std::size_t cols_{0};

    const int totalWaves_{5};
    const int enemiesPerWave_{10};
    const int baseAttack_{10};

    int currentWave_{0};
    bool waveInProgress_{false};
    int spawnedThisWave_{0};
    int waveDestroyed_{0};
    int waveCastleDamage_{0};
    int spawnedTotal_{0};
    int destroyedTotal_{0};
    int extraHP_{0};

    std::mt19937 rng_;

    int baseHP() const { return 3 + extraHP_; }
    bool spawnEnemy(Grid& grid, const std::vector<Tower>& towers, std::vector<Enemy>& enemies);
    std::size_t pickColumn(const Grid& grid, const std::vector<Tower>& towers);
    Enemy makeEnemy(std::size_t column);
};

#endif