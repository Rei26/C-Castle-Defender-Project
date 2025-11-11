#include "Grid.h"

Grid::Grid(std::size_t rows, std::size_t cols)
    : rows_(rows), cols_(cols), data_(rows * cols, Cell::Empty) {}

bool Grid::inBounds(std::size_t r, std::size_t c) const {
    return r < rows_ && c < cols_;
}

Cell Grid::at(std::size_t r, std::size_t c) const {
    return inBounds(r, c) ? data_[idx(r, c)] : Cell::Blocked; // out-of-bounds acts like blocked
}

bool Grid::isEmpty(std::size_t r, std::size_t c) const {
    return inBounds(r, c) && data_[idx(r, c)] == Cell::Empty;
}

bool Grid::setCell(std::size_t r, std::size_t c, Cell value) {
    if (!inBounds(r, c)) return false;
    data_[idx(r, c)] = value;
    return true;
}

bool Grid::placeTower(std::size_t r, std::size_t c) {
    if (!isEmpty(r, c)) return false;
    return setCell(r, c, Cell::Tower);
}

bool Grid::removeTower(std::size_t r, std::size_t c) {
    if (!inBounds(r, c)) return false;
    if (at(r, c) != Cell::Tower) return false;
    return setCell(r, c, Cell::Empty);
}

bool Grid::placeEnemy(std::size_t r, std::size_t c) {
    if (!isEmpty(r, c)) return false;
    return setCell(r, c, Cell::Enemy);
}

bool Grid::removeEnemy(std::size_t r, std::size_t c) {
    if (!inBounds(r, c)) return false;
    if (at(r, c) != Cell::Enemy) return false;
    return setCell(r, c, Cell::Empty);
}

void Grid::clear() {
    std::fill(data_.begin(), data_.end(), Cell::Empty);
    if (castleSet_) {
        setCell(castleRC_.first, castleRC_.second, Cell::Castle);
    }
}

void Grid::setCastle(std::size_t r, std::size_t c) {
    if (!inBounds(r, c)) return;
    // wipe any previous castle
    if (castleSet_) {
        setCell(castleRC_.first, castleRC_.second, Cell::Empty);
    }
    castleRC_ = {r, c};
    castleSet_ = true;
    setCell(r, c, Cell::Castle);
}
