#include "maze.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <functional>

Maze& Maze::get() { static Maze inst; return inst; }

bool Maze::isWall(int x, int y) const {
    if (x < 0 || x >= BOARD_W || y < 0 || y >= BOARD_H) return true;
    return m_grid[x][y];
}

void Maze::setWall(int x, int y, bool v) {
    if (x < 0 || x >= BOARD_W || y < 0 || y >= BOARD_H) return;
    m_grid[x][y] = v;
}

void Maze::rebuildList() {
    m_wallList.clear();
    for (int x = 0; x < BOARD_W; x++)
        for (int y = 0; y < BOARD_H; y++)
            if (m_grid[x][y]) m_wallList.push_back({x, y});
}

void Maze::generate(int seed) {
    srand(seed ? seed : (unsigned)time(nullptr));
    memset(m_grid, 0, sizeof(m_grid));

    const int CW = BOARD_W / 2;
    const int CH = BOARD_H / 2;

    for (int x = 0; x < BOARD_W; x++)
        for (int y = 0; y < BOARD_H; y++)
            m_grid[x][y] = true;

    std::vector<std::vector<bool>> visited(CW, std::vector<bool>(CH, false));

    std::function<void(int,int)> dfs = [&](int cx, int cy) {
        visited[cx][cy] = true;
        m_grid[cx*2][cy*2] = false;

        int dirs[4] = {0,1,2,3};
        for (int i = 3; i > 0; i--) std::swap(dirs[i], dirs[rand()%(i+1)]);

        const int dx[] = {0,0,-1,1};
        const int dy[] = {-1,1,0,0};

        for (int d : dirs) {
            int nx = cx + dx[d], ny = cy + dy[d];
            if (nx < 0 || nx >= CW || ny < 0 || ny >= CH) continue;
            if (visited[nx][ny]) continue;
            m_grid[cx*2 + dx[d]][cy*2 + dy[d]] = false;
            dfs(nx, ny);
        }
    };

    dfs(1, 1);

    // Clear center area for snake start
    int mx = BOARD_W / 2, my = BOARD_H / 2;
    for (int dx = -2; dx <= 2; dx++)
        for (int dy = -2; dy <= 2; dy++)
            setWall(mx+dx, my+dy, false);

    rebuildList();
}

void Maze::addRandomWalls(int count) {
    for (int i = 0; i < count; i++) {
        int x = 2 + rand() % (BOARD_W - 4);
        int y = 2 + rand() % (BOARD_H - 4);
        if (abs(x - BOARD_W/2) < 4 && abs(y - BOARD_H/2) < 4) continue;
        m_grid[x][y] = true;
        m_wallList.push_back({x, y});
    }
}

void Maze::bombClear(int cx, int cy, int radius) {
    for (int dx = -radius; dx <= radius; dx++)
        for (int dy = -radius; dy <= radius; dy++)
            if (dx*dx + dy*dy <= radius*radius)
                setWall(cx+dx, cy+dy, false);
    rebuildList();
}
