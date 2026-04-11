//
// Created by Jacob on Friday, April 10, 2026.
//

#ifndef MIDIJAM_TRUMPET_H
#define MIDIJAM_TRUMPET_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// TrumpetState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
struct TrumpetState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[54];
    int field_DC[54];
    int field_1B4[54][16];
    __int16 field_F34[54][16];
    __int16 field_15F4;
    __int16 field_15F6;
};

static_assert(sizeof(TrumpetState) == 0x15F8, "TrumpetState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(trumpet, TrumpetState)

extern Ms3dBundle* g_trumpetBody_ms3d;
extern Ms3dBundle* g_trumpetHorn_ms3d;
extern Ms3dBundle* g_trumpetMute_ms3d;
extern Ms3dBundle* g_trumpetKey1_ms3d;
extern Ms3dBundle* g_trumpetKey2_ms3d;
extern Ms3dBundle* g_trumpetKey3_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderTrumpet();
bool UpdateTrumpet(MUSIC_TIME pmtNow);

#endif // MIDIJAM_TRUMPET_H
