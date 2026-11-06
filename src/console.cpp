#include "console.h"
#include <cstdlib>

Console& Console::get() { static Console inst; return inst; }

void Console::init() {
    m_hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    // Set buffer then window size
    COORD bufSize = { (SHORT)m_cols, (SHORT)m_rows };
    SetConsoleScreenBufferSize(m_hOut, bufSize);
    SMALL_RECT wr = { 0, 0, (SHORT)(m_cols-1), (SHORT)(m_rows-1) };
    SetConsoleWindowInfo(m_hOut, TRUE, &wr);
    hideCursor();
    Cell blank{' ', C_BLACK};
    m_back.assign(m_cols * m_rows, blank);
    m_front.assign(m_cols * m_rows, Cell{' ', 0xFFFF});
}

void Console::hideCursor() {
    CONSOLE_CURSOR_INFO ci = {1, FALSE};
    SetConsoleCursorInfo(m_hOut, &ci);
}

void Console::put(int x, int y, char ch, WORD color) {
    x += m_shakeX; y += m_shakeY;
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
            rawPut(i % m_cols, i / m_cols, m_back[i].ch, m_back[i].color);
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
        m_shakeX = (rand() % (m_shakeIntensity*2+1)) - m_shakeIntensity;
        m_shakeY = (rand() % (m_shakeIntensity*2+1)) - m_shakeIntensity;
    } else {
        m_shakeX = 0; m_shakeY = 0;
    }
}

void Console::rawPut(int x, int y, char ch, WORD color) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(m_hOut, pos);
    SetConsoleTextAttribute(m_hOut, color);
    DWORD written;
    WriteConsoleA(m_hOut, &ch, 1, &written, nullptr);
}
