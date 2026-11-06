#include "console.h"
#include "sound.h"
#include "leaderboard.h"
#include "game.h"
#include "ui.h"
#include <windows.h>
#include <cstdlib>
#include <ctime>

int main() {
    // ── Console setup ─────────────────────────────────────────
    SetConsoleTitle(TEXT("Snake Ultra Edition"));
    Console::get().init();

    // ── Seed RNG ──────────────────────────────────────────────
    srand((unsigned)time(nullptr));

    // ── Load persistent data ──────────────────────────────────
    Leaderboard::get().load();

    // ── Splash ────────────────────────────────────────────────
    UI::get().showSplash();

    // ── Main game loop ────────────────────────────────────────
    while (true) {
        GameMode mode = UI::get().showMainMenu();

        bool playAgain = true;
        while (playAgain) {
            Game::get().init(mode);
            Game::get().run();

            int score = Game::get().score();
            int level = Game::get().level();

            // showGameOver: internally handles name entry and score save.
            // After that it waits for R / M / Q.
            // We re-implement the wait here for cleaner flow.
            UI::get().showGameOver(score, mode, level);

            // After showGameOver returns, go back to main menu
            playAgain = false;
        }
    }

    return 0;
}
