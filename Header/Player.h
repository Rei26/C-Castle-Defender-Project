#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <iostream>
#include "Grid.h"
#include "Tower.h"
#include "Castle.h"
using namespace std;

// Collects user input for initial tower placement in console mode.
class Player {
public:
    void placeTowers(int count, Grid& grid, const Castle& castle, vector<Tower>& towers) {
        cout << "Place exactly " << count << " towers (1-based row col)." << endl;
        cout << "Grid size: " << grid.rows() << "x" << grid.cols() << "; Castle at ("
                  << (castle.pos().r + 1) << ", " << (castle.pos().c + 1) << ")." << endl;
        int placed = 0;
        while (placed < count) {
            cout << "Tower " << (placed + 1) << " of " << count << " - enter row col: ";
            long rr, cc; if (!(cin >> rr >> cc)) { cin.clear(); cin.ignore(1024, '\n'); continue; }
            if (rr < 1 || cc < 1) { cout << "Use positive coordinates.\n"; continue; }
            size_t r = static_cast<size_t>(rr - 1);
            size_t c = static_cast<size_t>(cc - 1);
            if (!grid.inBounds(r, c)) { cout << "Out of bounds.\n"; continue; }
            if (grid.at(r, c) != Cell::Empty) { cout << "Cell not empty.\n"; continue; }
            if (r <= 1) { cout << "Top two rows are reserved for spawns.\n"; continue; }
            if (r == castle.pos().r && c == castle.pos().c) { cout << "Cannot place on castle.\n"; continue; }

            Tower t(r, c, 3, 2);
            towers.push_back(t);
            grid.placeTower(r, c);
            ++placed;
        }
    }
};

#endif

