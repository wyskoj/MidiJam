//
// Created by Jacob on 11/21/2025.
//

#ifndef MIDIJAM_BASS_H
#define MIDIJAM_BASS_H
#include "instrument.h"

DECL_INST(bass)

extern GLfloat BASS_FRET_HEIGHTS[24];
extern GLfloat flt_468BF4[24];
extern __int16 BASS_NOTES[24][4];

struct I_DS_Bass // sizeof=0x23CC
{
    __int16 isActive;
    __int16 timeToLive;
    int field_4[88];
    int field_164[88];
    __int16 frets[4];
    int field_2CC[88][16];
    __int16 field_18CC[88][16];
};

extern I_DS_Bass* g_ds_bass;

void I_Bass();
bool __cdecl I_Bass_MM(int mtNow);

#endif //MIDIJAM_BASS_H
