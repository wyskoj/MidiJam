//
// Created by Jacob on 04/09/2026.
//

#ifndef MIDIJAM_AGOGOS_H
#define MIDIJAM_AGOGOS_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <cstdint>
#include <dmusici.h>

// ---------------------------------------------------------------------------
// AgogosState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
struct AgogosState // sizeof=0x604
{
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

static_assert(sizeof(AgogosState) == 0x604, "AgogosState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(agogos, AgogosState)

// TODO: model pointer externs
extern Ms3dBundle* g_agogoSingle_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderAgogos();
bool UpdateAgogos(MUSIC_TIME pmtNow);

#endif // MIDIJAM_AGOGOS_H
