#ifndef GRID_H
#define GRID_H

#include <vector>
#include <cstddef>
using namespace std;

enum class Cell : unsigned char {
    Empty = 0,
    Tower,
    Enemy,
    Blocked,
    Castle
};

class Grid {
public:
    Grid(size_t rows, size_t cols);

    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }

    bool inBounds(size_t r, size_t c) const;
    Cell at(size_t r, size_t c) const;

    bool isEmpty(size_t r, size_t c) const;
    bool setCell(size_t r, size_t c, Cell value);

    bool placeTower(size_t r, size_t c);
    bool removeTower(size_t r, size_t c);
    bool placeEnemy(size_t r, size_t c);
    bool removeEnemy(size_t r, size_t c);

    void clear();

    void setCastle(size_t r, size_t c);
    bool hasCastle() const { return castleSet_; }
    pair<size_t, size_t> castleRC() const { return castleRC_; }

private:
    size_t rows_;
    size_t cols_;
    vector<Cell> data_;
    bool castleSet_ = false;
    pair<size_t, size_t> castleRC_{0,0};

    size_t idx(size_t r, size_t c) const { return r * cols_ + c; }
};

#endif
