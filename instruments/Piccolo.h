//
// Created by Jacob on Sunday, April 12, 2026.
//

#ifndef MIDIJAM_PICCOLO_H
#define MIDIJAM_PICCOLO_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// PiccoloState -- per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
#pragma pack(push, 1)
struct PiccoloState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[38];
    int field_9C[38];
    int field_134[38][16];
    __int16 field_AB4[38][16];
    __int16 field_F74;
    float field_F76[38][100];
    float field_4AD6[38][100];
    float field_8636[38][100];
    float field_C196[38][100];
    float field_FCF6[38][100];
    float field_13856[38][100];
    float field_173B6[38][100];
    float field_1AF16[38][100];
    float field_1EA76[38][100];
    float field_225D6[38][100];
    float field_26136[38][100];
    __int16 field_29C96[38];
    __int16 field_29CE2[38];
    float field_29D2E[38];
    float field_29DC6[38];
};
#pragma pack(pop)

static_assert(sizeof(PiccoloState) == 0x29E5E, "PiccoloState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(piccolo, PiccoloState)

extern Ms3dBundle* g_piccolo_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderPiccolo();
bool UpdatePiccolo(MUSIC_TIME pmtNow);

#endif // MIDIJAM_PICCOLO_H

