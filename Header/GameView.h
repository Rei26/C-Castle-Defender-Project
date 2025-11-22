#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <functional>
#include <optional>
#include "Grid.h"
#include "Enemy.h"

class GameView : public Fl_Widget {
public:
    GameView(int X, int Y, int W, int H, Grid& grid, int cellPx);

    void setCellSize(int px) { cellPx_ = px; redraw(); }
    void setOnClick(std::function<bool(std::size_t,std::size_t)> cb) { onClick_ = std::move(cb); }
    void setEnemyLookup(std::function<std::optional<EnemyType>(std::size_t,std::size_t)> cb) { enemyLookup_ = std::move(cb); }

protected:
    void draw() override;
    int handle(int event) override;

private:
    Grid& grid_;
    int cellPx_;
    std::function<bool(std::size_t,std::size_t)> onClick_;
    std::function<std::optional<EnemyType>(std::size_t,std::size_t)> enemyLookup_;
};

#endif
