#pragma once
#include "globals.h"
#include <string>
#include <vector>
#include <functional>

// ═══════════════════════════════════════════════════════════════
//  UI screens: splash, main menu, mode select, game over, scores
// ═══════════════════════════════════════════════════════════════
class UI {
public:
    static UI& get();

    GameMode  showMainMenu();          // returns chosen mode
    void      showSplash();
    void      showGameOver(int score, GameMode mode, int level);
    void      showLeaderboard();
    void      showControls();
    std::string promptName();          // ask player for 3-char name

private:
    UI() = default;

    void animateSplash();
    void drawTitle(int offsetY = 0);
    void drawMenuBox(int x, int y, int w, int h,
                     const std::string& title, WORD borderColor);

    int  menuSelect(int x, int y, const std::vector<std::string>& items,
                    WORD normalColor, WORD selectColor,
                    bool animated = true);
};
