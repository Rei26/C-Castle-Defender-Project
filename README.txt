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
- A C++17-compatible compiler (MSYS2 UCRT64 g++, clang++, or MSVC /std:c++17).
- For the GUI build: FLTK development headers + libraries and `fltk-config` on the PATH.
- Windows instructions below assume MSYS2 UCRT64 is installed at `C:\msys64`.

How to Build (console)
----------------------

### Option A: MSVC (Developer Command Prompt)
```
cl /std:c++17 /EHsc Sources\Grid.cpp Sources\Tower.cpp Sources\AIController.cpp Sources\Game.cpp Sources\main.cpp /I Header
```

### Option B: g++ / clang++
```
g++ -std=c++17 -O2 Sources/Grid.cpp Sources/Tower.cpp Sources/AIController.cpp Sources/Game.cpp Sources/main.cpp -I Header -o castle_defender.exe
```

> Note: `GameView.*`, `GuiGame.*`, and `main_fltk.cpp` are only required for the FLTK GUI build.

VS Code Tasks (optional convenience)
------------------------------------
- `Terminal → Run Task… → Build Console (MSYS2 UCRT64)` builds `castle_defender.exe` via MSYS2 g++ (see `.vscode/tasks.json`).
- `Terminal → Run Task… → Run Console (Terminal)` builds and immediately runs the console game.
- `Terminal → Run Task… → Build GUI (MSYS2 FLTK)` builds `castle_defender_fltk.exe` using `fltk-config` from MSYS2 UCRT64.
- `Terminal → Run Task… → Run GUI (Terminal)` builds and launches the FLTK GUI.
- Launch configurations (“Console Game (UCRT64)” and “GUI Game (FLTK)”) are wired to these tasks; hitting F5 will build the correct target if the MSYS2 paths match your machine.

FLTK GUI Build
---------------
Interactive GUI with mouse-based tower placement, upgrade clicks, and auto-turn timer.

### Recommended command (MSYS2 UCRT64 shell)
```
g++ -std=c++17 -O2 \
    Sources/Grid.cpp Sources/Tower.cpp Sources/AIController.cpp Sources/GuiGame.cpp Sources/GameView.cpp Sources/main_fltk.cpp \
    -I Header $(fltk-config --cxxflags) $(fltk-config --ldflags) -o castle_defender_fltk.exe
```

### If `fltk-config` is unavailable
Add include/lib paths and Windows libraries manually:
```
g++ ... -I C:/msys64/ucrt64/include -L C:/msys64/ucrt64/lib \
    -lfltk -lfltk_images -lfltk_forms -lole32 -luuid -lcomctl32 -lws2_32 -lgdi32 -o castle_defender_fltk.exe
```


Running the executables
-----------------------
- Console (Windows): `./castle_defender.exe`
- Console (Linux/macOS): `./castle_defender`
- GUI: `./castle_defender_fltk.exe` (opens the FLTK window). No extra setup once built.

Gameplay quick reference
------------------------
- Place 5 towers (console: 1-based row/col; GUI: click) anywhere except the top row or castle cell.
- Enemies spawn one per turn; AI prefers columns at least three cells away from towers ~70 % of the time but occasionally targets defended lanes.
- Towers fire each turn at the closest enemy in range; ties break toward the enemy closest to the castle.
- When a wave ends, you receive one upgrade point. Console prompts for tower coordinates and D/R; GUI requires clicking “Upgrade Dmg/Upgrade Rng” then clicking a tower. Waves resume only after spending or forfeiting the upgrade.
- Score: +10 per enemy destroyed; final summary prints Player Score, Enemies Destroyed, Castle Health, and Winner (Player vs AI).

Troubleshooting / portability notes
-----------------------------------
1. **Include path** – Every compile command must contain `-I Header` (or VS projects must add `Header/` to Additional Include Directories) or `Game.h` will not be found.
2. **Toolchain** – The VS Code tasks assume MSYS2 lives in `C:/msys64`. If your installation is elsewhere, update the paths in `.vscode/tasks.json` (and `.vscode/launch.json`) accordingly.
3. **FLTK availability** – The GUI build requires `fltk-config` on the PATH. If it is missing, install `mingw-w64-ucrt-x86_64-fltk` via MSYS2 or point the command at your FLTK include/lib directories.
4. **Prebuilt binaries** – Optional: if you compile the EXEs on your machine, include `castle_defender.exe` and/or `castle_defender_fltk.exe` in the submission zip. Anyone who already has the MSYS2 runtime DLLs (or who rebuilds from source) can run them immediately.
5. **Other platforms** – The code is portable C++17; any OS with a modern compiler and (optionally) FLTK can build it by following the commands above.

Files
-----
- `Header/` and `Sources/` contain all headers/implementation files.
- Key classes: `Game`, `GuiGame`, `Grid`, `Castle`, `Tower`, `Enemy`, `AIController`, `Player`, `GameView`.
- `.vscode/` holds optional VS Code tasks/launchers that automate builds via MSYS2.
