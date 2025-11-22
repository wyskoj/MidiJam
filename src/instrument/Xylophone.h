//
// Created by Jacob on 11/22/2025.
//

#ifndef MIDIJAM_XYLOPHONE_H
#define MIDIJAM_XYLOPHONE_H

#include "Instrument.h"

class Ms3dBundle;
DECL_INST(xylophone)

struct I_DS_Xylophone {
    __int16 isActive;
    __int16 timeToLive;
    float field_6[16];
    int field_B6[88];
    int field_2C6[88][16];
    unsigned __int8 field_18C6[88][16];
    __int16 queue[88][16];
};

struct XylophoneModels {
    Ms3dBundle *xylophoneWhiteBar;
    Ms3dBundle *xylophoneWhiteBarDown;
    Ms3dBundle *xylophoneBlackBar;
    Ms3dBundle *xylophoneBlackBarDown;
};

extern I_DS_Xylophone* g_ds_xylophone;
extern __int16 g_xylophone_types[300];

bool __cdecl I_Xylophone_MM(int mtNow);
void I_Xylophone();

#endif //MIDIJAM_XYLOPHONE_H