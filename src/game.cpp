#include "game.h"
#include "console.h"
#include "sound.h"
#include "particles.h"
#include "maze.h"
#include "leaderboard.h"
#include <conio.h>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <string>
#include <functional>

Game& Game::get() { static Game inst; return inst; }

// ---------------------------------------------------------------
//  Helpers
// ---------------------------------------------------------------
void Game::drawCell(int gx, int gy, char ch1, char ch2, WORD color) {
    Console::get().put(cx(gx),   cy(gy), ch1, color);
    Console::get().put(cx(gx)+1, cy(gy), ch2, color);
}

bool Game::isFree(const Point& p) const {
    if (p.x < 0 || p.x >= BOARD_W || p.y < 0 || p.y >= BOARD_H) return false;
    if (Maze::get().isWall(p)) return false;
    for (auto& s : m_snake) if (s == p) return false;
    for (auto& f : m_food)  if (f == p) return false;
    for (auto& pu : m_powerups) if (pu.pos == p) return false;
    return true;
}

Point Game::randomFreeCell() const {
    Point p = {BOARD_W/2, BOARD_H/2};
    for (int attempts = 0; attempts < 1000; attempts++) {
        p = { rand() % BOARD_W, rand() % BOARD_H };
        if (isFree(p)) break;
    }
    return p;
}

Point Game::wrapPoint(Point p) const {
    if (p.x < 0)        p.x = BOARD_W - 1;
    if (p.x >= BOARD_W) p.x = 0;
    if (p.y < 0)        p.y = BOARD_H - 1;
    if (p.y >= BOARD_H) p.y = 0;
    return p;
}

Point Game::nextHead() const {
    Point h = m_snake.front();
    switch (m_dir) {
        case DIR_UP:    h.y--; break;
        case DIR_DOWN:  h.y++; break;
        case DIR_LEFT:  h.x--; break;
        case DIR_RIGHT: h.x++; break;
    }
    return h;
}

bool Game::collidesWall(const Point& p) const {
    if (m_mode == MODE_PORTAL) return false;
    if (p.x < 0 || p.x >= BOARD_W || p.y < 0 || p.y >= BOARD_H) return true;
    if (m_mode == MODE_MAZE || m_mode == MODE_CHAOS)
        return Maze::get().isWall(p);
    return false;
}

bool Game::collidesSelf(const Point& p) const {
    if (m_fx.ghost > 0 || m_fx.star > 0) return false;
    for (size_t i = 1; i < m_snake.size(); i++)
        if (m_snake[i] == p) return true;
    return false;
}

int Game::baseScore() const {
    return 10 * m_level * (m_fx.doubleScore > 0 ? 2 : 1);
}

int Game::effectiveSpeed() const {
    int s = m_tickSpeed;
    if (m_fx.speedBoost > 0) s = (std::max)(40, s - 60);
    if (m_fx.slowMo > 0)     s = (std::min)(400, s + 100);
    if (m_fx.star > 0)       s = (std::max)(50, s - 40);
    return s;
}

// ---------------------------------------------------------------
//  Init
// ---------------------------------------------------------------
void Game::init(GameMode mode) {
    m_mode = mode;
    m_snake.clear(); m_food.clear();
    m_powerups.clear(); m_enemies.clear();
    m_gameOver = false; m_paused = false;
    m_score = 0; m_level = 1; m_combo = 0; m_comboTimer = 0;
    m_tickSpeed = 150; m_fx = {};
    m_powerupSpawnTimer = 100;
    m_enemySpawnTimer   = 200;
    m_chaosEventTimer   = 80;
    Particles::get().clear();

    if (mode == MODE_MAZE) {
        Maze::get().generate();
    } else {
        for (int x = 0; x < BOARD_W; x++)
            for (int y = 0; y < BOARD_H; y++)
                Maze::get().setWall(x, y, false);
        Maze::get().rebuildList();
        if (mode == MODE_CHAOS)
            Maze::get().addRandomWalls(30);
    }

    int mx = BOARD_W/2, my = BOARD_H/2;
    m_snake.push_back({mx,   my});
    m_snake.push_back({mx-1, my});
    m_snake.push_back({mx-2, my});
    m_dir = DIR_RIGHT; m_nextDir = DIR_RIGHT;

    spawnFood();
    if (mode == MODE_CHAOS) spawnFood();
}

// ---------------------------------------------------------------
//  Spawn
// ---------------------------------------------------------------
void Game::spawnFood() {
    if ((int)m_food.size() >= 3) return;
    m_food.push_back(randomFreeCell());
}

void Game::spawnPowerup() {
    if ((int)m_powerups.size() >= 3) return;
    PowerUp pu;
    pu.pos = randomFreeCell();
    pu.type = (PowerUpType)(rand() % PU_COUNT);
    pu.blinkTimer = 0; pu.active = true;
    m_powerups.push_back(pu);
}

void Game::spawnEnemy() {
    if ((int)m_enemies.size() >= 4) return;
    Enemy e;
    e.pos = randomFreeCell();
    e.dir = (Direction)(rand() % 4);
    e.moveTimer = 0; e.moveInterval = 4 + rand() % 6;
    e.frozen = false; e.frozenTimer = 0;
    static const Color ecols[] = {C_RED, C_MAGENTA, C_DARK_RED, C_YELLOW};
    e.color = ecols[rand() % 4];
    m_enemies.push_back(e);
}

// ---------------------------------------------------------------
//  Collect events
// ---------------------------------------------------------------
void Game::collectFood(int idx) {
    Point fp = m_food[idx];
    m_food.erase(m_food.begin() + idx);
    m_comboTimer = 30;
    m_combo++;
    int earned = baseScore() * (m_combo > 1 ? m_combo : 1);
    m_score += earned;

    std::string txt = "+" + std::to_string(earned);
    if (m_combo > 1) txt += " x" + std::to_string(m_combo);
    Particles::get().text((float)fp.x, (float)fp.y - 0.5f, txt,
                          m_combo > 2 ? C_YELLOW : C_WHITE);
    Particles::get().burst((float)fp.x, (float)fp.y, 8, C_GREEN, '*');

    if (m_combo >= 3) Sound::get().play(SND_COMBO);
    else              Sound::get().play(SND_EAT);

    int newLevel = m_score / 150 + 1;
    if (newLevel > m_level) {
        m_level = newLevel;
        m_tickSpeed = (std::max)(55, 150 - (m_level-1)*12);
        Sound::get().play(SND_LEVEL_UP);
        Particles::get().text((float)BOARD_W/2, (float)BOARD_H/2,
                              "LEVEL " + std::to_string(m_level) + "!", C_YELLOW);
    }
    spawnFood();
}

void Game::collectPowerup(int idx) {
    PowerUpType type = m_powerups[idx].type;
    Point pos        = m_powerups[idx].pos;
    m_powerups.erase(m_powerups.begin() + idx);

    Sound::get().play(SND_POWERUP);
    Particles::get().burst((float)pos.x, (float)pos.y, 15, PU_COLORS[type], PU_GLYPHS[type]);
    Particles::get().text((float)pos.x, (float)pos.y - 1, PU_NAMES[type], PU_COLORS[type]);

    int dur = 60 + m_level * 5;
    switch (type) {
        case PU_SPEED_BOOST:  m_fx.speedBoost  = dur; break;
        case PU_SLOW_MO:      m_fx.slowMo      = dur; break;
        case PU_SHIELD:       m_fx.shield = dur; m_fx.hasShield = true; break;
        case PU_GHOST:        m_fx.ghost       = dur; break;
        case PU_MAGNET:       m_fx.magnet      = dur; break;
        case PU_FREEZE:
            m_fx.freeze = dur;
            for (auto& e : m_enemies) { e.frozen = true; e.frozenTimer = dur; }
            break;
        case PU_DOUBLE_SCORE: m_fx.doubleScore = dur; break;
        case PU_STAR:         m_fx.star = dur; Sound::get().play(SND_STAR); break;
        case PU_SHRINK:
            for (int i = 0; i < 4 && (int)m_snake.size() > 3; i++)
                m_snake.pop_back();
            break;
        case PU_BOMB: triggerBomb(); break;
        default: break;
    }
}

void Game::hitEnemy(int idx) {
    if (m_fx.star > 0) {
        Particles::get().explosion((float)m_enemies[idx].pos.x,
                                   (float)m_enemies[idx].pos.y, 20);
        m_score += 50 * m_level;
        m_enemies.erase(m_enemies.begin() + idx);
        return;
    }
    if (m_fx.hasShield) {
        m_fx.hasShield = false; m_fx.shield = 0;
        Sound::get().play(SND_SHIELD_HIT);
        Console::get().shake(6, 2);
        Particles::get().burst((float)m_snake.front().x,
                               (float)m_snake.front().y, 20, C_BLUE, 'O');
        m_enemies.erase(m_enemies.begin() + idx);
        return;
    }
    triggerDeath();
}

void Game::triggerBomb() {
    Point h = m_snake.front();
    Maze::get().bombClear(h.x, h.y, 4);
    Particles::get().explosion((float)h.x, (float)h.y, 40);
    Console::get().shake(10, 3);
    Sound::get().play(SND_BOMB);
    for (int i = (int)m_enemies.size()-1; i >= 0; i--) {
        int dx = m_enemies[i].pos.x - h.x;
        int dy = m_enemies[i].pos.y - h.y;
        if (dx*dx + dy*dy <= 25) {
            m_score += 30;
            m_enemies.erase(m_enemies.begin() + i);
        }
    }
}

void Game::triggerDeath() {
    m_gameOver = true;
    Sound::get().play(SND_DEATH);
    Console::get().shake(20, 3);
    Particles::get().explosion((float)m_snake.front().x,
                               (float)m_snake.front().y, 50);
    for (auto& s : m_snake)
        Particles::get().burst((float)s.x, (float)s.y, 3, C_RED, 'X');
}

// ---------------------------------------------------------------
//  Update sub-systems
// ---------------------------------------------------------------
void Game::updateEffects() {
    auto tick = [](int& v){ if (v > 0) v--; };
    tick(m_fx.speedBoost); tick(m_fx.slowMo);  tick(m_fx.shield);
    tick(m_fx.ghost);      tick(m_fx.magnet);  tick(m_fx.freeze);
    tick(m_fx.doubleScore);tick(m_fx.star);
    if (m_fx.shield == 0) m_fx.hasShield = false;
}

void Game::updateCombo() {
    if (m_comboTimer > 0) { m_comboTimer--; if (m_comboTimer == 0) m_combo = 0; }
}

void Game::updateMagnet() {
    if (m_fx.magnet <= 0) return;
    Point h = m_snake.front();
    for (auto& f : m_food) {
        Point nf = f;
        if (f.x < h.x) nf.x++;
        else if (f.x > h.x) nf.x--;
        if (f.y < h.y) nf.y++;
        else if (f.y > h.y) nf.y--;
        if (isFree(nf)) f = nf;
    }
}

void Game::updateEnemies() {
    for (auto& e : m_enemies) {
        if (e.frozen) {
            if (e.frozenTimer > 0) e.frozenTimer--; else e.frozen = false;
            continue;
        }
        e.moveTimer++;
        if (e.moveTimer < e.moveInterval) continue;
        e.moveTimer = 0;
        Point target = m_snake.front();
        bool chase = (rand() % 10) < 7;
        if (chase) {
            int dx = target.x - e.pos.x;
            int dy = target.y - e.pos.y;
            Point np = e.pos;
            if (abs(dx) >= abs(dy)) np.x += (dx > 0) ? 1 : -1;
            else                    np.y += (dy > 0) ? 1 : -1;
            if (!Maze::get().isWall(np) && np.x >= 0 && np.x < BOARD_W &&
                np.y >= 0 && np.y < BOARD_H)
                e.pos = np;
        } else {
            const int ddx[] = {0,0,-1,1};
            const int ddy[] = {-1,1,0,0};
            int d = rand() % 4;
            Point np = { e.pos.x + ddx[d], e.pos.y + ddy[d] };
            if (!Maze::get().isWall(np) && np.x >= 0 && np.x < BOARD_W &&
                np.y >= 0 && np.y < BOARD_H)
                e.pos = np;
        }
    }
}

void Game::triggerChaosEvent() {
    int ev = rand() % 8;
    switch (ev) {
        case 0:
            if (m_dir == DIR_UP)         m_dir = DIR_DOWN;
            else if (m_dir == DIR_DOWN)  m_dir = DIR_UP;
            else if (m_dir == DIR_LEFT)  m_dir = DIR_RIGHT;
            else                         m_dir = DIR_LEFT;
            m_nextDir = m_dir;
            Particles::get().text((float)BOARD_W/2, 2, "REVERSED!", C_RED);
            break;
        case 1:
            m_fx.speedBoost = 30;
            Particles::get().text((float)BOARD_W/2, 2, "SPEED SURGE!", C_YELLOW);
            break;
        case 2:
            Maze::get().addRandomWalls(5);
            Particles::get().text((float)BOARD_W/2, 2, "WALLS!", C_DARK_RED);
            break;
        case 3:
            spawnFood(); spawnFood();
            Particles::get().text((float)BOARD_W/2, 2, "BONUS FOOD!", C_GREEN);
            break;
        case 4: {
            Point np = randomFreeCell();
            m_snake.front() = np;
            Particles::get().text((float)BOARD_W/2, 2, "TELEPORT!", C_CYAN);
            Sound::get().play(SND_PORTAL);
            break;
        }
        case 5:
            spawnEnemy();
            Particles::get().text((float)BOARD_W/2, 2, "ENEMY!", C_RED);
            break;
        case 6:
            m_fx.doubleScore = 40;
            Particles::get().text((float)BOARD_W/2, 2, "2X SCORE!", C_YELLOW);
            break;
        case 7:
            m_fx.slowMo = 40;
            Particles::get().text((float)BOARD_W/2, 2, "SLOW-MO!", C_CYAN);
            break;
    }
}

// ---------------------------------------------------------------
//  Input
// ---------------------------------------------------------------
void Game::handleInput() {
    if (!_kbhit()) return;
    int key = _getch();
    if (key == 0 || key == 0xE0) {
        key = _getch();
        switch (key) {
            case 72: if (m_dir != DIR_DOWN)  m_nextDir = DIR_UP;    break;
            case 80: if (m_dir != DIR_UP)    m_nextDir = DIR_DOWN;  break;
            case 75: if (m_dir != DIR_RIGHT) m_nextDir = DIR_LEFT;  break;
            case 77: if (m_dir != DIR_LEFT)  m_nextDir = DIR_RIGHT; break;
        }
        return;
    }
    switch (tolower(key)) {
        case 'w': if (m_dir != DIR_DOWN)  m_nextDir = DIR_UP;    break;
        case 's': if (m_dir != DIR_UP)    m_nextDir = DIR_DOWN;  break;
        case 'a': if (m_dir != DIR_RIGHT) m_nextDir = DIR_LEFT;  break;
        case 'd': if (m_dir != DIR_LEFT)  m_nextDir = DIR_RIGHT; break;
        case 'p': m_paused = !m_paused; break;
        case 'q': m_gameOver = true;    break;
    }
}

// ---------------------------------------------------------------
//  Main update
// ---------------------------------------------------------------
void Game::update() {
    m_dir = m_nextDir;
    updateEffects();
    updateCombo();

    m_powerupSpawnTimer--;
    if (m_powerupSpawnTimer <= 0) {
        spawnPowerup();
        m_powerupSpawnTimer = 80 + rand() % 60;
    }

    if (m_mode != MODE_CLASSIC) {
        m_enemySpawnTimer--;
        if (m_enemySpawnTimer <= 0) {
            spawnEnemy();
            m_enemySpawnTimer = 150 + rand() % 100;
        }
        updateEnemies();
    }

    if (m_mode == MODE_CHAOS) {
        m_chaosEventTimer--;
        if (m_chaosEventTimer <= 0) {
            triggerChaosEvent();
            m_chaosEventTimer = 50 + rand() % 60;
        }
    }

    updateMagnet();
    for (auto& pu : m_powerups) pu.blinkTimer++;

    Point head = nextHead();
    if (m_mode == MODE_PORTAL) head = wrapPoint(head);

    if (collidesWall(head)) {
        if (m_fx.hasShield) {
            m_fx.hasShield = false; m_fx.shield = 0;
            Sound::get().play(SND_SHIELD_HIT);
            Console::get().shake(5, 2);
            if (m_dir == DIR_UP)         m_nextDir = DIR_DOWN;
            else if (m_dir == DIR_DOWN)  m_nextDir = DIR_UP;
            else if (m_dir == DIR_LEFT)  m_nextDir = DIR_RIGHT;
            else                         m_nextDir = DIR_LEFT;
            return;
        }
        triggerDeath(); return;
    }
    if (collidesSelf(head)) { triggerDeath(); return; }

    for (int i = 0; i < (int)m_enemies.size(); i++) {
        if (m_enemies[i].pos == head) {
            hitEnemy(i);
            if (m_gameOver) return;
            break;
        }
    }

    m_snake.push_front(head);

    bool ate = false;
    for (int i = 0; i < (int)m_food.size(); i++) {
        if (m_food[i] == head) { collectFood(i); ate = true; break; }
    }
    if (!ate) m_snake.pop_back();

    for (int i = 0; i < (int)m_powerups.size(); i++) {
        if (m_powerups[i].pos == head) { collectPowerup(i); break; }
    }

    if (m_fx.star > 0)
        Particles::get().trail((float)m_snake.back().x,
                               (float)m_snake.back().y, C_YELLOW);
}

// ---------------------------------------------------------------
//  Draw
// ---------------------------------------------------------------
void Game::drawBoard() {
    auto& con = Console::get();
    for (int x = 0; x <= BOARD_W*2+1; x++) {
        con.put(x, 1,          '-', C_DARK_CYAN);
        con.put(x, BOARD_H+2,  '-', C_DARK_CYAN);
    }
    for (int y = 1; y <= BOARD_H+2; y++) {
        con.put(0,            y, '|', C_DARK_CYAN);
        con.put(BOARD_W*2+1,  y, '|', C_DARK_CYAN);
    }
    con.put(0,           1,          '+', C_CYAN);
    con.put(BOARD_W*2+1, 1,          '+', C_CYAN);
    con.put(0,           BOARD_H+2,  '+', C_CYAN);
    con.put(BOARD_W*2+1, BOARD_H+2,  '+', C_CYAN);

    std::string badge = std::string(" ") + MODE_NAMES[m_mode] + " ";
    con.print(2, 1, badge, C_YELLOW);

    if (m_mode == MODE_MAZE || m_mode == MODE_CHAOS) {
        for (auto& w : Maze::get().walls())
            drawCell(w.x, w.y, '#', '#', C_DARK_YELLOW);
    }
}

void Game::drawSnake() {
    if (m_snake.empty()) return;

    WORD headColor = C_GREEN;
    WORD bodyColor = C_DARK_GREEN;

    if (m_fx.star > 0) {
        static int rt = 0; rt++;
        static const WORD rc[] = {C_RED,C_YELLOW,C_GREEN,C_CYAN,C_BLUE,C_MAGENTA,C_WHITE};
        headColor = rc[(rt/3) % 7];
        bodyColor = rc[(rt/3 + 2) % 7];
    } else if (m_fx.ghost > 0) {
        headColor = C_MAGENTA; bodyColor = C_DARK_MAGENTA;
    } else if (m_fx.hasShield) {
        headColor = C_CYAN; bodyColor = C_DARK_CYAN;
    } else if (m_fx.speedBoost > 0) {
        headColor = C_YELLOW; bodyColor = C_DARK_YELLOW;
    }

    for (int i = (int)m_snake.size()-1; i >= 1; i--)
        drawCell(m_snake[i].x, m_snake[i].y, '(', ')', bodyColor);

    const char* hg = "[]";
    switch (m_dir) {
        case DIR_UP:    hg = "/\\"; break;
        case DIR_DOWN:  hg = "\\/"; break;
        case DIR_LEFT:  hg = "<]";  break;
        case DIR_RIGHT: hg = "[>";  break;
    }
    drawCell(m_snake.front().x, m_snake.front().y, hg[0], hg[1], headColor);

    if (m_fx.hasShield) {
        Point h = m_snake.front();
        int hx = cx(h.x), hy = cy(h.y);
        WORD ac = (m_fx.shield / 5 % 2 == 0) ? C_CYAN : C_BLUE;
        Console::get().put(hx-1, hy-1, '*', ac);
        Console::get().put(hx+2, hy-1, '*', ac);
        Console::get().put(hx-1, hy+1, '*', ac);
        Console::get().put(hx+2, hy+1, '*', ac);
    }
}

void Game::drawFood() {
    static int blink = 0; blink++;
    for (auto& f : m_food) {
        WORD fc = (blink / 4 % 2 == 0) ? C_RED : C_YELLOW;
        drawCell(f.x, f.y, '@', '@', fc);
    }
}

void Game::drawPowerups() {
    for (auto& pu : m_powerups) {
        if (!pu.active) continue;
        if (pu.blinkTimer >= 220) { pu.active = false; continue; }
        bool show = (pu.blinkTimer < 180) || (pu.blinkTimer / 3 % 2 == 0);
        if (show) {
            Console::get().put(cx(pu.pos.x),   cy(pu.pos.y), '[',             PU_COLORS[pu.type]);
            Console::get().put(cx(pu.pos.x)+1, cy(pu.pos.y), PU_GLYPHS[pu.type], PU_COLORS[pu.type]);
        }
    }
    m_powerups.erase(
        std::remove_if(m_powerups.begin(), m_powerups.end(),
                       [](const PowerUp& p){ return !p.active; }),
        m_powerups.end());
}

void Game::drawEnemies() {
    for (auto& e : m_enemies) {
        WORD col  = e.frozen ? C_CYAN : e.color;
        char g1   = e.frozen ? '[' : '<';
        char g2   = e.frozen ? ']' : '>';
        drawCell(e.pos.x, e.pos.y, g1, g2, col);
    }
}

void Game::drawHUD() {
    auto& con = Console::get();
    int hudY = BOARD_H + 3;
    con.fill(0, hudY,   CONSOLE_COLS, 1, ' ', C_BLACK);
    con.fill(0, hudY+1, CONSOLE_COLS, 1, ' ', C_BLACK);

    con.print(1,  hudY, "SCORE:", C_GRAY);
    con.print(8,  hudY, std::to_string(m_score), C_YELLOW);
    con.print(18, hudY, "BEST:", C_GRAY);
    con.print(24, hudY, std::to_string(Leaderboard::get().globalBest()), C_GREEN);
    con.print(34, hudY, "LVL:", C_GRAY);
    con.print(39, hudY, std::to_string(m_level), C_CYAN);
    con.print(46, hudY, "LEN:", C_GRAY);
    con.print(51, hudY, std::to_string((int)m_snake.size()), C_WHITE);

    if (m_combo > 1) {
        std::string cs = "COMBO x" + std::to_string(m_combo) + "!";
        WORD cc = (m_comboTimer / 3 % 2 == 0) ? C_YELLOW : C_WHITE;
        con.print(60, hudY, cs, cc);
    }

    int ex = 1;
    auto printFx = [&](const char* name, int timer, WORD col) {
        if (timer <= 0) return;
        std::string s = std::string("[") + name;
        if (timer < 20) s += "!";
        s += "]";
        con.print(ex, hudY+1, s, col);
        ex += (int)s.size() + 1;
    };
    printFx("SPD",  m_fx.speedBoost,  C_YELLOW);
    printFx("SLO",  m_fx.slowMo,      C_CYAN);
    printFx("SHD",  m_fx.shield,      C_BLUE);
    printFx("GHO",  m_fx.ghost,       C_MAGENTA);
    printFx("MAG",  m_fx.magnet,      C_GREEN);
    printFx("FRZ",  m_fx.freeze,      C_DARK_CYAN);
    printFx("2XP",  m_fx.doubleScore, C_YELLOW);
    printFx("STAR", m_fx.star,        C_WHITE);

    con.print(CONSOLE_COLS-30, hudY+1, "[WASD] [P]ause [Q]uit", C_DARK_GRAY);
}

void Game::drawPauseOverlay() {
    auto& con = Console::get();
    int px = BOARD_W - 4, py = BOARD_H/2 - 2;
    con.fill(px, py, 18, 4, ' ', C_BG_BLUE);
    con.print(px+2, py+1, "*** PAUSED ***",  C_YELLOW);
    con.print(px+1, py+2, "P:Resume  Q:Quit", C_WHITE);
}

void Game::draw() {
    auto& con = Console::get();
    con.clear();
    con.tickShake();
    drawBoard();
    drawFood();
    drawPowerups();
    drawEnemies();
    drawSnake();
    drawHUD();
    Particles::get().draw();
    if (m_paused) drawPauseOverlay();
    con.flush();
}

// ---------------------------------------------------------------
//  Main game loop
// ---------------------------------------------------------------
void Game::run() {
    DWORD lastTick = GetTickCount();
    while (!m_gameOver) {
        handleInput();
        if (m_gameOver) break;
        DWORD now = GetTickCount();
        if (!m_paused && (int)(now - lastTick) >= effectiveSpeed()) {
            lastTick = now;
            update();
            Particles::get().update();
        }
        draw();
        Sleep(8);
    }
    // Death animation
    for (int i = 0; i < 40 && !_kbhit(); i++) {
        Particles::get().update();
        Console::get().tickShake();
        draw();
        Sleep(30);
    }
}
