#include "sound.h"

Sound& Sound::get() { static Sound inst; return inst; }

struct BeepNote { DWORD freq; DWORD dur; };

static void playNotes(const BeepNote* notes, int count) {
    for (int i = 0; i < count; i++) {
        if (notes[i].freq == 0) Sleep(notes[i].dur);
        else Beep(notes[i].freq, notes[i].dur);
    }
}

struct ThreadArg { SoundEvent ev; };

DWORD WINAPI Sound::threadProc(LPVOID param) {
    ThreadArg* arg = (ThreadArg*)param;
    SoundEvent ev  = arg->ev;
    delete arg;

    static const BeepNote EAT[]       = {{880,40},{1100,40}};
    static const BeepNote POWERUP[]   = {{523,60},{659,60},{784,60},{1047,80}};
    static const BeepNote DEATH[]     = {{400,100},{300,100},{200,150},{100,250}};
    static const BeepNote LEVELUP[]   = {{523,80},{659,80},{784,80},{1047,80},{1319,120}};
    static const BeepNote SHIELDHIT[] = {{600,50},{400,80}};
    static const BeepNote BOMB[]      = {{200,50},{150,80},{100,150}};
    static const BeepNote STAR[]      = {{1047,60},{1319,60},{1568,60},{2093,100}};
    static const BeepNote MMOVE[]     = {{600,30}};
    static const BeepNote MSEL[]      = {{800,40},{1000,60}};
    static const BeepNote COMBO[]     = {{880,40},{1100,40},{1320,40},{1760,60}};
    static const BeepNote PORTAL[]    = {{300,40},{600,40},{900,60}};

    switch (ev) {
        case SND_EAT:         playNotes(EAT,        2); break;
        case SND_POWERUP:     playNotes(POWERUP,    4); break;
        case SND_DEATH:       playNotes(DEATH,       4); break;
        case SND_LEVEL_UP:    playNotes(LEVELUP,     5); break;
        case SND_SHIELD_HIT:  playNotes(SHIELDHIT,   2); break;
        case SND_BOMB:        playNotes(BOMB,        3); break;
        case SND_STAR:        playNotes(STAR,        4); break;
        case SND_MENU_MOVE:   playNotes(MMOVE,       1); break;
        case SND_MENU_SELECT: playNotes(MSEL,        2); break;
        case SND_COMBO:       playNotes(COMBO,       4); break;
        case SND_PORTAL:      playNotes(PORTAL,      3); break;
    }
    return 0;
}

void Sound::play(SoundEvent ev) {
    if (!m_enabled) return;
    ThreadArg* arg = new ThreadArg{ev};
    HANDLE h = CreateThread(nullptr, 0, threadProc, arg, 0, nullptr);
    if (h) CloseHandle(h);
}
