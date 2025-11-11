#ifndef ENEMY_H
#define ENEMY_H

#include <cstddef>
#include "Position.h"

enum class EnemyType : unsigned char {
    Basic,
    Tank,
    Scout
};

class Enemy {
public:
    Enemy(std::size_t r, std::size_t c, int hp, int atk, int spd, EnemyType type)
        : pos_{r, c}, hp_(hp), attack_(atk), speed_(spd), type_(type) {}

    const Position& pos() const { return pos_; }
    Position& pos() { return pos_; }

    int hp() const { return hp_; }
    int attack() const { return attack_; }
    int speed() const { return speed_; }
    EnemyType type() const { return type_; }
    bool alive() const { return hp_ > 0; }

    void takeDamage(int dmg) { hp_ -= dmg; }
    void stepDown() { pos_.r += static_cast<std::size_t>(speed_); }

private:
    Position pos_{};
    int hp_{1};
    int attack_{1};
    int speed_{1};
    EnemyType type_{EnemyType::Basic};
};

#endif // ENEMY_H
