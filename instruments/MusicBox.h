//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_MUSICBOX_H
#define MIDIJAM_MUSICBOX_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// MusicBoxState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
struct MusicBoxState {
    __int16 isActive;
    __int16 timeToLive;
    float field_4;
    int field_8[12];
    int field_38[12];
    int field_68[12][16];
    __int16 field_368[12][16];
    float field_4E8[12][16];
    float field_7E8[12];
    unsigned __int8 field_818[12][16];
    float field_8D8;
};

static_assert(sizeof(MusicBoxState) == 0x8DC, "MusicBoxState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(musicBox, MusicBoxState)

extern Ms3dBundle *g_musicBoxSpindle_ms3d;
extern Ms3dBundle *g_musicBoxTopBlade_ms3d;
extern Ms3dBundle *g_musicBoxCase_ms3d;
extern Ms3dBundle *g_musicBoxPoint_ms3d;
extern Ms3dBundle *g_musicBoxKey_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderMusicBox();
bool UpdateMusicBox(MUSIC_TIME pmtNow);

#endif // MIDIJAM_MUSICBOX_H
