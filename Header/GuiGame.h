#ifndef GUIGAME_H
#define GUIGAME_H

#include <vector>
#include <string>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <optional>
#include <random>
#include "Grid.h"
#include "Castle.h"
#include "Tower.h"
#include "Enemy.h"
#include "AIController.h"

class GameView;

class GuiGame {
public:
    static constexpr std::size_t ROWS = 20;
    static constexpr std::size_t COLS = 20;
    static constexpr int STARTING_CASTLE_HP = 100;
    static constexpr int TOWERS_TO_PLACE = 5;
    static constexpr int SCORE_PER_ENEMY = 10;

    GuiGame();

    Grid& grid() { return grid_; }
    const Grid& grid() const { return grid_; }

    void attachView(GameView* view);
    void attachStatus(Fl_Box* box) { status_ = box; updateStatus(); }
    void setUpgradeTypePublic(bool range) { upgradeRangeMode_ = range; updateStatus(); }

    bool startNextWave();
    void nextButtonAction();

    bool onCellClick(std::size_t r, std::size_t c);

    void stepTurn();

    void startAuto(double intervalSec = 1.0);
    void stopAuto();

    int score() const { return score_; }
    int turn() const { return turn_; }
    int towersPlaced() const { return static_cast<int>(towers_.size()); }
    bool isRunning() const { return running_; }
    bool waitingForWave() const { return waitingForWave_; }
    bool finished() const { return finished_; }

private:
    Grid grid_;
    Castle castle_;
    std::vector<Tower> towers_;
    std::vector<Enemy> enemies_;
    AIController ai_;

    int score_{0};
    int turn_{0};
    bool running_{false};
    bool finished_{false};
    double interval_{1.0};
    bool upgradeMode_{false};
    bool upgradeRangeMode_{false};
    int upgradePoints_{0};
    bool waitingForWave_{true};
    int enemiesDestroyed_{0};
    int currentWaveScore_{0};
    int currentWaveMax_{0};

    std::mt19937 rng_;
    std::string statusText_;

    GameView* view_{nullptr};
    Fl_Box* status_{nullptr};

    void rebuildGrid();
    void updateStatus();
    bool canPlaceTower(std::size_t r, std::size_t c) const;
    bool checkVictoryOrDefeat();
    void beginUpgradePhase(int points);
    bool tryUpgradeAt(std::size_t r, std::size_t c);
    void moveEnemies();
    void handleWaveCompletion();
    void showGameOver(bool playerWon);
    std::optional<EnemyType> lookupEnemy(std::size_t r, std::size_t c) const;

    static void TimerCB(void* userdata);
};

#endif // GUIGAME_H
