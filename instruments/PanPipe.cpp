//
// Created by Jacob on Saturday, April 11, 2026.
//


#include "PanPipe.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

#include "scene/scene.h"

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(panPipe, PanPipeState)

Ms3dBundle* g_calliope_ms3d = nullptr;
Ms3dBundle* g_panPipe_ms3d = nullptr;
__int16 g_isPanPipeCalliope[300];

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern Ms3dBundle* g_steamCloud_0_ms3d;

// FUNCTION: MIDIJAM 0x406E10
void RenderPanPipe() {
    double v0; // st7
    GLfloat angle; // [esp+0h] [ebp-48h]
    GLfloat anglea; // [esp+0h] [ebp-48h]
    GLfloat x; // [esp+4h] [ebp-44h]
    GLfloat y; // [esp+8h] [ebp-40h]
    GLfloat ya; // [esp+8h] [ebp-40h]
    GLfloat yb; // [esp+8h] [ebp-40h]
    GLfloat z; // [esp+34h] [ebp-14h]
    __int16 k; // [esp+38h] [ebp-10h]
    __int16 i; // [esp+3Ch] [ebp-Ch]
    __int16 v10; // [esp+40h] [ebp-8h]
    __int16 j; // [esp+44h] [ebp-4h]

    v10 = 0;
    glPushMatrix();
    glTranslatef(EAST_SPIRAL_X, EAST_SPIRAL_Y, EAST_SPIRAL_Z);
    for (i = 0; i < g_panPipeCount; ++i) {
        if (g_panPipe[i].isActive == 1) {
            for (j = 0; j < 12; ++j) {
                glPushMatrix();
                angle = (double)j * 7.5 + (double)v10 * 90.0 + WHISTLES_ROTATION_FACTOR;
                glRotatef(angle, 0.0, 1.0, 0.0);
                y = (double)v10 * 3.0 + -6.0 + (double)j * 0.25;
                glTranslatef(0.0, y, 10.0);
                ya = (1.0 - (double)j / 12.0) * 0.5 + 1.0;
                glScalef(1.0, ya, 1.0);
                if (g_isPanPipeCalliope[i] == 1)
                    g_calliope_ms3d->RenderModelShiny();
                else
                    g_panPipe_ms3d->RenderModel();
                glPopMatrix();
                for (k = 0; k < 100; ++k) {
                    if (g_panPipe[i].field_4E4[j][k] > 0.0) {
                        glPushMatrix();
                        anglea = (double)j * 7.5 + (double)v10 * 90.0 + WHISTLES_ROTATION_FACTOR;
                        glRotatef(anglea, 0.0, 1.0, 0.0);
                        yb = (double)v10 * 3.0
                            + (double)j * 0.25
                            + g_panPipe[i].field_17A4[j][k]
                            + g_panPipe[i].field_4E4[j][k] / 20.0 * (g_panPipe[i].field_4E4[j][k] / 20.0)
                            + g_panPipe[i].field_7564[j][k]
                            + g_panPipe[i].field_4E4[j][k] * 1.5;
                        x = g_panPipe[i].field_62A4[j][k] + 0.0;
                        glTranslatef(x, yb, 10.0);
                        v0 = 1.1 - g_panPipe[i].field_2A64[j][k] / 0.25;
                        z = v0 + v0;
                        glScalef(z, z, z);
                        glRotatef(g_panPipe[i].field_3D24[j][k], 1.0, 0.0, 0.0);
                        glRotatef(g_panPipe[i].field_4FE4[j][k], 0.0, 0.0, 1.0);
                        g_steamCloud_0_ms3d->RenderModel();
                        glPopMatrix();
                    }
                }
            }
            ++v10;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x42AE20
bool UpdatePanPipe(MUSIC_TIME pmtNow) {
    __int16 v2; // [esp+18h] [ebp-18h]
    __int16 i; // [esp+1Ch] [ebp-14h]
    __int16 k; // [esp+20h] [ebp-10h]
    __int16 m; // [esp+24h] [ebp-Ch]
    __int16 j; // [esp+28h] [ebp-8h]
    __int16 v7; // [esp+2Ch] [ebp-4h]

    v7 = 0;
    for (i = 0; i < g_panPipeCount; ++i) {
        v2 = 0;
        for (j = 0; j < 12; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_panPipe[i].field_364[j][k] > 0) {
                    v7 = 1;
                    g_panPipe[i].field_364[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_panPipe[i].field_364[j][k] <= 0) {
                        g_panPipe[i].field_364[j][k] = 0;
                        g_panPipe[i].field_4[j] = g_panPipe[i].field_64[j][k];
                        g_panPipe[i].field_34[j] = g_panPipe[i].field_64[j][k];
                        g_panPipe[i].field_64[j][k] = 0;
                    }
                    g_panPipe[i].isActive = 1;
                    g_panPipe[i].timeToLive = 1000;
                    v2 = 1;
                    g_panPipe[i].field_D33C[j] = 0;
                }
            }
            if (g_panPipe[i].field_4[j] > 0) {
                g_panPipe[i].field_4[j] -= pmtNow - g_currentGlobalTime;
                if (g_panPipe[i].field_4[j] > 0) {
                    g_panPipe[i].isActive = 1;
                    g_panPipe[i].timeToLive = 1000;
                    v2 = 1;
                    v7 = 1;
                    if (g_panPipe[i].field_D33C[j] <= 0) {
                        g_panPipe[i].field_4E4[j][g_panPipe[i].field_D324[j]] = 0.1;
                        g_panPipe[i].field_2A64[j][g_panPipe[i].field_D324[j]] = 0.25;
                        g_panPipe[i].field_3D24[j][g_panPipe[i].field_D324[j]] = 0.0;
                        g_panPipe[i].field_4FE4[j][g_panPipe[i].field_D324[j]] = 0.0;
                        g_panPipe[i].field_62A4[j][g_panPipe[i].field_D324[j]] = 0.0;
                        g_panPipe[i].field_7564[j][g_panPipe[i].field_D324[j]] = 0.0;
                        g_panPipe[i].field_8824[j][g_panPipe[i].field_D324[j]] = ((double)(50 * rand() / 0x8000) - 25.0)
                            * 0.1;
                        g_panPipe[i].field_9AE4[j][g_panPipe[i].field_D324[j]] = ((double)(50 * rand() / 0x8000) - 25.0)
                            * 0.1;
                        g_panPipe[i].field_ADA4[j][g_panPipe[i].field_D324[j]] = ((double)(25 * rand() / 0x8000) - 12.5)
                            * 0.001;
                        g_panPipe[i].field_C064[j][g_panPipe[i].field_D324[j]] = ((double)(25 * rand() / 0x8000) - 12.5)
                            * 0.001;
                        g_panPipe[i].field_17A4[j][g_panPipe[i].field_D324[j]++] = (1.0 - (double)j / 12.0) * 3.5 +
                            0.25;
                        if (g_panPipe[i].field_D324[j] >= 100)
                            g_panPipe[i].field_D324[j] = 0;
                        g_panPipe[i].field_D33C[j] = 2;
                    }
                    else {
                        --g_panPipe[i].field_D33C[j];
                    }
                }
                else {
                    g_panPipe[i].field_4[j] = 0;
                }
            }
            for (m = 0; m < 100; ++m) {
                if (g_panPipe[i].field_4E4[j][m] <= 0.0) {
                    g_panPipe[i].field_4E4[j][m] = 0.0;
                }
                else {
                    g_panPipe[i].field_4E4[j][m] = g_panPipe[i].field_4E4[j][m] + g_panPipe[i].field_2A64[j][m];
                    g_panPipe[i].field_2A64[j][m] = g_panPipe[i].field_2A64[j][m] * 0.95999998;
                    g_panPipe[i].field_3D24[j][m] = g_panPipe[i].field_3D24[j][m] + g_panPipe[i].field_8824[j][m];
                    g_panPipe[i].field_4FE4[j][m] = g_panPipe[i].field_4FE4[j][m] + g_panPipe[i].field_9AE4[j][m];
                    g_panPipe[i].field_62A4[j][m] = g_panPipe[i].field_62A4[j][m] + g_panPipe[i].field_ADA4[j][m];
                    g_panPipe[i].field_7564[j][m] = g_panPipe[i].field_7564[j][m] + g_panPipe[i].field_C064[j][m];
                    if (g_panPipe[i].field_2A64[j][m] < 0.0099999998) {
                        g_panPipe[i].field_4E4[j][m] = 0.0;
                        g_panPipe[i].field_2A64[j][m] = 0.0;
                    }
                }
            }
        }
        if (!v2 && g_panPipe[i].isActive == 1 && --g_panPipe[i].timeToLive <= 0) {
            g_panPipe[i].timeToLive = 0;
            g_panPipe[i].isActive = 0;
        }
    }
    return v7 == 1;
}
