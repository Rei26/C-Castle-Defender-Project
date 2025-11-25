#include "Grid.h"
using namespace std;

Grid::Grid(size_t rows, size_t cols)
    : rows_(rows), cols_(cols), data_(rows * cols, Cell::Empty) {}

bool Grid::inBounds(size_t r, size_t c) const {
    return r < rows_ && c < cols_;
}

Cell Grid::at(size_t r, size_t c) const {
    return inBounds(r, c) ? data_[idx(r, c)] : Cell::Blocked;
}

bool Grid::isEmpty(size_t r, size_t c) const {
    return inBounds(r, c) && data_[idx(r, c)] == Cell::Empty;
}

bool Grid::setCell(size_t r, size_t c, Cell value) {
    if (!inBounds(r, c)) return false;
    data_[idx(r, c)] = value;
    return true;
}

bool Grid::placeTower(size_t r, size_t c) {
    if (!isEmpty(r, c)) return false;
    return setCell(r, c, Cell::Tower);
}

bool Grid::removeTower(size_t r, size_t c) {
    if (!inBounds(r, c)) return false;
    if (at(r, c) != Cell::Tower) return false;
    return setCell(r, c, Cell::Empty);
}

bool Grid::placeEnemy(size_t r, size_t c) {
    if (!isEmpty(r, c)) return false;
    return setCell(r, c, Cell::Enemy);
}

bool Grid::removeEnemy(size_t r, size_t c) {
    if (!inBounds(r, c)) return false;
    if (at(r, c) != Cell::Enemy) return false;
    return setCell(r, c, Cell::Empty);
}

void Grid::clear() {
    fill(data_.begin(), data_.end(), Cell::Empty);
    if (castleSet_) {
        setCell(castleRC_.first, castleRC_.second, Cell::Castle);
    }
}

void Grid::setCastle(size_t r, size_t c) {
    if (!inBounds(r, c)) return;
    if (castleSet_) {
        setCell(castleRC_.first, castleRC_.second, Cell::Empty);
    }
    castleRC_ = {r, c};
    castleSet_ = true;
    setCell(r, c, Cell::Castle);
}
