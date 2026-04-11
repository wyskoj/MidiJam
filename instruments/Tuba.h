//
// Created by Jacob on Friday, April 10, 2026.
//

#ifndef MIDIJAM_TUBA_H
#define MIDIJAM_TUBA_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// TubaState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
#pragma pack(push, 1)
struct TubaState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[44];
    int field_B4[44];
    int field_164[44][16];
    __int16 field_C64[44][16];
    __int16 field_11E4;
};
#pragma pack(pop)

static_assert(sizeof(TubaState) == 0x11E6, "TubaState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(tuba, TubaState)
extern Ms3dBundle *g_tubaBody_ms3d;
extern Ms3dBundle *g_tubaHorn_ms3d;
extern Ms3dBundle *g_tubaKey_1_ms3d;
extern Ms3dBundle *g_tubaKey_2_ms3d;
extern Ms3dBundle *g_tubaKey_3_ms3d;
extern Ms3dBundle *g_tubaKey_4_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderTuba();
bool UpdateTuba(MUSIC_TIME pmtNow);

#endif // MIDIJAM_TUBA_H
