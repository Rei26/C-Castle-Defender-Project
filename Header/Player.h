#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <iostream>
#include "Grid.h"
#include "Tower.h"
#include "Castle.h"

class Player {
public:
    // Places exactly count towers by reading from std::cin.
    // Uses 1-based input for convenience. Validates positions.
    void placeTowers(int count, Grid& grid, const Castle& castle, std::vector<Tower>& towers) {
        std::cout << "Place exactly " << count << " towers (1-based row col)." << std::endl;
        std::cout << "Grid size: " << grid.rows() << "x" << grid.cols() << "; Castle at ("
                  << (castle.pos().r + 1) << ", " << (castle.pos().c + 1) << ")." << std::endl;
        int placed = 0;
        while (placed < count) {
            std::cout << "Tower " << (placed + 1) << " of " << count << " - enter row col: ";
            long rr, cc; if (!(std::cin >> rr >> cc)) { std::cin.clear(); std::cin.ignore(1024, '\n'); continue; }
            if (rr < 1 || cc < 1) { std::cout << "Use positive coordinates.\n"; continue; }
            std::size_t r = static_cast<std::size_t>(rr - 1);
            std::size_t c = static_cast<std::size_t>(cc - 1);
            if (!grid.inBounds(r, c)) { std::cout << "Out of bounds.\n"; continue; }
            if (grid.at(r, c) != Cell::Empty) { std::cout << "Cell not empty.\n"; continue; }
            // Avoid top row (spawns) and castle cell
            if (r == 0) { std::cout << "Top row is reserved for spawns.\n"; continue; }
            if (r == castle.pos().r && c == castle.pos().c) { std::cout << "Cannot place on castle.\n"; continue; }

            // fixed stats for simplicity
            Tower t(r, c, /*range*/3, /*damage*/2);
            towers.push_back(t);
            grid.placeTower(r, c);
            ++placed;
        }
    }
};

#endif // PLAYER_H

