//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_MELODIC_TOM_H
#define MIDIJAM_MELODIC_TOM_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// MelodicTomState -- per-instance state
// ---------------------------------------------------------------------------
struct MelodicTomState {
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

static_assert(sizeof(MelodicTomState) == 0xB18, "MelodicTomState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(melodicTom, MelodicTomState)

extern Ms3dBundle* g_melodicTom_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderMelodicTom();
bool UpdateMelodicTom(MUSIC_TIME pmtNow);

#endif // MIDIJAM_MELODIC_TOM_H

