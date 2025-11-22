//
// Created by Jacob on 11/22/2025.
//

#ifndef MIDIJAM_STAGEHORN_H
#define MIDIJAM_STAGEHORN_H
#include "Instrument.h"

DECL_INST(stageHorn)

struct I_DS_StageHorn {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[12];
    int field_34[12];
    int field_64[12][16];
    __int16 field_364[12][16];
};

extern I_DS_StageHorn *g_ds_stageHorn;

void I_StageHorn();

bool __cdecl I_StageHorn_MM(int mtNow);


#endif //MIDIJAM_STAGEHORN_H
