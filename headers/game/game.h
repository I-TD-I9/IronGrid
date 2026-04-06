#pragma once
#include <vector>
#include <memory>
#include "../units/unit.h"
#include "../grid/grid.h"

class Game {
private:
    Grid grid;
    std::vector<std::unique_ptr<Unit>> units;
    int currentTurnIndex;

    void enemyUnits();
    void setupUnits();
    void displayGrid();
    void playerTurn(Unit& unit);
    void handleMove(Unit& unit);
    void nextTurn();
    bool isGameOver();
    void announceWinner();
    void clearInput();

public:
    Game();
    void run();
};
