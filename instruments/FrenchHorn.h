//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_FRENCHHORN_H
#define MIDIJAM_FRENCHHORN_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// FrenchHornState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
#pragma pack(push, 1)
struct FrenchHornState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[36];
    int field_94[36];
    int field_124[36][16];
    __int16 field_A24[36][16];
    __int16 field_EA4;
};
#pragma pack(pop)

static_assert(sizeof(FrenchHornState) == 0xEA6, "FrenchHornState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(frenchHorn, FrenchHornState)
extern Ms3dBundle* g_frenchHornBody_ms3d;
extern Ms3dBundle* g_frenchHornHorn_ms3d;
extern Ms3dBundle* g_frenchHornKey1_ms3d;
extern Ms3dBundle* g_frenchHornKey2_ms3d;
extern Ms3dBundle* g_frenchHornKey3_ms3d;
extern Ms3dBundle* g_frenchHornTrigger_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderFrenchHorn();
bool UpdateFrenchHorn(MUSIC_TIME pmtNow);

#endif // MIDIJAM_FRENCHHORN_H
