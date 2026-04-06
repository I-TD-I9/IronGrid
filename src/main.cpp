#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>

using namespace std;

// ANSI color codes for console output.
const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";
const string DIM     = "\033[2m";
const string GREEN   = "\033[32m";
const string RED     = "\033[31m";
const string YELLOW  = "\033[33m";
const string CYAN    = "\033[36m";
const string PURPLE  = "\033[35m";

// A simple struct to store a position on the grid.
struct Position {
    int x;
    int y;
};

// A Unit represents a character on the battlefield.
class Unit {
public:
    string name;
    int hp;
    int maxHp;
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
          maxHp(health),
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
    static const int WIDTH = 7;
    static const int HEIGHT = 7;

    vector<Unit> units;
    int currentTurnIndex;

public:
    Game() : currentTurnIndex(0) {
        setupUnits();
    }

    // Create the starting units for both teams.
    void setupUnits() {
        units.push_back(Unit("Player Knight", 12, 4, 2, 1, true,  {1, 1}, 'K'));
        units.push_back(Unit("Player Archer", 8, 3, 2, 2, true,  {1, 2}, 'A'));

        units.push_back(Unit("Enemy Knight", 12, 4, 2, 1, false, {6, 6}, 'k'));
        units.push_back(Unit("Enemy Archer", 8, 3, 2, 2, false, {6, 5}, 'a'));
    }

    // Main game loop.
    void run() {
        cout << "Welcome to IronGrid!\n";

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

            nextTurn();
        }
    }

    // Generate an HP bar.
    string hpBar(int hp, int maxHp, int barWidth = 12) {
        int filled = (maxHp > 0) ? (hp * barWidth / maxHp) : 0;
        string bar = "[";
        for (int i = 0; i < barWidth; i++) {
            bar += (i < filled) ? "#" : " ";
        }
        bar += "]";
        return bar;
    }

    // Print the battlefield with box-drawing, colors, range highlights, and HP bars.
    void displayGrid() {
        Unit& current = units[currentTurnIndex];
        bool showRanges = current.alive && current.isPlayer;

        // Column headers.
        cout << "\n     ";
        for (int x = 1; x < WIDTH; x++) {
            cout << x << "   ";
        }
        cout << "\n";

        // Top border.
        cout << "  +";
        for (int x = 1; x < WIDTH; x++) {
            cout << "---";
            cout << (x < WIDTH - 1 ? "+" : "+");
        }
        cout << "\n";

        for (int y = 1; y < HEIGHT; y++) {
            cout << y << " ";
            for (int x = 1; x < WIDTH; x++) {
                cout << "| ";
                Position p = {x, y};
                int unitIndex = getUnitAtPosition(p);

                if (unitIndex != -1 && units[unitIndex].alive) {
                    bool isActive = (unitIndex == currentTurnIndex);
                    if (isActive) {
                        cout << YELLOW << BOLD << units[unitIndex].symbol << RESET;
                    } else if (units[unitIndex].isPlayer) {
                        cout << GREEN << units[unitIndex].symbol << RESET;
                    } else {
                        cout << RED << units[unitIndex].symbol << RESET;
                    }
                } else if (showRanges) {
                    int dist = abs(current.pos.x - p.x) + abs(current.pos.y - p.y);
                    bool inAttack = dist > 0 && dist <= current.attackRange;
                    bool inMove = dist > 0 && dist <= current.moveRange && getUnitAtPosition(p) == -1;
                    if (inAttack && inMove) {
                        cout << BOLD << PURPLE << "#" << RESET;
                    } else if (inAttack) {
                        cout << DIM << RED << "x" << RESET;
                    } else if (inMove) {
                        cout << DIM << CYAN << "~" << RESET;
                    } else {
                        cout << " ";
                    }
                } else {
                    cout << " ";
                }
                cout << " ";
            }
            cout << "|\n";

            // Row separator or bottom border.
            if (y < HEIGHT - 1) {
                cout << "  +";
                for (int x = 1; x < WIDTH; x++) {
                    cout << "---";
                    cout << (x < WIDTH - 1 ? "+" : "+");
                }
                cout << "\n";
            }
        }

        // Bottom border.
        cout << "  +";
        for (int x = 1; x < WIDTH; x++) {
            cout << "---";
            cout << (x < WIDTH - 1 ? "+" : "+");
        }
        cout << "\n";

        // Unit legend with HP bars.
        cout << "\n";
        for (int i = 0; i < (int)units.size(); i++) {
            if (!units[i].alive) continue;
            bool isActive = (i == currentTurnIndex);
            string color = units[i].isPlayer ? GREEN : RED;

            if (isActive) {
                cout << YELLOW << BOLD << "> " << units[i].symbol << "  " << units[i].name << RESET;
            } else {
                cout << "  " << color << units[i].symbol << RESET << "  " << units[i].name;
            }

            cout << "  " << hpBar(units[i].hp, units[i].maxHp)
                 << " " << units[i].hp << "/" << units[i].maxHp << "\n";
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

    // Enemy AI.
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