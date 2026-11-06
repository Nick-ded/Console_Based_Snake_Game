#pragma once
#include "globals.h"
#include <string>

// ═══════════════════════════════════════════════════════════════
//  Low-level console abstraction
//  Uses a back-buffer to avoid flicker (only writes changed cells)
// ═══════════════════════════════════════════════════════════════

struct Cell {
    char  ch    = ' ';
    WORD  color = C_WHITE;
};

class Console {
public:
    static Console& get();

    void  init();
    void  hideCursor();
    void  resize(int cols, int rows);

    // Buffer API
    void  put(int x, int y, char ch, WORD color);
    void  print(int x, int y, const std::string& s, WORD color);
    void  fill(int x, int y, int w, int h, char ch, WORD color);
    void  flush();          // write back-buffer → screen
    void  clear();          // clear back-buffer (fill with spaces)
    void  shake(int frames, int intensity = 1);   // screen-shake effect
    void  tickShake();      // call once per frame

    int   shakeOffsetX() const { return m_shakeX; }
    int   shakeOffsetY() const { return m_shakeY; }

private:
    Console() = default;

    HANDLE m_hOut   = INVALID_HANDLE_VALUE;
    int    m_cols   = CONSOLE_COLS;
    int    m_rows   = CONSOLE_ROWS;

    std::vector<Cell> m_back;
    std::vector<Cell> m_front;  // what's currently on screen

    // shake
    int  m_shakeTicks   = 0;
    int  m_shakeIntensity = 1;
    int  m_shakeX = 0, m_shakeY = 0;

    void rawPut(int x, int y, char ch, WORD color);
};
