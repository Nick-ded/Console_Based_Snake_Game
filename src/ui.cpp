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

void UI::drawBox(int x, int y, int w, int h, const std::string& title, WORD bc) {
    auto& con = Console::get();
    con.put(x, y, '+', bc);
    for (int i = 1; i < w-1; i++) con.put(x+i, y, '-', bc);
    con.put(x+w-1, y, '+', bc);
    for (int row = 1; row < h-1; row++) {
        con.put(x, y+row, '|', bc);
        con.put(x+w-1, y+row, '|', bc);
        con.fill(x+1, y+row, w-2, 1, ' ', C_BLACK);
    }
    con.put(x, y+h-1, '+', bc);
    for (int i = 1; i < w-1; i++) con.put(x+i, y+h-1, '-', bc);
    con.put(x+w-1, y+h-1, '+', bc);
    if (!title.empty()) {
        int tx = x + (w - (int)title.size()) / 2;
        con.print(tx, y, title, C_YELLOW);
    }
}

int UI::menuSelect(int x, int y, const std::vector<std::string>& items,
                   WORD normal, WORD selected) {
    auto& con = Console::get();
    int sel = 0, n = (int)items.size();
    while (true) {
        for (int i = 0; i < n; i++) {
            std::string line = (i == sel ? " > " : "   ") + items[i] + "   ";
            con.print(x, y+i, line, i == sel ? selected : normal);
        }
        con.flush();
        int key = _getch();
        if (key == 0 || key == 0xE0) key = _getch();
        if      (key == 72 || tolower(key) == 'w') { sel = (sel-1+n)%n; Sound::get().play(SND_MENU_MOVE); }
        else if (key == 80 || tolower(key) == 's') { sel = (sel+1)%n;   Sound::get().play(SND_MENU_MOVE); }
        else if (key == 13 || key == ' ')           { Sound::get().play(SND_MENU_SELECT); return sel; }
        else if (key == 27) return -1;
        else if (key >= '1' && key <= '9') {
            int idx = key - '1';
            if (idx < n) { Sound::get().play(SND_MENU_SELECT); return idx; }
        }
    }
}

void UI::drawTitle(int offsetY) {
    auto& con = Console::get();
    static const char* T[] = {
        " #####  #    #   ###   #   #  #####",
        " #      ##   #  #   #  #  #   #    ",
        " #####  # #  #  #####  ###    #### ",
        "     #  #  # #  #   #  #  #   #    ",
        " #####  #   ##  #   #  #   #  #####",
    };
    static const WORD tc[] = {C_GREEN, C_CYAN, C_YELLOW, C_CYAN, C_GREEN};
    int sx = 8, sy = 2 + offsetY;
    for (int i = 0; i < 5; i++)
        con.print(sx, sy+i, T[i], tc[i]);
    con.print(sx+8, sy+6, "~ ULTRA EDITION ~", C_DARK_YELLOW);
}

void UI::showSplash() {
    auto& con = Console::get();
    con.clear(); con.flush();

    for (int frame = -18; frame <= 0; frame++) {
        con.clear();
        drawTitle(frame);
        con.flush();
        Sleep(18);
    }

    for (int i = 0; i < 20; i++) {
        float x = (float)(5 + rand() % 40);
        float y = (float)(rand() % 8);
        Particles::get().burst(x, y, 4, (Color)(C_GREEN + rand() % 6));
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

    int best = Leaderboard::get().globalBest();
    if (best > 0)
        con.print(28, 11, "Best Score: " + std::to_string(best), C_YELLOW);

    int frame = 0;
    while (!_kbhit()) {
        frame++;
        WORD col = (frame/8 % 2 == 0) ? C_WHITE : C_DARK_GRAY;
        con.print(24, 14, "   Press any key to start...   ", col);

        int sx = (frame * 2) % (CONSOLE_COLS + 12) - 6;
        for (int i = 0; i < 8; i++) {
            int px = sx - i*2;
            if (px >= 0 && px < CONSOLE_COLS)
                con.put(px, 17, i == 0 ? '>' : 'o',
                        i == 0 ? C_GREEN : C_DARK_GREEN);
        }
        con.flush();
        Sleep(50);
        con.fill(0, 17, CONSOLE_COLS, 1, ' ', C_BLACK);
    }
    _getch();
}

GameMode UI::showMainMenu() {
    auto& con = Console::get();
    while (true) {
        con.clear();
        drawTitle(0);
        drawBox(3, 11, 40, 14, " MAIN MENU ", C_CYAN);

        std::vector<std::string> items = {
            "1. PLAY  - Classic Mode",
            "2. PLAY  - Portal Mode",
            "3. PLAY  - Maze Mode",
            "4. PLAY  - CHAOS Mode",
            "5. Leaderboard",
            "6. Controls",
            "7. Toggle Sound (" + std::string(Sound::get().isEnabled() ? "ON" : "OFF") + ")",
            "8. Quit"
        };

        con.print(48, 13, "MODE INFO", C_YELLOW);
        for (int i = 0; i < MODE_COUNT; i++) {
            con.print(48, 15 + i*2, std::string(MODE_NAMES[i]) + ":", C_CYAN);
            con.print(50, 16 + i*2, MODE_DESC[i], C_GRAY);
        }

        con.print(48, 24, "TOP SCORES:", C_YELLOW);
        int row = 25;
        for (auto& e : Leaderboard::get().entries()) {
            if (row > 31) break;
            std::ostringstream ss;
            ss << std::left << std::setw(6) << e.name
               << std::right << std::setw(6) << e.score
               << " " << MODE_NAMES[e.mode];
            con.print(48, row++, ss.str(), C_GREEN);
        }

        int sel = menuSelect(5, 12, items, C_WHITE, C_YELLOW);
        if (sel == 0) return MODE_CLASSIC;
        if (sel == 1) return MODE_PORTAL;
        if (sel == 2) return MODE_MAZE;
        if (sel == 3) return MODE_CHAOS;
        if (sel == 4) { showLeaderboard(); continue; }
        if (sel == 5) { showControls();    continue; }
        if (sel == 6) { Sound::get().setEnabled(!Sound::get().isEnabled()); continue; }
        if (sel == 7 || sel == -1) exit(0);
    }
}

void UI::showLeaderboard() {
    auto& con = Console::get();
    con.clear();
    drawBox(8, 1, 82, 28, " LEADERBOARD ", C_YELLOW);

    con.print(12, 3, "RANK",  C_YELLOW);
    con.print(20, 3, "NAME",  C_YELLOW);
    con.print(30, 3, "SCORE", C_YELLOW);
    con.print(42, 3, "MODE",  C_YELLOW);
    con.print(54, 3, "LEVEL", C_YELLOW);
    con.fill(12, 4, 64, 1, '-', C_DARK_GRAY);

    auto& entries = Leaderboard::get().entries();
    if (entries.empty()) {
        con.print(28, 14, "No scores yet. Play a game!", C_GRAY);
    } else {
        for (int i = 0; i < (int)entries.size() && i < 10; i++) {
            auto& e = entries[i];
            WORD col = (i==0)?C_YELLOW:(i==1)?C_GRAY:(i==2)?C_DARK_YELLOW:C_WHITE;
            con.print(12, 5+i*2, "#" + std::to_string(i+1), col);
            con.print(20, 5+i*2, e.name.substr(0,10), col);
            con.print(30, 5+i*2, std::to_string(e.score), col);
            con.print(42, 5+i*2, MODE_NAMES[e.mode], col);
            con.print(54, 5+i*2, std::to_string(e.level), col);
            con.print(62, 5+i*2, std::string((std::min)(20, e.score/50), '|'), C_GREEN);
        }
    }
    con.print(30, 27, "Press any key to return...", C_GRAY);
    con.flush();
    while (!_kbhit()) Sleep(50);
    _getch();
}

void UI::showControls() {
    auto& con = Console::get();
    con.clear();
    drawBox(8, 1, 82, 30, " CONTROLS & POWER-UPS ", C_CYAN);
    int px = 12, py = 3;
    con.print(px, py++, "MOVEMENT", C_YELLOW);
    con.print(px, py++, "  W / Up     Move Up",    C_WHITE);
    con.print(px, py++, "  S / Down   Move Down",  C_WHITE);
    con.print(px, py++, "  A / Left   Move Left",  C_WHITE);
    con.print(px, py++, "  D / Right  Move Right", C_WHITE);
    py++;
    con.print(px, py++, "GAME KEYS", C_YELLOW);
    con.print(px, py++, "  P   Pause/Resume", C_WHITE);
    con.print(px, py++, "  Q   Quit to Menu", C_WHITE);
    py++;
    con.print(px, py++, "POWER-UPS", C_YELLOW);
    for (int i = 0; i < PU_COUNT; i++) {
        std::string line = std::string("  [") + PU_GLYPHS[i] + "] " + PU_NAMES[i];
        con.print(px, py++, line, PU_COLORS[i]);
    }
    py++;
    con.print(px, py++, "MODES", C_YELLOW);
    for (int i = 0; i < MODE_COUNT; i++) {
        con.print(px,    py, std::string("  ") + MODE_NAMES[i] + ":", C_CYAN);
        con.print(px+14, py, MODE_DESC[i], C_WHITE);
        py++;
    }
    con.print(30, 30, "Press any key to return...", C_GRAY);
    con.flush();
    while (!_kbhit()) Sleep(50);
    _getch();
}

std::string UI::promptName() {
    auto& con = Console::get();
    int bx = 32, by = 16;
    drawBox(bx-2, by-2, 22, 7, " YOUR NAME ", C_YELLOW);
    con.print(bx, by,   "Enter 3 chars:", C_WHITE);
    con.print(bx, by+2, "Press Enter when done", C_GRAY);
    con.flush();

    std::string name;
    while ((int)name.size() < 3) {
        if (!_kbhit()) { Sleep(30); continue; }
        char c = (char)_getch();
        if (c == 8 && !name.empty()) name.pop_back();
        else if (isalnum((unsigned char)c) && (int)name.size() < 3)
            name += (char)toupper((unsigned char)c);
        con.print(bx, by+1, name + std::string(3 - (int)name.size(), '_'), C_YELLOW);
        con.flush();
    }
    con.print(bx, by+3, "Press Enter...", C_GRAY);
    con.flush();
    while (true) {
        if (_kbhit()) { char c = (char)_getch(); if (c==13||c==' ') break; }
        Sleep(30);
    }
    return name.empty() ? "AAA" : name;
}

void UI::showGameOver(int score, GameMode mode, int level) {
    auto& con = Console::get();

    for (int f = 0; f < 6; f++) {
        con.clear();
        if (f % 2 == 0) con.fill(0, 0, CONSOLE_COLS, CONSOLE_ROWS, ' ', C_BG_RED);
        con.flush();
        Sleep(80);
    }
    con.clear();

    static const char* GO[] = {
        " ####    ##   #    #  ####      ####  #    # ###### #####  ",
        "#    #  #  #  ##  ## #    #    #    # #    # #      #    # ",
        "#      #    # # ## # #         #    # #    # #####  #    # ",
        "#  ### ###### #    # #  ###    #    # #    # #      #####  ",
        "#    # #    # #    # #    #    #    #  #  #  #      #   #  ",
        " ####  #    # #    #  ####      ####    ##   ###### #    # ",
    };
    for (int i = 0; i < 6; i++)
        con.print(4, 2+i, GO[i], i%2==0 ? C_RED : C_YELLOW);

    drawBox(24, 10, 46, 10, " STATS ", C_CYAN);
    con.print(26, 12, "Final Score:   ", C_WHITE);
    con.print(42, 12, std::to_string(score), C_YELLOW);
    con.print(26, 13, "Game Mode:     ", C_WHITE);
    con.print(42, 13, MODE_NAMES[mode], C_CYAN);
    con.print(26, 14, "Level Reached: ", C_WHITE);
    con.print(42, 14, std::to_string(level), C_GREEN);
    con.print(26, 15, "World Best:    ", C_WHITE);
    con.print(42, 15, std::to_string(Leaderboard::get().globalBest()), C_YELLOW);

    if (score >= Leaderboard::get().globalBest())
        con.print(26, 17, "*** NEW HIGH SCORE! ***", C_YELLOW);

    con.print(26, 19, "[R] Play Again", C_GREEN);
    con.print(26, 20, "[M] Main Menu",  C_CYAN);
    con.print(26, 21, "[Q] Quit",       C_RED);
    con.flush();

    std::string name = promptName();
    Leaderboard::get().addEntry(name, score, mode, level);

    while (true) {
        if (!_kbhit()) { Sleep(30); continue; }
        char k = (char)tolower(_getch());
        if (k=='r' || k=='m' || k=='q') {
            if (k=='q') exit(0);
            return;
        }
    }
}
