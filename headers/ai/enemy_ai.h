#pragma once
#include <vector>
#include <memory>
#include "../units/unit.h"
#include "../grid/grid.h"

class EnemyAI {
public:
    static void takeTurn(Unit& unit, std::vector<std::unique_ptr<Unit>>& units, Grid& grid);

private:
    static int findClosestPlayer(const Unit& enemyUnit, const std::vector<std::unique_ptr<Unit>>& units);
    static void moveToward(Unit& unit, const Unit& target, std::vector<std::unique_ptr<Unit>>& units, Grid& grid);
};
