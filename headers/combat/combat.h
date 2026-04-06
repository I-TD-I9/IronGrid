#pragma once
#include <vector>
#include <memory>
#include "../units/unit.h"

class Combat {
public:
    static void performAttack(Unit& attacker, Unit& defender);
    static void handleAttack(Unit& unit, std::vector<std::unique_ptr<Unit>>& units);
};
