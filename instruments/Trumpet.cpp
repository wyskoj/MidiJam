//
// Created by Jacob on Friday, April 10, 2026.
//


#include "Trumpet.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(trumpet, TrumpetState)

Ms3dBundle* g_trumpetBody_ms3d = nullptr;
Ms3dBundle* g_trumpetHorn_ms3d = nullptr;
Ms3dBundle* g_trumpetMute_ms3d = nullptr;
Ms3dBundle* g_trumpetKey1_ms3d = nullptr;
Ms3dBundle* g_trumpetKey2_ms3d = nullptr;
Ms3dBundle* g_trumpetKey3_ms3d = nullptr;

char TRUMPET_KEY_1_INDEX[56] = {
    1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0,
    1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0
};

char TRUMPET_KEY_2_INDEX[56] = {
    1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1,
    1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0
};

char TRUMPET_KEY_3_INDEX[56] = {
    1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0
};

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern int g_currentTempo_scaleFactor1_15;

// FUNCTION: MIDIJAM 0x4085F0
void RenderTrumpet() {
    GLfloat angle; // [esp+0h] [ebp-38h]
    GLfloat anglea; // [esp+0h] [ebp-38h]
    GLfloat angleb; // [esp+0h] [ebp-38h]
    GLfloat y; // [esp+8h] [ebp-30h]
    GLfloat ya; // [esp+8h] [ebp-30h]
    GLfloat z; // [esp+Ch] [ebp-2Ch]
    GLfloat za; // [esp+Ch] [ebp-2Ch]
    __int16 i; // [esp+28h] [ebp-10h]
    __int16 v8; // [esp+2Ch] [ebp-Ch]
    __int16 v9; // [esp+30h] [ebp-8h]
    __int16 j; // [esp+34h] [ebp-4h]

    v9 = 0;
    glPushMatrix();
    glTranslatef(-30.5, -22.0, -4.5);
    for (i = 0; i < g_trumpetCount; ++i) {
        if (g_trumpet[i].isActive == 1) {
            v8 = 0;
            for (j = 0; j < 54; ++j) {
                if (g_trumpet[i].field_4[j] > 0) {
                    glPushMatrix();
                    angle = 90.0 - (double)v8 * 12.5;
                    glRotatef(angle, 0.0, 1.0, 0.0);
                    y = 50.0 - (double)v8 * 0.75 + (double)v9 * 5.0;
                    glTranslatef(0.0, y, 9.0);
                    anglea = -10.0 - (double)g_trumpet[i].field_4[j] / (double)g_trumpet[i].field_DC[j] * 3.0;
                    glRotatef(anglea, 1.0, 0.0, 0.0);
                    g_trumpetBody_ms3d->RenderModelShiny();
                    glPushMatrix();
                    glTranslatef(0.0, 0.0, 5.5799999);
                    z = (double)g_trumpet[i].field_4[j] / (double)g_trumpet[i].field_DC[j] * 1.0 + 1.0;
                    glScalef(1.0, 1.0, z);
                    g_trumpetHorn_ms3d->RenderModelShiny();
                    glPopMatrix();
                    if (g_trumpet[i].field_15F6 == 1) {
                        glPushMatrix();
                        glTranslatef(0.0, 0.0, 5.5799999);
                        za = (double)g_trumpet[i].field_4[j] / (double)g_trumpet[i].field_DC[j] * 1.0 + 1.0;
                        glScalef(1.0, 1.0, za);
                        g_trumpetMute_ms3d->RenderModel();
                        glPopMatrix();
                    }
                    if (TRUMPET_KEY_1_INDEX[j] == 1) {
                        glPushMatrix();
                        glTranslatef(0.0, -0.5, 0.0);
                    }
                    g_trumpetKey1_ms3d->RenderModelShiny();
                    if (TRUMPET_KEY_1_INDEX[j] == 1)
                        glPopMatrix();
                    if (TRUMPET_KEY_2_INDEX[j] == 1) {
                        glPushMatrix();
                        glTranslatef(0.0, -0.5, 0.0);
                    }
                    g_trumpetKey2_ms3d->RenderModelShiny();
                    if (TRUMPET_KEY_2_INDEX[j] == 1)
                        glPopMatrix();
                    if (TRUMPET_KEY_3_INDEX[j] == 1) {
                        glPushMatrix();
                        glTranslatef(0.0, -0.5, 0.0);
                    }
                    g_trumpetKey3_ms3d->RenderModelShiny();
                    if (TRUMPET_KEY_3_INDEX[j] == 1)
                        glPopMatrix();
                    glPopMatrix();
                    ++v8;
                }
            }
            if (!v8) {
                glPushMatrix();
                angleb = 90.0 - (double)0 * 12.5;
                glRotatef(angleb, 0.0, 1.0, 0.0);
                ya = 50.0 - (double)0 * 0.75 + (double)v9 * 5.0;
                glTranslatef(0.0, ya, 9.0);
                glRotatef(-10.0, 1.0, 0.0, 0.0);
                g_trumpetBody_ms3d->RenderModelShiny();
                glPushMatrix();
                glTranslatef(0.0, 0.0, 5.5799999);
                g_trumpetHorn_ms3d->RenderModelShiny();
                if (g_trumpet[i].field_15F6 == 1)
                    g_trumpetMute_ms3d->RenderModel();
                glPopMatrix();
                if (TRUMPET_KEY_1_INDEX[g_trumpet[i].field_15F4] == 1) {
                    glPushMatrix();
                    glTranslatef(0.0, -0.5, 0.0);
                }
                g_trumpetKey1_ms3d->RenderModelShiny();
                if (TRUMPET_KEY_1_INDEX[g_trumpet[i].field_15F4] == 1)
                    glPopMatrix();
                if (TRUMPET_KEY_2_INDEX[g_trumpet[i].field_15F4] == 1) {
                    glPushMatrix();
                    glTranslatef(0.0, -0.5, 0.0);
                }
                g_trumpetKey2_ms3d->RenderModelShiny();
                if (TRUMPET_KEY_2_INDEX[g_trumpet[i].field_15F4] == 1)
                    glPopMatrix();
                if (TRUMPET_KEY_3_INDEX[g_trumpet[i].field_15F4] == 1) {
                    glPushMatrix();
                    glTranslatef(0.0, -0.5, 0.0);
                }
                g_trumpetKey3_ms3d->RenderModelShiny();
                if (TRUMPET_KEY_3_INDEX[g_trumpet[i].field_15F4] == 1)
                    glPopMatrix();
                glPopMatrix();
            }
            ++v9;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x432D00
bool UpdateTrumpet(MUSIC_TIME pmtNow) {
    __int16 v2; // [esp+4h] [ebp-1Ch]
    __int16 v3; // [esp+8h] [ebp-18h]
    __int16 v4; // [esp+Ch] [ebp-14h]
    __int16 i; // [esp+10h] [ebp-10h]
    __int16 k; // [esp+14h] [ebp-Ch]
    __int16 j; // [esp+18h] [ebp-8h]
    __int16 m; // [esp+18h] [ebp-8h]
    __int16 v9; // [esp+1Ch] [ebp-4h]

    v9 = 0;
    for (i = 0; i < g_trumpetCount; ++i) {
        v2 = 0;
        v4 = 0;
        v3 = 0;
        for (j = 0; j < 54; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_trumpet[i].field_F34[j][k] > 0) {
                    v9 = 1;
                    g_trumpet[i].field_F34[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_trumpet[i].field_F34[j][k] <= 0) {
                        ++v4;
                        g_trumpet[i].field_F34[j][k] = 0;
                        g_trumpet[i].field_4[j] = g_trumpet[i].field_1B4[j][k];
                        g_trumpet[i].field_DC[j] = g_trumpet[i].field_1B4[j][k];
                        g_trumpet[i].field_1B4[j][k] = 0;
                    }
                    g_trumpet[i].isActive = 1;
                    g_trumpet[i].timeToLive = 1000;
                    v2 = 1;
                }
            }
            if (g_trumpet[i].field_4[j] > 0) {
                g_trumpet[i].field_15F4 = j;
                g_trumpet[i].field_4[j] -= pmtNow - g_currentGlobalTime;
                if (g_trumpet[i].field_4[j] > 0) {
                    ++v3;
                    g_trumpet[i].isActive = 1;
                    g_trumpet[i].timeToLive = 1000;
                    v2 = 1;
                    v9 = 1;
                }
                else {
                    g_trumpet[i].field_4[j] = 0;
                }
            }
        }
        if (v4 > 0 && v3 > 1) {
            for (m = 0; m < 54; ++m) {
                if (g_trumpet[i].field_4[m] > 0 && g_trumpet[i].field_4[m] < g_currentTempo_scaleFactor1_15)
                    g_trumpet[i].field_4[m] = 0;
            }
        }
        if (!v2 && g_trumpet[i].isActive == 1 && --g_trumpet[i].timeToLive <= 0) {
            g_trumpet[i].timeToLive = 0;
            g_trumpet[i].isActive = 0;
        }
    }
    return v9 == 1;
}
