#pragma once
#include "globals.h"
#include <vector>
#include <functional>

class Maze {
public:
    static Maze& get();
    void generate(int seed = 0);
    void addRandomWalls(int count);
    bool isWall(int x, int y) const;
    bool isWall(const Point& p) const { return isWall(p.x, p.y); }
    void setWall(int x, int y, bool v);
    void clearWall(int x, int y) { setWall(x, y, false); }
    void bombClear(int cx, int cy, int radius);
    const std::vector<Point>& walls() const { return m_wallList; }
    void rebuildList();
private:
    Maze() = default;
    bool m_grid[BOARD_W][BOARD_H];
    std::vector<Point> m_wallList;
};
