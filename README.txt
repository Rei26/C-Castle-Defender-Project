Castle Defender - Fall 2025
===========================

Name: Andrei Vasile
Student Number: 2120731

Prerequisites
-------------
- C++17-capable compiler (tested with `g++` from MSYS2 UCRT64; MSVC 2019+ should also work for console builds).
- FLTK library (only needed for the GUI target; console build does not require it).
- MSYS2 UCRT64 toolchain with `g++`, `fltk-config`, and `gdb` (install via `pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-fltk mingw-w64-ucrt-x86_64-gdb`) and ensure the UCRT64 shell is used so `fltk-config` is on PATH.

How to Build and Run
----------------------

### Option A: Windows
```
Simply go to main.cpp, run the code through your compiler and select "GUI Game FLTK"
```

### Option B: MacOS
```
main.cpp->Run task->"Run GUI (MacOS terminal)"
```
Files
-----
- `Header/` and `Sources/` contain all headers/implementation files.
- Key classes: `Game`, `GuiGame`, `Grid`, `Castle`, `Tower`, `Enemy`, `AIController`, `Player`, `GameView`.
- `.vscode/` holds optional VS Code tasks/launchers that automate builds via MSYS2.


Notes:
------
Tested on Windows/MacOS, Compiler verseion C++ 17 and above