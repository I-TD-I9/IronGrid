#pragma once
#include <vector>
#include <string>
#include <memory>
#include "../units/unit.h"

// ANSI color codes for console output.
extern const std::string RESET;
extern const std::string BOLD;
extern const std::string DIM;
extern const std::string GREEN;
extern const std::string RED;
extern const std::string YELLOW;
extern const std::string CYAN;
extern const std::string PURPLE;

class Grid {
private:
    static const int WIDTH = 7;
    static const int HEIGHT = 7;

public:
    int getWidth() const;
    int getHeight() const;

    bool isPositionInsideGrid(Position p) const;
    int getUnitAtPosition(Position p, const std::vector<std::unique_ptr<Unit>>& units) const;

    std::string hpBar(int hp, int maxHp, int barWidth = 12) const;
    void display(const std::vector<std::unique_ptr<Unit>>& units, int currentTurnIndex) const;
};