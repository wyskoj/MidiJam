//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_HARMONICA_H
#define MIDIJAM_HARMONICA_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// HarmonicaState -- per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
struct HarmonicaState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[12];
    int field_34[12];
    int field_64[12][16];
    __int16 field_364[12][16];
    float field_4E4[12][100];
    float field_17A4[12][100];
    float field_2A64[12][100];
    float field_3D24[12][100];
    float field_4FE4[12][100];
    float field_62A4[12][100];
    float field_7564[12][100];
    float field_8824[12][100];
    float field_9AE4[12][100];
    float field_ADA4[12][100];
    float field_C064[12][100];
    __int16 field_D324[12];
    __int16 field_D33C[12];
};

static_assert(sizeof(HarmonicaState) == 0xD354, "HarmonicaState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(harmonica, HarmonicaState)

extern Ms3dBundle* g_harmonica_ms3d;
extern Ms3dBundle* g_steamPuff_harmonica_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderHarmonica();
bool UpdateHarmonica(MUSIC_TIME pmtNow);

#endif // MIDIJAM_HARMONICA_H

