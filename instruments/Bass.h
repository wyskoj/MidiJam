//
// Created by Jacob on 03/20/2026.
//

#ifndef MIDIJAM_BASS_H
#define MIDIJAM_BASS_H

#include "instrument.h"

#include <dmusici.h>

#include "model/Ms3dBundle.h"

// ---------------------------------------------------------------------------
// BassState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
struct BassState
{
    __int16 isActive;
    __int16 timeToLive;
    int field_4[88];
    int field_164[88];
    __int16 frets[4];
    int queue[88][16];
    __int16 field_18CC[88][16];
};

static_assert(sizeof(BassState) == 0x23CC, "BassState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(bass, BassState)

extern GLfloat BASS_FRET_HEIGHTS[24];
extern Ms3dBundle* g_bass_ms3d;
extern Ms3dBundle* g_bassShadow_ms3d;
extern Ms3dBundle* g_bassString_ms3d;
extern Ms3dBundle* g_bassNoteFinger_ms3d;
extern Ms3dBundle* g_bassStringBottomX_ms3d[5];
extern __int16 g_bassNotes[24][4];

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderBass();
bool UpdateBass(MUSIC_TIME pmtNow);

#endif // MIDIJAM_BASS_H
