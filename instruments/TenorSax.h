//
// Created by Jacob on Friday, April 10, 2026.
//

#ifndef MIDIJAM_TENORSAX_H
#define MIDIJAM_TENORSAX_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

#pragma pack(push, 1)
struct TenorSaxState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[33];
    int field_88[33];
    int field_10C[33][16];
    __int16 field_94C[33][16];
    __int16 field_D6C;
};
#pragma pack(pop)

static_assert(sizeof(TenorSaxState) == 0xD6E, "TenorSaxState layout mismatch");

DECL_INST_DS(tenorSax, TenorSaxState)

extern Ms3dBundle* g_tenorSaxBody_ms3d;
extern Ms3dBundle* g_tenorSaxHorn_ms3d;

void RenderTenorSax();
bool UpdateTenorSax(MUSIC_TIME pmtNow);

#endif // MIDIJAM_TENORSAX_H

