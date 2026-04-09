# Iron Grid

A turn-based tactical skirmish game built in C++ for the console.

## Overview

Command a squad of 2 units on a 6x6 grid battlefield. Choose your team from 4 unit types, then take turns moving and attacking to eliminate the enemy squad. Enemies are randomly assigned and controlled by AI.

Each turn you can **move**, **attack**, use an **ability**, or **end your turn**. The game ends when one side is eliminated. After each match you can play again with a new team.

## Unit Types

| Unit | HP | ATK | MOV | RNG | Ability |
|------|----|-----|-----|-----|---------|
| Knight | 12 | 4 | 2 | 2 | Shield Wall |
| Archer | 8 | 2 | 1 | 5 | Volley |
| Lancer | 10 | 3 | 2 | 4 | Charge |
| Assassin | 9 | 5 | 3 | 1 | Vanish |

Uppercase = player, lowercase = enemy.

### Abilities

| Ability | Unit | Cooldown | Effect |
|---------|------|----------|--------|
| Shield Wall | Knight | 3 turns | Halves incoming damage for 1 turn |
| Volley | Archer | 3 turns | Deals damage to all enemies in range |
| Charge | Lancer | 2 turns | Doubles damage on next attack |
| Vanish | Assassin | 3 turns | Cannot be targeted for 1 turn |

## Build & Run

Requires CMake 3.16+ and a C++17 compiler.

```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
./irongrid
```

## Project Structure

```
IronGrid/
├── CMakeLists.txt
├── headers/
│   ├── ai/
│   │   └── enemy_ai.h        # Enemy AI targeting & movement
│   ├── combat/
│   │   └── combat.h           # Attack resolution & target selection
│   ├── game/
│   │   └── game.h             # Game loop, turns, player input
│   ├── grid/
│   │   └── grid.h             # Grid display, position checks, HP bars
│   └── units/
│       ├── unit.h             # Base Unit class & Position struct
│       ├── knight.h           # Knight (tank, short range)
│       ├── archer.h           # Archer (fragile, long range)
│       ├── lancer.h           # Lancer (balanced, mid range)
│       └── assassin.h         # Assassin (high damage, melee)
└── src/
    ├── main.cpp               # Entry point
    ├── ai/
    │   └── enemy_ai.cpp
    ├── combat/
    │   └── combat.cpp
    ├── game/
    │   └── game.cpp
    ├── grid/
    │   └── grid.cpp
    └── units/
        └── unit.cpp
```

## Architecture

- **Unit** — Base class with HP, attack, movement, range stats, and ability system (cooldowns, per-turn buff flags). Knight, Archer, Lancer, and Assassin inherit from it with preset stats and unique abilities.
- **Grid** — Handles rendering the battlefield with ANSI colors, range indicators (`~` move, `x` attack, `#` both), and HP bars.
- **Combat** — Static methods for resolving attacks and presenting target choices to the player.
- **EnemyAI** — Finds the closest player unit, moves toward it using full move range with axis-fallback pathfinding, and attacks if in range.
- **Game** — Orchestrates the game loop: unit selection, turn order, player/AI dispatch, win detection, and replay.
