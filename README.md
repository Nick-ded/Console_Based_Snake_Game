<div align="center">

<!-- ANIMATED BANNER -->
<img src="https://readme-typing-svg.demolab.com?font=Press+Start+2P&size=28&duration=3000&pause=500&color=00FF41&center=true&vCenter=true&multiline=true&repeat=true&width=900&height=120&lines=%F0%9F%90%8D+SNAKE+ULTRA+EDITION;C%2B%2B+Console+Game" alt="Snake Ultra Edition" />

<br/>

<!-- MATRIX-STYLE DIVIDER -->
<img src="https://capsule-render.vercel.app/api?type=waving&color=0:00FF41,100:003300&height=120&section=header&text=SNAKE%20ULTRA%20EDITION&fontSize=40&fontColor=00FF41&animation=fadeIn&fontAlignY=38&desc=The%20Most%20Feature-Packed%20Console%20Snake%20Game%20Ever%20Built&descAlignY=60&descSize=16" width="100%"/>

</div>

---

<div align="center">

<!-- BADGES -->
[![Language](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)
[![Build](https://img.shields.io/badge/Build-MinGW--w64-FF6C37?style=for-the-badge&logo=gnu&logoColor=white)](https://www.mingw-w64.org/)
[![Status](https://img.shields.io/badge/Status-LEGENDARY-gold?style=for-the-badge&logo=star&logoColor=black)](https://github.com/Nick-ded/Console_Based_Snake_Game)
[![Lines](https://img.shields.io/badge/Lines%20of%20Code-2380%2B-blueviolet?style=for-the-badge)](https://github.com/Nick-ded/Console_Based_Snake_Game)

</div>

---

<div align="center">

<!-- GAMEPLAY GIF - animated terminal snake -->
<img src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExcDd4bWZ6bGR4aGtzaGZ5aXVrdTExbHZxOGJqamlxOGQ5bm80aXlwNiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/LmNwrBhejkK9EFP504/giphy.gif" width="600" alt="Snake Game Gameplay"/>

### *A console snake game so packed with features, it feels illegal.*

</div>

---

## 📋 Table of Contents

- [🎮 About The Project](#-about-the-project)
- [✨ Features](#-features)
- [🕹️ Game Modes](#️-game-modes)
- [⚡ Power-Ups](#-power-ups)
- [👾 Enemy AI](#-enemy-ai)
- [🏗️ Architecture](#️-architecture)
- [🚀 Getting Started](#-getting-started)
- [🎯 Controls](#-controls)
- [🏆 Leaderboard](#-leaderboard)
- [📁 Project Structure](#-project-structure)
- [🛠️ Tech Stack](#️-tech-stack)
- [🤝 Contributing](#-contributing)

---

## 🎮 About The Project

<div align="center">
<img src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExOWN6Z3lkZHh0dTZnem5raTduMnkycHpuY2ZycjJnb3p5bHZ5bWI0cyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/3oEjI6SIIHBdRxXI40/giphy.gif" width="400" alt="Coding"/>
</div>

> **Snake Ultra Edition** is not your grandma's snake game. This is a **fully-featured, multi-mode, particle-exploding, enemy-chasing, power-up-collecting, leaderboard-breaking** console masterpiece — built entirely in **C++17** with zero external libraries.

Built from scratch using raw Windows Console API, this project showcases:
- **Low-level console rendering** with a custom back-buffer (zero flicker)
- **Real-time particle physics** system
- **Procedural maze generation** via recursive backtracker algorithm
- **Async sound** using Windows `Beep()` on dedicated threads
- **Persistent data** with binary file I/O for the leaderboard
- **Entity-component-style architecture** split across 9 modular files

---

## ✨ Features

<div align="center">
<img src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExMnVtcGkwZm84NW9vaHEwd2NocGV3d3Z5NXZhb3QzZnQ4ZzF4dzJ2NCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/SWoSkN6DxTszqIKEqv/giphy.gif" width="500" alt="Features"/>
</div>

| Feature | Description |
|:---|:---|
| 🎮 **4 Game Modes** | Classic, Portal, Maze, Chaos — each completely different |
| ⚡ **10 Power-Ups** | Shield, Ghost, Magnet, Star, Bomb, Freeze, 2× Score, Shrink, Speed, Slow-Mo |
| 👾 **Enemy AI** | Enemies that actively hunt your head with pathfinding |
| 💥 **Particle System** | Physics-based explosions, trails, and floating score text |
| 📺 **Zero Flicker** | Custom back-buffer — only changed cells are redrawn |
| 📳 **Screen Shake** | Procedural shake on death, bomb detonations, shield hits |
| 🌀 **Procedural Maze** | Randomized DFS maze generator — different every game |
| 🔊 **Sound Effects** | Async beep melodies — eat, combo, death, power-up jingles |
| 🏆 **Leaderboard** | Persistent top-10 scores saved to disk between sessions |
| ✖️ **Combo System** | Rapid-eat combos multiply your score exponentially |
| 🌈 **Rainbow Snake** | Star power-up triggers full animated rainbow mode |
| 🎯 **Dynamic Difficulty** | Speed increases every level — no mercy at level 10+ |

---

## 🕹️ Game Modes

<div align="center">
<img src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExYndiNDZ0dnV5YWZ3dDd3eTAwaHN2bHZiMHllNGJiaWlhbmFtNXh3byZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/l3vR85wkETp2cFhBK/giphy.gif" width="500" alt="Game Modes"/>
</div>

### 🟢 Classic Mode
> The original. Hit a wall and it's game over. Simple, clean, brutal at higher levels.

### 🔵 Portal Mode
> **Walls don't kill — they teleport you.** Fly off the right edge, reappear on the left. Fly off the top, reappear at the bottom. Spatial awareness becomes your most important skill.

### 🟡 Maze Mode
> **Navigate a procedurally generated maze.** Every game is different. The recursive backtracker algorithm guarantees a solvable path — but enemies don't care about paths.

### 🔴 Chaos Mode
> **Everything is random. Every 50 ticks, something wild happens:**
> - ↩️ Direction reversal
> - ⚡ Speed surge
> - 🧱 Random wall spawns
> - 🍎 Bonus food drops
> - 🌀 Random teleportation
> - 👾 Enemy spawns
> - 💰 2× score activation

---

## ⚡ Power-Ups

<div align="center">
<img src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExcWJ3dWtxajdwbzYzeXloeXZxanFhNW45djh4NzF3NHBjMm82NGQ2dCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/xT9IgzoKnwFNmISR8I/giphy.gif" width="500" alt="Power Ups"/>
</div>

```
┌─────────────────────────────────────────────────────────────────────┐
│  GLYPH   NAME            EFFECT                                     │
├─────────────────────────────────────────────────────────────────────┤
│   [>]    SPEED BOOST   │ Movement speed +40%                        │
│   [s]    SLOW-MO       │ Time slows — think faster                  │
│   [O]    SHIELD        │ Survive 1 fatal hit. Bounces walls too.    │
│   [g]    GHOST         │ Phase through your own body                │
│   [m]    MAGNET        │ Food items pulled toward your head         │
│   [*]    FREEZE        │ All enemies frozen solid                   │
│   [B]    BOMB          │ Destroys walls + kills nearby enemies      │
│   [2]    2× SCORE      │ Double points for entire duration          │
│   [<]    SHRINK        │ Instantly shed 4 body segments             │
│   [S]    STAR          │ Full invincibility + rainbow mode ★        │
└─────────────────────────────────────────────────────────────────────┘
```

> Power-ups **blink** when about to expire. Stack multiple for insane combos.

---

## 👾 Enemy AI

<div align="center">
<img src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExNWZ3NmpkbWl5ejd1ZWJuOWFzMm5kaTFhaDFveHJmaHBzZGkzYzZhYiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/077i6AULCXc0FKTj9s/giphy.gif" width="400" alt="Enemy AI"/>
</div>

Enemies are not random wanderers. They're hunting you:

- **70% of the time** → direct pursuit — enemy moves one step toward your head
- **30% of the time** → random direction — keeps them unpredictable
- Enemies **respect maze walls** — they pathfind around them
- **Freeze power-up** stops all enemy movement
- **Star mode** lets you destroy enemies for bonus points (+50 × level)
- **Bomb power-up** kills enemies within a 4-cell radius

Enemies spawn progressively — the longer you survive, the more crowded it gets.

---

## 💥 Combo System

<div align="center">
<img src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExZm44OGExcHNla3dwZXR5NnFsMHlqYng1YjFqZzFkdGV3MzFtbWdiZSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/l3vR3h5YjnqB6lnWo/giphy.gif" width="400" alt="Combo"/>
</div>

Eat food within **30 ticks** of the last food to build a combo:

```
x1 → Normal points
x2 → 2× points
x3 → 3× points + COMBO sound effect
x4 → 4× points  ← floating text gets GOLDEN
x5+ → Pure chaos money
```

The combo **resets** if you take too long between meals. Keep the momentum going.

---

## 🏗️ Architecture

<div align="center">
<img src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExNW50dHh2OGZsMmZsdHYxY2o2Mjh0aGM3YzdmaTlwMXZoNnJ1bW9qeCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/coxQHKASG60HrHtvkt/giphy.gif" width="500" alt="Architecture"/>
</div>

The project is split into **9 decoupled modules**, each with a single responsibility:

```
┌──────────────────────────────────────────────────────────────┐
│                        main.cpp                              │
│              Entry point · Game loop orchestration           │
└──────────────────┬───────────────────────────────────────────┘
                   │
        ┌──────────┼──────────┐
        ▼          ▼          ▼
   ┌─────────┐ ┌──────┐ ┌──────────┐
   │  game   │ │  ui  │ │leaderboard│
   │ engine  │ │menus │ │ top 10   │
   └────┬────┘ └──────┘ └──────────┘
        │
   ┌────┼────────────┬──────────────┐
   ▼    ▼            ▼              ▼
┌──────────┐  ┌──────────┐  ┌──────────┐
│ console  │  │particles │  │   maze   │
│ renderer │  │  system  │  │generator │
└──────────┘  └──────────┘  └──────────┘
        │
   ┌────┴──────┐
   │  sound    │
   │ (async)   │
   └───────────┘
```

### Key Design Decisions

| Decision | Rationale |
|:---|:---|
| **Back-buffer rendering** | Diff-based redraw eliminates all console flicker without needing curses |
| **Singleton modules** | Console, Sound, Maze, Particles are stateful singletons — clean access across the codebase |
| **Async Beep threads** | Sound never blocks the game loop — `CreateThread` + `Beep()` per sound event |
| **Binary leaderboard** | Compact, fast, no dependencies — reads/writes in microseconds |
| **Recursive DFS maze** | Guarantees a fully connected maze with one solution path every generation |

---

## 🚀 Getting Started

<div align="center">
<img src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExd2l4ejkybHg1bHo0OXM1bGVuMnZmbzQzM25iZncwdW50cTZvdm5hYSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/qgQUggAC3Pfv687qPC/giphy.gif" width="500" alt="Setup"/>
</div>

### Prerequisites

- **Windows 10/11** (uses Windows Console API)
- **MinGW-w64** with g++ 11+ OR Visual Studio 2019+

### Option 1 — Quick Install (MSYS2)

```powershell
# 1. Install MSYS2
winget install MSYS2.MSYS2

# 2. Install g++ through MSYS2 bash
C:\msys64\usr\bin\bash.exe -lc "pacman -S --noconfirm mingw-w64-x86_64-gcc"
```

### Option 2 — WinLibs (No package manager)
Download a pre-built MinGW-w64 ZIP from [winlibs.com](https://winlibs.com/), extract it, and add the `bin\` folder to your PATH.

### Build & Run

```powershell
# Clone
git clone https://github.com/Nick-ded/Console_Based_Snake_Game.git
cd Console_Based_Snake_Game

# Build (double-click or run in terminal)
build.bat

# Run
./snake_ultra.exe
```

### Manual Build

```bash
g++ -std=c++17 -O2 -o snake_ultra.exe \
    src/main.cpp src/game.cpp src/console.cpp \
    src/sound.cpp src/particles.cpp src/maze.cpp \
    src/leaderboard.cpp src/ui.cpp
```

---

## 🎯 Controls

<div align="center">

| Key | Action |
|:---:|:---|
| `W` / `↑` | Move Up |
| `S` / `↓` | Move Down |
| `A` / `←` | Move Left |
| `D` / `→` | Move Right |
| `P` | Pause / Resume |
| `Q` | Quit to Menu |

</div>

---

## 🏆 Leaderboard

<div align="center">
<img src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExdzRxb2FxdnVidG1vM3VvNHZkZnZqemFtcHE3aGdsMXN1bWFkbThmYyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/26u4lOMA8JKSnL9Uk/giphy.gif" width="400" alt="Trophy"/>
</div>

Scores are saved to `scores.dat` (binary format) in the game directory. The top 10 entries are stored with:

- **Player name** (3 characters)
- **Score**
- **Game mode**
- **Level reached**

Scores persist between sessions and survive game restarts. Beat your own record to flex.

---

## 📁 Project Structure

```
Console_Based_Snake_Game/
│
├── 📄 README.md              ← You are here
├── 🔨 build.bat              ← One-click Windows build script
├── 📋 Makefile               ← GNU Make build file
├── 🚫 .gitignore
│
└── 📁 src/
    ├── 🎯 main.cpp           ← Entry point & outer game loop
    ├── 🌐 globals.h          ← All shared types, enums, constants
    │
    ├── 🖥️  console.h/cpp     ← Back-buffered renderer + screen shake
    ├── 🐍 game.h/cpp         ← Core game engine (~600 lines)
    ├── 🎨 ui.h/cpp           ← Splash, menus, game-over, leaderboard screens
    ├── 💥 particles.h/cpp    ← Physics-based particle system
    ├── 🌀 maze.h/cpp         ← Procedural maze generator (recursive DFS)
    ├── 🔊 sound.h/cpp        ← Async beep-based sound effects
    └── 🏆 leaderboard.h/cpp  ← Persistent binary score storage
```

---

## 🛠️ Tech Stack

<div align="center">

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Windows API](https://img.shields.io/badge/Windows%20API-Console-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![MinGW](https://img.shields.io/badge/MinGW--w64-GCC%2016-FF6C37?style=for-the-badge&logo=gnu&logoColor=white)
![Zero Dependencies](https://img.shields.io/badge/External%20Deps-ZERO-brightgreen?style=for-the-badge)

</div>

**No third-party libraries. No game engines. No frameworks. Just raw C++ and Windows.**

| Component | Technology |
|:---|:---|
| Rendering | Windows Console API (`WriteConsoleA`, `SetConsoleCursorPosition`) |
| Sound | Windows `Beep()` on async threads via `CreateThread` |
| Input | `_kbhit()` / `_getch()` (non-blocking) |
| File I/O | Binary `fstream` for leaderboard persistence |
| Randomness | `rand()` seeded with `time()` |
| Build | g++ / MinGW-w64 / `cl.exe` (MSVC) |

---

## 🤝 Contributing

<div align="center">
<img src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExcnU1cG54OGVrZDdsMnY0azduNWk0YnN3Z3BoMXQybW43YmZidHFwdCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/du3J3cXyzhj75IOgvA/giphy.gif" width="400" alt="Contributing"/>
</div>

Pull requests are welcome. For major changes, open an issue first.

**Ideas for contributions:**
- 🎨 Color theme selector
- 🌐 Network multiplayer via WinSock
- 🤖 Auto-play AI (BFS pathfinding)
- 📊 Stats screen (games played, avg score)
- 🗺️ Custom maze editor

---

<div align="center">

<!-- FOOTER WAVE -->
<img src="https://capsule-render.vercel.app/api?type=waving&color=0:003300,100:00FF41&height=100&section=footer" width="100%"/>

**Built with 🐍 and way too much caffeine**

[![GitHub](https://img.shields.io/badge/GitHub-Nick--ded-181717?style=for-the-badge&logo=github)](https://github.com/Nick-ded)
[![Repo](https://img.shields.io/badge/Repo-Console_Based_Snake_Game-00FF41?style=for-the-badge&logo=github)](https://github.com/Nick-ded/Console_Based_Snake_Game)

*"The only console game that gives you a combo multiplier, a particle explosion, AND screen shake when you die."*

</div>
