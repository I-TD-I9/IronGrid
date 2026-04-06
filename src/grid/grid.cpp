#include "../headers/grid/grid.h"
#include <iostream>
#include <cmath>

const std::string RESET   = "\033[0m";
const std::string BOLD    = "\033[1m";
const std::string DIM     = "\033[2m";
const std::string GREEN   = "\033[32m";
const std::string RED     = "\033[31m";
const std::string YELLOW  = "\033[33m";
const std::string CYAN    = "\033[36m";
const std::string PURPLE  = "\033[35m";

int Grid::getWidth() const { return WIDTH; }
int Grid::getHeight() const { return HEIGHT; }

bool Grid::isPositionInsideGrid(Position p) const {
    return p.x >= 0 && p.x < WIDTH && p.y >= 0 && p.y < HEIGHT;
}

int Grid::getUnitAtPosition(Position p, const std::vector<std::unique_ptr<Unit>>& units) const {
    for (int i = 0; i < (int)units.size(); i++) {
        if (units[i]->alive && units[i]->pos.x == p.x && units[i]->pos.y == p.y) {
            return i;
        }
    }
    return -1;
}

std::string Grid::hpBar(int hp, int maxHp, int barWidth) const {
    int filled = (maxHp > 0) ? (hp * barWidth / maxHp) : 0;
    std::string bar = "[";
    for (int i = 0; i < barWidth; i++) {
        bar += (i < filled) ? "#" : " ";
    }
    bar += "]";
    return bar;
}

void Grid::display(const std::vector<std::unique_ptr<Unit>>& units, int currentTurnIndex) const {
    const Unit& current = *units[currentTurnIndex];
    bool showRanges = current.alive && current.isPlayer;

    // Column headers.
    std::cout << "\n     ";
    for (int x = 1; x < WIDTH; x++) {
        std::cout << x << "   ";
    }
    std::cout << "\n";

    // Top border.
    std::cout << "  +";
    for (int x = 1; x < WIDTH; x++) {
        std::cout << "---";
        std::cout << (x < WIDTH - 1 ? "+" : "+");
    }
    std::cout << "\n";

    for (int y = 1; y < HEIGHT; y++) {
        std::cout << y << " ";
        for (int x = 1; x < WIDTH; x++) {
            std::cout << "| ";
            Position p = {x, y};
            int unitIndex = getUnitAtPosition(p, units);

            if (unitIndex != -1 && units[unitIndex]->alive) {
                bool isActive = (unitIndex == currentTurnIndex);
                if (isActive) {
                    std::cout << YELLOW << BOLD << units[unitIndex]->symbol << RESET;
                } else if (units[unitIndex]->isPlayer) {
                    std::cout << GREEN << units[unitIndex]->symbol << RESET;
                } else {
                    std::cout << RED << units[unitIndex]->symbol << RESET;
                }
            } else if (showRanges) {
                int dist = abs(current.pos.x - p.x) + abs(current.pos.y - p.y);
                bool inAttack = dist > 0 && dist <= current.attackRange;
                bool inMove = dist > 0 && dist <= current.moveRange && getUnitAtPosition(p, units) == -1;
                if (inAttack && inMove) {
                    std::cout << BOLD << PURPLE << "#" << RESET;
                } else if (inAttack) {
                    std::cout << DIM << RED << "x" << RESET;
                } else if (inMove) {
                    std::cout << DIM << CYAN << "~" << RESET;
                } else {
                    std::cout << " ";
                }
            } else {
                std::cout << " ";
            }
            std::cout << " ";
        }
        std::cout << "|\n";

        // Row separator.
        if (y < HEIGHT - 1) {
            std::cout << "  +";
            for (int x = 1; x < WIDTH; x++) {
                std::cout << "---";
                std::cout << (x < WIDTH - 1 ? "+" : "+");
            }
            std::cout << "\n";
        }
    }

    // Bottom border.
    std::cout << "  +";
    for (int x = 1; x < WIDTH; x++) {
        std::cout << "---";
        std::cout << (x < WIDTH - 1 ? "+" : "+");
    }
    std::cout << "\n";

    // Unit legend with HP bars.
    std::cout << "\n";
    for (int i = 0; i < (int)units.size(); i++) {
        if (!units[i]->alive) continue;
        bool isActive = (i == currentTurnIndex);
        std::string color = units[i]->isPlayer ? GREEN : RED;

        if (isActive) {
            std::cout << YELLOW << BOLD << "> " << units[i]->symbol << "  " << units[i]->name << RESET;
        } else {
            std::cout << "  " << color << units[i]->symbol << RESET << "  " << units[i]->name;
        }

        std::cout << "  " << hpBar(units[i]->hp, units[i]->maxHp)
             << " " << units[i]->hp << "/" << units[i]->maxHp << "\n";
    }
}