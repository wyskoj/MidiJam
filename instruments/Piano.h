//
// Created by Jacob on 3/17/2026.
//

#ifndef MIDIJAM_PIANO_H
#define MIDIJAM_PIANO_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

struct PianoState
{
    short isActive;
    short timeToLive;
    short field_4;
    short materialIndex;
    float keyAngles[88];
    int durations[88];
    int queue[88][16];
    unsigned char velocities[88][16];
    short timeDeltas[88][16];
    unsigned char trailer[16];
};

static_assert(sizeof(PianoState) == 0x2958, "PianoState layout mismatch");

struct PianoModels
{
    Ms3dBundle* pianoCase;
    Ms3dBundle* pianoKeyBlack;
    Ms3dBundle* pianoKeyBlackDown;
    Ms3dBundle* pianoKeyWhiteFront;
    Ms3dBundle* pianoKeyWhiteBack;
    Ms3dBundle* pianoKeyWhiteFrontDown;
    Ms3dBundle* pianoKeyWhiteBackDown;
};

static_assert(sizeof(PianoModels) == 0x1C, "PianoModels layout mismatch");

extern Ms3dBundle* g_pianoStand_ms3d;
extern PianoModels g_pianoModels[4];
extern GLfloat g_pianoKeyOffsetX[14];
extern GLfloat g_pianoKeyBackScale[14];

DECL_INST_DS(piano, PianoState)

void RenderPiano();
bool UpdatePiano(MUSIC_TIME pmtNow);

#endif // MIDIJAM_PIANO_H
