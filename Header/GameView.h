#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <functional>
#include <optional>
#include "Grid.h"
#include "Enemy.h"
using namespace std;

class GameView : public Fl_Widget {
public:
    GameView(int X, int Y, int W, int H, Grid& grid, int cellPx);

    void setCellSize(int px) { cellPx_ = px; redraw(); }
    void setOnClick(function<bool(size_t,size_t)> cb) { onClick_ = move(cb); }
    void setEnemyLookup(function<optional<EnemyType>(size_t,size_t)> cb) { enemyLookup_ = move(cb); }

protected:
    void draw() override;
    int handle(int event) override;

private:
    Grid& grid_;
    int cellPx_;
    function<bool(size_t,size_t)> onClick_;
    function<optional<EnemyType>(size_t,size_t)> enemyLookup_;
};

#endif
