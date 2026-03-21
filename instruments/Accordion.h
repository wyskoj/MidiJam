//
// Created by Jacob on 03/20/2026.
//

#ifndef MIDIJAM_ACCORDION_H
#define MIDIJAM_ACCORDION_H

#include "../model/Ms3dBundle.h"
#include "instrument.h"

#include <cstdint>
#include <dmusici.h>

enum AccordionDirection : __int16 {
    CONTRACTING = 0x0,
    EXPANDING = 0x1,
};

struct AccordionState {
    __int16 isActive;
    __int16 timeToLive;
    int field_4[26];
    int queue[25][16];
    __int16 timeDeltas[25][16];
    float keyAngles[26];
    float squeezeAngle;
    AccordionDirection direction;
    float field_B02;
};

// static_assert(sizeof(AccordionState) == 0xB06, "AccordionState layout mismatch");

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DECL_INST_DS(accordion, AccordionState)

extern Ms3dBundle* g_accordionKeyWhiteBack_ms3d;
extern Ms3dBundle* g_accordionKeyWhiteFront_ms3d;
extern Ms3dBundle* g_accordionKeyWhiteBack_Down_ms3d;
extern Ms3dBundle* g_accordionKeyWhiteFront_Down_ms3d;
extern Ms3dBundle* g_accordionKeyBlack_ms3d;
extern Ms3dBundle* g_accordionKeyBlack_Down_ms3d;
extern Ms3dBundle* g_accordionFold_ms3d;
extern Ms3dBundle* g_accordionLeftHand_ms3d;
extern Ms3dBundle* g_accordionRightHand_ms3d;

// ---------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------
void RenderAccordion();
bool UpdateAccordion(MUSIC_TIME pmtNow);

#endif // MIDIJAM_ACCORDION_H
