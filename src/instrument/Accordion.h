//
// Created by Jacob on 11/20/2025.
//

#ifndef MIDIJAM_ACCORDION_H
#define MIDIJAM_ACCORDION_H
#include "dmplugin.h"

enum ACCORDION_DIRECTION : __int16 {
    CONTRACTING = 0x0,
    EXPANDING = 0x1,
};

struct I_DS_Accordion {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[26];
    int queueDurations[25][16];
    __int16 timeDeltas[25][16];
    float keyAngles[26];
    float squeezeAngle;
    ACCORDION_DIRECTION direction;
    float field_B02;
};

extern I_DS_Accordion *g_ds_accordion;
extern __int16 g_inst_visible_accordion;
extern __int16 g_ialloc_accordion;
extern __int16 g_accordion_assignment[300];

void I_Accordion();
bool __cdecl I_Accordion_MM(MUSIC_TIME mtNow);

#endif //MIDIJAM_ACCORDION_H
