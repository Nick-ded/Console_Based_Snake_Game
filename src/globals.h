#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <deque>
#include <functional>

constexpr int BOARD_W      = 40;
constexpr int BOARD_H      = 22;
constexpr int CONSOLE_COLS = 100;
constexpr int CONSOLE_ROWS = 34;

enum Color : WORD {
    C_BLACK        = 0x00,
    C_DARK_BLUE    = 0x01,
    C_DARK_GREEN   = 0x02,
    C_DARK_CYAN    = 0x03,
    C_DARK_RED     = 0x04,
    C_DARK_MAGENTA = 0x05,
    C_DARK_YELLOW  = 0x06,
    C_GRAY         = 0x07,
    C_DARK_GRAY    = 0x08,
    C_BLUE         = 0x09,
    C_GREEN        = 0x0A,
    C_CYAN         = 0x0B,
    C_RED          = 0x0C,
    C_MAGENTA      = 0x0D,
    C_YELLOW       = 0x0E,
    C_WHITE        = 0x0F,
    C_BG_BLUE      = 0x10,
    C_BG_GREEN     = 0x20,
    C_BG_RED       = 0x40,
    C_BG_CYAN      = 0x30,
    C_BG_MAGENTA   = 0x50,
    C_BG_YELLOW    = 0x60,
    C_BG_WHITE     = 0x70,
    C_BG_BLACK     = 0x00,
};

enum Direction { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };

enum GameMode {
    MODE_CLASSIC = 0,
    MODE_PORTAL,
    MODE_MAZE,
    MODE_CHAOS,
    MODE_COUNT
};

static const char* MODE_NAMES[MODE_COUNT] = {
    "CLASSIC", "PORTAL", "MAZE", "CHAOS"
};

static const char* MODE_DESC[MODE_COUNT] = {
    "Traditional snake. Hit a wall = death.",
    "Walls are portals. Teleport through them!",
    "Navigate a maze. Walls everywhere.",
    "Everything is random. Expect the unexpected."
};

enum PowerUpType {
    PU_NONE = -1,
    PU_SPEED_BOOST = 0,
    PU_SLOW_MO,
    PU_SHIELD,
    PU_GHOST,
    PU_MAGNET,
    PU_FREEZE,
    PU_BOMB,
    PU_DOUBLE_SCORE,
    PU_SHRINK,
    PU_STAR,
    PU_COUNT
};

static const char* PU_NAMES[PU_COUNT] = {
    "SPEED BOOST","SLOW-MO","SHIELD","GHOST","MAGNET",
    "FREEZE","BOMB","2X SCORE","SHRINK","STAR"
};
static const char PU_GLYPHS[PU_COUNT] = {
    '>', 's', 'O', 'g', 'm', '*', 'B', '2', '<', 'S'
};
static const Color PU_COLORS[PU_COUNT] = {
    C_YELLOW, C_CYAN, C_BLUE, C_MAGENTA, C_GREEN,
    C_DARK_CYAN, C_RED, C_YELLOW, C_DARK_MAGENTA, C_WHITE
};

struct Point {
    int x, y;
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
    bool operator!=(const Point& o) const { return !(*this == o); }
    Point operator+(const Point& o) const { return {x+o.x, y+o.y}; }
};

struct Particle {
    float x, y;
    float vx, vy;
    int   life;
    char  glyph;
    Color color;
};

struct PowerUp {
    Point       pos;
    PowerUpType type;
    int         blinkTimer;
    bool        active;
};

struct Enemy {
    Point     pos;
    Direction dir;
    int       moveTimer;
    int       moveInterval;
    bool      frozen;
    int       frozenTimer;
    Color     color;
};

struct ScoreEntry {
    std::string name;
    int         score;
    GameMode    mode;
    int         level;
};

struct ActiveEffects {
    int  speedBoost  = 0;
    int  slowMo      = 0;
    int  shield      = 0;
    int  ghost       = 0;
    int  magnet      = 0;
    int  freeze      = 0;
    int  doubleScore = 0;
    int  star        = 0;
    bool hasShield   = false;
};
