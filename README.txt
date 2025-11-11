Castle Defender (Console Version)
=================================

Overview
--------
- 20x20 grid with the castle in the bottom center (100 HP).
- Player places exactly 5 towers before combat begins.
- Five waves total, 10 enemies per wave. Enemies spawn one per turn on the top row.
- Enemies move down (or diagonally when blocked) and deal 10 castle damage on contact.
- Enemy types: Basic (red), Tank (purple, tougher), Scout (orange, faster).
- AI monitors performance; if a wave scores ≥80% of the max with no castle damage, it grants +1 enemy HP for later waves.

How to Build
------------

Option A: MSVC (Developer Command Prompt)
- cl /std:c++17 /EHsc Sources\\Grid.cpp Sources\\Tower.cpp Sources\\AIController.cpp Sources\\Game.cpp Sources\\main.cpp /I Header

Option B: g++ (MinGW)
- g++ -std=c++17 -O2 Sources/Grid.cpp Sources/Tower.cpp Sources/AIController.cpp Sources/Game.cpp Sources/main.cpp -I Header -o castle_defender.exe

Note: GameView.*, GuiGame.*, and main_fltk.cpp are only needed when building the FLTK GUI. Exclude them for the console build.

VS Code Tasks (recommended)
--------------------------
- `Terminal → Run Task… → Build Console (MSYS2 UCRT64)` builds `castle_defender.exe` using the MSYS2 toolchain configured in `.vscode/tasks.json`.
- `Terminal → Run Task… → Run Console (Terminal)` builds + launches the console game in the integrated terminal.
- `Terminal → Run Task… → Build GUI (MSYS2 FLTK)` builds `castle_defender_fltk.exe` using `fltk-config` from MSYS2 UCRT64.
- `Terminal → Run Task… → Run GUI (Terminal)` builds + launches the FLTK GUI (opens a window, no extra commands needed).
- Two launch configurations are also provided (see Run and Debug panel): “Console Game (UCRT64)” and “GUI Game (FLTK)”. Each rebuilds the correct binary before debugging.

FLTK GUI Build (optional)
-------------------------
This builds the interactive GUI with click-to-place towers and optional 1s auto-turn timer.

Prerequisites: FLTK development libraries installed and on your compiler's include/lib paths.

Recommended (MSYS2 UCRT64 shell):
- g++ -std=c++17 -O2 \
    Sources/Grid.cpp Sources/Tower.cpp Sources/AIController.cpp Sources/GuiGame.cpp Sources/GameView.cpp Sources/main_fltk.cpp \
    -I Header $(fltk-config --cxxflags) $(fltk-config --ldflags) -o castle_defender_fltk.exe

Notes:
- Without fltk-config, add `-I C:\msys64\ucrt64\include -L C:\msys64\ucrt64\lib -lfltk -lfltk_images -lfltk_forms -lole32 -luuid -lcomctl32 -lws2_32 -lgdi32`.
- On macOS: add `-framework Cocoa -lfltk` and ensure you use Homebrew FLTK include/lib paths.
- On Linux: link `-lfltk` and ensure X11 dev packages are available.

Running the GUI
---------------
- Windows: .\castle_defender_fltk.exe
- Place exactly 5 towers by clicking valid cells (not top row, not on castle).
- Use "Start Wave" to begin each wave. "Next Turn" advances a single turn (or starts the wave if you are waiting).
- "Start Auto" runs one turn per second; "Stop Auto" pauses. Auto automatically halts when a wave ends.
- After a wave, select "Upgrade Dmg" or "Upgrade Rng" then click a tower to spend the upgrade point. Click "Start Wave" to continue.

How to Run
----------
- Windows: .\\castle_defender.exe
- Linux/macOS: ./castle_defender (if compiled that way)

Gameplay
--------
- Console: place 5 towers via 1-based row/col input (not on the top row or castle). GUI: click to place.
- Waves: 5 maximum, 10 enemies/wave, 1 spawn per turn. AI prefers columns ≥3 cells away from towers about 70% of the time but still mixes in risky spawns.
- Movement: enemies fall straight down; if blocked by a tower/enemy they may shift diagonally down-left/right when open. They never move sideways without descending.
- Enemy types: Basic (3 HP, speed 1), Tank (purple, +2 HP, speed 1), Scout (orange, HP-1, speed 2).
- Castle collision deals 10 damage (castle starts at 100 HP) and removes the enemy.
- Between waves you receive 1 upgrade point to boost any tower's damage or range. Spend it before starting the next wave.
- AI adaptation: if the wave's score is ≥80% of the theoretical max (all kills) **and** the castle takes no damage, upcoming enemies gain +1 base HP.
- Console loop prompts you to press Enter each turn (or `q` + Enter to quit) and shows ASCII symbols when Unicode is unavailable.

Scoring
-------
- +10 points per enemy destroyed.
- Final summary (console + GUI) prints: Player Score, Enemies Destroyed, Castle Health, Winner.

Winning & Losing
----------------
- Lose if the castle HP reaches 0.
- Win after all 5 waves are cleared and no enemies remain.

Files
-----
- Header/ and Sources/ contain the implementation.
- Key classes: Game, Grid, Castle, Tower, Enemy, AIController, Player.
