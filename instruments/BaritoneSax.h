//
// Created by Jacob on Friday, April 10, 2026.
//

#ifndef MIDIJAM_BARITONESAX_H
#define MIDIJAM_BARITONESAX_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

#pragma pack(push, 1)
struct BaritoneSaxState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[33];
    int field_88[33];
    int field_10C[33][16];
    __int16 field_94C[33][16];
    __int16 field_D6C;
};
#pragma pack(pop)

static_assert(sizeof(BaritoneSaxState) == 0xD6E, "BaritoneSaxState layout mismatch");

DECL_INST_DS(baritoneSax, BaritoneSaxState)

extern Ms3dBundle* g_baritoneSaxBody_ms3d;
extern Ms3dBundle* g_baritoneSaxHorn_ms3d;

void RenderBaritoneSax();
bool UpdateBaritoneSax(MUSIC_TIME pmtNow);

#endif // MIDIJAM_BARITONESAX_H

