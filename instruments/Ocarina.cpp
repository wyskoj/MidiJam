//
// Created by Jacob on Saturday, April 11, 2026.
//


#include "Ocarina.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(ocarina, OcarinaState)

Ms3dBundle* g_ocarina_ms3d = nullptr;
Ms3dBundle* g_ocarinaHandX_ms3d[12] = {};

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern int g_currentTempo_scaleFactor1_15;

// FUNCTION: MIDIJAM 0xXXXXXXXX
void RenderOcarina() {
    GLfloat angle; // [esp+0h] [ebp-2Ch]
    GLfloat y; // [esp+8h] [ebp-24h]
    GLfloat ya; // [esp+8h] [ebp-24h]
    GLfloat z; // [esp+Ch] [ebp-20h]
    __int16 i; // [esp+1Ch] [ebp-10h]
    __int16 v5; // [esp+20h] [ebp-Ch]
    __int16 v6; // [esp+24h] [ebp-8h]
    __int16 j; // [esp+28h] [ebp-4h]

    v5 = 0;
    glPushMatrix();
    glTranslatef(30.5, -5.0, 15.5);
    for (i = 0; i < g_ocarinaCount; ++i) {
        if (g_ocarina[i].isActive == 1) {
            v6 = 0;
            for (j = 0; j < 12; ++j) {
                if (g_ocarina[i].field_4[j] > 0) {
                    glPushMatrix();
                    angle = (double)v6 * 17.5 + 130.0;
                    glRotatef(angle, 0.0, 1.0, 0.0);
                    y = (double)v5 * 15.0 + 20.0;
                    glTranslatef(0.0, y, 18.0);
                    glRotatef(-15.0, 1.0, 0.0, 0.0);
                    if (g_ocarina[i].field_34[j] > 0) {
                        z = (double)g_ocarina[i].field_4[j] / (double)g_ocarina[i].field_34[j] * 3.0;
                        glTranslatef(0.0, 0.0, z);
                    }
                    g_ocarina_ms3d->RenderModel();
                    g_ocarinaHandX_ms3d[j]->RenderModel();
                    glPopMatrix();
                    ++v6;
                }
            }
            if (!v6) {
                glPushMatrix();
                glRotatef(130.0, 0.0, 1.0, 0.0);
                ya = (double)v5 * 15.0 + 20.0;
                glTranslatef(0.0, ya, 18.0);
                glRotatef(-15.0, 1.0, 0.0, 0.0);
                g_ocarina_ms3d->RenderModel();
                g_ocarinaHandX_ms3d[g_ocarina[i].field_4E4]->RenderModel();
                glPopMatrix();
            }
            ++v5;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0xXXXXXXXX
bool UpdateOcarina(MUSIC_TIME pmtNow) {
    __int16 v2; // [esp+4h] [ebp-1Ch]
    __int16 v3; // [esp+8h] [ebp-18h]
    __int16 v4; // [esp+Ch] [ebp-14h]
    __int16 i; // [esp+10h] [ebp-10h]
    __int16 k; // [esp+14h] [ebp-Ch]
    __int16 j; // [esp+18h] [ebp-8h]
    __int16 m; // [esp+18h] [ebp-8h]
    __int16 v9; // [esp+1Ch] [ebp-4h]

    v9 = 0;
    for (i = 0; i < g_ocarinaCount; ++i) {
        v2 = 0;
        v4 = 0;
        v3 = 0;
        for (j = 0; j < 12; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_ocarina[i].field_364[j][k] > 0) {
                    v9 = 1;
                    g_ocarina[i].field_364[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_ocarina[i].field_364[j][k] <= 0) {
                        ++v4;
                        g_ocarina[i].field_364[j][k] = 0;
                        g_ocarina[i].field_4[j] = g_ocarina[i].field_64[j][k];
                        g_ocarina[i].field_34[j] = g_ocarina[i].field_64[j][k];
                        g_ocarina[i].field_64[j][k] = 0;
                    }
                    g_ocarina[i].isActive = 1;
                    g_ocarina[i].timeToLive = 1000;
                    v2 = 1;
                }
            }
            if (g_ocarina[i].field_4[j] > 0) {
                g_ocarina[i].field_4E4 = j;
                g_ocarina[i].field_4[j] -= pmtNow - g_currentGlobalTime;
                if (g_ocarina[i].field_4[j] > 0) {
                    ++v3;
                    g_ocarina[i].isActive = 1;
                    g_ocarina[i].timeToLive = 1000;
                    v2 = 1;
                    v9 = 1;
                }
                else {
                    g_ocarina[i].field_4[j] = 0;
                }
            }
        }
        if (v4 > 0 && v3 > 1) {
            for (m = 0; m < 12; ++m) {
                if (g_ocarina[i].field_4[m] > 0 && g_ocarina[i].field_4[m] < g_currentTempo_scaleFactor1_15)
                    g_ocarina[i].field_4[m] = 0;
            }
        }
        if (!v2 && g_ocarina[i].isActive == 1 && --g_ocarina[i].timeToLive <= 0) {
            g_ocarina[i].timeToLive = 0;
            g_ocarina[i].isActive = 0;
        }
    }
    return v9 == 1;
}
