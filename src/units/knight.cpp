#include "../../headers/units/knight.h"
#include <iostream>

void Knight::useAbility(std::vector<std::unique_ptr<Unit>>& units) {
    shielded = true;
    abilityCooldown = abilityCooldownMax;
    std::cout << name << " raises Shield Wall! Damage halved until next turn.\n";
}
