//
// Created by Jacob on 03/22/2026.
//

#ifndef MIDIJAM_XYLOPHONE_H
#define MIDIJAM_XYLOPHONE_H

#include "instrument.h"
#include "dmusici.h"
#include "model/Ms3dBundle.h"

#pragma pack(push, 1)
struct XylophoneState {
    __int16 field_0;
    __int16 field_2;
    __int16 field_4;
    float field_6[88];
    int field_166[88];
    int field_2C6[88][16];
    unsigned __int8 field_18C6[88][16];
    __int16 field_1E46[88][16];
};
#pragma pack(pop)

struct XylophoneModels {
    Ms3dBundle* xylophoneWhiteBar;
    Ms3dBundle* xylophoneWhiteBarDown;
    Ms3dBundle* xylophoneBlackBar;
    Ms3dBundle* xylophoneBlackBarDown;
};

static_assert(sizeof(XylophoneState) == 0x2946, "XylophoneState layout mismatch");

DECL_INST_DS(xylophone, XylophoneState);

extern Ms3dBundle* g_xylophoneMalletWhite_ms3d;
extern Ms3dBundle* g_malletHitShadow_ms3d;
extern Ms3dBundle* g_xylophoneLegs_ms3d;
extern Ms3dBundle* g_xylophoneCase_ms3d;
extern XylophoneModels* g_xylophoneModels[4];
extern Ms3dBundle* g_xylophoneShadow_ms3d;
extern __int16 g_xylophone_types[300];

void RenderXylophone();
bool UpdateXylophone(MUSIC_TIME pmtNow);

#endif // MIDIJAM_XYLOPHONE_H
