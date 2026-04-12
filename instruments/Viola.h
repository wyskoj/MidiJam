//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_VIOLA_H
#define MIDIJAM_VIOLA_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// ViolaState - per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
#pragma pack(push, 1)
struct ViolaState {
    __int16 isActive; // 0x0000
    __int16 timeToLive; // 0x0002
    int durations[88]; // 0x0004
    int stringAssignment[88]; // 0x0164  (1-4: which string; 0 = unassigned)
    __int16 frets[4]; // 0x02C4  (1-indexed fret per string; 0 = open)
    int queue[88][16]; // 0x02CC
    __int16 timeDeltas[88][16]; // 0x18CC
    float bowX; // 0x23CC  (range -7.5 to 7.5)
    __int16 bowDirection; // 0x23D0  (0 = down-bow, 1 = up-bow)
    float bowSpeed; // 0x23D2
    float bowZ; // 0x23D6
};
#pragma pack(pop)

static_assert(sizeof(ViolaState) == 0x23DA, "ViolaState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(viola, ViolaState)

extern Ms3dBundle* g_viola_ms3d;
extern __int16 VIOLA_NOTES[18][4];

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderViola();
bool UpdateViola(MUSIC_TIME pmtNow);

#endif // MIDIJAM_VIOLA_H

