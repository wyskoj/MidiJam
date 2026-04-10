//
// Created by Jacob on 03/22/2026.
//

#ifndef MIDIJAM_TROMBONE_H
#define MIDIJAM_TROMBONE_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// TromboneState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
struct TromboneState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[12];
    int field_34[12];
    int field_64[12][16];
    __int16 field_364[12][16];
    float slidePosition;
};

// static_assert(sizeof(TromboneState) == 0x4E8, "TromboneState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(trombone, TromboneState)

extern Ms3dBundle* g_trombone_ms3d;
extern Ms3dBundle* g_tromboneSlide_ms3d;

void RenderTrombone();
bool UpdateTrombone(MUSIC_TIME pmtNow);

#endif // MIDIJAM_TROMBONE_H