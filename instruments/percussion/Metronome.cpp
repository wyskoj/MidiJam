//
// Created by Jacob on 4/12/2026.
//

#include "Metronome.h"

float g_prevClickOscillation;
float g_prevClickSwingDelta;
float g_clickPendulumDirection;
float g_prevBellOscillation;
float g_prevBellSwingDelta;
float g_bellPendulumDirection;

extern int g_percussion_time_queue[88][32];
extern short g_stick_visible[37];
extern float MAX_RECOIL;

// FUNCTION: MIDIJAM 0x405DB0
double CalculatePendulumOscillation(GM_PERCUSSION patch, float recoil) {
    __int16 i; // [esp+8h] [ebp-1Ch]
    float v4; // [esp+14h] [ebp-10h]
    __int16 earliestHitTime; // [esp+18h] [ebp-Ch]
    __int16 activeHitCount; // [esp+1Ch] [ebp-8h]

    v4 = 1.0;
    earliestHitTime = 10000;
    activeHitCount = 0;
    for (i = 0; i < 32; ++i) {
        if (g_percussion_time_queue[patch][i] > 0 && g_percussion_time_queue[patch][i] < earliestHitTime) {
            earliestHitTime = g_percussion_time_queue[patch][i];
            ++activeHitCount;
        }
    }
    if (earliestHitTime <= 0 || (double)earliestHitTime >= 200.0) {
        if (recoil <= 0.0) {
            if (activeHitCount <= 0) {
                return 1.0;
            }
            else if (g_stick_visible[patch] == 1) {
                return 1.0;
            }
        }
        else {
            g_stick_visible[patch] = 1;
            return (float)-((MAX_RECOIL - recoil) / MAX_RECOIL);
        }
    }
    else {
        g_stick_visible[patch] = 1;
        return (float)((double)earliestHitTime / 200.0);
    }
    return v4;
}
