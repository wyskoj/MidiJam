 //
// Created by Jacob on 04/10/2026.
//

#ifndef MIDIJAM_WOODBLOCKS_H
#define MIDIJAM_WOODBLOCKS_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// WoodblocksState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
struct WoodblocksState {
    __int16 isActive;
    __int16 timeToLive;
    int hitHoldTimerMs[12];
    int hitHoldDurationMs[12];
    int quededHitDurationMs[12][16];
    __int16 quededStartDelayMs[12][16];
    char queuedVelocity[12][16];
    float stickSwingTimer[12];
    float recoilOffset[12];
};

static_assert(sizeof(WoodblocksState) == 0x604, "WoodblocksState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(woodblocks, WoodblocksState)

// TODO: model pointer externs
extern Ms3dBundle* g_woodBlockSingle_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderWoodblocks();
bool UpdateWoodblocks(MUSIC_TIME pmtNow);

#endif // MIDIJAM_WOODBLOCKS_H