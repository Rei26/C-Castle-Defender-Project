Castle Defender
================

Overview
--------
- 20×20 grid with the castle anchored at the bottom center (100 HP).
- Player places exactly five towers before the first wave begins.
- Five waves total; each wave spawns ten enemies (one per turn) across the top row.
- Enemies walk straight down and try diagonal detours when blocked; reaching the castle deals 10 damage and removes the enemy.
- Enemy types: Basic (3 HP, speed 1), Tank (purple, +2 HP, speed 1), Scout (orange, HP − 1, speed 2).
- Adaptive AI: if the player finishes a wave with ≥80 % of the possible score and zero castle damage, all future enemies gain +1 base HP.

Prerequisites
-------------
- C++ Compilter
-FLTK library

How to Build (console)
----------------------

### Option A: Windows
```
Simply go to main.cpp, run the code through your compiler and select "GUI Game FLTK"
```

### Option B: MacOS
```
main.cpp->Run task->"Run GUI (MacOS terminal)"
```


Gameplay quick reference
------------------------
- Place 5 towers (console: 1-based row/col; GUI: click) anywhere except the top row or castle cell.
- Enemies spawn one per turn; AI prefers columns at least three cells away from towers ~70 % of the time but occasionally targets defended lanes.
- Towers fire each turn at the closest enemy in range; ties break toward the enemy closest to the castle.
- When a wave ends, you receive one upgrade point. Console prompts for tower coordinates and D/R; GUI requires clicking “Upgrade Dmg/Upgrade Rng” then clicking a tower. Waves resume only after spending or forfeiting the upgrade.
- Score: +10 per enemy destroyed; final summary prints Player Score, Enemies Destroyed, Castle Health, and Winner (Player vs AI).

Files
-----
- `Header/` and `Sources/` contain all headers/implementation files.
- Key classes: `Game`, `GuiGame`, `Grid`, `Castle`, `Tower`, `Enemy`, `AIController`, `Player`, `GameView`.
- `.vscode/` holds optional VS Code tasks/launchers that automate builds via MSYS2.
