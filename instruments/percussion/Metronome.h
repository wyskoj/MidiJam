//
// Created by Jacob on 4/12/2026.
//

#ifndef MIDIJAM_METRONOME_H
#define MIDIJAM_METRONOME_H
#include "instruments/instrument_ids.h"

double CalculatePendulumOscillation(GM_PERCUSSION patch, float recoil);

extern float g_prevClickOscillation;
extern float g_prevClickSwingDelta;
extern float g_clickPendulumDirection;
extern float g_prevBellOscillation;
extern float g_prevBellSwingDelta;
extern float g_bellPendulumDirection;

#endif //MIDIJAM_METRONOME_H
