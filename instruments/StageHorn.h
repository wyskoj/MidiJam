//
// Created by Jacob on 03/21/2026.
//

#ifndef MIDIJAM_STAGEHORN_H
#define MIDIJAM_STAGEHORN_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include "dmusici.h"

struct StageHornState {
    __int16 isActive;
    __int16 timeToLive;
    int riseTimer[12];
    int riseDuration[12];
    int queue[12][16];
    __int16 timeDeltas[12][16];
};

static_assert(sizeof(StageHornState) == 0x4E4, "StageHornState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(stageHorn, StageHornState)

extern Ms3dBundle* g_stageHorn_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderStageHorn();
bool UpdateStageHorn(MUSIC_TIME pmtNow);

#endif // MIDIJAM_STAGEHORN_H