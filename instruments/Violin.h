//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_VIOLIN_H
#define MIDIJAM_VIOLIN_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// ViolinState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
#pragma pack(push, 1)
struct ViolinState {
    __int16 isActive; // 0x0000
    __int16 timeToLive; // 0x0002
    int durations[88]; // 0x0004
    int stringAssignment[88]; // 0x0164  (1–4: which string; 0 = unassigned)
    __int16 frets[4]; // 0x02C4  (1-indexed fret per string; 0 = open)
    int queue[88][16]; // 0x02CC
    __int16 timeDeltas[88][16]; // 0x18CC
    float bowX; // 0x23CC  (range −7.5 to 7.5)
    __int16 bowDirection; // 0x23D0  (0 = down-bow, 1 = up-bow)
    float bowSpeed; // 0x23D2
    float bowZ; // 0x23D6
};
#pragma pack(pop)

static_assert(sizeof(ViolinState) == 0x23DA, "ViolinState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(violin, ViolinState)

extern Ms3dBundle* g_violin_ms3d;
extern Ms3dBundle* g_violinBow_ms3d;    
extern Ms3dBundle* g_violinString_ms3d;
extern Ms3dBundle* g_violinFinger_ms3d;
extern Ms3dBundle* g_violinStringPlayedX_ms3d[5];
extern __int16 VIOLIN_NOTES[18][4];

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderViolin();
bool UpdateViolin(MUSIC_TIME pmtNow);

#endif // MIDIJAM_VIOLIN_H
