//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_SYNTH_DRUM_H
#define MIDIJAM_SYNTH_DRUM_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// SynthDrumState -- per-instance state
// ---------------------------------------------------------------------------
struct SynthDrumState {
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

static_assert(sizeof(SynthDrumState) == 0xB18, "SynthDrumState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(synthDrum, SynthDrumState)

extern Ms3dBundle* g_synthDrum_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderSynthDrum();
bool UpdateSynthDrum(MUSIC_TIME pmtNow);

#endif // MIDIJAM_SYNTH_DRUM_H

