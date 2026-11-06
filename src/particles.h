#pragma once
#include "globals.h"
#include <vector>
#include <string>

class Particles {
public:
    static Particles& get();
    void burst(float cx, float cy, int count, Color col, char glyph = '*');
    void explosion(float cx, float cy, int count);
    void trail(float x, float y, Color col);
    void text(float x, float y, const std::string& s, Color col);
    void update();
    void draw();
    void clear() { m_particles.clear(); m_texts.clear(); }

private:
    Particles() = default;

    struct FloatText {
        float x, y, vy;
        int   life;
        std::string text;
        Color color;
    };

    std::vector<Particle>  m_particles;
    std::vector<FloatText> m_texts;
};
