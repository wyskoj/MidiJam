//
// Created by Jacob on 03/21/2026.
//

#include "Harp.h"

DEF_INST(harp, HarpState)

float g_harpStringRestLength[47] = {
    69.010002f, 68.606003f, 68.093002f, 67.471001f, 66.876999f,
    65.849998f, 64.686996f, 63.417000f, 62.335999f, 61.146999f,
    60.012001f, 59.066002f, 58.362999f, 57.551998f, 56.958000f,
    56.417000f, 55.876999f, 55.417000f, 55.039001f, 54.687000f,
    54.282001f, 53.903999f, 53.551998f, 53.174000f, 52.876999f,
    52.632999f, 52.335999f, 52.201000f, 52.066002f, 51.958000f,
    51.931000f, 51.985001f, 52.034000f, 52.094002f, 52.194000f,
    52.313999f, 52.414001f, 52.694000f, 52.973999f, 53.254002f,
    53.613998f, 54.153999f, 54.594002f, 54.894001f, 55.133999f,
    55.113998f, 55.113998f,
};

HarpStringColor g_harpStringColors[47] = {};
float g_harpStringY[47] = {};
float g_harpStringZ[47] = {};
float g_harpStringScale[47] = {};
Ms3dBundle* g_harp_ms3d = nullptr;
Ms3dBundle* g_harpString_ms3d[3] = {};
Ms3dBundle* g_harpStringPlaying_ms3d[3][5] = {};
Ms3dBundle* g_harpShadow_ms3d = nullptr;

extern MUSIC_TIME g_currentGlobalTime;
extern int g_vibratingString_frame;

void RenderHarp() {
    __int16 nVisible = 0;
    for (__int16 h = 0; h < g_harpCount; ++h) {
        if (g_harp[h].isActive == 1) {
            g_harp_ms3d->RenderModel();
            for (__int16 string = 0; string < 47; ++string) {
                glPushMatrix();
                glTranslatef(0.0, g_harpStringY[string], g_harpStringZ[string]);
                glRotatef(4.0, 1.0, 0.0, 0.0);
                glScalef(1.0, g_harpStringScale[string], 1.0);
                if (g_harp[h].stringPlayTimer[string] > 0) {
                    // Render vibrating string
                    g_harpStringPlaying_ms3d[g_harpStringColors[string]][g_vibratingString_frame]->RenderModel();
                }
                else {
                    // Render static string
                    g_harpString_ms3d[g_harpStringColors[string]]->RenderModel();
                }
                glPopMatrix();
            }
            glTranslatef(16.0, 0.0, 0.0);
            ++nVisible;
        }
    }
    g_harpVisible = 0;
    if (nVisible > 0) {
        g_harpVisible = nVisible;
    }
}

bool UpdateHarp(const MUSIC_TIME pmtNow) {
    __int16 anyHarpActive = 0;
    for (__int16 h = 0; h < g_harpCount; ++h) {
        __int16 anyStringActive = 0;
        for (__int16 j = 0; j < 47; ++j) {
            for (__int16 k = 0; k < 16; ++k) {
                if (g_harp[h].timeDeltas[j][k] > 0) {
                    anyHarpActive = 1;
                    g_harp[h].timeDeltas[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_harp[h].timeDeltas[j][k] <= 0) {
                        g_harp[h].timeDeltas[j][k] = 0;
                        g_harp[h].stringPlayTimer[j] = g_harp[h].queue[j][k];
                        g_harp[h].durations[j] = g_harp[h].queue[j][k];
                        g_harp[h].queue[j][k] = 0;
                    }
                    g_harp[h].isActive = 1;
                    g_harp[h].timeToLive = 1000;
                    anyStringActive = 1;
                }
            }
            if (g_harp[h].stringPlayTimer[j] > 0) {
                g_harp[h].stringPlayTimer[j] -= pmtNow - g_currentGlobalTime;
                if (g_harp[h].stringPlayTimer[j] > 0) {
                    g_harp[h].isActive = 1;
                    g_harp[h].timeToLive = 1000;
                    anyStringActive = 1;
                    anyHarpActive = 1;
                }
                else {
                    g_harp[h].stringPlayTimer[j] = 0;
                }
            }
        }
        if (!anyStringActive && g_harp[h].isActive == 1 && --g_harp[h].timeToLive <= 0) {
            g_harp[h].timeToLive = 0;
            g_harp[h].isActive = 0;
        }
    }
    return anyHarpActive == 1;
}
