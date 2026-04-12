//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_OCARINA_H
#define MIDIJAM_OCARINA_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// OcarinaState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
#pragma pack(push, 1)
struct OcarinaState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[12];
    int field_34[12];
    int field_64[12][16];
    __int16 field_364[12][16];
    __int16 field_4E4;
};
#pragma pack(pop)

static_assert(sizeof(OcarinaState) == 0x4E6, "OcarinaState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(ocarina, OcarinaState)

extern Ms3dBundle* g_ocarina_ms3d;
extern Ms3dBundle* g_ocarinaHandX_ms3d[12];

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderOcarina();
bool UpdateOcarina(MUSIC_TIME pmtNow);

#endif // MIDIJAM_OCARINA_H
