#pragma once
#include "globals.h"
#include <string>
#include <vector>

class UI {
public:
    static UI& get();
    GameMode    showMainMenu();
    void        showSplash();
    void        showGameOver(int score, GameMode mode, int level);
    void        showLeaderboard();
    void        showControls();
    std::string promptName();

private:
    UI() = default;
    void drawTitle(int offsetY = 0);
    void drawBox(int x, int y, int w, int h, const std::string& title, WORD bc);
    int  menuSelect(int x, int y, const std::vector<std::string>& items,
                    WORD normal, WORD selected);
};
