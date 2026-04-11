//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_TIMPANI_H
#define MIDIJAM_TIMPANI_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// TimpaniState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
struct TimpaniState {
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

static_assert(sizeof(TimpaniState) == 0xB18, "TimpaniState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(timpani, TimpaniState)

extern Ms3dBundle* g_timpaniBody_ms3d;
extern Ms3dBundle* g_timpaniHead_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderTimpani();
bool UpdateTimpani(MUSIC_TIME pmtNow);

#endif // MIDIJAM_TIMPANI_H
