#ifndef POSITION_H
#define POSITION_H

#include <cstddef>
using namespace std;

// Row/column tuple used by all board entities.
struct Position {
    size_t r{0};
    size_t c{0};
};

#endif
