#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <deque>
#include <functional>

// ═══════════════════════════════════════════════════════════════
//  BOARD DIMENSIONS
// ═══════════════════════════════════════════════════════════════
constexpr int BOARD_W       = 40;   // playfield cells wide
constexpr int BOARD_H       = 22;   // playfield cells tall
constexpr int CONSOLE_COLS  = 100;
constexpr int CONSOLE_ROWS  = 34;

// ═══════════════════════════════════════════════════════════════
//  COLORS  (Windows console palette indices)
// ═══════════════════════════════════════════════════════════════
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
    // backgrounds
    C_BG_BLUE      = 0x10,
    C_BG_GREEN     = 0x20,
    C_BG_RED       = 0x40,
    C_BG_CYAN      = 0x30,
    C_BG_MAGENTA   = 0x50,
    C_BG_YELLOW    = 0x60,
    C_BG_WHITE     = 0x70,
    C_BG_BLACK     = 0x00,
};

// ═══════════════════════════════════════════════════════════════
//  DIRECTION
// ═══════════════════════════════════════════════════════════════
enum Direction { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };

// ═══════════════════════════════════════════════════════════════
//  GAME MODES
// ═══════════════════════════════════════════════════════════════
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

// ═══════════════════════════════════════════════════════════════
//  POWER-UP TYPES
// ═══════════════════════════════════════════════════════════════
enum PowerUpType {
    PU_NONE = -1,
    PU_SPEED_BOOST = 0,   // faster movement
    PU_SLOW_MO,           // slow down time
    PU_SHIELD,            // survive one collision
    PU_GHOST,             // pass through self
    PU_MAGNET,            // food attracted to head
    PU_FREEZE,            // freeze enemies/obstacles
    PU_BOMB,              // explode nearby walls
    PU_DOUBLE_SCORE,      // 2× points
    PU_SHRINK,            // shrink snake by 3
    PU_STAR,              // invincible + bonus points
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

// ═══════════════════════════════════════════════════════════════
//  POINT
// ═══════════════════════════════════════════════════════════════
struct Point {
    int x, y;
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
    bool operator!=(const Point& o) const { return !(*this == o); }
    Point operator+(const Point& o) const { return {x+o.x, y+o.y}; }
};

// ═══════════════════════════════════════════════════════════════
//  PARTICLE
// ═══════════════════════════════════════════════════════════════
struct Particle {
    float x, y;
    float vx, vy;
    int   life;      // frames remaining
    char  glyph;
    Color color;
};

// ═══════════════════════════════════════════════════════════════
//  POWER-UP INSTANCE
// ═══════════════════════════════════════════════════════════════
struct PowerUp {
    Point       pos;
    PowerUpType type;
    int         blinkTimer;   // for flashing animation
    bool        active;
};

// ═══════════════════════════════════════════════════════════════
//  ENEMY  (roaming obstacle)
// ═══════════════════════════════════════════════════════════════
struct Enemy {
    Point     pos;
    Direction dir;
    int       moveTimer;
    int       moveInterval;  // ticks between moves
    bool      frozen;
    int       frozenTimer;
    Color     color;
};

// ═══════════════════════════════════════════════════════════════
//  SCORE ENTRY
// ═══════════════════════════════════════════════════════════════
struct ScoreEntry {
    std::string name;
    int         score;
    GameMode    mode;
    int         level;
};

// ═══════════════════════════════════════════════════════════════
//  ACTIVE EFFECTS on the snake
// ═══════════════════════════════════════════════════════════════
struct ActiveEffects {
    int  speedBoost    = 0;   // ticks remaining
    int  slowMo        = 0;
    int  shield        = 0;
    int  ghost         = 0;
    int  magnet        = 0;
    int  freeze        = 0;
    int  doubleScore   = 0;
    int  star          = 0;
    bool hasShield     = false;
};
