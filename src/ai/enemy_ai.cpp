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

    // Move toward the target using full move range.
    moveToward(unit, target, units, grid);

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

// Move the unit toward the target, one step at a time, up to moveRange.
void EnemyAI::moveToward(Unit& unit, const Unit& target, std::vector<std::unique_ptr<Unit>>& units, Grid& grid) {
    Position startPos = unit.pos;

    for (int step = 0; step < unit.moveRange; step++) {
        int dx = target.pos.x - unit.pos.x;
        int dy = target.pos.y - unit.pos.y;

        if (dx == 0 && dy == 0) break;

        // Try the axis with the larger gap first.
        Position tryPos1 = unit.pos;
        Position tryPos2 = unit.pos;

        if (abs(dx) >= abs(dy)) {
            tryPos1.x += (dx > 0) ? 1 : -1;
            tryPos2.y += (dy > 0) ? 1 : (dy < 0 ? -1 : 0);
        } else {
            tryPos1.y += (dy > 0) ? 1 : -1;
            tryPos2.x += (dx > 0) ? 1 : (dx < 0 ? -1 : 0);
        }

        if (grid.isPositionInsideGrid(tryPos1) && grid.getUnitAtPosition(tryPos1, units) == -1) {
            unit.pos = tryPos1;
        } else if (grid.isPositionInsideGrid(tryPos2) && grid.getUnitAtPosition(tryPos2, units) == -1) {
            unit.pos = tryPos2;
        } else {
            break; // Blocked on both axes.
        }

        // Stop early if we're now in attack range.
        if (unit.isInRange(target)) break;
    }

    if (unit.pos.x != startPos.x || unit.pos.y != startPos.y) {
        std::cout << unit.name << " moves from (" << startPos.x << ", " << startPos.y
             << ") to (" << unit.pos.x << ", " << unit.pos.y << ").\n";
    } else {
        std::cout << unit.name << " could not move.\n";
    }
}
