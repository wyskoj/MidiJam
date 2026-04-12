//
// Created by Jacob on Sunday, April 12, 2026.
//


#include "Recorder.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(recorder, RecorderState)

Ms3dBundle* g_recorder_ms3d = nullptr;
Ms3dBundle* g_recorderLeftHandX_ms3d[13] = {};
Ms3dBundle* g_recorderRightHandX_ms3d[11] = {};

char RECORDER_LEFT_HAND_INDEX[64] = {
    0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x2, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x9, 0x9, 0x9, 0xA, 0xA, 0xA,
    0xA, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x2, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x9, 0x9, 0x9, 0xA, 0xA,
    0xA, 0xA, 0xB, 0xC, 0xC, 0xD, 0xD, 0xA, 0xA, 0xB, 0xD, 0xD, 0xA, 0xA, 0xB, 0x0, 0x0, 0x0
};

char RECORDER_RIGHT_HAND_INDEX[64] = {
    0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x4, 0x8, 0x9, 0x8, 0x8, 0x8, 0x8, 0x3, 0x5, 0xA, 0xB, 0x8, 0x9, 0x8, 0x7,
    0x5, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x4, 0x8, 0x9, 0x8, 0x8, 0x8, 0x8, 0x3, 0x5, 0xA, 0xB, 0x8, 0x9, 0x8,
    0x7, 0x5, 0x5, 0xA, 0xA, 0x7, 0x7, 0x5, 0x5, 0x9, 0x7, 0x7, 0x5, 0x5, 0x9, 0x0, 0x0, 0x0
};

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern Ms3dBundle* g_steamCloud_2_ms3d;
extern int g_currentTempo_scaleFactor1_15;


// FUNCTION: MIDIJAM 0xXXXXXXXX
void RenderRecorder() {
    double v0; // st7
    GLfloat angle; // [esp+0h] [ebp-44h]
    GLfloat anglea; // [esp+0h] [ebp-44h]
    GLfloat x; // [esp+4h] [ebp-40h]
    GLfloat y; // [esp+8h] [ebp-3Ch]
    GLfloat ya; // [esp+8h] [ebp-3Ch]
    GLfloat yb; // [esp+8h] [ebp-3Ch]
    GLfloat yc; // [esp+8h] [ebp-3Ch]
    GLfloat z; // [esp+2Ch] [ebp-18h]
    __int16 m; // [esp+30h] [ebp-14h]
    __int16 i; // [esp+34h] [ebp-10h]
    __int16 v11; // [esp+38h] [ebp-Ch]
    __int16 j; // [esp+3Ch] [ebp-8h]
    __int16 k; // [esp+3Ch] [ebp-8h]
    __int16 v14; // [esp+40h] [ebp-4h]

    v11 = 0;
    glPushMatrix();
    glTranslatef(-10.5, -30.0, -44.5);
    for (i = 0; i < g_recorderCount; ++i) {
        if (g_recorder[i].isActive == 1) {
            v14 = 0;
            for (j = 0; j < 61; ++j) {
                if (g_recorder[i].field_4[j] > 0) {
                    glPushMatrix();
                    angle = (double)v14 * 12.5 + 20.0;
                    glRotatef(angle, 0.0, 1.0, 0.0);
                    g_recorder[i].field_43232[j] = (double)v14 * 12.5 + 20.0;
                    y = (double)v11 * 15.0 + 50.0;
                    glTranslatef(0.0, y, 18.0);
                    glRotatef(-50.0, 1.0, 0.0, 0.0);
                    g_recorder_ms3d->RenderModel();
                    if (RECORDER_LEFT_HAND_INDEX[j] > 0)
                        g_recorderLeftHandX_ms3d[RECORDER_LEFT_HAND_INDEX[j] - 1]->RenderModel();
                    if (RECORDER_RIGHT_HAND_INDEX[j] > 0)
                        g_recorderRightHandX_ms3d[RECORDER_RIGHT_HAND_INDEX[j] - 1]->RenderModel();
                    glPopMatrix();
                    ++v14;
                }
            }
            if (!v14) {
                glPushMatrix();
                anglea = (double)0 * 12.5 + 20.0;
                glRotatef(anglea, 0.0, 1.0, 0.0);
                ya = (double)v11 * 15.0 + 50.0;
                glTranslatef(0.0, ya, 18.0);
                glRotatef(-50.0, 1.0, 0.0, 0.0);
                g_recorder_ms3d->RenderModel();
                if (RECORDER_LEFT_HAND_INDEX[g_recorder[i].field_18CC] > 0)
                    g_recorderLeftHandX_ms3d[RECORDER_LEFT_HAND_INDEX[g_recorder[i].field_18CC] - 1]->RenderModel();
                if (RECORDER_RIGHT_HAND_INDEX[g_recorder[i].field_18CC] > 0)
                    g_recorderRightHandX_ms3d[RECORDER_RIGHT_HAND_INDEX[g_recorder[i].field_18CC] - 1]->RenderModel();
                glPopMatrix();
            }
            for (k = 0; k < 61; ++k) {
                for (m = 0; m < 100; ++m) {
                    if (g_recorder[i].field_18CE[k][m] > 0.0) {
                        glPushMatrix();
                        glRotatef(g_recorder[i].field_43232[k], 0.0, 1.0, 0.0);
                        yb = (double)v11 * 15.0 + 50.0;
                        glTranslatef(0.0, yb, 18.0);
                        glRotatef(-50.0, 1.0, 0.0, 0.0);
                        yc = -(g_recorder[i].field_18CE[k][m] / 20.0 * (g_recorder[i].field_18CE[k][m] / 20.0)
                            + g_recorder[i].field_781E[k][m]
                            + g_recorder[i].field_254AE[k][m]
                            + g_recorder[i].field_18CE[k][m] * 1.5);
                        x = g_recorder[i].field_1F55E[k][m] + 0.0;
                        glTranslatef(x, yc, 0.0);
                        v0 = 1.1 - g_recorder[i].field_D76E[k][m] / 0.25;
                        z = v0 + v0;
                        glScalef(z, z, z);
                        glRotatef(g_recorder[i].field_136BE[k][m], 1.0, 0.0, 0.0);
                        glRotatef(g_recorder[i].field_1960E[k][m], 0.0, 0.0, 1.0);
                        g_steamCloud_2_ms3d->RenderModel();
                        glPopMatrix();
                    }
                }
            }
            ++v11;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0xXXXXXXXX
bool UpdateRecorder(MUSIC_TIME pmtNow) {
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
    for (i = 0; i < g_recorderCount; ++i) {
        v2 = 0;
        v4 = 0;
        v3 = 0;
        for (j = 0; j < 61; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_recorder[i].field_112C[j][k] > 0) {
                    v11 = 1;
                    g_recorder[i].field_112C[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_recorder[i].field_112C[j][k] <= 0) {
                        ++v4;
                        g_recorder[i].field_112C[j][k] = 0;
                        g_recorder[i].field_4[j] = g_recorder[i].field_1EC[j][k];
                        g_recorder[i].field_F8[j] = g_recorder[i].field_1EC[j][k];
                        g_recorder[i].field_1EC[j][k] = 0;
                    }
                    g_recorder[i].isActive = 1;
                    g_recorder[i].timeToLive = 1000;
                    v2 = 1;
                }
            }
            if (g_recorder[i].field_4[j] > 0) {
                g_recorder[i].field_18CC = j;
                g_recorder[i].field_4[j] -= pmtNow - g_currentGlobalTime;
                if (g_recorder[i].field_4[j] > 0) {
                    ++v3;
                    g_recorder[i].isActive = 1;
                    g_recorder[i].timeToLive = 1000;
                    v2 = 1;
                    v11 = 1;
                }
                else {
                    g_recorder[i].field_4[j] = 0;
                }
            }
        }
        if (v4 > 0 && v3 > 1) {
            for (m = 0; m < 61; ++m) {
                if (g_recorder[i].field_4[m] > 0 && g_recorder[i].field_4[m] < g_currentTempo_scaleFactor1_15)
                    g_recorder[i].field_4[m] = 0;
            }
        }
        for (n = 0; n < 61; ++n) {
            if (g_recorder[i].field_4[n] > 0) {
                if (g_recorder[i].field_431B8[n] <= 0) {
                    g_recorder[i].field_18CE[n][g_recorder[i].field_4313E[n]] = 0.1;
                    g_recorder[i].field_D76E[n][g_recorder[i].field_4313E[n]] = 0.25;
                    g_recorder[i].field_136BE[n][g_recorder[i].field_4313E[n]] = 0.0;
                    g_recorder[i].field_1960E[n][g_recorder[i].field_4313E[n]] = 0.0;
                    g_recorder[i].field_1F55E[n][g_recorder[i].field_4313E[n]] = 0.0;
                    g_recorder[i].field_254AE[n][g_recorder[i].field_4313E[n]] = 0.0;
                    g_recorder[i].field_2B3FE[n][g_recorder[i].field_4313E[n]] = ((double)(50 * rand() / 0x8000) -
                            25.0)
                        * 0.1;
                    g_recorder[i].field_3134E[n][g_recorder[i].field_4313E[n]] = ((double)(50 * rand() / 0x8000) -
                            25.0)
                        * 0.1;
                    g_recorder[i].field_3729E[n][g_recorder[i].field_4313E[n]] = ((double)(25 * rand() / 0x8000) -
                            12.5)
                        * 0.001;
                    g_recorder[i].field_3D1EE[n][g_recorder[i].field_4313E[n]] = ((double)(25 * rand() / 0x8000) -
                            12.5)
                        * 0.001;
                    g_recorder[i].field_781E[n][g_recorder[i].field_4313E[n]++] = 10.5;
                    if (g_recorder[i].field_4313E[n] >= 100)
                        g_recorder[i].field_4313E[n] = 0;
                    g_recorder[i].field_431B8[n] = 2;
                }
                else {
                    --g_recorder[i].field_431B8[n];
                }
            }
            for (ii = 0; ii < 100; ++ii) {
                if (g_recorder[i].field_18CE[n][ii] <= 0.0) {
                    g_recorder[i].field_18CE[n][ii] = 0.0;
                }
                else {
                    g_recorder[i].field_18CE[n][ii] = g_recorder[i].field_18CE[n][ii] + g_recorder[i].
                        field_D76E[n][ii];
                    g_recorder[i].field_D76E[n][ii] = g_recorder[i].field_D76E[n][ii] * 0.95999998;
                    g_recorder[i].field_136BE[n][ii] = g_recorder[i].field_136BE[n][ii]
                        + g_recorder[i].field_2B3FE[n][ii];
                    g_recorder[i].field_1960E[n][ii] = g_recorder[i].field_1960E[n][ii]
                        + g_recorder[i].field_3134E[n][ii];
                    g_recorder[i].field_1F55E[n][ii] = g_recorder[i].field_1F55E[n][ii]
                        + g_recorder[i].field_3729E[n][ii];
                    g_recorder[i].field_254AE[n][ii] = g_recorder[i].field_254AE[n][ii]
                        + g_recorder[i].field_3D1EE[n][ii];
                    if (g_recorder[i].field_D76E[n][ii] < 0.0099999998) {
                        g_recorder[i].field_18CE[n][ii] = 0.0;
                        g_recorder[i].field_D76E[n][ii] = 0.0;
                    }
                }
            }
        }
        if (!v2 && g_recorder[i].isActive == 1 && --g_recorder[i].timeToLive <= 0) {
            g_recorder[i].timeToLive = 0;
            g_recorder[i].isActive = 0;
        }
    }
    return v11 == 1;
}
