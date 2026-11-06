#include "leaderboard.h"
#include <fstream>
#include <algorithm>

Leaderboard& Leaderboard::get() {
    static Leaderboard inst;
    return inst;
}

void Leaderboard::load() {
    m_entries.clear();
    std::ifstream f("scores.dat", std::ios::binary);
    if (!f) return;
    int count = 0;
    f.read((char*)&count, sizeof(count));
    for (int i = 0; i < count; i++) {
        ScoreEntry e;
        int nameLen = 0;
        f.read((char*)&nameLen, sizeof(nameLen));
        e.name.resize(nameLen);
        f.read(&e.name[0], nameLen);
        f.read((char*)&e.score, sizeof(e.score));
        int mode = 0, level = 0;
        f.read((char*)&mode,  sizeof(mode));
        f.read((char*)&level, sizeof(level));
        e.mode  = (GameMode)mode;
        e.level = level;
        m_entries.push_back(e);
    }
}

void Leaderboard::save() {
    std::ofstream f("scores.dat", std::ios::binary);
    if (!f) return;
    int count = (int)m_entries.size();
    f.write((char*)&count, sizeof(count));
    for (auto& e : m_entries) {
        int nameLen = (int)e.name.size();
        f.write((char*)&nameLen, sizeof(nameLen));
        f.write(e.name.data(), nameLen);
        f.write((char*)&e.score, sizeof(e.score));
        int mode  = (int)e.mode;
        int level = e.level;
        f.write((char*)&mode,  sizeof(mode));
        f.write((char*)&level, sizeof(level));
    }
}

void Leaderboard::addEntry(const std::string& name, int score, GameMode mode, int level) {
    m_entries.push_back({name, score, mode, level});
    std::sort(m_entries.begin(), m_entries.end(),
              [](const ScoreEntry& a, const ScoreEntry& b){ return a.score > b.score; });
    if ((int)m_entries.size() > MAX_ENTRIES)
        m_entries.resize(MAX_ENTRIES);
    save();
}

int Leaderboard::globalBest() const {
    if (m_entries.empty()) return 0;
    return m_entries.front().score;
}
