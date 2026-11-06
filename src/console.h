#pragma once
#include "globals.h"
#include <string>
#include <vector>

struct Cell {
    char ch    = ' ';
    WORD color = C_WHITE;
};

class Console {
public:
    static Console& get();
    void  init();
    void  hideCursor();
    void  put(int x, int y, char ch, WORD color);
    void  print(int x, int y, const std::string& s, WORD color);
    void  fill(int x, int y, int w, int h, char ch, WORD color);
    void  flush();
    void  clear();
    void  shake(int frames, int intensity = 1);
    void  tickShake();
    int   shakeOffsetX() const { return m_shakeX; }
    int   shakeOffsetY() const { return m_shakeY; }

private:
    Console() = default;
    HANDLE m_hOut = INVALID_HANDLE_VALUE;
    int    m_cols = CONSOLE_COLS;
    int    m_rows = CONSOLE_ROWS;
    std::vector<Cell> m_back;
    std::vector<Cell> m_front;
    int  m_shakeTicks    = 0;
    int  m_shakeIntensity= 1;
    int  m_shakeX = 0, m_shakeY = 0;
    void rawPut(int x, int y, char ch, WORD color);
};
