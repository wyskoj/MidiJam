//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_TELEPHONE_H
#define MIDIJAM_TELEPHONE_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

// ---------------------------------------------------------------------------
// TelephoneState — per-instance state
// sizeof = 0xXXXX
// ---------------------------------------------------------------------------
struct TelephoneState {
    __int16 isActive;
    __int16 timeToLive;
    float field_4;
    int field_8[12];
    int field_38[12];
    int field_68[12][16];
    __int16 field_368[12][16];
    float field_4E8[12];
    unsigned __int8 field_518[12][16];
};

static_assert(sizeof(TelephoneState) == 0x5D8, "TelephoneState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(telephone, TelephoneState)

extern Ms3dBundle* g_telephoneHandle_ms3d;
extern Ms3dBundle* g_telephoneKeyX_ms3d[];
extern Ms3dBundle* g_telephoneBase_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderTelephone();
bool UpdateTelephone(MUSIC_TIME pmtNow);

#endif // MIDIJAM_TELEPHONE_H
