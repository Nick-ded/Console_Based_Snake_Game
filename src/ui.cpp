#include "ui.h"
#include "console.h"
#include "sound.h"
#include "leaderboard.h"
#include "particles.h"
#include <conio.h>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <iomanip>

UI& UI::get() { static UI inst; return inst; }

// ─────────────────────────────────────────────────────────────
//  Helpers
// ─────────────────────────────────────────────────────────────
void UI::drawMenuBox(int x, int y, int w, int h,
                     const std::string& title, WORD bc) {
    auto& con = Console::get();
    // Top
    con.put(x, y, '+', bc);
    for (int i = 1; i < w-1; i++) con.put(x+i, y, '-', bc);
    con.put(x+w-1, y, '+', bc);
    // Sides
    for (int row = 1; row < h-1; row++) {
        con.put(x, y+row, '|', bc);
        con.put(x+w-1, y+row, '|', bc);
    }
    // Bottom
    con.put(x, y+h-1, '+', bc);
    for (int i = 1; i < w-1; i++) con.put(x+i, y+h-1, '-', bc);
    con.put(x+w-1, y+h-1, '+', bc);
    // Title centered
    if (!title.empty()) {
        int tx = x + (w - (int)title.size()) / 2;
        con.print(tx, y, title, C_YELLOW);
    }
}

// Returns index of selected item
int UI::menuSelect(int x, int y,
                   const std::vector<std::string>& items,
                   WORD normalColor, WORD selectColor,
                   bool animated) {
    auto& con = Console::get();
    int   sel = 0;
    int   n   = (int)items.size();

    while (true) {
        for (int i = 0; i < n; i++) {
            bool isSelected = (i == sel);
            std::string prefix = isSelected ? " > " : "   ";
            WORD color = isSelected ? selectColor : normalColor;
            con.print(x, y + i, prefix + items[i] + "  ", color);
        }
        con.flush();

        // Wait for input
        int key = _getch();
        if (key == 0 || key == 0xE0) key = _getch();

        if (key == 72 || key == 'w' || key == 'W') { // up
            sel = (sel - 1 + n) % n;
            Sound::get().play(SND_MENU_MOVE);
        } else if (key == 80 || key == 's' || key == 'S') { // down
            sel = (sel + 1) % n;
            Sound::get().play(SND_MENU_MOVE);
        } else if (key == 13 || key == ' ') { // enter/space
            Sound::get().play(SND_MENU_SELECT);
            return sel;
        } else if (key == 27) { // ESC
            return -1;
        }
        // number shortcuts
        if (key >= '1' && key <= '9') {
            int idx = key - '1';
            if (idx < n) { Sound::get().play(SND_MENU_SELECT); return idx; }
        }
    }
}

// ─────────────────────────────────────────────────────────────
//  ASCII art title (big letters via block chars)
// ─────────────────────────────────────────────────────────────
void UI::drawTitle(int offsetY) {
    auto& con = Console::get();
    int startX = 8;
    int startY = 2 + offsetY;

    // "SNAKE" in block art
    static const char* TITLE[] = {
        " #####  #    #   ###   #   #  #####",
        " #      ##   #  #   #  #  #   #    ",
        " #####  # #  #  #####  ###    #### ",
        "     #  #  # #  #   #  #  #   #    ",
        " #####  #   ##  #   #  #   #  #####",
    };
    static const WORD tcolors[] = {C_GREEN, C_CYAN, C_YELLOW, C_CYAN, C_GREEN};

    for (int i = 0; i < 5; i++)
        con.print(startX, startY + i, TITLE[i], tcolors[i]);

    // Subtitle
    con.print(startX + 8, startY + 6,
              "~ ULTRA EDITION ~", C_DARK_YELLOW);
}

// ─────────────────────────────────────────────────────────────
//  Animated splash
// ─────────────────────────────────────────────────────────────
void UI::animateSplash() {
    auto& con = Console::get();
    Particles::get().clear();

    // Slide-in animation: title comes from top
    for (int frame = 0; frame < 20; frame++) {
        con.clear();
        drawTitle(frame - 20); // negative = above screen, slides in
        con.flush();
        Sleep(20);
    }

    // Burst of particles
    for (int i = 0; i < 30; i++) {
        float x = (float)(10 + rand() % 60);
        float y = (float)(rand() % 8);
        Particles::get().burst(x/2.f, y, 5,
            (Color)(C_GREEN + rand() % 6));
    }

    for (int frame = 0; frame < 20; frame++) {
        con.clear();
        drawTitle(0);
        Particles::get().update();
        Particles::get().draw();
        con.flush();
        Sleep(20);
    }
    Particles::get().clear();
}

// ─────────────────────────────────────────────────────────────
//  Splash screen
// ─────────────────────────────────────────────────────────────
void UI::showSplash() {
    auto& con = Console::get();
    con.clear(); con.flush();
    animateSplash();

    con.clear();
    drawTitle(0);

    // Animated "press any key" blink
    int best = Leaderboard::get().globalBest();
    if (best > 0) {
        con.print(28, 11, "Best Score: " + std::to_string(best), C_YELLOW);
    }

    int frame = 0;
    while (!_kbhit()) {
        frame++;
        WORD col = (frame / 8 % 2 == 0) ? C_WHITE : C_DARK_GRAY;
        con.print(28, 14, "  Press any key to start...  ", col);

        // Idle snake animation along the bottom
        int sx = (frame * 2) % (CONSOLE_COLS + 10) - 5;
        for (int i = 0; i < 8; i++) {
            int px = sx - i * 2;
            if (px >= 0 && px < CONSOLE_COLS)
                con.put(px, 17, i == 0 ? '>' : 'o',
                        i == 0 ? C_GREEN : C_DARK_GREEN);
        }

        con.flush();
        Sleep(50);
        // Clear snake line
        con.fill(0, 17, CONSOLE_COLS, 1, ' ', C_BLACK);
    }
    _getch();
}

// ─────────────────────────────────────────────────────────────
//  Main menu
// ─────────────────────────────────────────────────────────────
GameMode UI::showMainMenu() {
    auto& con = Console::get();

    while (true) {
        con.clear();
        drawTitle(0);

        // Main menu box
        drawMenuBox(5, 11, 38, 12, " MAIN MENU ", C_CYAN);

        std::vector<std::string> items = {
            "1. PLAY - Classic Mode",
            "2. PLAY - Portal Mode",
            "3. PLAY - Maze Mode",
            "4. PLAY - CHAOS Mode",
            "5. Leaderboard",
            "6. Controls",
            "7. Toggle Sound",
            "8. Quit"
        };

        // Mode descriptions on the right
        con.print(48, 13, "MODE DESCRIPTIONS", C_YELLOW);
        for (int i = 0; i < MODE_COUNT; i++) {
            con.print(48, 15 + i * 2,
                      std::string(MODE_NAMES[i]) + ":", C_CYAN);
            con.print(48, 16 + i * 2, MODE_DESC[i], C_GRAY);
        }

        // Best scores per mode on right panel
        con.print(48, 24, "TOP SCORES:", C_YELLOW);
        int row = 25;
        for (auto& e : Leaderboard::get().entries()) {
            if (row > 30) break;
            std::ostringstream ss;
            ss << std::left << std::setw(8) << e.name
               << std::right << std::setw(6) << e.score
               << " " << MODE_NAMES[e.mode];
            con.print(48, row++, ss.str(), C_GREEN);
        }

        int sel = menuSelect(7, 12, items, C_WHITE, C_YELLOW | C_BG_BLUE);

        if (sel == 0) return MODE_CLASSIC;
        if (sel == 1) return MODE_PORTAL;
        if (sel == 2) return MODE_MAZE;
        if (sel == 3) return MODE_CHAOS;
        if (sel == 4) { showLeaderboard(); continue; }
        if (sel == 5) { showControls(); continue; }
        if (sel == 6) {
            Sound::get().setEnabled(!Sound::get().isEnabled());
            continue;
        }
        if (sel == 7 || sel == -1) exit(0);
    }
}

// ─────────────────────────────────────────────────────────────
//  Leaderboard screen
// ─────────────────────────────────────────────────────────────
void UI::showLeaderboard() {
    auto& con = Console::get();
    con.clear();

    drawMenuBox(10, 1, 78, 26, " LEADERBOARD ", C_YELLOW);

    // Header
    con.print(12, 3,  "RANK", C_YELLOW);
    con.print(20, 3,  "NAME", C_YELLOW);
    con.print(30, 3,  "SCORE", C_YELLOW);
    con.print(42, 3,  "MODE", C_YELLOW);
    con.print(54, 3,  "LEVEL", C_YELLOW);
    con.print(12, 4,  std::string(64, '-'), C_DARK_GRAY);

    auto& entries = Leaderboard::get().entries();
    if (entries.empty()) {
        con.print(28, 14, "No scores yet. Play a game!", C_GRAY);
    } else {
        for (int i = 0; i < (int)entries.size() && i < 10; i++) {
            auto& e = entries[i];
            WORD col = (i == 0) ? C_YELLOW :
                       (i == 1) ? C_GRAY :
                       (i == 2) ? C_DARK_YELLOW : C_WHITE;

            std::string rank = "#" + std::to_string(i+1);
            con.print(12, 5 + i*2, rank, col);
            con.print(20, 5 + i*2, e.name.substr(0, 10), col);
            con.print(30, 5 + i*2, std::to_string(e.score), col);
            con.print(42, 5 + i*2, MODE_NAMES[e.mode], col);
            con.print(54, 5 + i*2, std::to_string(e.level), col);

            // mini bar
            int barLen = std::min(20, e.score / 50);
            con.print(62, 5 + i*2, std::string(barLen, '|'), C_GREEN);
        }
    }

    con.print(30, 27, "Press any key to return...", C_GRAY);
    con.flush();
    while (!_kbhit()) Sleep(50);
    _getch();
}

// ─────────────────────────────────────────────────────────────
//  Controls screen
// ─────────────────────────────────────────────────────────────
void UI::showControls() {
    auto& con = Console::get();
    con.clear();
    drawMenuBox(10, 1, 78, 30, " CONTROLS & POWER-UPS ", C_CYAN);

    int cx = 14, cy = 3;
    con.print(cx, cy++, "MOVEMENT", C_YELLOW);
    con.print(cx, cy++, "  W / Up Arrow    Move Up", C_WHITE);
    con.print(cx, cy++, "  S / Down Arrow  Move Down", C_WHITE);
    con.print(cx, cy++, "  A / Left Arrow  Move Left", C_WHITE);
    con.print(cx, cy++, "  D / Right Arrow Move Right", C_WHITE);
    cy++;
    con.print(cx, cy++, "GAME", C_YELLOW);
    con.print(cx, cy++, "  P               Pause / Resume", C_WHITE);
    con.print(cx, cy++, "  Q               Quit to Menu", C_WHITE);
    cy++;
    con.print(cx, cy++, "POWER-UPS", C_YELLOW);

    for (int i = 0; i < PU_COUNT; i++) {
        std::string line = std::string("  [") + PU_GLYPHS[i] + "] " + PU_NAMES[i];
        con.print(cx, cy++, line, PU_COLORS[i]);
    }
    cy++;
    con.print(cx, cy++, "GAME MODES", C_YELLOW);
    for (int i = 0; i < MODE_COUNT; i++) {
        con.print(cx,    cy, std::string("  ") + MODE_NAMES[i] + ":", C_CYAN);
        con.print(cx+14, cy, MODE_DESC[i], C_WHITE);
        cy++;
    }

    con.print(28, 30, "Press any key to return...", C_GRAY);
    con.flush();
    while (!_kbhit()) Sleep(50);
    _getch();
}

// ─────────────────────────────────────────────────────────────
//  Name prompt  (3 chars)
// ─────────────────────────────────────────────────────────────
std::string UI::promptName() {
    auto& con = Console::get();
    int bx = 30, by = 16;
    drawMenuBox(bx-2, by-2, 20, 6, " ENTER NAME ", C_YELLOW);
    con.print(bx, by, "Name (3 chars): ", C_WHITE);
    con.flush();

    std::string name;
    while ((int)name.size() < 3) {
        if (!_kbhit()) { Sleep(30); continue; }
        char c = (char)_getch();
        if (c == 8 && !name.empty()) { // backspace
            name.pop_back();
        } else if (isalnum(c) && (int)name.size() < 3) {
            name += (char)toupper(c);
        }
        // Redraw input line
        con.print(bx, by+1, name + std::string(3 - name.size(), '_'), C_YELLOW);
        con.flush();
    }
    // Wait for Enter
    con.print(bx, by+2, "Press Enter...", C_GRAY);
    con.flush();
    while (true) {
        if (_kbhit()) { char c = (char)_getch(); if (c == 13 || c == ' ') break; }
        Sleep(30);
    }
    return name.empty() ? "AAA" : name;
}

// ─────────────────────────────────────────────────────────────
//  Game Over screen
// ─────────────────────────────────────────────────────────────
void UI::showGameOver(int score, GameMode mode, int level) {
    auto& con = Console::get();

    // Dramatic flash
    for (int flash = 0; flash < 6; flash++) {
        con.clear();
        if (flash % 2 == 0)
            con.fill(0, 0, CONSOLE_COLS, CONSOLE_ROWS, ' ', C_BG_RED);
        con.flush();
        Sleep(80);
    }

    con.clear();

    // Big GAME OVER text
    static const char* GO[] = {
        " ####    ##   #    #  ####      ####  #    # ###### #####  ",
        "#    #  #  #  ##  ## #    #    #    # #    # #      #    # ",
        "#      #    # # ## # #         #    # #    # #####  #    # ",
        "#  ### ###### #    # #  ###    #    # #    # #      #####  ",
        "#    # #    # #    # #    #    #    #  #  #  #      #   #  ",
        " ####  #    # #    #  ####      ####    ##   ###### #    # ",
    };

    for (int i = 0; i < 6; i++) {
        WORD col = (i % 2 == 0) ? C_RED : C_YELLOW;
        con.print(5, 2 + i, GO[i], col);
    }

    // Stats box
    drawMenuBox(25, 10, 44, 12, " STATS ", C_CYAN);
    con.print(27, 12, "Final Score:  ", C_WHITE);
    con.print(41, 12, std::to_string(score), C_YELLOW);

    con.print(27, 13, "Game Mode:    ", C_WHITE);
    con.print(41, 13, MODE_NAMES[mode], C_CYAN);

    con.print(27, 14, "Level Reached:", C_WHITE);
    con.print(42, 14, std::to_string(level), C_GREEN);

    con.print(27, 15, "World Best:   ", C_WHITE);
    con.print(41, 15, std::to_string(Leaderboard::get().globalBest()), C_YELLOW);

    // Check if it's a new best
    if (score >= Leaderboard::get().globalBest()) {
        con.print(27, 17, "*** NEW HIGH SCORE! ***", C_YELLOW);
    }

    con.print(27, 19, "[R] Play Again", C_GREEN);
    con.print(27, 20, "[M] Main Menu ", C_CYAN);
    con.print(27, 21, "[Q] Quit      ", C_RED);

    con.flush();

    // Prompt for name entry
    std::string name = promptName();
    Leaderboard::get().addEntry(name, score, mode, level);

    // Wait for R/M/Q
    while (true) {
        if (!_kbhit()) { Sleep(30); continue; }
        char k = (char)tolower(_getch());
        if (k == 'r') return;           // caller handles restart
        if (k == 'm') return;           // back to menu
        if (k == 'q') exit(0);
    }
}
