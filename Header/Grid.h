#ifndef GRID_H
#define GRID_H

#include <vector>
#include <cstddef>

enum class Cell : unsigned char {
    Empty = 0,
    Tower,
    Enemy,
    Blocked,
    Castle
};

class Grid {
public:
    Grid(std::size_t rows, std::size_t cols);

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }

    bool inBounds(std::size_t r, std::size_t c) const;
    Cell at(std::size_t r, std::size_t c) const;

    bool isEmpty(std::size_t r, std::size_t c) const;
    bool setCell(std::size_t r, std::size_t c, Cell value);

    bool placeTower(std::size_t r, std::size_t c);
    bool removeTower(std::size_t r, std::size_t c);
    bool placeEnemy(std::size_t r, std::size_t c);
    bool removeEnemy(std::size_t r, std::size_t c);

    void clear();

    void setCastle(std::size_t r, std::size_t c);
    bool hasCastle() const { return castleSet_; }
    std::pair<std::size_t, std::size_t> castleRC() const { return castleRC_; }

private:
    std::size_t rows_;
    std::size_t cols_;
    std::vector<Cell> data_;
    bool castleSet_ = false;
    std::pair<std::size_t, std::size_t> castleRC_{0,0};

    std::size_t idx(std::size_t r, std::size_t c) const { return r * cols_ + c; }
};

#endif // GRID_H
