//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_STEELDRUM_H
#define MIDIJAM_STEELDRUM_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// SteelDrumState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
struct SteelDrumState {
    __int16 isActive;
    __int16 timeToLive;
    float recoilOffset;
    int field_8[12];
    int field_38[12];
    int field_68[12][32];
    __int16 field_668[12][32];
    float field_968[12];
    unsigned __int8 field_998[12][32];
};

static_assert(sizeof(SteelDrumState) == 0xB18, "SteelDrumState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(steelDrum, SteelDrumState)

extern Ms3dBundle* g_steelDrum_ms3d;
extern Ms3dBundle* g_steelDrumMallet_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderSteelDrum();
bool UpdateSteelDrum(MUSIC_TIME pmtNow);

#endif // MIDIJAM_STEELDRUM_H
