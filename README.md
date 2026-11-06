# 🐍 Snake Ultra Edition — C++ Console Game

A feature-packed, multi-mode console snake game written in C++17 for Windows.

---

## How to Build

### Step 1 — Install MinGW-w64 (one-time setup)
**Easiest method:** Install via [MSYS2](https://www.msys2.org/)
1. Download and run the MSYS2 installer
2. Open "MSYS2 MinGW 64-bit" terminal and run:
   ```
   pacman -S mingw-w64-x86_64-gcc
   ```
3. Add `C:\msys64\mingw64\bin` to your Windows PATH

**Alternative:** [WinLibs standalone](https://winlibs.com/) — download ZIP, extract, add `bin\` to PATH.

### Step 2 — Build
Double-click `build.bat`, or in a terminal:
```
cd C:\Console_Based_Snake_Game
g++ -std=c++17 -O2 -o snake_ultra.exe src/main.cpp src/game.cpp src/console.cpp src/sound.cpp src/particles.cpp src/maze.cpp src/leaderboard.cpp src/ui.cpp
```

### Step 3 — Run
```
snake_ultra.exe
```

---

## Features

### 4 Game Modes
| Mode    | Description |
|---------|-------------|
| Classic | Traditional snake — hit a wall = death |
| Portal  | Walls are portals, teleport through them |
| Maze    | Navigate a procedurally generated maze |
| Chaos   | Random events every few seconds — expect anything |

### 10 Power-Ups
| Glyph | Name         | Effect |
|-------|--------------|--------|
| `>`   | Speed Boost  | Move faster |
| `s`   | Slow-Mo      | Slow down time |
| `O`   | Shield       | Survive one fatal collision (bounces off walls) |
| `g`   | Ghost        | Pass through your own body |
| `m`   | Magnet       | Food is attracted to your head |
| `*`   | Freeze       | Freeze all enemies |
| `B`   | Bomb         | Explode nearby walls and kill nearby enemies |
| `2`   | 2× Score     | Double points for a duration |
| `<`   | Shrink       | Instantly shrink your snake |
| `S`   | Star         | Full invincibility + rainbow snake + bonus points |

### Enemy AI
- Enemies spawn after a short delay (all modes except Classic)
- 70% of the time they chase your head, 30% random movement
- Freeze them with the Freeze power-up
- Kill them instantly in Star mode
- Shield bounces you away from enemies too

### Combo System
- Eat food quickly to build a combo multiplier
- Combo × score multiplier stacks up to huge points
- Special sound effect at combo ×3+

### Chaos Events (Chaos Mode)
Random events fire every ~50 ticks:
- Direction reversal
- Speed surge
- Wall spawns
- Bonus food drops
- Random teleport
- Extra enemy spawn
- 2× score activation
- Slow-motion

### Visual Effects
- Back-buffered rendering — zero flicker
- Screen shake on death, bomb, shield hit
- Particle explosions with physics
- Floating score text
- Power-up blink warning when about to expire
- Direction-aware snake head glyph (`/\`, `\/`, `[>`, `<]`)
- Shield aura animation around head
- Rainbow snake during Star mode
- Animated splash screen with sliding title

### Sound Effects (Windows Beep, async)
- Eat, combo, level-up jingles
- Power-up collect, star activate
- Death sequence, shield hit
- Bomb explosion, portal teleport
- Menu navigation sounds

### Leaderboard
- Top 10 scores saved to `scores.dat`
- Stores: name (3 chars), score, mode, level reached
- Persists between sessions

---

## Controls
| Key | Action |
|-----|--------|
| W / ↑ | Move Up |
| S / ↓ | Move Down |
| A / ← | Move Left |
| D / → | Move Right |
| P | Pause / Resume |
| Q | Quit to Menu |

---

## Project Structure
```
Console_Based_Snake_Game/
├── src/
│   ├── main.cpp        Entry point, game loop
│   ├── globals.h       All shared types, enums, constants
│   ├── console.h/cpp   Back-buffered console renderer + screen shake
│   ├── game.h/cpp      Core game engine (snake, food, enemies, power-ups)
│   ├── ui.h/cpp        All menus, splash, game over, leaderboard screens
│   ├── particles.h/cpp Particle system (bursts, explosions, floating text)
│   ├── maze.h/cpp      Procedural maze generator (recursive backtracker)
│   ├── sound.h/cpp     Async beep-based sound effects
│   └── leaderboard.h/cpp Persistent score storage
├── build.bat           One-click Windows build script
├── Makefile            GNU Make build file
└── README.md           This file
```
