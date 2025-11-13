#include "GameView.h"
#include <FL/fl_draw.H>

GameView::GameView(int X, int Y, int W, int H, Grid& grid, int cellPx)
    : Fl_Widget(X, Y, W, H), grid_(grid), cellPx_(cellPx) {}

void GameView::draw() {
    fl_push_clip(x(), y(), w(), h());
    fl_color(FL_WHITE);
    fl_rectf(x(), y(), w(), h());

    for (std::size_t r = 0; r < grid_.rows(); ++r) {
        for (std::size_t c = 0; c < grid_.cols(); ++c) {
            int cx = x() + static_cast<int>(c) * cellPx_;
            int cy = y() + static_cast<int>(r) * cellPx_;
            int cw = cellPx_;
            int ch = cellPx_;

            switch (grid_.at(r, c)) {
                case Cell::Empty:   fl_color(FL_WHITE); break;
                case Cell::Tower:   fl_color(FL_GREEN); break;
                case Cell::Enemy: {
                    EnemyType type = EnemyType::Basic;
                    if (enemyLookup_) {
                        auto info = enemyLookup_(r, c);
                        if (info.has_value()) type = info.value();
                    }
                    if (type == EnemyType::Basic) fl_color(FL_RED);
                    else if (type == EnemyType::Tank) fl_color(FL_MAGENTA);
                    else fl_color(fl_rgb_color(255, 140, 0));
                    break;
                }
                case Cell::Blocked: fl_color(FL_DARK3); break;
                case Cell::Castle:  fl_color(FL_BLUE); break;
            }
            fl_rectf(cx, cy, cw, ch);

            fl_color(FL_LIGHT2);
            fl_rect(cx, cy, cw, ch);
        }
    }

    fl_pop_clip();
}

int GameView::handle(int event) {
    switch (event) {
        case FL_PUSH: {
            if (!onClick_) return 0;
            int mx = Fl::event_x();
            int my = Fl::event_y();
            if (mx < x() || my < y() || mx >= x() + w() || my >= y() + h()) return 0;
            std::size_t c = static_cast<std::size_t>((mx - x()) / cellPx_);
            std::size_t r = static_cast<std::size_t>((my - y()) / cellPx_);
            onClick_(r, c);
            return 1;
        }
        default: break;
    }
    return Fl_Widget::handle(event);
}
