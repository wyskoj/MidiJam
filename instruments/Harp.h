//
// Created by Jacob on 03/21/2026.
//

#ifndef MIDIJAM_HARP_H
#define MIDIJAM_HARP_H

#include "model/Ms3dBundle.h"
#include "instrument.h"

#include "dmplugin.h"

struct HarpState {
    __int16 isActive;
    __int16 timeToLive;
    int stringPlayTimer[47];
    int durations[47];
    int queue[47][16];
    __int16 timeDeltas[47][16];
};

static_assert(sizeof(HarpState) == 0x131C, "HarpState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(harp, HarpState)
#if _MSC_VER < 1400
enum HarpStringColor
#else
enum HarpStringColor : short
#endif
{
    White = 0,
    Red = 1,
    Blue = 2,
};

extern HarpStringColor g_harpStringColors[47];
extern float g_harpStringRestLength[47];
extern float g_harpStringY[47];
extern float g_harpStringZ[47];
extern float g_harpStringScale[47];
extern Ms3dBundle* g_harp_ms3d;
extern Ms3dBundle* g_harpString_ms3d[3];
extern Ms3dBundle* g_harpStringPlayingX_ms3d[3][5];
extern Ms3dBundle* g_harpShadow_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderHarp();
bool UpdateHarp(MUSIC_TIME pmtNow);

#endif // MIDIJAM_HARP_H
