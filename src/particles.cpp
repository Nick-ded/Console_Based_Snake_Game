#include "particles.h"
#include "console.h"
#include <cstdlib>
#include <cmath>

static int bx(float gx) { return (int)(gx * 2) + 2; }
static int by(float gy) { return (int)gy + 2; }

Particles& Particles::get() { static Particles inst; return inst; }

void Particles::burst(float cx, float cy, int count, Color col, char glyph) {
    for (int i = 0; i < count; i++) {
        float angle = (float)(rand() % 628) / 100.f;
        float speed = 0.3f + (rand() % 100) / 200.f;
        Particle p;
        p.x = cx; p.y = cy;
        p.vx    = cosf(angle) * speed;
        p.vy    = sinf(angle) * speed * 0.5f;
        p.life  = 8 + rand() % 8;
        p.glyph = glyph;
        p.color = col;
        m_particles.push_back(p);
    }
}

void Particles::explosion(float cx, float cy, int count) {
    static const Color cols[]  = { C_RED, C_YELLOW, C_WHITE, C_CYAN, C_MAGENTA };
    static const char glyphs[] = { '*', '+', '.', 'o', '#' };
    for (int i = 0; i < count; i++) {
        float angle = (float)(rand() % 628) / 100.f;
        float speed = 0.5f + (rand() % 100) / 100.f;
        Particle p;
        p.x = cx; p.y = cy;
        p.vx    = cosf(angle) * speed;
        p.vy    = sinf(angle) * speed * 0.5f;
        p.life  = 10 + rand() % 10;
        p.glyph = glyphs[rand() % 5];
        p.color = cols[rand() % 5];
        m_particles.push_back(p);
    }
}

void Particles::trail(float x, float y, Color col) {
    Particle p;
    p.x     = x; p.y = y;
    p.vx    = ((rand() % 20) - 10) / 100.f;
    p.vy    = ((rand() % 10) - 5)  / 100.f;
    p.life  = 3 + rand() % 4;
    p.glyph = '.';
    p.color = col;
    m_particles.push_back(p);
}

void Particles::text(float x, float y, const std::string& s, Color col) {
    FloatText ft;
    ft.x = x; ft.y = y; ft.vy = -0.05f;
    ft.life = 20; ft.text = s; ft.color = col;
    m_texts.push_back(ft);
}

void Particles::update() {
    for (auto it = m_particles.begin(); it != m_particles.end();) {
        it->x += it->vx; it->y += it->vy; it->life--;
        if (it->life <= 0) it = m_particles.erase(it); else ++it;
    }
    for (auto it = m_texts.begin(); it != m_texts.end();) {
        it->y += it->vy; it->life--;
        if (it->life <= 0) it = m_texts.erase(it); else ++it;
    }
}

void Particles::draw() {
    auto& con = Console::get();
    for (auto& p : m_particles)
        con.put(bx(p.x), by(p.y), p.glyph, p.color);
    for (auto& ft : m_texts) {
        int px = bx(ft.x) - (int)ft.text.size();
        con.print(px, by(ft.y), ft.text, ft.color);
    }
}
