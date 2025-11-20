//
// Created by Jacob on 11/18/2025.
//

#ifndef MIDIJAM_PIANO_H
#define MIDIJAM_PIANO_H
#include <cstdint>
#include <windows.h>
#include <GL/gl.h>

#include "dmplugin.h"

class Ms3dBundle;

extern __int16 g_inst_visible_piano;
extern __int16 g_ialloc_piano;
extern __int16 PIANO_KEY_PATTERN[88];
extern GLfloat g_pianokey_scale[14];
extern __int16 g_piano_assignment[300];

struct I_DS_Piano // sizeof=0x2958
{
    __int16 isActive;
    __int16 timeToLive;
    __int16 field_4;
    __int16 materialIndex;
    float keyAngles[88];
    int durations[88];
    int32_t queueDurations[88][16];
    uint8_t velocities[88][16];
    int16_t timeDeltas[88][16];
    uint8_t trailer[16];
};

struct PianoModels {
    Ms3dBundle* pianoCase;                    // "PianoCase.ms3d"
    Ms3dBundle* pianoKeyBlack;               // "PianoKeyBlack.ms3d"
    Ms3dBundle* pianoKeyBlackDown;           // "PianoKeyBlackDown.ms3d"
    Ms3dBundle* pianoKeyWhiteFront;          // "PianoKeyWhiteFront.ms3d"
    Ms3dBundle* pianoKeyWhiteBack;           // "PianoKeyWhiteBack.ms3d"
    Ms3dBundle* pianoKeyWhiteFrontDown;      // "PianoKeyWhiteFrontDown.ms3d"
    Ms3dBundle* pianoKeyWhiteBackDown;       // "PianoKeyWhiteBackDown.ms3d"
};

extern I_DS_Piano* g_ds_piano;

void I_Piano();

bool I_Piano_MM(MUSIC_TIME mtNow);

#endif //MIDIJAM_PIANO_H
