//
// Created by Jacob on Sunday, April 12, 2026.
//

#ifndef MIDIJAM_RECORDER_H
#define MIDIJAM_RECORDER_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// RecorderState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
#pragma pack(push, 1)
struct RecorderState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[61];
    int field_F8[61];
    int field_1EC[61][16];
    __int16 field_112C[61][16];
    __int16 field_18CC;
    float field_18CE[61][100];
    float field_781E[61][100];
    float field_D76E[61][100];
    float field_136BE[61][100];
    float field_1960E[61][100];
    float field_1F55E[61][100];
    float field_254AE[61][100];
    float field_2B3FE[61][100];
    float field_3134E[61][100];
    float field_3729E[61][100];
    float field_3D1EE[61][100];
    __int16 field_4313E[61];
    __int16 field_431B8[61];
    float field_43232[61];
};
#pragma pack(pop)

static_assert(sizeof(RecorderState) == 0x43326, "RecorderState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(recorder, RecorderState)

extern Ms3dBundle* g_recorder_ms3d;
extern Ms3dBundle* g_recorderLeftHandX_ms3d[13];
extern Ms3dBundle* g_recorderRightHandX_ms3d[11];

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderRecorder();
bool UpdateRecorder(MUSIC_TIME pmtNow);

#endif // MIDIJAM_RECORDER_H
