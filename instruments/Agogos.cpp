//
// Created by Jacob on 4/9/2026.
//

#include "Agogos.h"

#include "scene/scene.h"

DEF_INST(agogos, AgogosState)

Ms3dBundle* g_agogoSingle_ms3d;

extern Ms3dBundle* g_drumSet_Stick_ms3d;
extern float MAX_RECOIL;

void RenderAgogos() {
    double v0; // st7
    double v1; // st7
    GLfloat angle; // [esp+0h] [ebp-58h]
    GLfloat anglea; // [esp+0h] [ebp-58h]
    GLfloat angleb; // [esp+0h] [ebp-58h]
    GLfloat y; // [esp+8h] [ebp-50h]
    GLfloat ya; // [esp+8h] [ebp-50h]
    GLfloat yb; // [esp+8h] [ebp-50h]
    GLfloat yc; // [esp+8h] [ebp-50h]
    GLfloat z; // [esp+Ch] [ebp-4Ch]
    __int16 q; // [esp+38h] [ebp-20h]
    __int16 v11; // [esp+44h] [ebp-14h]
    __int16 a; // [esp+4Ch] [ebp-Ch]
    __int16 nActive; // [esp+50h] [ebp-8h]
    __int16 i; // [esp+54h] [ebp-4h]

    nActive = 0;
    glPushMatrix();
    glTranslatef(EAST_SPIRAL_X, EAST_SPIRAL_Y, EAST_SPIRAL_Z);
    for (a = 0; a < g_agogosCount; ++a) {
        if (g_agogos[a].isActive == 1) {
            for (i = 0; i < 12; ++i) {
                glPushMatrix();
                angle = (double)i * 7.5 + (double)nActive * 90.0 + WHISTLES_ROTATION_FACTOR;
                glRotatef(angle, 0.0, 1.0, 0.0);
                y = (double)nActive * 3.0 + -6.0 + (double)i * 0.25 - g_agogos[a].recoilOffset[i];
                glTranslatef(0.0, y, 16.0);
                z = (1.0 - (double)i / 12.0) * 0.25 + 0.75;
                ya = (1.0 - (double)i / 12.0) * 0.34999999 + 0.75;
                glScalef(ya, ya, z);
                g_agogoSingle_ms3d->RenderModelShiny();
                v11 = 10000;
                for (q = 0; q < 16; ++q) {
                    if (g_agogos[a].quededStartDelayMs[i][q] > 0 && g_agogos[a].quededStartDelayMs[i][q] < v11)
                        v11 = g_agogos[a].quededStartDelayMs[i][q];
                }
                if (v11 <= 0 || (double)v11 >= 300.0) {
                    if (g_agogos[a].stickSwingTimer[i] > 0.0) {
                        v1 = g_agogos[a].recoilOffset[i];
                        yc = v1 + v1 + -1.0 + (10.0 - g_agogos[a].stickSwingTimer[i]) / 10.0 * 7.0;
                        glTranslatef(0.0, yc, 13.0);
                        angleb = (10.0 - g_agogos[a].stickSwingTimer[i]) / 10.0 * 65.0;
                        glRotatef(angleb, 1.0, 0.0, 0.0);
                        g_drumSet_Stick_ms3d->RenderModel();
                    }
                }
                else {
                    v0 = g_agogos[a].recoilOffset[i];
                    yb = v0 + v0 + -1.0 + (double)v11 / 300.0 * 7.0;
                    glTranslatef(0.0, yb, 13.0);
                    anglea = (double)v11 / 300.0 * 65.0;
                    glRotatef(anglea, 1.0, 0.0, 0.0);
                    g_drumSet_Stick_ms3d->RenderModel();
                }
                glPopMatrix();
            }
            ++nActive;
        }
    }
    glPopMatrix();
}

bool UpdateAgogos(MUSIC_TIME pmtNow) {
    int v2; // [esp+8h] [ebp-1Ch]
    float v3; // [esp+Ch] [ebp-18h]
    __int16 v4; // [esp+10h] [ebp-14h]
    __int16 a; // [esp+14h] [ebp-10h]
    __int16 j; // [esp+18h] [ebp-Ch]
    __int16 i; // [esp+1Ch] [ebp-8h]
    __int16 v8; // [esp+20h] [ebp-4h]

    v8 = 0;
    for (a = 0; a < g_agogosCount; ++a) {
        v4 = 0;
        for (i = 0; i < 12; ++i) {
            for (j = 0; j < 16; ++j) {
                if (g_agogos[a].quededStartDelayMs[i][j] > 0) {
                    v8 = 1;
                    g_agogos[a].quededStartDelayMs[i][j] -= pmtNow - g_currentGlobalTime;
                    if (g_agogos[a].quededStartDelayMs[i][j] <= 0) {
                        v2 = (unsigned __int8)g_agogos[a].queuedVelocity[i][j];
                        v3 = ((double)v2 * 0.75 + 32.0 + (double)v2 * 0.75 + 32.0) / 128.0;
                        g_agogos[a].recoilOffset[i] = v3 + g_agogos[a].recoilOffset[i];
                        if (g_agogos[a].recoilOffset[i] > (double)MAX_RECOIL)
                            g_agogos[a].recoilOffset[i] = MAX_RECOIL;
                        g_agogos[a].quededStartDelayMs[i][j] = 0;
                        g_agogos[a].stickSwingTimer[i] = (double)(unsigned __int8)g_agogos[a].queuedVelocity[i][j] * 8.0 / 128.0 +
                            1.75;
                        g_agogos[a].hitHoldTimerMs[i] = g_agogos[a].quededHitDurationMs[i][j];
                        g_agogos[a].hitHoldDurationMs[i] = g_agogos[a].quededHitDurationMs[i][j];
                        g_agogos[a].quededHitDurationMs[i][j] = 0;
                    }
                    g_agogos[a].isActive = 1;
                    g_agogos[a].timeToLive = 1000;
                    v4 = 1;
                }
            }
            if (g_agogos[a].hitHoldTimerMs[i] <= 0) {
                if (g_agogos[a].stickSwingTimer[i] > 0.0) {
                    v8 = 1;
                    g_agogos[a].stickSwingTimer[i] = g_agogos[a].stickSwingTimer[i] - 0.30000001;
                    if (g_agogos[a].stickSwingTimer[i] > 0.0) {
                        g_agogos[a].isActive = 1;
                        g_agogos[a].timeToLive = 1000;
                        v4 = 1;
                    }
                    else {
                        g_agogos[a].stickSwingTimer[i] = 0.0;
                    }
                }
            }
            else {
                g_agogos[a].hitHoldTimerMs[i] -= pmtNow - g_currentGlobalTime;
                if (g_agogos[a].hitHoldTimerMs[i] > 0) {
                    g_agogos[a].isActive = 1;
                    g_agogos[a].timeToLive = 1000;
                    v4 = 1;
                    v8 = 1;
                }
                else {
                    g_agogos[a].hitHoldTimerMs[i] = 0;
                }
                g_agogos[a].stickSwingTimer[i] = g_agogos[a].stickSwingTimer[i] - 0.15000001;
                if (g_agogos[a].stickSwingTimer[i] > 0.0) {
                    g_agogos[a].isActive = 1;
                    g_agogos[a].timeToLive = 1000;
                    v4 = 1;
                }
                else {
                    g_agogos[a].stickSwingTimer[i] = 0.0;
                }
            }
        }
        if (!v4 && g_agogos[a].isActive == 1 && --g_agogos[a].timeToLive <= 0) {
            g_agogos[a].timeToLive = 0;
            g_agogos[a].isActive = 0;
        }
    }
    return v8 == 1;
}
