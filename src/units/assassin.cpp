#include "../../headers/units/assassin.h"
#include <iostream>

void Assassin::useAbility(std::vector<std::unique_ptr<Unit>>& units) {
    hidden = true;
    abilityCooldown = abilityCooldownMax;
    std::cout << name << " vanishes into the shadows! Untargetable until next turn.\n";
}
