#pragma once
#include "unit.h"

class Knight : public Unit {
public:
    Knight(bool playerControlled, Position startPos)
        : Unit("Knight", 12, 4, 2, 2, playerControlled, startPos,
               playerControlled ? 'K' : 'k', "Shield Wall", 3) {}

    void useAbility(std::vector<std::unique_ptr<Unit>>& units) override;
};
