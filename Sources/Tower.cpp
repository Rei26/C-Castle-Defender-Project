#include "Tower.h"
#include <limits>

static inline int manhattan(std::size_t r1, std::size_t c1, std::size_t r2, std::size_t c2) {
    std::size_t dr = (r1 > r2) ? (r1 - r2) : (r2 - r1);
    std::size_t dc = (c1 > c2) ? (c1 - c2) : (c2 - c1);
    return static_cast<int>(dr + dc);
}

std::optional<std::size_t> Tower::pickTarget(const std::vector<Enemy>& enemies) const {
    int bestDist = std::numeric_limits<int>::max();
    std::optional<std::size_t> bestIdx;
    for (std::size_t i = 0; i < enemies.size(); ++i) {
        if (!enemies[i].alive()) continue;
        int d = manhattan(pos_.r, pos_.c, enemies[i].pos().r, enemies[i].pos().c);
        if (d <= range_) {
            if (!bestIdx.has_value() || d < bestDist) {
                bestDist = d;
                bestIdx = i;
            } else if (d == bestDist) {
                // tie-breaker: enemy closer to the castle row (larger r)
                if (enemies[i].pos().r > enemies[bestIdx.value()].pos().r) {
                    bestIdx = i;
                }
            }
        }
    }
    return bestIdx;
}

void Tower::fire(std::vector<Enemy>& enemies) const {
    auto target = pickTarget(enemies);
    if (target.has_value()) {
        enemies[target.value()].takeDamage(damage_);
    }
}
