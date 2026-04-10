//
// Created by Jacob on 04/10/2026.
//

#ifndef MIDIJAM_STAGECHOIR_H
#define MIDIJAM_STAGECHOIR_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// StageChoirState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
struct StageChoirState {
    __int16 isVisible;
    __int16 timeToLive;
    int field_4[12];
    int field_34[12];
    int field_64[12][16];
    __int16 field_364[12][16];
};

static_assert(sizeof(StageChoirState) == 0x4E4, "StageChoirState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(stageChoir, StageChoirState)

extern Ms3dBundle* g_stageChoir_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderStageChoir();
bool UpdateStageChoir(MUSIC_TIME pmtNow);

#endif // MIDIJAM_STAGECHOIR_H
