//
// Created by Jacob on 3/17/2026.
//

#ifndef MIDIJAM_PIANO_H
#define MIDIJAM_PIANO_H

#include <windows.h>
#include <dmusici.h>
#include <cstdint>

class Ms3dBundle;

/**
 * Per-piano dynamic state. Tracks key angles, note durations,
 * queued notes, velocities, and time deltas for up to 88 keys
 * across 16 simultaneous slots.
 *
 * Original struct layout (sizeof = 0x2958):
 *   +0x0000  short            isActive
 *   +0x0002  short            timeToLive
 *   +0x0004  short            field_4
 *   +0x0006  short            materialIndex
 *   +0x0008  float[88]        keyAngles
 *   +0x0168  int[88]          durations
 *   +0x02C8  int[88][16]      queueDurations
 *   +0x18C8  uint8_t[88][16]  velocities
 *   +0x1E48  int16_t[88][16]  timeDeltas
 *   +0x2948  uint8_t[16]      trailer
 */
struct I_DS_Piano
{
    short    isActive;
    short    timeToLive;
    short    field_4;
    short    materialIndex;
    float    keyAngles[88];
    int      durations[88];
    int      queueDurations[88][16];
    uint8_t  velocities[88][16];
    int16_t  timeDeltas[88][16];
    uint8_t  trailer[16];
};

static_assert(sizeof(I_DS_Piano) == 0x2958, "I_DS_Piano layout mismatch");

/**
 * Model bundle pointers for a single piano color/material variant.
 *
 * Original struct layout (sizeof = 0x1C):
 *   +0x00  Ms3dBundle*  pianoCase
 *   +0x04  Ms3dBundle*  pianoKeyBlack
 *   +0x08  Ms3dBundle*  pianoKeyBlackDown
 *   +0x0C  Ms3dBundle*  pianoKeyWhiteFront
 *   +0x10  Ms3dBundle*  pianoKeyWhiteBack
 *   +0x14  Ms3dBundle*  pianoKeyWhiteFrontDown
 *   +0x18  Ms3dBundle*  pianoKeyWhiteBackDown
 */
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

void  I_Piano();
bool  I_Piano_MM(MUSIC_TIME pmtNow);

#endif // MIDIJAM_PIANO_H