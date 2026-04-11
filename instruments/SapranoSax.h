//
// Created by Jacob on Friday, April 10, 2026.
//

#ifndef MIDIJAM_SAPRANOSAX_H
#define MIDIJAM_SAPRANOSAX_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

#pragma pack(push, 1)
struct SapranoSaxState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[33];
    int field_88[33];
    int field_10C[33][16];
    __int16 field_94C[33][16];
    __int16 field_D6C;
};
#pragma pack(pop)

static_assert(sizeof(SapranoSaxState) == 0xD6E, "SapranoSaxState layout mismatch");

DECL_INST_DS(sapranoSax, SapranoSaxState)

extern Ms3dBundle* g_sapranoSaxBody_ms3d;
extern Ms3dBundle* g_sapranoSaxHorn_ms3d;

void RenderSapranoSax();
bool UpdateSapranoSax(MUSIC_TIME pmtNow);

#endif // MIDIJAM_SAPRANOSAX_H

