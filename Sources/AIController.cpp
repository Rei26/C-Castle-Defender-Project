#include "AIController.h"
#include <algorithm>
#include <chrono>

namespace {
double randUnit(std::mt19937& rng) {
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}
}

AIController::AIController(std::size_t topRow, std::size_t cols)
    : topRow_(topRow), cols_(cols) {
    auto seed = static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count());
    rng_.seed(seed);
}

void AIController::startNextWave() {
    if (waveInProgress_ || currentWave_ >= totalWaves_) return;
    ++currentWave_;
    waveInProgress_ = true;
    spawnedThisWave_ = 0;
    waveDestroyed_ = 0;
    waveCastleDamage_ = 0;
}

void AIController::advanceTurn(Grid& grid, const std::vector<Tower>& towers, std::vector<Enemy>& enemies) {
    if (!waveInProgress_) return;
    if (spawnedThisWave_ >= enemiesPerWave_) return;
    if (spawnEnemy(grid, towers, enemies)) {
        ++spawnedThisWave_;
        ++spawnedTotal_;
    }
}

void AIController::notifyDestroyed(int count) {
    destroyedTotal_ += count;
    waveDestroyed_ += count;
}

void AIController::notifyCastleDamage(int dmg) {
    waveCastleDamage_ += dmg;
}

bool AIController::finalizeWave(int waveScoreEarned, int waveScoreMax) {
    waveInProgress_ = false;
    if (waveScoreMax <= 0) return false;
    double ratio = static_cast<double>(waveScoreEarned) / static_cast<double>(waveScoreMax);
    bool excellent = (waveCastleDamage_ == 0) && (ratio >= 0.80);
    if (excellent) {
        ++extraHP_;
    }
    waveCastleDamage_ = 0;
    waveDestroyed_ = 0;
    return excellent;
}

std::size_t AIController::pickColumn(const Grid& grid, const std::vector<Tower>& towers) {
    std::vector<std::size_t> preferred;
    std::vector<std::size_t> fallback;
    for (std::size_t c = 0; c < cols_; ++c) {
        if (!grid.isEmpty(topRow_, c)) continue;
        bool nearTower = false;
        for (const auto& t : towers) {
            if (t.pos().r == 0) continue;
            std::size_t tc = t.pos().c;
            std::size_t diff = (tc > c) ? (tc - c) : (c - tc);
            if (diff < 3) {
                nearTower = true;
                break;
            }
        }
        if (nearTower) fallback.push_back(c);
        else preferred.push_back(c);
    }

    if (preferred.empty() && fallback.empty()) return cols_;
    bool usePreferred = !preferred.empty() && (fallback.empty() || randUnit(rng_) < 0.7);
    auto& pool = usePreferred ? preferred : fallback;
    std::size_t idx = static_cast<std::size_t>(rng_() % pool.size());
    return pool[idx];
}

Enemy AIController::makeEnemy(std::size_t column) {
    double roll = randUnit(rng_);
    EnemyType type = EnemyType::Basic;
    int hp = baseHP();
    int speed = 1;
    if (roll < 0.15) {
        type = EnemyType::Tank;
        hp = baseHP() + 2;
        speed = 1;
    } else if (roll < 0.40) {
        type = EnemyType::Scout;
        hp = std::max(1, baseHP() - 1);
        speed = 2;
    }
    return Enemy(topRow_, column, hp, baseAttack_, speed, type);
}

bool AIController::spawnEnemy(Grid& grid, const std::vector<Tower>& towers, std::vector<Enemy>& enemies) {
    std::size_t column = pickColumn(grid, towers);
    if (column >= cols_) return false;
    auto enemy = makeEnemy(column);
    enemies.push_back(enemy);
    grid.placeEnemy(topRow_, column);
    return true;
}
