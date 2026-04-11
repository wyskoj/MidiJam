//
// Created by Jacob on 03/22/2026.
//

#ifndef MIDIJAM_GUITAR_H
#define MIDIJAM_GUITAR_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// GuitarState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
#pragma pack(push, 1)
struct GuitarState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[88];
    int field_164[88];
    __int16 field_2C4[6];
    __int16 field_2D0;
    int field_2D2[88][16];
    __int16 field_18D2[88][16];
};
#pragma pack(pop)

static_assert(sizeof(GuitarState) == 0x23D2, "GuitarState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(guitar, GuitarState)

extern Ms3dBundle* g_guitar_ms3d;
extern Ms3dBundle* g_guitarStringLow_ms3d;
extern Ms3dBundle* g_guitarLowStringBottomX_ms3d[5];
extern Ms3dBundle* g_guitarHighStringBottomX_ms3d[5];
extern Ms3dBundle* g_guitarStringHigh_ms3d;
extern Ms3dBundle* g_guitarNoteFinger_ms3d;

extern GLfloat GUITAR_FRET_HEIGHTS[25];
extern GLfloat GUITAR_FRET_HEIGHTS_AS_PERCENT[23];

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderGuitar();
bool UpdateGuitar(MUSIC_TIME pmtNow);

#endif // MIDIJAM_GUITAR_H
