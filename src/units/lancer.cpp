#include "../../headers/units/lancer.h"
#include <iostream>

void Lancer::useAbility(std::vector<std::unique_ptr<Unit>>& units) {
    charged = true;
    abilityCooldown = abilityCooldownMax;
    std::cout << name << " prepares to Charge! Next attack deals double damage.\n";
}
