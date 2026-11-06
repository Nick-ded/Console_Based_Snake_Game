#include "console.h"
#include <cstdlib>
#include <cmath>

Console& Console::get() {
    static Console inst;
    return inst;
}

void Console::init() {
    m_hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    resize(m_cols, m_rows);
    hideCursor();
    m_back.assign(m_cols * m_rows, Cell{});
    m_front.assign(m_cols * m_rows, Cell{' ', 0xFFFF}); // force first flush
}

void Console::hideCursor() {
    CONSOLE_CURSOR_INFO ci = {1, FALSE};
    SetConsoleCursorInfo(m_hOut, &ci);
}

void Console::resize(int cols, int rows) {
    m_cols = cols; m_rows = rows;
    // Set buffer first, then window (avoids size conflicts)
    COORD bufSize = { (SHORT)cols, (SHORT)rows };
    SetConsoleScreenBufferSize(m_hOut, bufSize);
    SMALL_RECT winRect = { 0, 0, (SHORT)(cols-1), (SHORT)(rows-1) };
    SetConsoleWindowInfo(m_hOut, TRUE, &winRect);
}

void Console::put(int x, int y, char ch, WORD color) {
    x += m_shakeX;
    y += m_shakeY;
    if (x < 0 || x >= m_cols || y < 0 || y >= m_rows) return;
    auto& cell = m_back[y * m_cols + x];
    cell.ch    = ch;
    cell.color = color;
}

void Console::print(int x, int y, const std::string& s, WORD color) {
    for (int i = 0; i < (int)s.size(); i++)
        put(x + i, y, s[i], color);
}

void Console::fill(int x, int y, int w, int h, char ch, WORD color) {
    for (int row = y; row < y + h; row++)
        for (int col = x; col < x + w; col++)
            put(col, row, ch, color);
}

void Console::flush() {
    for (int i = 0; i < m_cols * m_rows; i++) {
        if (m_back[i].ch != m_front[i].ch || m_back[i].color != m_front[i].color) {
            int x = i % m_cols;
            int y = i / m_cols;
            rawPut(x, y, m_back[i].ch, m_back[i].color);
            m_front[i] = m_back[i];
        }
    }
}

void Console::clear() {
    Cell blank{' ', C_BLACK};
    std::fill(m_back.begin(), m_back.end(), blank);
}

void Console::shake(int frames, int intensity) {
    m_shakeTicks     = frames;
    m_shakeIntensity = intensity;
}

void Console::tickShake() {
    if (m_shakeTicks > 0) {
        m_shakeTicks--;
        m_shakeX = (rand() % (m_shakeIntensity * 2 + 1)) - m_shakeIntensity;
        m_shakeY = (rand() % (m_shakeIntensity * 2 + 1)) - m_shakeIntensity;
    } else {
        m_shakeX = 0;
        m_shakeY = 0;
    }
}

void Console::rawPut(int x, int y, char ch, WORD color) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(m_hOut, pos);
    SetConsoleTextAttribute(m_hOut, color);
    DWORD written;
    WriteConsoleA(m_hOut, &ch, 1, &written, nullptr);
}
