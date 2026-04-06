#pragma once
#include "unit.h"

class Archer : public Unit {
public:
    Archer(bool playerControlled, Position startPos)
        : Unit("Archer", 8, 2, 1, 5, playerControlled, startPos,
               playerControlled ? 'A' : 'a') {}
};
