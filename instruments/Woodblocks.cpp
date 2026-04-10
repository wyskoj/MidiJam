//
// Created by Jacob on 04/10/2026.
//


#include "Woodblocks.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

#include "scene/scene.h"

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(woodblocks, WoodblocksState)

Ms3dBundle* g_woodBlockSingle_ms3d = nullptr;

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern Ms3dBundle* g_drumSet_Stick_ms3d;
extern float MAX_RECOIL;

// FUNCTION: MIDIJAM 0x407EE0
void RenderWoodblocks() {
    double v0; // st7
    double v1; // st7
    GLfloat angle; // [esp+0h] [ebp-58h]
    GLfloat anglea; // [esp+0h] [ebp-58h]
    GLfloat angleb; // [esp+0h] [ebp-58h]
    GLfloat x; // [esp+4h] [ebp-54h]
    GLfloat y; // [esp+8h] [ebp-50h]
    GLfloat ya; // [esp+8h] [ebp-50h]
    GLfloat yb; // [esp+8h] [ebp-50h]
    GLfloat yc; // [esp+8h] [ebp-50h]
    GLfloat z; // [esp+Ch] [ebp-4Ch]
    __int16 k; // [esp+38h] [ebp-20h]
    __int16 v12; // [esp+44h] [ebp-14h]
    __int16 i; // [esp+4Ch] [ebp-Ch]
    __int16 j; // [esp+50h] [ebp-8h]
    __int16 v15; // [esp+54h] [ebp-4h]

    v15 = 0;
    glPushMatrix();
    glTranslatef(EAST_SPIRAL_X, EAST_SPIRAL_Y, EAST_SPIRAL_Z);
    for (i = 0; i < g_woodblocksCount; ++i) {
        if (g_woodblocks[i].isActive == 1) {
            for (j = 0; j < 12; ++j) {
                glPushMatrix();
                angle = (double)j * 7.5 + 3.75 + (double)v15 * 90.0 + WHISTLES_ROTATION_FACTOR;
                glRotatef(angle, 0.0, 1.0, 0.0);
                y = (double)v15 * 3.0 + -9.0 + (double)j * 0.25 - g_woodblocks[i].recoilOffset[j];
                glTranslatef(0.0, y, 20.0);
                z = (1.0 - (double)j / 12.0) * 0.25 + 0.75;
                ya = (1.0 - (double)j / 12.0) * 0.34999999 + 0.75;
                x = (1.0 - (double)j / 12.0) * 0.34999999 + 0.60000002;
                glScalef(x, ya, z);
                g_woodBlockSingle_ms3d->RenderModelShiny();
                v12 = 10000;
                for (k = 0; k < 16; ++k) {
                    if (g_woodblocks[i].quededStartDelayMs[j][k] > 0 && g_woodblocks[i].quededStartDelayMs[j][k] < v12)
                        v12 = g_woodblocks[i].quededStartDelayMs[j][k];
                }
                if (v12 <= 0 || (double)v12 >= 300.0) {
                    if (g_woodblocks[i].stickSwingTimer[j] > 0.0) {
                        v1 = g_woodblocks[i].recoilOffset[j];
                        yc = v1 + v1 + -1.0 + (10.0 - g_woodblocks[i].stickSwingTimer[j]) / 10.0 * 7.0;
                        glTranslatef(0.0, yc, 13.0);
                        angleb = (10.0 - g_woodblocks[i].stickSwingTimer[j]) / 10.0 * 65.0;
                        glRotatef(angleb, 1.0, 0.0, 0.0);
                        g_drumSet_Stick_ms3d->RenderModel();
                    }
                }
                else {
                    v0 = g_woodblocks[i].recoilOffset[j];
                    yb = v0 + v0 + -1.0 + (double)v12 / 300.0 * 7.0;
                    glTranslatef(0.0, yb, 13.0);
                    anglea = (double)v12 / 300.0 * 65.0;
                    glRotatef(anglea, 1.0, 0.0, 0.0);
                    g_drumSet_Stick_ms3d->RenderModel();
                }
                glPopMatrix();
            }
            ++v15;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x42D8F0
bool UpdateWoodblocks(MUSIC_TIME pmtNow) {
    int v2; // [esp+8h] [ebp-1Ch]
    float v3; // [esp+Ch] [ebp-18h]
    __int16 v4; // [esp+10h] [ebp-14h]
    __int16 i; // [esp+14h] [ebp-10h]
    __int16 k; // [esp+18h] [ebp-Ch]
    __int16 j; // [esp+1Ch] [ebp-8h]
    __int16 v8; // [esp+20h] [ebp-4h]

    v8 = 0;
    for (i = 0; i < g_woodblocksCount; ++i) {
        v4 = 0;
        for (j = 0; j < 12; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_woodblocks[i].quededStartDelayMs[j][k] > 0) {
                    v8 = 1;
                    g_woodblocks[i].quededStartDelayMs[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_woodblocks[i].quededStartDelayMs[j][k] <= 0) {
                        v2 = (unsigned __int8)g_woodblocks[i].queuedVelocity[j][k];
                        v3 = ((double)v2 * 0.75 + 32.0 + (double)v2 * 0.75 + 32.0) / 128.0;
                        g_woodblocks[i].recoilOffset[j] = v3 + g_woodblocks[i].recoilOffset[j];
                        if (g_woodblocks[i].recoilOffset[j] > (double)MAX_RECOIL)
                            g_woodblocks[i].recoilOffset[j] = MAX_RECOIL;
                        g_woodblocks[i].quededStartDelayMs[j][k] = 0;
                        g_woodblocks[i].stickSwingTimer[j] = (double)(unsigned __int8)g_woodblocks[i].queuedVelocity[j][
                                k]
                            * 8.0
                            / 128.0
                            + 1.75;
                        g_woodblocks[i].hitHoldTimerMs[j] = g_woodblocks[i].quededHitDurationMs[j][k];
                        g_woodblocks[i].hitHoldDurationMs[j] = g_woodblocks[i].quededHitDurationMs[j][k];
                        g_woodblocks[i].quededHitDurationMs[j][k] = 0;
                    }
                    g_woodblocks[i].isActive = 1;
                    g_woodblocks[i].timeToLive = 1000;
                    v4 = 1;
                }
            }
            if (g_woodblocks[i].hitHoldTimerMs[j] <= 0) {
                if (g_woodblocks[i].stickSwingTimer[j] > 0.0) {
                    v8 = 1;
                    g_woodblocks[i].stickSwingTimer[j] = g_woodblocks[i].stickSwingTimer[j] - 0.30000001;
                    if (g_woodblocks[i].stickSwingTimer[j] > 0.0) {
                        g_woodblocks[i].isActive = 1;
                        g_woodblocks[i].timeToLive = 1000;
                        v4 = 1;
                    }
                    else {
                        g_woodblocks[i].stickSwingTimer[j] = 0.0;
                    }
                }
            }
            else {
                g_woodblocks[i].hitHoldTimerMs[j] -= pmtNow - g_currentGlobalTime;
                if (g_woodblocks[i].hitHoldTimerMs[j] > 0) {
                    g_woodblocks[i].isActive = 1;
                    g_woodblocks[i].timeToLive = 1000;
                    v4 = 1;
                    v8 = 1;
                }
                else {
                    g_woodblocks[i].hitHoldTimerMs[j] = 0;
                }
                g_woodblocks[i].stickSwingTimer[j] = g_woodblocks[i].stickSwingTimer[j] - 0.15000001;
                if (g_woodblocks[i].stickSwingTimer[j] > 0.0) {
                    g_woodblocks[i].isActive = 1;
                    g_woodblocks[i].timeToLive = 1000;
                    v4 = 1;
                }
                else {
                    g_woodblocks[i].stickSwingTimer[j] = 0.0;
                }
            }
        }
        if (!v4 && g_woodblocks[i].isActive == 1 && --g_woodblocks[i].timeToLive <= 0) {
            g_woodblocks[i].timeToLive = 0;
            g_woodblocks[i].isActive = 0;
        }
    }
    return v8 == 1;
}
