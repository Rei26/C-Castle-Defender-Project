#include "Tower.h"
#include <limits>
using namespace std;

static inline int manhattan(size_t r1, size_t c1, size_t r2, size_t c2) {
    size_t dr = (r1 > r2) ? (r1 - r2) : (r2 - r1);
    size_t dc = (c1 > c2) ? (c1 - c2) : (c2 - c1);
    return static_cast<int>(dr + dc);
}

optional<size_t> Tower::pickTarget(const vector<Enemy>& enemies) const {
    int bestDist = numeric_limits<int>::max();
    optional<size_t> bestIdx;
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (!enemies[i].alive()) continue;
        int d = manhattan(pos_.r, pos_.c, enemies[i].pos().r, enemies[i].pos().c);
        if (d <= range_) {
            if (!bestIdx.has_value() || d < bestDist) {
                bestDist = d;
                bestIdx = i;
            } else if (d == bestDist) {
                if (enemies[i].pos().r > enemies[bestIdx.value()].pos().r) {
                    bestIdx = i;
                }
            }
        }
    }
    return bestIdx;
}

void Tower::fire(vector<Enemy>& enemies) const {
    auto target = pickTarget(enemies);
    if (target.has_value()) {
        enemies[target.value()].takeDamage(damage_);
    }
}
