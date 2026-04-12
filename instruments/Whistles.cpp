//
// Created by Jacob on Saturday, April 11, 2026.
//


#include "Whistles.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

#include "scene/scene.h"

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(whistles, WhistlesState)

// TODO: model pointer definitions
Ms3dBundle* g_whistle_ms3d = nullptr;

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern Ms3dBundle* g_steamCloud_1_ms3d;

// FUNCTION: MIDIJAM 0xXXXXXXXX
void RenderWhistles() {
    double v0; // st7
    GLfloat angle; // [esp+0h] [ebp-50h]
    GLfloat anglea; // [esp+0h] [ebp-50h]
    GLfloat x; // [esp+4h] [ebp-4Ch]
    float y; // [esp+8h] [ebp-48h]
    GLfloat ya; // [esp+8h] [ebp-48h]
    GLfloat yb; // [esp+8h] [ebp-48h]
    GLfloat z; // [esp+Ch] [ebp-44h]
    GLfloat v8; // [esp+3Ch] [ebp-14h]
    __int16 k; // [esp+40h] [ebp-10h]
    __int16 i; // [esp+44h] [ebp-Ch]
    __int16 j; // [esp+48h] [ebp-8h]
    __int16 v12; // [esp+4Ch] [ebp-4h]

    v12 = 0;
    glPushMatrix();
    glTranslatef(EAST_SPIRAL_X, EAST_SPIRAL_Y, EAST_SPIRAL_Z);
    for (i = 0; i < g_whistlesCount; ++i) {
        if (g_whistles[i].isActive == 1) {
            for (j = 0; j < 12; ++j) {
                glPushMatrix();
                angle = (double)j * 7.5 + (double)v12 * 90.0 + WHISTLES_ROTATION_FACTOR;
                glRotatef(angle, 0.0, 1.0, 0.0);
                if (g_whistles[i].field_4[j] <= 0)
                    y = (double)v12 * 3.0 + -6.0 + (double)j * 0.25;
                else
                    y = (1.0 - (double)g_whistles[i].field_4[j] / (double)g_whistles[i].field_34[j]) * -2.0
                        + -4.0
                        + (double)v12 * 3.0
                        + (double)j * 0.25;
                glTranslatef(0.0, y, 12.0);
                ya = (1.0 - (double)j / 12.0) * 0.5 + 1.0;
                glScalef(1.0, ya, 1.0);
                g_whistle_ms3d->RenderModelShiny();
                glPopMatrix();
                for (k = 0; k < 100; ++k) {
                    if (g_whistles[i].field_4E4[j][k] > 0.0) {
                        glPushMatrix();
                        anglea = (double)j * 7.5 + (double)v12 * 90.0 + WHISTLES_ROTATION_FACTOR;
                        glRotatef(anglea, 0.0, 1.0, 0.0);
                        z = g_whistles[i].field_4E4[j][k] + 12.0;
                        yb = (double)v12 * 3.0
                            + (double)j * 0.25
                            + g_whistles[i].field_17A4[j][k]
                            + g_whistles[i].field_4E4[j][k] / 20.0 * (g_whistles[i].field_4E4[j][k] / 20.0)
                            + g_whistles[i].field_7564[j][k];
                        x = g_whistles[i].field_62A4[j][k] + 0.0;
                        glTranslatef(x, yb, z);
                        v0 = 1.1 - g_whistles[i].field_2A64[j][k] / 0.25;
                        v8 = v0 + v0;
                        glScalef(v8, v8, v8);
                        glRotatef(g_whistles[i].field_3D24[j][k], 1.0, 0.0, 0.0);
                        glRotatef(g_whistles[i].field_4FE4[j][k], 0.0, 0.0, 1.0);
                        g_steamCloud_1_ms3d->RenderModel();
                        glPopMatrix();
                    }
                }
            }
            ++v12;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0xXXXXXXXX
bool UpdateWhistles(MUSIC_TIME pmtNow) {
    __int16 v2; // [esp+18h] [ebp-18h]
    __int16 i; // [esp+1Ch] [ebp-14h]
    __int16 k; // [esp+20h] [ebp-10h]
    __int16 m; // [esp+24h] [ebp-Ch]
    __int16 j; // [esp+28h] [ebp-8h]
    __int16 v7; // [esp+2Ch] [ebp-4h]

    v7 = 0;
    for (i = 0; i < g_whistlesCount; ++i) {
        v2 = 0;
        for (j = 0; j < 12; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_whistles[i].field_364[j][k] > 0) {
                    v7 = 1;
                    g_whistles[i].field_364[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_whistles[i].field_364[j][k] <= 0) {
                        g_whistles[i].field_364[j][k] = 0;
                        g_whistles[i].field_4[j] = g_whistles[i].field_64[j][k];
                        g_whistles[i].field_34[j] = g_whistles[i].field_64[j][k];
                        g_whistles[i].field_64[j][k] = 0;
                    }
                    g_whistles[i].isActive = 1;
                    g_whistles[i].timeToLive = 1000;
                    v2 = 1;
                    g_whistles[i].field_D33C[j] = 0;
                }
            }
            if (g_whistles[i].field_4[j] > 0) {
                g_whistles[i].field_4[j] -= pmtNow - g_currentGlobalTime;
                if (g_whistles[i].field_4[j] > 0) {
                    g_whistles[i].isActive = 1;
                    g_whistles[i].timeToLive = 1000;
                    v2 = 1;
                    v7 = 1;
                    if (g_whistles[i].field_D33C[j] <= 0) {
                        g_whistles[i].field_4E4[j][g_whistles[i].field_D324[j]] = 0.1;
                        g_whistles[i].field_2A64[j][g_whistles[i].field_D324[j]] = 0.25;
                        g_whistles[i].field_3D24[j][g_whistles[i].field_D324[j]] = 0.0;
                        g_whistles[i].field_4FE4[j][g_whistles[i].field_D324[j]] = 0.0;
                        g_whistles[i].field_62A4[j][g_whistles[i].field_D324[j]] = 0.0;
                        g_whistles[i].field_7564[j][g_whistles[i].field_D324[j]] = 0.0;
                        g_whistles[i].field_8824[j][g_whistles[i].field_D324[j]] = ((double)(50 * rand() / 0x8000) -
                                25.0)
                            * 0.1;
                        g_whistles[i].field_9AE4[j][g_whistles[i].field_D324[j]] = ((double)(50 * rand() / 0x8000) -
                                25.0)
                            * 0.1;
                        g_whistles[i].field_ADA4[j][g_whistles[i].field_D324[j]] = ((double)(25 * rand() / 0x8000) -
                                12.5)
                            * 0.001;
                        g_whistles[i].field_C064[j][g_whistles[i].field_D324[j]] = ((double)(25 * rand() / 0x8000) -
                                12.5)
                            * 0.001;
                        g_whistles[i].field_17A4[j][g_whistles[i].field_D324[j]] = 1.0
                            - (double)j / 12.0
                            + 1.0
                            - (double)j / 12.0
                            + 0.0;
                        g_whistles[i].field_17A4[j][g_whistles[i].field_D324[j]] = (1.0
                                - (double)g_whistles[i].field_4[j]
                                / (double)g_whistles[i].field_34[j])
                            * -2.0
                            + g_whistles[i].field_17A4[j][g_whistles[i].field_D324[j]];
                        if (++g_whistles[i].field_D324[j] >= 100)
                            g_whistles[i].field_D324[j] = 0;
                        g_whistles[i].field_D33C[j] = 2;
                    }
                    else {
                        --g_whistles[i].field_D33C[j];
                    }
                }
                else {
                    g_whistles[i].field_4[j] = 0;
                }
            }
            for (m = 0; m < 100; ++m) {
                if (g_whistles[i].field_4E4[j][m] <= 0.0) {
                    g_whistles[i].field_4E4[j][m] = 0.0;
                }
                else {
                    g_whistles[i].field_4E4[j][m] = g_whistles[i].field_4E4[j][m] + g_whistles[i].field_2A64[j][m];
                    g_whistles[i].field_2A64[j][m] = g_whistles[i].field_2A64[j][m] * 0.95999998;
                    g_whistles[i].field_3D24[j][m] = g_whistles[i].field_3D24[j][m] + g_whistles[i].field_8824[j][m];
                    g_whistles[i].field_4FE4[j][m] = g_whistles[i].field_4FE4[j][m] + g_whistles[i].field_9AE4[j][m];
                    g_whistles[i].field_62A4[j][m] = g_whistles[i].field_62A4[j][m] + g_whistles[i].field_ADA4[j][m];
                    g_whistles[i].field_7564[j][m] = g_whistles[i].field_7564[j][m] + g_whistles[i].field_C064[j][m];
                    if (g_whistles[i].field_2A64[j][m] < 0.0099999998) {
                        g_whistles[i].field_4E4[j][m] = 0.0;
                        g_whistles[i].field_2A64[j][m] = 0.0;
                    }
                }
            }
        }
        if (!v2 && g_whistles[i].isActive == 1 && --g_whistles[i].timeToLive <= 0) {
            g_whistles[i].timeToLive = 0;
            g_whistles[i].isActive = 0;
        }
    }
    return v7 == 1;
}
