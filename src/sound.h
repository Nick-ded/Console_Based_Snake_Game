#pragma once
#include <windows.h>

enum SoundEvent {
    SND_EAT, SND_POWERUP, SND_DEATH, SND_LEVEL_UP,
    SND_SHIELD_HIT, SND_BOMB, SND_STAR,
    SND_MENU_MOVE, SND_MENU_SELECT, SND_COMBO, SND_PORTAL
};

class Sound {
public:
    static Sound& get();
    void play(SoundEvent ev);
    void setEnabled(bool v) { m_enabled = v; }
    bool isEnabled() const  { return m_enabled; }
private:
    Sound() = default;
    bool m_enabled = true;
    static DWORD WINAPI threadProc(LPVOID param);
};
