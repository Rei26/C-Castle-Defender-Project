#include "GuiGame.h"
#include "GameView.h"
#include <algorithm>
#include <string>
#include <cstdio>
#include <chrono>
#include <FL/fl_ask.H>

GuiGame::GuiGame()
    : grid_(ROWS, COLS),
      castle_(ROWS - 1, COLS / 2, STARTING_CASTLE_HP),
      ai_(0, COLS) {
    grid_.setCastle(castle_.pos().r, castle_.pos().c);
    rng_.seed(static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));
}

void GuiGame::attachView(GameView* view) {
    view_ = view;
    if (!view_) return;
    view_->setEnemyLookup([this](std::size_t r, std::size_t c){ return lookupEnemy(r, c); });
}

void GuiGame::rebuildGrid() {
    grid_.clear();
    grid_.setCastle(castle_.pos().r, castle_.pos().c);
    for (const auto& t : towers_) {
        grid_.placeTower(t.pos().r, t.pos().c);
    }
    for (const auto& e : enemies_) {
        if (e.alive()) grid_.placeEnemy(e.pos().r, e.pos().c);
    }
}

bool GuiGame::canPlaceTower(std::size_t r, std::size_t c) const {
    if (!grid_.inBounds(r, c)) return false;
    if (grid_.at(r, c) != Cell::Empty) return false;
    if (r == 0) return false;
    if (r == castle_.pos().r && c == castle_.pos().c) return false;
    return true;
}

std::optional<EnemyType> GuiGame::lookupEnemy(std::size_t r, std::size_t c) const {
    for (const auto& e : enemies_) {
        if (!e.alive()) continue;
        if (e.pos().r == r && e.pos().c == c) return e.type();
    }
    return std::nullopt;
}

bool GuiGame::onCellClick(std::size_t r, std::size_t c) {
    if (finished_) return false;
    if (upgradeMode_) return tryUpgradeAt(r, c);
    if (towers_.size() >= TOWERS_TO_PLACE) return false;
    if (!canPlaceTower(r, c)) return false;
    towers_.emplace_back(r, c, 3, 2);
    grid_.placeTower(r, c);
    updateStatus();
    if (view_) view_->redraw();
    return true;
}

void GuiGame::updateStatus() {
    if (!status_) return;
    statusText_ = "Turn: " + std::to_string(turn_);
    statusText_ += "  Score: " + std::to_string(score_);
    statusText_ += "  Castle HP: " + std::to_string(castle_.hp());
    statusText_ += "  Towers: " + std::to_string(towers_.size()) + "/" + std::to_string(TOWERS_TO_PLACE);
    statusText_ += "  Wave: " + std::to_string(ai_.currentWaveNumber()) + "/" + std::to_string(ai_.totalWaves());
    if (running_) statusText_ += "  (Auto)";
    if (waitingForWave_) statusText_ += "  [Waiting for wave]";
    if (upgradeMode_) {
        statusText_ += "  [UPGRADE]";
        statusText_ += upgradeRangeMode_ ? " (Range)" : " (Damage)";
        statusText_ += " Pts:" + std::to_string(upgradePoints_);
    }
    status_->label(statusText_.c_str());
    status_->redraw();
}

bool GuiGame::startNextWave() {
    if (finished_ || waitingForWave_ == false || !ai_.hasMoreWaves()) return false;
    if (towers_.size() < TOWERS_TO_PLACE) return false;
    if (upgradeMode_ && upgradePoints_ > 0) {
        int res = fl_choice("You still have upgrade points. Start the wave and forfeit them?", "Cancel", "Start Wave", nullptr);
        if (res != 1) {
            return false;
        }
        upgradePoints_ = 0;
        upgradeMode_ = false;
    }
    ai_.startNextWave();
    waitingForWave_ = false;
    upgradeMode_ = false;
    upgradePoints_ = 0;
    currentWaveScore_ = 0;
    currentWaveMax_ = ai_.enemiesPerWave() * SCORE_PER_ENEMY;
    updateStatus();
    return true;
}

void GuiGame::nextButtonAction() {
    if (waitingForWave_) {
        startNextWave();
        return;
    }
    stepTurn();
}

void GuiGame::moveEnemies() {
    std::vector<std::vector<bool>> occupied(ROWS, std::vector<bool>(COLS, false));
    for (const auto& t : towers_) occupied[t.pos().r][t.pos().c] = true;
    for (const auto& e : enemies_) {
        if (e.pos().r < ROWS && e.pos().c < COLS) occupied[e.pos().r][e.pos().c] = true;
    }

    auto randomDir = [&]() { return (rng_() % 2 == 0) ? -1 : 1; };

    for (auto& e : enemies_) {
        if (e.pos().r < ROWS && e.pos().c < COLS)
            occupied[e.pos().r][e.pos().c] = false;
        for (int step = 0; step < e.speed(); ++step) {
            std::size_t nextR = e.pos().r + 1;
            if (nextR >= ROWS) { e.pos().r = nextR; break; }
            if (!occupied[nextR][e.pos().c]) {
                e.pos().r = nextR;
                continue;
            }
            int dirs[2] = { randomDir(), 0 };
            dirs[1] = -dirs[0];
            bool moved = false;
            for (int d : dirs) {
                if ((d < 0 && e.pos().c == 0) || (d > 0 && e.pos().c + 1 >= COLS)) continue;
                std::size_t newC = static_cast<std::size_t>(static_cast<int>(e.pos().c) + d);
                if (!occupied[nextR][newC]) {
                    e.pos().r = nextR;
                    e.pos().c = newC;
                    moved = true;
                    break;
                }
            }
            if (!moved) break;
        }
        if (e.pos().r < ROWS && e.pos().c < COLS)
            occupied[e.pos().r][e.pos().c] = true;
    }
}

void GuiGame::startAuto(double intervalSec) {
    if (finished_ || waitingForWave_) return;
    if (towers_.size() < TOWERS_TO_PLACE) return;
    interval_ = intervalSec;
    if (!running_) {
        running_ = true;
        Fl::add_timeout(interval_, TimerCB, this);
        updateStatus();
    }
}

void GuiGame::stopAuto() {
    if (running_) {
        running_ = false;
        updateStatus();
    }
}

void GuiGame::beginUpgradePhase(int points) {
    upgradePoints_ += points;
    upgradeMode_ = true;
    running_ = false;
    waitingForWave_ = true;
    updateStatus();
}

bool GuiGame::tryUpgradeAt(std::size_t r, std::size_t c) {
    if (!upgradeMode_ || upgradePoints_ <= 0) return false;
    auto it = std::find_if(towers_.begin(), towers_.end(), [&](const Tower& t){ return t.pos().r == r && t.pos().c == c; });
    if (it == towers_.end()) return false;
    if (upgradeRangeMode_) const_cast<Tower&>(*it).upgradeRange();
    else const_cast<Tower&>(*it).upgradeDamage();
    --upgradePoints_;
    if (upgradePoints_ <= 0) upgradeMode_ = false;
    updateStatus();
    if (view_) view_->redraw();
    return true;
}

void GuiGame::handleWaveCompletion() {
    bool excellent = ai_.finalizeWave(currentWaveScore_, currentWaveMax_);
    currentWaveScore_ = 0;
    currentWaveMax_ = 0;
    castle_.setHP(STARTING_CASTLE_HP);
    if (ai_.hasMoreWaves()) {
        beginUpgradePhase(1);
        fl_message(excellent ? "Wave complete! Enemy HP increased." : "Wave complete.");
    } else {
        waitingForWave_ = false;
        upgradeMode_ = false;
        upgradePoints_ = 0;
        updateStatus();
        fl_message(excellent ? "Final wave complete! Enemy HP increased for next playthrough." : "All waves defeated!");
        checkVictoryOrDefeat();
    }
}

void GuiGame::showGameOver(bool playerWon) {
    finished_ = true;
    running_ = false;
    waitingForWave_ = false;
    char buf[256];
    std::snprintf(buf, sizeof(buf),
                  "GAME OVER\nPlayer Score: %d\nEnemies Destroyed: %d\nCastle Health: %d\nWinner: %s",
                  score_, enemiesDestroyed_, castle_.hp(), playerWon ? "Player" : "AI");
    fl_message("%s", buf);
}

bool GuiGame::checkVictoryOrDefeat() {
    if (!castle_.alive()) {
        showGameOver(false);
        return true;
    }
    if (ai_.allWavesFinished() && enemies_.empty()) {
        showGameOver(true);
        return true;
    }
    return false;
}

void GuiGame::stepTurn() {
    if (finished_ || waitingForWave_) return;
    if (towers_.size() < TOWERS_TO_PLACE) return;

    ++turn_;
    rebuildGrid();
    ai_.advanceTurn(grid_, towers_, enemies_);

    for (const auto& t : towers_) {
        t.fire(enemies_);
    }

    int destroyedThisTurn = 0;
    enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [&](const Enemy& e){
        if (!e.alive()) { ++destroyedThisTurn; return true; }
        return false;
    }), enemies_.end());
    if (destroyedThisTurn > 0) {
        ai_.notifyDestroyed(destroyedThisTurn);
        enemiesDestroyed_ += destroyedThisTurn;
        score_ += destroyedThisTurn * SCORE_PER_ENEMY;
        currentWaveScore_ += destroyedThisTurn * SCORE_PER_ENEMY;
    }

    moveEnemies();

    int totalDamage = 0;
    enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [&](const Enemy& e){
        if (e.pos().r >= castle_.pos().r) {
            totalDamage += e.attack();
            return true;
        }
        return false;
    }), enemies_.end());
    if (totalDamage > 0) {
        castle_.takeDamage(totalDamage);
        ai_.notifyCastleDamage(totalDamage);
    }

    rebuildGrid();
    updateStatus();
    if (view_) view_->redraw();

    if (checkVictoryOrDefeat()) {
        running_ = false;
        return;
    }

    if (ai_.spawnQuotaMet() && enemies_.empty()) {
        handleWaveCompletion();
    }
}

void GuiGame::TimerCB(void* userdata) {
    auto* self = static_cast<GuiGame*>(userdata);
    if (!self) return;
    if (!self->running_) return;
    self->stepTurn();
    if (self->running_ && !self->finished_ && !self->waitingForWave_) {
        Fl::repeat_timeout(self->interval_, TimerCB, userdata);
    }
}
