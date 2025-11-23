//
// Created by Jacob on 11/22/2025.
//

#ifndef MIDIJAM_HARP_H
#define MIDIJAM_HARP_H
#include <windows.h>
#include <GL/gl.h>

#include "dmusici.h"
#include "Instrument.h"

DECL_INST(harp)
extern float HARP_STRING_SCALE[47];
extern __int16 word_45EC60[12];

struct I_DS_Harp {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[47];
    int field_C0[47];
    int field_17C[47][16];
    __int16 field_D3C[47][16];
};

enum HARP_STRING_COLOR {
    WHITE = 0x0,
    RED = 0x1,
    BLUE = 0x2,
};

extern I_DS_Harp *g_ds_harp;
extern HARP_STRING_COLOR g_harpStringColors[48];
extern GLfloat g_harp_string_y[47];
extern GLfloat g_harp_string_z[47];
extern GLfloat g_harp_string_scale[47];

void I_Harp();
bool __cdecl I_Harp_MM(MUSIC_TIME mtNow);

#endif //MIDIJAM_HARP_H
