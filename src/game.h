#pragma once
#include "globals.h"
#include <deque>
#include <vector>

class Game {
public:
    static Game& get();
    void init(GameMode mode);
    void run();
    bool isOver() const { return m_gameOver; }
    int  score()  const { return m_score; }
    int  level()  const { return m_level; }
    int  combo()  const { return m_combo; }
    GameMode mode()const { return m_mode; }
    const ActiveEffects& effects() const { return m_fx; }

private:
    Game() = default;

    std::deque<Point>    m_snake;
    std::vector<Point>   m_food;
    std::vector<PowerUp> m_powerups;
    std::vector<Enemy>   m_enemies;

    Direction  m_dir     = DIR_RIGHT;
    Direction  m_nextDir = DIR_RIGHT;
    GameMode   m_mode    = MODE_CLASSIC;

    int  m_score     = 0;
    int  m_level     = 1;
    int  m_combo     = 0;
    int  m_comboTimer= 0;
    int  m_tickSpeed = 150;
    bool m_gameOver  = false;
    bool m_paused    = false;

    ActiveEffects m_fx;

    int  m_powerupSpawnTimer = 0;
    int  m_enemySpawnTimer   = 0;
    int  m_chaosEventTimer   = 0;

    void handleInput();
    void update();
    void draw();
    void drawBoard();
    void drawSnake();
    void drawFood();
    void drawPowerups();
    void drawEnemies();
    void drawHUD();
    void drawPauseOverlay();

    Point nextHead() const;
    bool  collidesWall(const Point& p) const;
    bool  collidesSelf(const Point& p) const;
    Point wrapPoint(Point p) const;

    void spawnFood();
    void spawnPowerup();
    void spawnEnemy();
    void collectFood(int idx);
    void collectPowerup(int idx);
    void hitEnemy(int idx);
    void triggerDeath();
    void triggerBomb();

    void updateEffects();
    void updateEnemies();
    void updateCombo();
    void updateMagnet();
    void triggerChaosEvent();

    int  baseScore() const;
    int  effectiveSpeed() const;
    bool isFree(const Point& p) const;
    Point randomFreeCell() const;

    void drawCell(int gx, int gy, char ch1, char ch2, WORD color);
    int  cx(int gx) const { return gx * 2 + 2; }
    int  cy(int gy) const { return gy + 2; }
};
