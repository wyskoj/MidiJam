//
// Created by Jacob on Saturday, April 11, 2026.
//


#include "SteelDrum.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

#include "scene/scene.h"

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(steelDrum, SteelDrumState)

Ms3dBundle* g_steelDrum_ms3d = nullptr;
Ms3dBundle* g_steelDrumMallet_ms3d = nullptr;

float flt_46E390 = -16.875;
float flt_46E3C8 = -40.5;

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern float MAX_RECOIL;

// FUNCTION: MIDIJAM 0x40D520
void RenderSteelDrum() {
    GLfloat angle; // [esp+0h] [ebp-50h]
    GLfloat anglea; // [esp+0h] [ebp-50h]
    GLfloat angleb; // [esp+0h] [ebp-50h]
    GLfloat anglec; // [esp+0h] [ebp-50h]
    GLfloat angled; // [esp+0h] [ebp-50h]
    GLfloat anglee; // [esp+0h] [ebp-50h]
    GLfloat anglef; // [esp+0h] [ebp-50h]
    GLfloat y; // [esp+8h] [ebp-48h]
    GLfloat ya; // [esp+8h] [ebp-48h]
    GLfloat yb; // [esp+8h] [ebp-48h]
    GLfloat z; // [esp+Ch] [ebp-44h]
    __int16 k; // [esp+30h] [ebp-20h]
    __int16 v12; // [esp+3Ch] [ebp-14h]
    __int16 i; // [esp+44h] [ebp-Ch]
    __int16 v14; // [esp+48h] [ebp-8h]
    __int16 j; // [esp+4Ch] [ebp-4h]

    v14 = 0;
    glPushMatrix();
    glTranslatef(0.0, 0.0, -14.0);
    for (i = 0; i < g_steelDrumCount; ++i) {
        if (g_steelDrum[i].isActive == 1) {
            glPushMatrix();
            angle = (double)v14 * flt_46E390 + flt_46E3C8;
            glRotatef(angle, 0.0, 1.0, 0.0);
            z = STAGE_BASE_Z + 61.0;
            glTranslatef(0.0, 15.0, z);
            glRotatef(25.0, 1.0, 0.0, 0.0);
            y = -g_steelDrum[i].recoilOffset;
            glTranslatef(0.0, y, 0.0);
            g_steelDrum_ms3d->RenderModelShiny();
            for (j = 0; j < 12; ++j) {
                v12 = 10000;
                for (k = 0; k < 32; ++k) {
                    if (g_steelDrum[i].field_668[j][k] > 0 && g_steelDrum[i].field_668[j][k] < v12)
                        v12 = g_steelDrum[i].field_668[j][k];
                }
                if (v12 <= 0 || (double)v12 >= 300.0) {
                    if (g_steelDrum[i].field_968[j] > 0.0) {
                        glPushMatrix();
                        angled = (double)j * 30.0 + 15.0;
                        glRotatef(angled, 0.0, 1.0, 0.0);
                        yb = -4.25 - g_steelDrum[i].recoilOffset + (10.0 - g_steelDrum[i].field_968[j]) / 10.0 * 7.0;
                        glTranslatef(0.0, yb, 0.0);
                        glRotatef(-37.0, 1.0, 0.0, 0.0);
                        glTranslatef(0.0, 0.0, 7.0);
                        glRotatef(-10.0, 1.0, 0.0, 0.0);
                        anglee = (double)j * -30.0 - 15.0;
                        glRotatef(anglee, 0.0, 1.0, 0.0);
                        glTranslatef(0.0, 0.0, 9.0);
                        anglef = (10.0 - g_steelDrum[i].field_968[j]) / 10.0 * 65.0 - 35.0;
                        glRotatef(anglef, 1.0, 0.0, 0.0);
                        g_steelDrumMallet_ms3d->RenderModel();
                        glPopMatrix();
                    }
                }
                else {
                    glPushMatrix();
                    anglea = (double)j * 30.0 + 15.0;
                    glRotatef(anglea, 0.0, 1.0, 0.0);
                    ya = -4.25 - g_steelDrum[i].recoilOffset + (double)v12 / 300.0 * 7.0;
                    glTranslatef(0.0, ya, 0.0);
                    glRotatef(-37.0, 1.0, 0.0, 0.0);
                    glTranslatef(0.0, 0.0, 7.0);
                    glRotatef(-10.0, 1.0, 0.0, 0.0);
                    angleb = (double)j * -30.0 - 15.0;
                    glRotatef(angleb, 0.0, 1.0, 0.0);
                    glTranslatef(0.0, 0.0, 9.0);
                    anglec = (double)v12 / 300.0 * 65.0;
                    glRotatef(anglec, 1.0, 0.0, 0.0);
                    g_steelDrumMallet_ms3d->RenderModel();
                    glPopMatrix();
                }
            }
            glPopMatrix();
            ++v14;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x434480
bool UpdateSteelDrum(MUSIC_TIME pmtNow) {
    int v2; // [esp+8h] [ebp-1Ch]
    float v3; // [esp+Ch] [ebp-18h]
    __int16 v4; // [esp+10h] [ebp-14h]
    __int16 i; // [esp+14h] [ebp-10h]
    __int16 k; // [esp+18h] [ebp-Ch]
    __int16 j; // [esp+1Ch] [ebp-8h]
    __int16 v8; // [esp+20h] [ebp-4h]

    v8 = 0;
    for (i = 0; i < g_steelDrumCount; ++i) {
        v4 = 0;
        for (j = 0; j < 12; ++j) {
            for (k = 0; k < 32; ++k) {
                if (g_steelDrum[i].field_668[j][k] > 0) {
                    v8 = 1;
                    g_steelDrum[i].field_668[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_steelDrum[i].field_668[j][k] <= 0) {
                        v2 = g_steelDrum[i].field_998[j][k];
                        v3 = ((double)v2 * 0.75 + 32.0 + (double)v2 * 0.75 + 32.0) / 128.0;
                        g_steelDrum[i].recoilOffset = v3 * 0.5 + g_steelDrum[i].recoilOffset;
                        if (g_steelDrum[i].recoilOffset > (double)MAX_RECOIL)
                            g_steelDrum[i].recoilOffset = MAX_RECOIL;
                        g_steelDrum[i].field_668[j][k] = 0;
                        g_steelDrum[i].field_968[j] = (double)g_steelDrum[i].field_998[j][k] * 8.0 / 128.0 + 1.75;
                        g_steelDrum[i].field_8[j] = g_steelDrum[i].field_68[j][k];
                        g_steelDrum[i].field_38[j] = g_steelDrum[i].field_68[j][k];
                        g_steelDrum[i].field_68[j][k] = 0;
                    }
                    g_steelDrum[i].isActive = 1;
                    g_steelDrum[i].timeToLive = 1000;
                    v4 = 1;
                }
            }
            if (g_steelDrum[i].field_8[j] <= 0) {
                if (g_steelDrum[i].field_968[j] > 0.0) {
                    v8 = 1;
                    g_steelDrum[i].field_968[j] = g_steelDrum[i].field_968[j] - 0.30000001;
                    if (g_steelDrum[i].field_968[j] > 0.0) {
                        g_steelDrum[i].isActive = 1;
                        g_steelDrum[i].timeToLive = 1000;
                        v4 = 1;
                    }
                    else {
                        g_steelDrum[i].field_968[j] = 0.0;
                    }
                }
            }
            else {
                g_steelDrum[i].field_8[j] -= pmtNow - g_currentGlobalTime;
                if (g_steelDrum[i].field_8[j] > 0) {
                    g_steelDrum[i].isActive = 1;
                    g_steelDrum[i].timeToLive = 1000;
                    v4 = 1;
                    v8 = 1;
                }
                else {
                    g_steelDrum[i].field_8[j] = 0;
                }
                g_steelDrum[i].field_968[j] = g_steelDrum[i].field_968[j] - 0.15000001;
                if (g_steelDrum[i].field_968[j] > 0.0) {
                    g_steelDrum[i].isActive = 1;
                    g_steelDrum[i].timeToLive = 1000;
                    v4 = 1;
                }
                else {
                    g_steelDrum[i].field_968[j] = 0.0;
                }
            }
        }
        if (!v4 && g_steelDrum[i].isActive == 1 && --g_steelDrum[i].timeToLive <= 0) {
            g_steelDrum[i].timeToLive = 0;
            g_steelDrum[i].isActive = 0;
        }
    }
    return v8 == 1;
}
