//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_PIZZICATOSTRINGS_H
#define MIDIJAM_PIZZICATOSTRINGS_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// PizzicatoStringsState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
struct PizzicatoStringsState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[12];
    int field_34[12];
    int field_64[16][12];
    __int16 field_364[12];
    __int16 field_37C[12][16];
};

static_assert(sizeof(PizzicatoStringsState) == 0x4FC, "PizzicatoStringsState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(pizzicatoStrings, PizzicatoStringsState)

extern Ms3dBundle* g_pizzicatoStringHolder_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderPizzicatoStrings();
bool UpdatePizzicatoStrings(MUSIC_TIME pmtNow);

#endif // MIDIJAM_PIZZICATOSTRINGS_H
