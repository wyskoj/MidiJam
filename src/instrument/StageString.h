//
// Created by Jacob on 11/22/2025.
//

#ifndef MIDIJAM_STAGESTRING_H
#define MIDIJAM_STAGESTRING_H
#include "Instrument.h"

DECL_INST(stageString)

struct I_DS_StageString {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[12];
    int field_34[12];
    int field_64[12][16];
    __int16 field_364[12][16];
};

extern I_DS_StageString *g_ds_stageString;
void I_StageString();
bool __cdecl I_StageString_MM(int mtNow);

#endif //MIDIJAM_STAGESTRING_H
