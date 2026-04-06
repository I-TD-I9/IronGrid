#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>

using namespace std;

// A simple struct to store a position on the grid.
// "struct" is like a lightweight class that usually just holds data.
struct Position {
    int x;
    int y;
};

// A Unit represents a character on the battlefield.
class Unit {
public:
    string name;
    int hp;
    int attackDamage;
    int moveRange;
    int attackRange;
    bool isPlayer;
    bool alive;
    Position pos;
    char symbol;

    // Constructor: used to create a Unit with starting values.
    Unit(string unitName, int health, int damage, int move, int range,
         bool playerControlled, Position startPos, char displaySymbol)
        : name(unitName),
          hp(health),
          attackDamage(damage),
          moveRange(move),
          attackRange(range),
          isPlayer(playerControlled),
          alive(true),
          pos(startPos),
          symbol(displaySymbol) {}

    // Check if another unit is within attack range.
    bool isInRange(const Unit& other) const {
        int distance = abs(pos.x - other.pos.x) + abs(pos.y - other.pos.y);
        return distance <= attackRange;
    }

    // Check if a tile is within movement range.
    bool canMoveTo(Position newPos) const {
        int distance = abs(pos.x - newPos.x) + abs(pos.y - newPos.y);
        return distance <= moveRange;
    }
};

// The Game class controls the grid, turns, combat, and game loop.
class Game {
private:
    static const int WIDTH = 6;
    static const int HEIGHT = 6;

    vector<Unit> units;
    int currentTurnIndex;

public:
    Game() : currentTurnIndex(0) {
        setupUnits();
    }

    // Create the starting units for both teams.
    void setupUnits() {
        units.push_back(Unit("Player Knight", 12, 4, 2, 1, true,  {0, 0}, 'K'));
        units.push_back(Unit("Player Archer", 8, 3, 2, 2, true,  {0, 1}, 'A'));

        units.push_back(Unit("Enemy Knight", 12, 4, 2, 1, false, {5, 5}, 'k'));
        units.push_back(Unit("Enemy Archer", 8, 3, 2, 2, false, {5, 4}, 'a'));
    }

    // Main game loop.
    void run() {
        cout << "Welcome to Irongrid Skirmish!\n";

        while (true) {
            if (isGameOver()) {
                displayGrid();
                announceWinner();
                break;
            }

            Unit& currentUnit = units[currentTurnIndex];

            // Skip dead units.
            if (!currentUnit.alive) {
                nextTurn();
                continue;
            }

            displayGrid();
            cout << "\nCurrent turn: " << currentUnit.name << "\n";

            if (currentUnit.isPlayer) {
                playerTurn(currentUnit);
            } else {
                enemyTurn(currentUnit);
            }

            removeDeadUnits();
            nextTurn();
        }
    }

    // Print the battlefield.
    void displayGrid() {
        cout << "\n   ";
        for (int x = 0; x < WIDTH; x++) {
            cout << x << " ";
        }
        cout << "\n";

        for (int y = 0; y < HEIGHT; y++) {
            cout << y << ": ";
            for (int x = 0; x < WIDTH; x++) {
                Position p = {x, y};
                int unitIndex = getUnitAtPosition(p);

                if (unitIndex != -1 && units[unitIndex].alive) {
                    cout << units[unitIndex].symbol << " ";
                } else {
                    cout << ". ";
                }
            }
            cout << "\n";
        }

        cout << "\nUnits:\n";
        for (int i = 0; i < (int)units.size(); i++) {
            if (units[i].alive) {
                cout << i << " - " << units[i].name
                     << " | HP: " << units[i].hp
                     << " | Pos: (" << units[i].pos.x << ", " << units[i].pos.y << ")\n";
            }
        }
    }

    // Handle the player's turn.
    void playerTurn(Unit& unit) {
        int choice;

        while (true) {
            cout << "Choose action:\n";
            cout << "1. Move\n";
            cout << "2. Attack\n";
            cout << "3. End Turn\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (cin.fail()) {
                clearInput();
                cout << "Invalid input. Try again.\n";
                continue;
            }

            if (choice == 1) {
                handleMove(unit);
                break;
            } else if (choice == 2) {
                handleAttack(unit);
                break;
            } else if (choice == 3) {
                cout << unit.name << " ends their turn.\n";
                break;
            } else {
                cout << "Invalid choice. Try again.\n";
            }
        }
    }

    // Very simple enemy AI.
    void enemyTurn(Unit& unit) {
        cout << unit.name << " is taking its turn...\n";

        int targetIndex = findClosestPlayer(unit);

        if (targetIndex == -1) {
            cout << "No player targets found.\n";
            return;
        }

        Unit& target = units[targetIndex];

        // Attack if already in range.
        if (unit.isInRange(target)) {
            performAttack(unit, target);
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

        if (isPositionInsideGrid(newPos) && getUnitAtPosition(newPos) == -1) {
            cout << unit.name << " moves from (" << unit.pos.x << ", " << unit.pos.y
                 << ") to (" << newPos.x << ", " << newPos.y << ").\n";
            unit.pos = newPos;
        } else {
            cout << unit.name << " could not move.\n";
        }

        // Try to attack after moving.
        if (unit.isInRange(target) && target.alive) {
            performAttack(unit, target);
        }
    }

    // Move a unit to a new tile if valid.
    void handleMove(Unit& unit) {
        int x, y;
        cout << "Enter new x y position: ";
        cin >> x >> y;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid input.\n";
            return;
        }

        Position newPos = {x, y};

        if (!isPositionInsideGrid(newPos)) {
            cout << "That position is outside the grid.\n";
            return;
        }

        if (!unit.canMoveTo(newPos)) {
            cout << "That position is too far away.\n";
            return;
        }

        if (getUnitAtPosition(newPos) != -1) {
            cout << "That tile is already occupied.\n";
            return;
        }

        cout << unit.name << " moves from (" << unit.pos.x << ", " << unit.pos.y
             << ") to (" << newPos.x << ", " << newPos.y << ").\n";

        unit.pos = newPos;
    }

    // Let the player choose a target to attack.
    void handleAttack(Unit& unit) {
        vector<int> validTargets;

        cout << "Targets in range:\n";
        for (int i = 0; i < (int)units.size(); i++) {
            if (units[i].alive && units[i].isPlayer != unit.isPlayer && unit.isInRange(units[i])) {
                cout << i << " - " << units[i].name
                     << " (HP: " << units[i].hp << ")\n";
                validTargets.push_back(i);
            }
        }

        if (validTargets.empty()) {
            cout << "No enemies are in range.\n";
            return;
        }

        int targetIndex;
        cout << "Enter target index: ";
        cin >> targetIndex;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid input.\n";
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
            cout << "That is not a valid target.\n";
            return;
        }

        performAttack(unit, units[targetIndex]);
    }

    // Damage another unit.
    void performAttack(Unit& attacker, Unit& defender) {
        cout << attacker.name << " attacks " << defender.name
             << " for " << attacker.attackDamage << " damage!\n";

        defender.hp -= attacker.attackDamage;

        if (defender.hp <= 0) {
            defender.hp = 0;
            defender.alive = false;
            cout << defender.name << " has been defeated!\n";
        } else {
            cout << defender.name << " now has " << defender.hp << " HP.\n";
        }
    }

    // Check if a position is on the board.
    bool isPositionInsideGrid(Position p) {
        return p.x >= 0 && p.x < WIDTH && p.y >= 0 && p.y < HEIGHT;
    }

    // Return the index of the unit at a position, or -1 if empty.
    int getUnitAtPosition(Position p) {
        for (int i = 0; i < (int)units.size(); i++) {
            if (units[i].alive && units[i].pos.x == p.x && units[i].pos.y == p.y) {
                return i;
            }
        }
        return -1;
    }

    // Move to the next unit's turn.
    void nextTurn() {
        currentTurnIndex = (currentTurnIndex + 1) % units.size();
    }

    // This function is here mostly for clarity and future expansion.
    void removeDeadUnits() {
        // We are not actually erasing dead units from the vector.
        // Instead, we keep them and mark alive = false.
        // That makes the turn logic simpler for now.
    }

    // Returns true if either side has lost all living units.
    bool isGameOver() {
        bool playerAlive = false;
        bool enemyAlive = false;

        for (const Unit& unit : units) {
            if (!unit.alive) continue;

            if (unit.isPlayer) {
                playerAlive = true;
            } else {
                enemyAlive = true;
            }
        }

        return !playerAlive || !enemyAlive;
    }

    // Print which team won.
    void announceWinner() {
        bool playerAlive = false;
        bool enemyAlive = false;

        for (const Unit& unit : units) {
            if (!unit.alive) continue;

            if (unit.isPlayer) {
                playerAlive = true;
            } else {
                enemyAlive = true;
            }
        }

        cout << "\nGame Over!\n";
        if (playerAlive && !enemyAlive) {
            cout << "You win!\n";
        } else if (!playerAlive && enemyAlive) {
            cout << "The enemy wins!\n";
        } else {
            cout << "It's a draw.\n";
        }
    }

    // Find the closest living player unit for enemy AI.
    int findClosestPlayer(const Unit& enemyUnit) {
        int closestIndex = -1;
        int closestDistance = numeric_limits<int>::max();

        for (int i = 0; i < (int)units.size(); i++) {
            if (!units[i].alive || !units[i].isPlayer) {
                continue;
            }

            int distance = abs(enemyUnit.pos.x - units[i].pos.x)
                         + abs(enemyUnit.pos.y - units[i].pos.y);

            if (distance < closestDistance) {
                closestDistance = distance;
                closestIndex = i;
            }
        }

        return closestIndex;
    }

    // Clear bad input from cin.
    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}