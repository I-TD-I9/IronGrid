#pragma once
#include <string>
#include <vector>
#include <memory>
#include <cmath>

struct Position {
    int x;
    int y;
};

class Unit {
public:
    std::string name;
    int hp;
    int maxHp;
    int attackDamage;
    int moveRange;
    int attackRange;
    bool isPlayer;
    bool alive;
    Position pos;
    char symbol;

    // Ability system.
    std::string abilityName;
    int abilityCooldownMax;
    int abilityCooldown;
    bool shielded;
    bool charged;
    bool hidden;

    Unit(std::string unitName, int health, int damage, int move, int range,
         bool playerControlled, Position startPos, char displaySymbol,
         std::string ability, int cooldown);

    virtual ~Unit() = default;

    bool isInRange(const Unit& other) const;
    bool canMoveTo(Position newPos) const;
    bool canUseAbility() const;
    void tickCooldown();
    void clearTurnBuffs();

    virtual void useAbility(std::vector<std::unique_ptr<Unit>>& units) = 0;
};
