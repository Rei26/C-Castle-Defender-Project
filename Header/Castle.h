#ifndef CASTLE_H
#define CASTLE_H

#include <cstddef>
#include "Position.h"
using namespace std;

// Player base that loses when its HP drops to zero.
class Castle {
public:
    Castle(size_t r, size_t c, int hp)
        : pos_{r, c}, hp_(hp) {}

    const Position& pos() const { return pos_; }
    int hp() const { return hp_; }
    bool alive() const { return hp_ > 0; }
    void takeDamage(int dmg) { hp_ -= dmg; }
    void setHP(int value) { hp_ = value; }

private:
    Position pos_{};
    int hp_{100};
};

#endif
