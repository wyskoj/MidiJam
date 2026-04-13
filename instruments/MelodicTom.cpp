//
// Created by Jacob on Saturday, April 11, 2026.
//

#include "MelodicTom.h"

#include "scene/scene.h"

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(melodicTom, MelodicTomState)

Ms3dBundle* g_melodicTom_ms3d = nullptr;

extern Ms3dBundle* g_drumSet_Stick_ms3d;
extern float MAX_RECOIL;

// FUNCTION: MIDIJAM 0x40DDF0
void RenderMelodicTom() {
    double v0; // st7
    GLfloat angle; // [esp+0h] [ebp-50h]
    GLfloat anglea; // [esp+0h] [ebp-50h]
    GLfloat angleb; // [esp+0h] [ebp-50h]
    GLfloat anglec; // [esp+0h] [ebp-50h]
    GLfloat angled; // [esp+0h] [ebp-50h]
    GLfloat y; // [esp+8h] [ebp-48h]
    GLfloat ya; // [esp+8h] [ebp-48h]
    GLfloat yb; // [esp+8h] [ebp-48h]
    GLfloat z; // [esp+Ch] [ebp-44h]
    GLfloat za; // [esp+Ch] [ebp-44h]
    GLfloat zb; // [esp+Ch] [ebp-44h]
    __int16 k; // [esp+30h] [ebp-20h]
    __int16 v13; // [esp+3Ch] [ebp-14h]
    __int16 i; // [esp+44h] [ebp-Ch]
    __int16 j; // [esp+48h] [ebp-8h]
    __int16 v16; // [esp+4Ch] [ebp-4h]

    v16 = 0;
    glPushMatrix();
    glTranslatef(0.0, 0.0, -14.0);
    for (i = 0; i < g_melodicTomCount; ++i) {
        if (g_melodicTom[i].isActive == 1) {
            glPushMatrix();
            angle = (double)v16 * MELODIC_PERCUSSION_STAGE_ROTATION_DELTA + MELODIC_PERCUSSION_STAGE_ROTATION_BASE;
            glRotatef(angle, 0.0, 1.0, 0.0);
            z = STAGE_BASE_Z + 24.0;
            glTranslatef(0.0, 30.0, z);
            glRotatef(35.0, 1.0, 0.0, 0.0);
            y = -(g_melodicTom[i].recoilOffset + g_melodicTom[i].recoilOffset);
            glTranslatef(0.0, y, 0.0);
            g_melodicTom_ms3d->RenderModel();
            glPopMatrix();
            for (j = 0; j < 12; ++j) {
                v13 = 10000;
                for (k = 0; k < 32; ++k) {
                    if (g_melodicTom[i].field_668[j][k] > 0 && g_melodicTom[i].field_668[j][k] < v13)
                        v13 = g_melodicTom[i].field_668[j][k];
                }
                if (v13 <= 0 || (double)v13 >= 300.0) {
                    if (g_melodicTom[i].field_968[j] > 0.0) {
                        glPushMatrix();
                        anglec = MELODIC_PERCUSSION_STAGE_ROTATION_BASE
                            + 3.5999999
                            - (double)j * 0.60000002
                            + (double)v16 * MELODIC_PERCUSSION_STAGE_ROTATION_DELTA;
                        glRotatef(anglec, 0.0, 1.0, 0.0);
                        zb = STAGE_BASE_Z + 36.0;
                        v0 = g_melodicTom[i].recoilOffset;
                        yb = 30.5 - (v0 + v0) + (10.0 - g_melodicTom[i].field_968[j]) / 10.0 * 7.0;
                        glTranslatef(0.0, yb, zb);
                        angled = (10.0 - g_melodicTom[i].field_968[j]) / 10.0 * 65.0;
                        glRotatef(angled, 1.0, 0.0, 0.0);
                        g_drumSet_Stick_ms3d->RenderModel();
                        glPopMatrix();
                    }
                }
                else {
                    glPushMatrix();
                    anglea = MELODIC_PERCUSSION_STAGE_ROTATION_BASE
                        + 3.5999999
                        - (double)j * 0.60000002
                        + (double)v16 * MELODIC_PERCUSSION_STAGE_ROTATION_DELTA;
                    glRotatef(anglea, 0.0, 1.0, 0.0);
                    za = STAGE_BASE_Z + 36.0;
                    ya = 30.5 - (g_melodicTom[i].recoilOffset + g_melodicTom[i].recoilOffset) + (double)v13 / 300.0 *
                        7.0;
                    glTranslatef(0.0, ya, za);
                    angleb = (double)v13 / 300.0 * 65.0;
                    glRotatef(angleb, 1.0, 0.0, 0.0);
                    g_drumSet_Stick_ms3d->RenderModel();
                    glPopMatrix();
                }
            }
            ++v16;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x433E20
bool UpdateMelodicTom(MUSIC_TIME pmtNow) {
    int v2; // [esp+8h] [ebp-1Ch]
    float v3; // [esp+Ch] [ebp-18h]
    __int16 v4; // [esp+10h] [ebp-14h]
    __int16 i; // [esp+14h] [ebp-10h]
    __int16 k; // [esp+18h] [ebp-Ch]
    __int16 j; // [esp+1Ch] [ebp-8h]
    __int16 v8; // [esp+20h] [ebp-4h]

    v8 = 0;
    for (i = 0; i < g_melodicTomCount; ++i) {
        v4 = 0;
        for (j = 0; j < 12; ++j) {
            for (k = 0; k < 32; ++k) {
                if (g_melodicTom[i].field_668[j][k] > 0) {
                    v8 = 1;
                    g_melodicTom[i].field_668[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_melodicTom[i].field_668[j][k] <= 0) {
                        v2 = g_melodicTom[i].field_998[j][k];
                        v3 = ((double)v2 * 0.75 + 32.0 + (double)v2 * 0.75 + 32.0) / 128.0;
                        g_melodicTom[i].recoilOffset = v3 + g_melodicTom[i].recoilOffset;
                        if (g_melodicTom[i].recoilOffset > (double)MAX_RECOIL)
                            g_melodicTom[i].recoilOffset = MAX_RECOIL;
                        g_melodicTom[i].field_668[j][k] = 0;
                        g_melodicTom[i].field_968[j] = (double)g_melodicTom[i].field_998[j][k] * 8.0 / 128.0 + 1.75;
                        g_melodicTom[i].field_8[j] = g_melodicTom[i].field_68[j][k];
                        g_melodicTom[i].field_38[j] = g_melodicTom[i].field_68[j][k];
                        g_melodicTom[i].field_68[j][k] = 0;
                    }
                    g_melodicTom[i].isActive = 1;
                    g_melodicTom[i].timeToLive = 1000;
                    v4 = 1;
                }
            }
            if (g_melodicTom[i].field_8[j] <= 0) {
                if (g_melodicTom[i].field_968[j] > 0.0) {
                    v8 = 1;
                    g_melodicTom[i].field_968[j] = g_melodicTom[i].field_968[j] - 0.30000001;
                    if (g_melodicTom[i].field_968[j] > 0.0) {
                        g_melodicTom[i].isActive = 1;
                        g_melodicTom[i].timeToLive = 1000;
                        v4 = 1;
                    }
                    else {
                        g_melodicTom[i].field_968[j] = 0.0;
                    }
                }
            }
            else {
                g_melodicTom[i].field_8[j] -= pmtNow - g_currentGlobalTime;
                if (g_melodicTom[i].field_8[j] > 0) {
                    g_melodicTom[i].isActive = 1;
                    g_melodicTom[i].timeToLive = 1000;
                    v4 = 1;
                    v8 = 1;
                }
                else {
                    g_melodicTom[i].field_8[j] = 0;
                }
                g_melodicTom[i].field_968[j] = g_melodicTom[i].field_968[j] - 0.15000001;
                if (g_melodicTom[i].field_968[j] > 0.0) {
                    g_melodicTom[i].isActive = 1;
                    g_melodicTom[i].timeToLive = 1000;
                    v4 = 1;
                }
                else {
                    g_melodicTom[i].field_968[j] = 0.0;
                }
            }
        }
        if (!v4 && g_melodicTom[i].isActive == 1 && --g_melodicTom[i].timeToLive <= 0) {
            g_melodicTom[i].timeToLive = 0;
            g_melodicTom[i].isActive = 0;
        }
    }
    return v8 == 1;
}
