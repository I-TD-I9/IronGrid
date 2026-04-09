#include "../../headers/units/archer.h"
#include <iostream>

void Archer::useAbility(std::vector<std::unique_ptr<Unit>>& units) {
    abilityCooldown = abilityCooldownMax;
    std::cout << name << " fires a Volley!\n";

    bool hitAny = false;
    for (auto& target : units) {
        if (!target->alive || target->isPlayer == isPlayer) continue;
        if (!isInRange(*target)) continue;
        if (target->hidden) {
            std::cout << "  " << target->name << " is hidden and dodges!\n";
            continue;
        }

        int damage = attackDamage;
        if (target->shielded) damage /= 2;

        target->hp -= damage;
        std::cout << "  " << target->name << " takes " << damage << " damage!";

        if (target->hp <= 0) {
            target->hp = 0;
            target->alive = false;
            std::cout << " Defeated!";
        }
        std::cout << "\n";
        hitAny = true;
    }

    if (!hitAny) {
        std::cout << "  No enemies in range.\n";
    }
}
