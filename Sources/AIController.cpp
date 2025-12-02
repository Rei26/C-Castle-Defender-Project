#include "AIController.h"
#include <algorithm>
#include <chrono>
using namespace std;

namespace {
double randUnit(mt19937& rng) {
    static uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}
}

AIController::AIController(size_t topRow, size_t cols)
    : topRow_(topRow), cols_(cols) {
    // Seed once per controller instance so runs differ.
    auto seed = static_cast<unsigned int>(
        chrono::high_resolution_clock::now().time_since_epoch().count());
    rng_.seed(seed);
}

void AIController::startNextWave() {
    // Only advance if the prior wave is finished and we have waves left.
    if (waveInProgress_ || currentWave_ >= totalWaves_) return;
    ++currentWave_;
    waveInProgress_ = true;
    spawnedThisWave_ = 0;
    waveDestroyed_ = 0;
    waveCastleDamage_ = 0;
}

void AIController::advanceTurn(Grid& grid, const vector<Tower>& towers, vector<Enemy>& enemies) {
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

size_t AIController::pickColumn(const Grid& grid, const vector<Tower>& towers) {
    vector<size_t> preferred;
    vector<size_t> fallback;
    for (size_t c = 0; c < cols_; ++c) {
        if (!grid.isEmpty(topRow_, c)) continue;
        bool nearTower = false;
        for (const auto& t : towers) {
            if (t.pos().r == 0) continue;
            size_t tc = t.pos().c;
            size_t diff = (tc > c) ? (tc - c) : (c - tc);
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
    size_t idx = static_cast<size_t>(rng_() % pool.size());
    return pool[idx];
}

Enemy AIController::makeEnemy(size_t column) {
    // Mix basic, tanky, and speedy enemies with weighted rolls.
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
        hp = max(1, baseHP() - 1);
        speed = 2;
    }
    return Enemy(topRow_, column, hp, baseAttack_, speed, type);
}

size_t AIController::pickRandomColumn(const Grid& grid) {
    vector<size_t> open;
    for (size_t c = 0; c < cols_; ++c) {
        if (grid.isEmpty(topRow_, c)) open.push_back(c);
    }
    if (open.empty()) return cols_;
    size_t idx = static_cast<size_t>(rng_() % open.size());
    return open[idx];
}

bool AIController::spawnEnemy(Grid& grid, const vector<Tower>& towers, vector<Enemy>& enemies) {
    bool firstWave = (currentWave_ == 1);
    // First wave uses pure randomness; later waves bias away from towers.
    size_t column = firstWave ? pickRandomColumn(grid) : pickColumn(grid, towers);
    if (column >= cols_) return false;
    auto enemy = makeEnemy(column);
    enemies.push_back(enemy);
    grid.placeEnemy(topRow_, column);
    return true;
}
