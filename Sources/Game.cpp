#include "Game.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <limits>
using namespace std;

namespace {
bool useAsciiSymbols() {
#if defined(_WIN32)
    return true;
#else
    return false;
#endif
}
}

Game::Game()
    : grid_(ROWS, COLS),
      castle_(ROWS - 1, COLS / 2, STARTING_CASTLE_HP),
      ai_(0, COLS),
      ascii_(useAsciiSymbols()) {
    grid_.setCastle(castle_.pos().r, castle_.pos().c);
    rng_.seed(static_cast<unsigned int>(
        chrono::high_resolution_clock::now().time_since_epoch().count()));
}

void Game::rebuildGrid() {
    grid_.clear();
    grid_.setCastle(castle_.pos().r, castle_.pos().c);
    for (const auto& t : towers_) {
        grid_.placeTower(t.pos().r, t.pos().c);
    }
    for (const auto& e : enemies_) {
        if (e.alive()) grid_.placeEnemy(e.pos().r, e.pos().c);
    }
}

string Game::cellSymbol(size_t r, size_t c) const {
    if (ascii_) {
        switch (grid_.at(r, c)) {
            case Cell::Empty:   return ".";
            case Cell::Tower:   return "T";
            case Cell::Enemy:   return "E";
            case Cell::Blocked: return "#";
            case Cell::Castle:  return "C";
        }
    } else {
        switch (grid_.at(r, c)) {
            case Cell::Empty:   return "☐";
            case Cell::Tower:   return "🧱";
            case Cell::Enemy:   return "👾";
            case Cell::Blocked: return "■";
            case Cell::Castle:  return "🏰";
        }
    }
    return "?";
}

void Game::printGrid() const {
    for (size_t r = 0; r < grid_.rows(); ++r) {
        for (size_t c = 0; c < grid_.cols(); ++c) {
            cout << cellSymbol(r, c) << ' ';
        }
        cout << '\n';
    }
}

void Game::ensureWaveReady() {
    if (!waitingForWave_ || !ai_.hasMoreWaves()) return;
    cout << "\nWave " << ai_.currentWaveNumber() << " ready. Press Enter to launch.";
    string line;
    getline(cin, line);
    ai_.startNextWave();
    waitingForWave_ = false;
    currentWaveScore_ = 0;
    currentWaveMaxScore_ = ai_.enemiesPerWave() * SCORE_PER_ENEMY;
}

void Game::performUpgrades() {
    int points = 1;
    cout << "\nUpgrade phase: " << points << " point available." << endl;
    while (points > 0) {
        cout << "Choose tower row col (1-based, 0 0 to skip): ";
        long rr, cc;
        if (!(cin >> rr >> cc)) {
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }
        if (rr == 0 && cc == 0) break;
        if (rr < 1 || cc < 1) {
            cout << "Use positive coordinates.\n";
            continue;
        }
        size_t r = static_cast<size_t>(rr - 1);
        size_t c = static_cast<size_t>(cc - 1);
        auto it = find_if(towers_.begin(), towers_.end(), [&](const Tower& t){ return t.pos().r == r && t.pos().c == c; });
        if (it == towers_.end()) {
            cout << "No tower at that location.\n";
            continue;
        }
        cout << "Upgrade (d=damage, r=range): ";
        char choice;
        cin >> choice;
        if (choice == 'd' || choice == 'D') {
            const_cast<Tower&>(*it).upgradeDamage();
            --points;
            cout << "Damage increased.\n";
        } else if (choice == 'r' || choice == 'R') {
            const_cast<Tower&>(*it).upgradeRange();
            --points;
            cout << "Range increased.\n";
        } else {
            cout << "Invalid choice.\n";
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void Game::moveEnemies() {
    vector<vector<bool>> occupied(ROWS, vector<bool>(COLS, false));
    for (const auto& t : towers_) {
        occupied[t.pos().r][t.pos().c] = true;
    }
    for (const auto& e : enemies_) {
        if (e.pos().r < ROWS && e.pos().c < COLS)
            occupied[e.pos().r][e.pos().c] = true;
    }

    auto randomDir = [&]() { return (rng_() % 2 == 0) ? -1 : 1; };

    for (auto& e : enemies_) {
        if (e.pos().r < ROWS && e.pos().c < COLS)
            occupied[e.pos().r][e.pos().c] = false;
        for (int step = 0; step < e.speed(); ++step) {
            size_t nextR = e.pos().r + 1;
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
                size_t newC = static_cast<size_t>(static_cast<int>(e.pos().c) + d);
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

void Game::waveCompleted(bool excellent) {
    waitingForWave_ = ai_.hasMoreWaves();
    currentWaveScore_ = 0;
    currentWaveMaxScore_ = 0;
    cout << "\n--- Wave complete ---";
    if (excellent) cout << " (Enemy HP increased!)";
    cout << '\n';
    performUpgrades();
    rebuildGrid();
    printGrid();
}

void Game::showGameOver(bool playerWon) const {
    cout << "\nGAME OVER\n";
    cout << "Player Score: " << score_ << '\n';
    cout << "Enemies Destroyed: " << enemiesDestroyed_ << '\n';
    cout << "Castle Health: " << castle_.hp() << '\n';
    cout << "Winner: " << (playerWon ? "Player" : "AI") << '\n';
}

void Game::run() {
    cout << "\n=== Castle Defender (Console) ===\n";
    cout << "Board: " << ROWS << "x" << COLS << "\n";
    cout << "Place exactly " << TOWERS_TO_PLACE << " towers.\n";

    rebuildGrid();
    printGrid();
    player_.placeTowers(TOWERS_TO_PLACE, grid_, castle_, towers_);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    while (castle_.alive() && (!ai_.allWavesFinished() || !enemies_.empty())) {
        ensureWaveReady();
        if (!ai_.waveInProgress()) {
            break;
        }

        ++turn_;
        rebuildGrid();
        ai_.advanceTurn(grid_, towers_, enemies_);

        for (const auto& t : towers_) {
            t.fire(enemies_);
        }

        int destroyedThisTurn = 0;
        enemies_.erase(remove_if(enemies_.begin(), enemies_.end(), [&](const Enemy& e){
            if (!e.alive()) { ++destroyedThisTurn; score_ += SCORE_PER_ENEMY; return true; }
            return false;
        }), enemies_.end());
        if (destroyedThisTurn > 0) {
            ai_.notifyDestroyed(destroyedThisTurn);
            enemiesDestroyed_ += destroyedThisTurn;
            currentWaveScore_ += destroyedThisTurn * SCORE_PER_ENEMY;
        }

        moveEnemies();

        int totalDamage = 0;
        enemies_.erase(remove_if(enemies_.begin(), enemies_.end(), [&](const Enemy& e){
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
        cout << "\nTurn " << turn_ << " | Castle HP: " << castle_.hp() << " | Score: " << score_;
        cout << " | Wave: " << ai_.currentWaveNumber() << "\n";
        printGrid();

        if (!castle_.alive()) {
            showGameOver(false);
            return;
        }

        if (ai_.spawnQuotaMet() && enemies_.empty()) {
            waveCompleted(ai_.finalizeWave(currentWaveScore_, currentWaveMaxScore_));
            continue;
        }

        cout << "Press Enter for next turn (q + Enter to quit): ";
        string line;
        getline(cin, line);
        if (!line.empty() && (line[0] == 'q' || line[0] == 'Q')) {
            cout << "\nExiting early.\n";
            break;
        }
    }

    showGameOver(castle_.alive());
}
