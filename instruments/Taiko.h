//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_TAIKO_H
#define MIDIJAM_TAIKO_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// TaikoState -- per-instance state
// ---------------------------------------------------------------------------
struct TaikoState {
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

static_assert(sizeof(TaikoState) == 0xB18, "TaikoState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(taiko, TaikoState)

extern Ms3dBundle* g_taiko_ms3d;
extern Ms3dBundle* g_taikoStick_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderTaiko();
bool UpdateTaiko(MUSIC_TIME pmtNow);

#endif // MIDIJAM_TAIKO_H

