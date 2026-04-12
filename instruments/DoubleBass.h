//
// Created by Jacob on Saturday, April 11, 2026.
//

#ifndef MIDIJAM_DOUBLEBASS_H
#define MIDIJAM_DOUBLEBASS_H


#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <dmusici.h>

#pragma pack(push, 1)
struct DoubleBassState {
	__int16 isActive;
	__int16 timeToLive;
	int durations[88];
	int stringAssignment[88];
	__int16 frets[4];
	int queue[88][16];
	__int16 timeDeltas[88][16];
	float bowX;
	__int16 bowDirection;
	float bowSpeed;
	float bowZ;
};
#pragma pack(pop)

static_assert(sizeof(DoubleBassState) == 0x23DA, "DoubleBassState layout mismatch");

DECL_INST_DS(doubleBass, DoubleBassState)

extern Ms3dBundle* g_doubleBass_ms3d;
extern __int16 DOUBLE_BASS_NOTES[49][4];

void RenderDoubleBass();
bool UpdateDoubleBass(MUSIC_TIME pmtNow);

#endif // MIDIJAM_DOUBLEBASS_H

