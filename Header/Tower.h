#ifndef TOWER_H
#define TOWER_H

#include <vector>
#include <cstddef>
#include <optional>
#include "Position.h"
#include "Enemy.h"
using namespace std;

class Tower {
public:
    Tower(size_t r, size_t c, int range, int damage)
        : pos_{r, c}, range_(range), damage_(damage) {}

    // Position and stats accessors.
    const Position& pos() const { return pos_; }
    int range() const { return range_; }
    int damage() const { return damage_; }

    // Choose and attack a target based on Manhattan distance.
    optional<size_t> pickTarget(const vector<Enemy>& enemies) const;
    void fire(vector<Enemy>& enemies) const;

    void upgradeDamage(int by = 1) { damage_ += by; }
    void upgradeRange(int by = 1) { range_ += by; }

private:
    Position pos_{};
    int range_{3};
    int damage_{2};
};

#endif
