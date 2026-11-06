#pragma once
#include "globals.h"
#include <vector>

// ═══════════════════════════════════════════════════════════════
//  Particle system  – purely visual, drawn over the board
// ═══════════════════════════════════════════════════════════════
class Particles {
public:
    static Particles& get();

    void burst(float cx, float cy, int count, Color col, char glyph = '*');
    void explosion(float cx, float cy, int count);    // multi-color boom
    void trail(float x, float y, Color col);          // single trailing spark
    void text(float x, float y, const std::string& s, Color col); // floating text
    void update();                                     // advance physics, decay
    void draw();                                       // render to Console buffer

    void clear() { m_particles.clear(); m_texts.clear(); }

private:
    Particles() = default;

    struct FloatText {
        float x, y;
        float vy;
        int   life;
        std::string text;
        Color color;
    };

    std::vector<Particle>  m_particles;
    std::vector<FloatText> m_texts;
};
