//
// Created by Jacob on Sunday, April 12, 2026.
//


#include "Flute.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(flute, FluteState)

Ms3dBundle* g_flute_ms3d = nullptr;
Ms3dBundle* g_fluteLeftHandX_ms3d[13] = {};
Ms3dBundle* g_fluteRightHandX_ms3d[10] = {};

char FLUTE_LEFT_HAND_INDEX[40] = {
    0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x2, 0x6, 0x7, 0x7, 0x9, 0xA, 0x5, 0x5, 0x4, 0x4, 0x4, 0x4, 0x2, 0x6,
    0x7, 0x7, 0x9, 0xA, 0x5, 0x2, 0x6, 0xB, 0xB, 0x0, 0x3, 0xC, 0x8, 0xB, 0x1, 0x0, 0x0
};

char FLUTE_RIGHT_HAND_INDEX[40] = {
    0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x8, 0x8, 0x6, 0x8, 0x8, 0x8, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x8, 0x8,
    0x6, 0x8, 0x8, 0x8, 0x8, 0x4, 0x5, 0x6, 0x7, 0x8, 0x8, 0x6, 0x9, 0xA, 0xB, 0x0, 0x0
};

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern Ms3dBundle* g_steamCloud_1_ms3d;
extern int g_currentTempo_scaleFactor1_15;

// FUNCTION: MIDIJAM 0x40B2F0
void RenderFlute() {
    double v0; // st7
    GLfloat x; // [esp+0h] [ebp-4Ch]
    GLfloat xa; // [esp+0h] [ebp-4Ch]
    GLfloat xb; // [esp+0h] [ebp-4Ch]
    GLfloat y; // [esp+4h] [ebp-48h]
    GLfloat ya; // [esp+4h] [ebp-48h]
    GLfloat yb; // [esp+4h] [ebp-48h]
    GLfloat yc; // [esp+4h] [ebp-48h]
    GLfloat yd; // [esp+4h] [ebp-48h]
    GLfloat ye; // [esp+4h] [ebp-48h]
    GLfloat z; // [esp+34h] [ebp-18h]
    __int16 m; // [esp+38h] [ebp-14h]
    __int16 i; // [esp+3Ch] [ebp-10h]
    __int16 v13; // [esp+40h] [ebp-Ch]
    __int16 j; // [esp+44h] [ebp-8h]
    __int16 k; // [esp+44h] [ebp-8h]
    __int16 v16; // [esp+48h] [ebp-4h]

    v16 = 0;
    glPushMatrix();
    glTranslatef(20.5, -30.0, -44.5);
    glRotatef(-55.0, 0.0, 1.0, 0.0);
    for (i = 0; i < g_fluteCount; ++i) {
        if (g_flute[i].isActive == 1) {
            v13 = 0;
            for (j = 0; j < 38; ++j) {
                if (g_flute[i].field_4[j] > 0) {
                    glPushMatrix();
                    y = (double)v13 * -1.5;
                    x = (double)v13 * 6.5;
                    glTranslatef(x, y, 0.0);
                    g_flute[i].field_29D2E[j] = (double)v13 * 6.5;
                    g_flute[i].field_29DC6[j] = (double)v13 * -1.5;
                    ya = (double)v16 * 5.0 + 50.0;
                    glTranslatef(0.0, ya, 18.0);
                    glRotatef(-80.0, 1.0, 0.0, 0.0);
                    g_flute_ms3d->RenderModelShiny();
                    g_fluteLeftHandX_ms3d[FLUTE_LEFT_HAND_INDEX[j]]->RenderModel();
                    g_fluteRightHandX_ms3d[FLUTE_RIGHT_HAND_INDEX[j]]->RenderModel();
                    glPopMatrix();
                    ++v13;
                }
            }
            if (!v13) {
                glPushMatrix();
                yb = (double)0 * -1.5;
                xa = (double)0 * 6.5;
                glTranslatef(xa, yb, 0.0);
                yc = (double)v16 * 5.0 + 50.0;
                glTranslatef(0.0, yc, 18.0);
                glRotatef(-80.0, 1.0, 0.0, 0.0);
                g_flute_ms3d->RenderModelShiny();
                g_fluteLeftHandX_ms3d[FLUTE_LEFT_HAND_INDEX[g_flute[i].field_F74]]->RenderModel();
                g_fluteRightHandX_ms3d[FLUTE_RIGHT_HAND_INDEX[g_flute[i].field_F74]]->RenderModel();
                glPopMatrix();
            }
            for (k = 0; k < 38; ++k) {
                for (m = 0; m < 100; ++m) {
                    if (g_flute[i].field_F76[k][m] > 0.0) {
                        glPushMatrix();
                        glTranslatef(g_flute[i].field_29D2E[k], g_flute[i].field_29DC6[k], 0.0);
                        yd = (double)v16 * 5.0 + 50.0;
                        glTranslatef(0.0, yd, 18.0);
                        glRotatef(-80.0, 1.0, 0.0, 0.0);
                        ye = -(g_flute[i].field_F76[k][m] / 20.0 * (g_flute[i].field_F76[k][m] / 20.0)
                            + g_flute[i].field_4AD6[k][m]
                            + g_flute[i].field_173B6[k][m]
                            + g_flute[i].field_F76[k][m] * 1.5);
                        xb = g_flute[i].field_13856[k][m] + 0.0;
                        glTranslatef(xb, ye, 0.0);
                        v0 = 1.1 - g_flute[i].field_8636[k][m] / 0.25;
                        z = v0 + v0;
                        glScalef(z, z, z);
                        glRotatef(g_flute[i].field_C196[k][m], 1.0, 0.0, 0.0);
                        glRotatef(g_flute[i].field_FCF6[k][m], 0.0, 0.0, 1.0);
                        g_steamCloud_1_ms3d->RenderModel();
                        glPopMatrix();
                    }
                }
            }
            ++v16;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x431FF0
bool UpdateFlute(MUSIC_TIME pmtNow) {
    __int16 v2; // [esp+14h] [ebp-20h]
    __int16 v3; // [esp+18h] [ebp-1Ch]
    __int16 v4; // [esp+1Ch] [ebp-18h]
    __int16 i; // [esp+20h] [ebp-14h]
    __int16 k; // [esp+24h] [ebp-10h]
    __int16 ii; // [esp+28h] [ebp-Ch]
    __int16 j; // [esp+2Ch] [ebp-8h]
    __int16 m; // [esp+2Ch] [ebp-8h]
    __int16 n; // [esp+2Ch] [ebp-8h]
    __int16 v11; // [esp+30h] [ebp-4h]

    v11 = 0;
    for (i = 0; i < g_fluteCount; ++i) {
        v2 = 0;
        v4 = 0;
        v3 = 0;
        for (j = 0; j < 38; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_flute[i].field_AB4[j][k] > 0) {
                    v11 = 1;
                    g_flute[i].field_AB4[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_flute[i].field_AB4[j][k] <= 0) {
                        ++v4;
                        g_flute[i].field_AB4[j][k] = 0;
                        g_flute[i].field_4[j] = g_flute[i].field_134[j][k];
                        g_flute[i].field_9C[j] = g_flute[i].field_134[j][k];
                        g_flute[i].field_134[j][k] = 0;
                    }
                    g_flute[i].isActive = 1;
                    g_flute[i].timeToLive = 1000;
                    v2 = 1;
                }
            }
            if (g_flute[i].field_4[j] > 0) {
                g_flute[i].field_F74 = j;
                g_flute[i].field_4[j] -= pmtNow - g_currentGlobalTime;
                if (g_flute[i].field_4[j] > 0) {
                    ++v3;
                    g_flute[i].isActive = 1;
                    g_flute[i].timeToLive = 1000;
                    v2 = 1;
                    v11 = 1;
                }
                else {
                    g_flute[i].field_4[j] = 0;
                }
            }
        }
        if (v4 > 0 && v3 > 1) {
            for (m = 0; m < 38; ++m) {
                if (g_flute[i].field_4[m] > 0 && g_flute[i].field_4[m] < g_currentTempo_scaleFactor1_15)
                    g_flute[i].field_4[m] = 0;
            }
        }
        for (n = 0; n < 38; ++n) {
            if (g_flute[i].field_4[n] > 0) {
                if (g_flute[i].field_29CE2[n] <= 0) {
                    g_flute[i].field_F76[n][g_flute[i].field_29C96[n]] = 0.1;
                    g_flute[i].field_8636[n][g_flute[i].field_29C96[n]] = 0.25;
                    g_flute[i].field_C196[n][g_flute[i].field_29C96[n]] = 0.0;
                    g_flute[i].field_FCF6[n][g_flute[i].field_29C96[n]] = 0.0;
                    g_flute[i].field_13856[n][g_flute[i].field_29C96[n]] = 0.0;
                    g_flute[i].field_173B6[n][g_flute[i].field_29C96[n]] = 0.0;
                    g_flute[i].field_1AF16[n][g_flute[i].field_29C96[n]] = ((double)(50 * rand() / 0x8000) - 25.0) *
                        0.1;
                    g_flute[i].field_1EA76[n][g_flute[i].field_29C96[n]] = ((double)(50 * rand() / 0x8000) - 25.0) *
                        0.1;
                    g_flute[i].field_225D6[n][g_flute[i].field_29C96[n]] = ((double)(25 * rand() / 0x8000) - 12.5) *
                        0.001;
                    g_flute[i].field_26136[n][g_flute[i].field_29C96[n]] = ((double)(25 * rand() / 0x8000) - 12.5) *
                        0.001;
                    g_flute[i].field_4AD6[n][g_flute[i].field_29C96[n]++] = 10.5;
                    if (g_flute[i].field_29C96[n] >= 100)
                        g_flute[i].field_29C96[n] = 0;
                    g_flute[i].field_29CE2[n] = 2;
                }
                else {
                    --g_flute[i].field_29CE2[n];
                }
            }
            for (ii = 0; ii < 100; ++ii) {
                if (g_flute[i].field_F76[n][ii] <= 0.0) {
                    g_flute[i].field_F76[n][ii] = 0.0;
                }
                else {
                    g_flute[i].field_F76[n][ii] = g_flute[i].field_F76[n][ii] + g_flute[i].field_8636[n][ii];
                    g_flute[i].field_8636[n][ii] = g_flute[i].field_8636[n][ii] * 0.95999998;
                    g_flute[i].field_C196[n][ii] = g_flute[i].field_C196[n][ii] + g_flute[i].field_1AF16[n][ii];
                    g_flute[i].field_FCF6[n][ii] = g_flute[i].field_FCF6[n][ii] + g_flute[i].field_1EA76[n][ii];
                    g_flute[i].field_13856[n][ii] = g_flute[i].field_13856[n][ii] + g_flute[i].field_225D6[n][ii];
                    g_flute[i].field_173B6[n][ii] = g_flute[i].field_173B6[n][ii] + g_flute[i].field_26136[n][ii];
                    if (g_flute[i].field_8636[n][ii] < 0.0099999998) {
                        g_flute[i].field_F76[n][ii] = 0.0;
                        g_flute[i].field_8636[n][ii] = 0.0;
                    }
                }
            }
        }
        if (!v2 && g_flute[i].isActive == 1 && --g_flute[i].timeToLive <= 0) {
            g_flute[i].timeToLive = 0;
            g_flute[i].isActive = 0;
        }
    }
    return v11 == 1;
}
