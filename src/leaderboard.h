#pragma once
#include "globals.h"
#include <vector>
#include <string>

class Leaderboard {
public:
    static Leaderboard& get();
    void  load();
    void  save();
    void  addEntry(const std::string& name, int score, GameMode mode, int level);
    const std::vector<ScoreEntry>& entries() const { return m_entries; }
    int   globalBest() const;
private:
    Leaderboard() = default;
    std::vector<ScoreEntry> m_entries;
    static constexpr int MAX_ENTRIES = 10;
};
