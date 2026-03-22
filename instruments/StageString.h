//
// Created by Jacob on 3/22/2026.
//

#ifndef MIDIJAM_STAGESTRING_H
#define MIDIJAM_STAGESTRING_H

#include "dmusici.h"
#include "instrument.h"
#include "model/Ms3dBundle.h"

struct StageStringState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[12];
    int field_34[12];
    int field_64[12][16];
    __int16 field_364[12][16];
};
static_assert(sizeof(StageStringState) == 0x4E4, "StageStringState layout mismatch");

DECL_INST_DS(stageString, StageStringState)

extern Ms3dBundle* g_stageStringHolder_ms3d;
extern Ms3dBundle* g_stageStringBow_ms3d;
extern Ms3dBundle* g_stageString_ms3d;
extern Ms3dBundle* g_stageStringBottomX_ms3d[5];

void RenderStageString();
bool UpdateStageString(MUSIC_TIME pmtNow);

#endif //MIDIJAM_STAGESTRING_H
