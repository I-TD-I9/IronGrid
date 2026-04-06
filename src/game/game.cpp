#include "../../headers/game/game.h"
#include "../../headers/units/knight.h"
#include "../../headers/units/archer.h"
#include "../../headers/units/lancer.h"
#include "../../headers/units/assassin.h"
#include "../../headers/combat/combat.h"
#include "../../headers/ai/enemy_ai.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>

Game::Game() : currentTurnIndex(0) {
    setupUnits();
}

void Game::setupUnits() {

    std::cout << "Select two units for your team:\n";
    int choice;
    
    std::cout << "1. Knight   - HP:12  ATK:4  MOV:2  RNG:2\n";
    std::cout << "2. Archer   - HP:8   ATK:2  MOV:1  RNG:5\n";
    std::cout << "3. Lancer   - HP:10  ATK:3  MOV:2  RNG:4\n";
    std::cout << "4. Assassin - HP:9   ATK:5  MOV:3  RNG:1\n";
    std::cout << "First unit: ";
    std::cin >> choice;

    if (std::cin.fail() || choice < 1 || choice > 4) {
        clearInput();
        std::cout << "Invalid choice. Try again.\n";
        units.clear();
        return setupUnits();
    }
    switch (choice) {
    case 1:
        units.push_back(std::make_unique<Knight>(true, Position{1, 1}));
        break;
    case 2:
        units.push_back(std::make_unique<Archer>(true, Position{1, 1}));
        break;
    case 3:
        units.push_back(std::make_unique<Lancer>(true, Position{1, 1}));
        break;
    case 4:
        units.push_back(std::make_unique<Assassin>(true, Position{1, 1}));
        break;
    }
    std::cout << "Second unit: ";
    std::cin >> choice;
    if (std::cin.fail() || choice < 1 || choice > 4) {
        clearInput();
        std::cout << "Invalid choice. Try again.\n";
        units.clear();
        return setupUnits();
    }
    switch (choice) {
    case 1:
        units.push_back(std::make_unique<Knight>(true, Position{1, 2}));
        break;
    case 2:
        units.push_back(std::make_unique<Archer>(true, Position{1, 2}));
        break;
    case 3:
        units.push_back(std::make_unique<Lancer>(true, Position{1, 2}));
        break;
    case 4:
        units.push_back(std::make_unique<Assassin>(true, Position{1, 2}));
        break;
    }

    enemyUnits();
}

void Game::enemyUnits() {

    std::cout << "Setting up enemy units...\n";

    std::srand(std::time(0));
    
    int enemyUnit1 = (std::rand() % 4) + 1;
    int enemyUnit2 = (std::rand() % 4) + 1;

    switch (enemyUnit1) {
    case 1:
        units.push_back(std::make_unique<Knight>(false, Position{6, 6}));
        break;
    case 2:
        units.push_back(std::make_unique<Archer>(false, Position{6, 6}));
        break;
    case 3:
        units.push_back(std::make_unique<Lancer>(false, Position{6, 6}));
        break;
    case 4:
        units.push_back(std::make_unique<Assassin>(false, Position{6, 6}));
        break;
    }

    switch (enemyUnit2) {
    case 1:
        units.push_back(std::make_unique<Knight>(false, Position{6, 5}));
        break;
    case 2:
        units.push_back(std::make_unique<Archer>(false, Position{6, 5}));
        break;
    case 3:
        units.push_back(std::make_unique<Lancer>(false, Position{6, 5}));
        break;
    case 4:
        units.push_back(std::make_unique<Assassin>(false, Position{6, 5}));
        break;
    }
}

void Game::run() {
    std::cout << "Welcome to IronGrid!\n";

    while (true) {
        if (isGameOver()) {
            displayGrid();
            announceWinner();
            break;
        }

        Unit& currentUnit = *units[currentTurnIndex];

        // Skip dead units.
        if (!currentUnit.alive) {
            nextTurn();
            continue;
        }

        displayGrid();
        std::cout << "\nCurrent turn: " << currentUnit.name << "\n";

        if (currentUnit.isPlayer) {
            playerTurn(currentUnit);
        } else {
            EnemyAI::takeTurn(currentUnit, units, grid);
        }

        nextTurn();
    }
}

void Game::displayGrid() {
    grid.display(units, currentTurnIndex);
}

void Game::playerTurn(Unit& unit) {
    int choice;

    while (true) {
        std::cout << "Choose action:\n";
        std::cout << "1. Move\n";
        std::cout << "2. Attack\n";
        std::cout << "3. End Turn\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            clearInput();
            std::cout << "Invalid input. Try again.\n";
            continue;
        }

        if (choice == 1) {
            handleMove(unit);
            break;
        } else if (choice == 2) {
            Combat::handleAttack(unit, units);
            break;
        } else if (choice == 3) {
            std::cout << unit.name << " ends their turn.\n";
            break;
        } else {
            std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void Game::handleMove(Unit& unit) {
    int x, y;
    std::cout << "Enter new x y position: ";
    std::cin >> x >> y;

    if (std::cin.fail()) {
        clearInput();
        std::cout << "Invalid input.\n";
        return;
    }

    Position newPos = {x, y};

    if (!grid.isPositionInsideGrid(newPos)) {
        std::cout << "That position is outside the grid.\n";
        return;
    }

    if (!unit.canMoveTo(newPos)) {
        std::cout << "That position is too far away.\n";
        return;
    }

    if (grid.getUnitAtPosition(newPos, units) != -1) {
        std::cout << "That tile is already occupied.\n";
        return;
    }

    std::cout << unit.name << " moves from (" << unit.pos.x << ", " << unit.pos.y
         << ") to (" << newPos.x << ", " << newPos.y << ").\n";

    unit.pos = newPos;
}

void Game::nextTurn() {
    currentTurnIndex = (currentTurnIndex + 1) % units.size();
}

bool Game::isGameOver() {
    bool playerAlive = false;
    bool enemyAlive = false;

    for (const auto& unit : units) {
        if (!unit->alive) continue;

        if (unit->isPlayer) {
            playerAlive = true;
        } else {
            enemyAlive = true;
        }
    }

    return !playerAlive || !enemyAlive;
}

void Game::announceWinner() {
    bool playerAlive = false;
    bool enemyAlive = false;

    for (const auto& unit : units) {
        if (!unit->alive) continue;

        if (unit->isPlayer) {
            playerAlive = true;
        } else {
            enemyAlive = true;
        }
    }

    std::cout << "\nGame Over!\n";
    if (playerAlive && !enemyAlive) {
        std::cout << "You win!\n";
    } else if (!playerAlive && enemyAlive) {
        std::cout << "The enemy wins!\n";
    } else {
        std::cout << "It's a draw.\n";
    }
}

void Game::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
