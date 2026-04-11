//
// Created by Jacob on Friday, April 10, 2026.
//


#include "Tuba.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(tuba, TubaState)

Ms3dBundle* g_tubaBody_ms3d = nullptr;
Ms3dBundle* g_tubaHorn_ms3d = nullptr;
Ms3dBundle* g_tubaKey_1_ms3d = nullptr;
Ms3dBundle* g_tubaKey_2_ms3d = nullptr;
Ms3dBundle* g_tubaKey_3_ms3d = nullptr;
Ms3dBundle* g_tubaKey_4_ms3d = nullptr;

char TUBA_KEY_1_INDEX[44] = {
    0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1,
    0, 1, 1, 0, 0
};

char TUBA_KEY_2_INDEX[44] = {
    1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1,
    1, 1, 0, 1, 0
};

char TUBA_KEY_3_INDEX[44] = {
    0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0
};

char TUBA_KEY_4_INDEX[44] = {
    1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
    0, 0, 0, 0, 0
};

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern int g_currentTempo_scaleFactor1_15;

// FUNCTION: MIDIJAM 0x408BB0
void RenderTuba() {
    GLfloat angle; // [esp+0h] [ebp-30h]
    GLfloat anglea; // [esp+0h] [ebp-30h]
    GLfloat angleb; // [esp+0h] [ebp-30h]
    GLfloat y; // [esp+8h] [ebp-28h]
    GLfloat ya; // [esp+8h] [ebp-28h]
    GLfloat yb; // [esp+8h] [ebp-28h]
    __int16 i; // [esp+20h] [ebp-10h]
    __int16 v7; // [esp+24h] [ebp-Ch]
    __int16 v8; // [esp+28h] [ebp-8h]
    __int16 j; // [esp+2Ch] [ebp-4h]

    v8 = 0;
    glPushMatrix();
    glTranslatef(-70.5, -53.0, -110.5);
    for (i = 0; i < g_tubaCount; ++i) {
        if (g_tuba[i].isActive == 1) {
            v7 = 0;
            for (j = 0; j < 44; ++j) {
                if (g_tuba[i].field_4[j] > 0) {
                    glPushMatrix();
                    angle = (double)v7 * 50.0 + 90.0;
                    glRotatef(angle, 0.0, 1.0, 0.0);
                    y = (double)v8 * 38.0 + 50.0;
                    glTranslatef(0.0, y, 9.0);
                    anglea = -10.0 - (double)g_tuba[i].field_4[j] / (double)g_tuba[i].field_B4[j] * 3.0;
                    glRotatef(anglea, 1.0, 0.0, 0.0);
                    g_tubaBody_ms3d->RenderModelShiny();
                    glPushMatrix();
                    ya = (double)g_tuba[i].field_4[j] / (double)g_tuba[i].field_B4[j] * 1.0 + 1.0;
                    glScalef(1.0, ya, 1.0);
                    g_tubaHorn_ms3d->RenderModelShiny();
                    glPopMatrix();
                    if (TUBA_KEY_1_INDEX[j] == 1) {
                        glPushMatrix();
                        glTranslatef(0.0, -0.5, 0.0);
                    }
                    g_tubaKey_1_ms3d->RenderModelShiny();
                    if (TUBA_KEY_1_INDEX[j] == 1)
                        glPopMatrix();
                    if (TUBA_KEY_2_INDEX[j] == 1) {
                        glPushMatrix();
                        glTranslatef(0.0, -0.5, 0.0);
                    }
                    g_tubaKey_2_ms3d->RenderModelShiny();
                    if (TUBA_KEY_2_INDEX[j] == 1)
                        glPopMatrix();
                    if (TUBA_KEY_3_INDEX[j] == 1) {
                        glPushMatrix();
                        glTranslatef(0.0, -0.5, 0.0);
                    }
                    g_tubaKey_3_ms3d->RenderModelShiny();
                    if (TUBA_KEY_3_INDEX[j] == 1)
                        glPopMatrix();
                    if (TUBA_KEY_4_INDEX[j] == 1) {
                        glPushMatrix();
                        glTranslatef(0.0, -0.5, 0.0);
                    }
                    g_tubaKey_4_ms3d->RenderModelShiny();
                    if (TUBA_KEY_4_INDEX[j] == 1)
                        glPopMatrix();
                    glPopMatrix();
                    ++v7;
                }
            }
            if (!v7) {
                glPushMatrix();
                angleb = (double)0 * 50.0 + 90.0;
                glRotatef(angleb, 0.0, 1.0, 0.0);
                yb = (double)v8 * 38.0 + 50.0;
                glTranslatef(0.0, yb, 9.0);
                glRotatef(-10.0, 1.0, 0.0, 0.0);
                g_tubaBody_ms3d->RenderModelShiny();
                glPushMatrix();
                g_tubaHorn_ms3d->RenderModelShiny();
                glPopMatrix();
                if (TUBA_KEY_1_INDEX[g_tuba[i].field_11E4] == 1) {
                    glPushMatrix();
                    glTranslatef(0.0, -0.5, 0.0);
                }
                g_tubaKey_1_ms3d->RenderModelShiny();
                if (TUBA_KEY_1_INDEX[g_tuba[i].field_11E4] == 1)
                    glPopMatrix();
                if (TUBA_KEY_2_INDEX[g_tuba[i].field_11E4] == 1) {
                    glPushMatrix();
                    glTranslatef(0.0, -0.5, 0.0);
                }
                g_tubaKey_2_ms3d->RenderModelShiny();
                if (TUBA_KEY_2_INDEX[g_tuba[i].field_11E4] == 1)
                    glPopMatrix();
                if (TUBA_KEY_3_INDEX[g_tuba[i].field_11E4] == 1) {
                    glPushMatrix();
                    glTranslatef(0.0, -0.5, 0.0);
                }
                g_tubaKey_3_ms3d->RenderModelShiny();
                if (TUBA_KEY_3_INDEX[g_tuba[i].field_11E4] == 1)
                    glPopMatrix();
                if (TUBA_KEY_4_INDEX[g_tuba[i].field_11E4] == 1) {
                    glPushMatrix();
                    glTranslatef(0.0, -0.5, 0.0);
                }
                g_tubaKey_4_ms3d->RenderModelShiny();
                if (TUBA_KEY_4_INDEX[g_tuba[i].field_11E4] == 1)
                    glPopMatrix();
                glPopMatrix();
            }
            ++v8;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x042E5F0
bool UpdateTuba(MUSIC_TIME pmtNow) {
    __int16 v2; // [esp+4h] [ebp-1Ch]
    __int16 v3; // [esp+8h] [ebp-18h]
    __int16 v4; // [esp+Ch] [ebp-14h]
    __int16 i; // [esp+10h] [ebp-10h]
    __int16 k; // [esp+14h] [ebp-Ch]
    __int16 j; // [esp+18h] [ebp-8h]
    __int16 m; // [esp+18h] [ebp-8h]
    __int16 v9; // [esp+1Ch] [ebp-4h]

    v9 = 0;
    for (i = 0; i < g_tubaCount; ++i) {
        v2 = 0;
        v4 = 0;
        v3 = 0;
        for (j = 0; j < 44; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_tuba[i].field_C64[j][k] > 0) {
                    v9 = 1;
                    g_tuba[i].field_C64[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_tuba[i].field_C64[j][k] <= 0) {
                        ++v4;
                        g_tuba[i].field_C64[j][k] = 0;
                        g_tuba[i].field_4[j] = g_tuba[i].field_164[j][k];
                        g_tuba[i].field_B4[j] = g_tuba[i].field_164[j][k];
                        g_tuba[i].field_164[j][k] = 0;
                    }
                    g_tuba[i].isActive = 1;
                    g_tuba[i].timeToLive = 1000;
                    v2 = 1;
                }
            }
            if (g_tuba[i].field_4[j] > 0) {
                g_tuba[i].field_11E4 = j;
                g_tuba[i].field_4[j] -= pmtNow - g_currentGlobalTime;
                if (g_tuba[i].field_4[j] > 0) {
                    ++v3;
                    g_tuba[i].isActive = 1;
                    g_tuba[i].timeToLive = 1000;
                    v2 = 1;
                    v9 = 1;
                }
                else {
                    g_tuba[i].field_4[j] = 0;
                }
            }
        }
        if (v4 > 0 && v3 > 1) {
            for (m = 0; m < 44; ++m) {
                if (g_tuba[i].field_4[m] > 0 && g_tuba[i].field_4[m] < g_currentTempo_scaleFactor1_15)
                    g_tuba[i].field_4[m] = 0;
            }
        }
        if (!v2 && g_tuba[i].isActive == 1 && --g_tuba[i].timeToLive <= 0) {
            g_tuba[i].timeToLive = 0;
            g_tuba[i].isActive = 0;
        }
    }
    return v9 == 1;
}
