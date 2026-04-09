#include "../../headers/combat/combat.h"
#include <iostream>

void Combat::performAttack(Unit& attacker, Unit& defender) {
    if (defender.hidden) {
        std::cout << defender.name << " is hidden and cannot be targeted!\n";
        return;
    }

    int damage = attacker.attackDamage;
    if (attacker.charged) {
        damage *= 2;
        attacker.charged = false;
    }
    if (defender.shielded) {
        damage /= 2;
    }

    std::cout << attacker.name << " attacks " << defender.name
         << " for " << damage << " damage!\n";

    defender.hp -= damage;

    if (defender.hp <= 0) {
        defender.hp = 0;
        defender.alive = false;
        std::cout << defender.name << " has been defeated!\n";
    } else {
        std::cout << defender.name << " now has " << defender.hp << " HP.\n";
    }
}

void Combat::handleAttack(Unit& unit, std::vector<std::unique_ptr<Unit>>& units) {
    std::vector<int> validTargets;

    std::cout << "Targets in range:\n";
    for (int i = 0; i < (int)units.size(); i++) {
        if (units[i]->alive && !units[i]->hidden && units[i]->isPlayer != unit.isPlayer && unit.isInRange(*units[i])) {
            std::cout << i << " - " << units[i]->name
                 << " (HP: " << units[i]->hp << ")\n";
            validTargets.push_back(i);
        }
    }

    if (validTargets.empty()) {
        std::cout << "No enemies are in range.\n";
        return;
    }

    int targetIndex;
    std::cout << "Enter target index: ";
    std::cin >> targetIndex;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input.\n";
        return;
    }

    bool found = false;
    for (int index : validTargets) {
        if (index == targetIndex) {
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "That is not a valid target.\n";
        return;
    }

    performAttack(unit, *units[targetIndex]);
}
