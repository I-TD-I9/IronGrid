#include "../../headers/units/unit.h"

Unit::Unit(std::string unitName, int health, int damage, int move, int range,
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

bool Unit::isInRange(const Unit& other) const {
    int distance = abs(pos.x - other.pos.x) + abs(pos.y - other.pos.y);
    return distance <= attackRange;
}

bool Unit::canMoveTo(Position newPos) const {
    int distance = abs(pos.x - newPos.x) + abs(pos.y - newPos.y);
    return distance <= moveRange;
}