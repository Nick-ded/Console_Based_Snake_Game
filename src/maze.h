#pragma once
#include "globals.h"
#include <vector>

// ═══════════════════════════════════════════════════════════════
//  Maze generator (randomized DFS / recursive backtracker)
//  Produces a set of wall tiles for MAZE mode
// ═══════════════════════════════════════════════════════════════
class Maze {
public:
    static Maze& get();

    void generate(int seed = 0);   // fills m_walls
    void addRandomWalls(int count);// for CHAOS mode scatter

    bool isWall(int x, int y) const;
    bool isWall(const Point& p)  const { return isWall(p.x, p.y); }
    void setWall(int x, int y, bool v);
    void clearWall(int x, int y)       { setWall(x, y, false); }

    void bombClear(int cx, int cy, int radius); // PU_BOMB effect

    const std::vector<Point>& walls() const { return m_wallList; }
    void rebuildList();

private:
    Maze() = default;

    bool m_grid[BOARD_W][BOARD_H] = {};
    std::vector<Point> m_wallList;

    void dfs(int x, int y, std::vector<std::vector<bool>>& visited);
};
