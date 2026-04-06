# IronGrid

A turn-based tactical skirmish console game

## Overview

Command a small squad of units on a 6x6 grid battlefield. Take turns moving and attacking to defeat the enemy team

**Player units:** Knight (`S`) and Archer (`A`)
**Enemy units:** Knight (`s`) and Archer (`a`)

```
   0 1 2 3 4 5
0: S . . . . .
1: A . . . . .
2: . . . . . .
3: . . . . . .
4: . . . . . a
5: . . . . . s
```

Each turn you can **move**, **attack**, or **end your turn**. Enemies act automatically with simple AI. The game ends when one side is eliminated

## Build & Run

```bash
g++ src/main.cpp -o irongrid && ./irongrid
```
