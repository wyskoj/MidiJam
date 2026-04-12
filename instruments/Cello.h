//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_CELLO_H
#define MIDIJAM_CELLO_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// CelloState - per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
#pragma pack(push, 1)
struct CelloState {
    __int16 isActive;
    __int16 timeToLive;
    int durations[88];
    int stringAssignment[88];
    __int16 frets[4];
    int queue[88][16];
    __int16 timeDeltas[88][16];
    float bowX;
    __int16 bowDirection;
    float bowSpeed;
    float bowZ;
};
#pragma pack(pop)

static_assert(sizeof(CelloState) == 0x23DA, "CelloState layout mismatch");

DECL_INST_DS(cello, CelloState)

extern Ms3dBundle* g_cello_ms3d;
extern __int16 CELLO_NOTES[28][4];

void RenderCello();
bool UpdateCello(MUSIC_TIME pmtNow);

#endif // MIDIJAM_CELLO_H

