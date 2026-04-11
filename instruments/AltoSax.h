//
// Created by Jacob on Friday, April 10, 2026.
//

#ifndef MIDIJAM_ALTOSAX_H
#define MIDIJAM_ALTOSAX_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// AltoSaxState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
#pragma pack(push, 1)
struct AltoSaxState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[33];
    int field_88[33];
    int field_10C[33][16];
    __int16 field_94C[33][16];
    __int16 field_D6C;
};
#pragma pack(pop)

static_assert(sizeof(AltoSaxState) == 0xD6E, "AltoSaxState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(altoSax, AltoSaxState)

// TODO: model pointer externs
extern Ms3dBundle* g_altoSaxBody_ms3d;
extern Ms3dBundle* g_altoSaxHorn_ms3d;
extern Ms3dBundle *g_altoSaxKeyX_ms3d[20][2];

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderAltoSax();
bool UpdateAltoSax(MUSIC_TIME pmtNow);

#endif // MIDIJAM_ALTOSAX_H
