#include "../../headers/ai/enemy_ai.h"
#include "../../headers/combat/combat.h"
#include <iostream>
#include <cmath>
#include <limits>

void EnemyAI::takeTurn(Unit& unit, std::vector<std::unique_ptr<Unit>>& units, Grid& grid) {
    std::cout << unit.name << " is taking its turn...\n";

    int targetIndex = findClosestPlayer(unit, units);

    if (targetIndex == -1) {
        std::cout << "No player targets found.\n";
        return;
    }

    Unit& target = *units[targetIndex];

    // Attack if already in range.
    if (unit.isInRange(target)) {
        Combat::performAttack(unit, target);
        return;
    }

    // Otherwise move one step closer.
    Position newPos = unit.pos;

    if (target.pos.x < unit.pos.x) {
        newPos.x--;
    } else if (target.pos.x > unit.pos.x) {
        newPos.x++;
    } else if (target.pos.y < unit.pos.y) {
        newPos.y--;
    } else if (target.pos.y > unit.pos.y) {
        newPos.y++;
    }

    if (grid.isPositionInsideGrid(newPos) && grid.getUnitAtPosition(newPos, units) == -1) {
        std::cout << unit.name << " moves from (" << unit.pos.x << ", " << unit.pos.y
             << ") to (" << newPos.x << ", " << newPos.y << ").\n";
        unit.pos = newPos;
    } else {
        std::cout << unit.name << " could not move.\n";
    }

    // Try to attack after moving.
    if (unit.isInRange(target) && target.alive) {
        Combat::performAttack(unit, target);
    }
}

int EnemyAI::findClosestPlayer(const Unit& enemyUnit, const std::vector<std::unique_ptr<Unit>>& units) {
    int closestIndex = -1;
    int closestDistance = std::numeric_limits<int>::max();

    for (int i = 0; i < (int)units.size(); i++) {
        if (!units[i]->alive || !units[i]->isPlayer) {
            continue;
        }

        int distance = abs(enemyUnit.pos.x - units[i]->pos.x)
                     + abs(enemyUnit.pos.y - units[i]->pos.y);

        if (distance < closestDistance) {
            closestDistance = distance;
            closestIndex = i;
        }
    }

    return closestIndex;
}
