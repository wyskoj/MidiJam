//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_TUBULARBELLS_H
#define MIDIJAM_TUBULARBELLS_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// TubularBellsState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
struct TubularBellsState {
    __int16 isActive;
    __int16 timeToLive;
    float field_4;
    int field_8[12];
    int field_38[12];
    int field_68[12][16];
    __int16 field_368[12][16];
    float field_4E8[12];
    unsigned __int8 field_518[12][16];
    float field_5D8[12];
    float field_608[12];
    float field_638[12];
    __int16 field_668[12];
};

static_assert(sizeof(TubularBellsState) == 0x680, "TubularBellsState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(tubularBells, TubularBellsState)

extern Ms3dBundle* g_tubularBellDark_ms3d;
extern Ms3dBundle* g_tubularBell_ms3d;
extern Ms3dBundle* g_tubularBellMallet_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderTubularBells();
bool UpdateTubularBells(MUSIC_TIME pmtNow);

#endif // MIDIJAM_TUBULARBELLS_H
