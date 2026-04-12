//
// Created by Jacob on 4/12/2026.
//

#ifndef MIDIJAM_PERCUSSION_H
#define MIDIJAM_PERCUSSION_H

#include "instruments/instrument_ids.h"
#include "model/Ms3dBundle.h"

bool IsShowPercussionInstrument(GM_PERCUSSION percussionPatch, float recoil);
bool UpdatePercussionStriker(GM_PERCUSSION percussionPatch, float recoilDistance, float a3, Ms3dBundle *stick_model_ms3d, bool isShiny, GLfloat a6);

#endif //MIDIJAM_PERCUSSION_H
