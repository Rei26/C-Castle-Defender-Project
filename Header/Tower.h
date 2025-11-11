#ifndef TOWER_H
#define TOWER_H

#include <vector>
#include <cstddef>
#include <optional>
#include "Position.h"
#include "Enemy.h"

class Tower {
public:
    Tower(std::size_t r, std::size_t c, int range, int damage)
        : pos_{r, c}, range_(range), damage_(damage) {}

    const Position& pos() const { return pos_; }
    int range() const { return range_; }
    int damage() const { return damage_; }

    // Returns index of targeted enemy if any
    std::optional<std::size_t> pickTarget(const std::vector<Enemy>& enemies) const;
    void fire(std::vector<Enemy>& enemies) const;

    void upgradeDamage(int by = 1) { damage_ += by; }
    void upgradeRange(int by = 1) { range_ += by; }

private:
    Position pos_{};
    int range_{3};
    int damage_{2};
};

#endif // TOWER_H
