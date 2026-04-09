#pragma once
#include "unit.h"

class Lancer : public Unit {
public:
    Lancer(bool playerControlled, Position startPos)
        : Unit("Lancer", 10, 3, 2, 4, playerControlled, startPos,
               playerControlled ? 'L' : 'l', "Charge", 2) {}

    void useAbility(std::vector<std::unique_ptr<Unit>>& units) override;
};
