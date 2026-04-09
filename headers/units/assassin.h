#pragma once
#include "unit.h"

class Assassin : public Unit {
public:
    Assassin(bool playerControlled, Position startPos)
        : Unit("Assassin", 9, 5, 3, 1, playerControlled, startPos,
               playerControlled ? 'S' : 's', "Vanish", 3) {}

    void useAbility(std::vector<std::unique_ptr<Unit>>& units) override;
};
