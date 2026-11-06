#include "console.h"
#include "leaderboard.h"
#include "game.h"
#include "ui.h"
#include <windows.h>
#include <cstdlib>
#include <ctime>

int main() {
    SetConsoleTitle(TEXT("Snake Ultra Edition"));
    Console::get().init();
    srand((unsigned)time(nullptr));
    Leaderboard::get().load();

    UI::get().showSplash();

    while (true) {
        GameMode mode = UI::get().showMainMenu();
        Game::get().init(mode);
        Game::get().run();
        UI::get().showGameOver(Game::get().score(), mode, Game::get().level());
    }
    return 0;
}
