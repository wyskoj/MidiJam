//
// Created by Jacob on Saturday, April 11, 2026.
//

#include "Taiko.h"

#include "scene/scene.h"

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(taiko, TaikoState)

Ms3dBundle* g_taiko_ms3d = nullptr;
Ms3dBundle* g_taikoStick_ms3d = nullptr;

extern float MAX_RECOIL;

float flt_46E3BC = -11.475;
float flt_46E3A8 = -27;

void RenderTaiko() {
    double v0; // st6
    double v1; // st6
    double v2; // st6
    double v3; // st7
    GLfloat angle; // [esp+0h] [ebp-54h]
    GLfloat anglea; // [esp+0h] [ebp-54h]
    GLfloat angleb; // [esp+0h] [ebp-54h]
    GLfloat anglec; // [esp+0h] [ebp-54h]
    GLfloat angled; // [esp+0h] [ebp-54h]
    GLfloat y; // [esp+8h] [ebp-4Ch]
    GLfloat ya; // [esp+8h] [ebp-4Ch]
    GLfloat yb; // [esp+8h] [ebp-4Ch]
    GLfloat zb; // [esp+Ch] [ebp-48h]
    float z; // [esp+Ch] [ebp-48h]
    float za; // [esp+Ch] [ebp-48h]
    __int16 queue; // [esp+34h] [ebp-20h]
    __int16 min_value; // [esp+40h] [ebp-14h]
    __int16 i; // [esp+48h] [ebp-Ch]
    __int16 v18; // [esp+4Ch] [ebp-8h]
    __int16 stick; // [esp+50h] [ebp-4h]

    v18 = 0;
    glPushMatrix();
    glTranslatef(0.0, 0.0, -14.0);
    for (i = 0; i < g_taikoCount; ++i) {
        if (g_taiko[i].isActive == 1) {
            glPushMatrix();
            angle = (double)v18 * flt_46E3BC + flt_46E3A8;
            glRotatef(angle, 0.0, 1.0, 0.0);
            v0 = g_taiko[i].recoilOffset;
            zb = STAGE_BASE_Z - 20.0 - (v0 + v0);
            y = 60.0 - (v0 + v0);
            glTranslatef(0.0, y, zb);
            glRotatef(55.0, 1.0, 0.0, 0.0);
            g_taiko_ms3d->RenderModel();
            glPopMatrix();
            for (stick = 0; stick < 12; ++stick) {
                min_value = 10000;
                for (queue = 0; queue < 32; ++queue) {
                    if (g_taiko[i].field_668[stick][queue] > 0 && g_taiko[i].field_668[stick][queue] < min_value)
                        min_value = g_taiko[i].field_668[stick][queue];
                }
                if (min_value <= 0 || (double)min_value >= 300.0) {
                    if (g_taiko[i].field_968[stick] > 0.0) {
                        glPushMatrix();
                        anglec = flt_46E3A8 + 3.5999999 - (double)stick * 0.60000002 + (double)v18 * flt_46E3BC;
                        glRotatef(anglec, 0.0, 1.0, 0.0);
                        v2 = g_taiko[i].recoilOffset;
                        if (stick % 2)
                            za = STAGE_BASE_Z - 3.0 - (v2 + v2);
                        else
                            za = STAGE_BASE_Z - 7.0 - (v2 + v2);
                        v3 = g_taiko[i].recoilOffset;
                        yb = 1.0 - (v3 + v3) + (10.0 - g_taiko[i].field_968[stick]) / 10.0 * 7.0 + 60.0;
                        glTranslatef(0.0, yb, za);
                        angled = (10.0 - g_taiko[i].field_968[stick]) / 10.0 * 65.0;
                        glRotatef(angled, 1.0, 0.0, 0.0);
                        g_taikoStick_ms3d->RenderModel();
                        glPopMatrix();
                    }
                }
                else {
                    glPushMatrix();
                    anglea = flt_46E3A8 + 3.5999999 - (double)stick * 0.60000002 + (double)v18 * flt_46E3BC;
                    glRotatef(anglea, 0.0, 1.0, 0.0);
                    v1 = g_taiko[i].recoilOffset;
                    if (stick % 2)
                        z = STAGE_BASE_Z - 3.0 - (v1 + v1);
                    else
                        z = STAGE_BASE_Z - 7.0 - (v1 + v1);
                    ya = 1.0 - (g_taiko[i].recoilOffset + g_taiko[i].recoilOffset) + (double)min_value / 300.0 * 7.0 +
                        60.0;
                    glTranslatef(0.0, ya, z);
                    angleb = (double)min_value / 300.0 * 65.0;
                    glRotatef(angleb, 1.0, 0.0, 0.0);
                    g_taikoStick_ms3d->RenderModel();
                    glPopMatrix();
                }
            }
            ++v18;
        }
    }
    glPopMatrix();
}

bool UpdateTaiko(MUSIC_TIME pmtNow) {
    int v2; // [esp+8h] [ebp-1Ch]
    float v3; // [esp+Ch] [ebp-18h]
    __int16 v4; // [esp+10h] [ebp-14h]
    __int16 i; // [esp+14h] [ebp-10h]
    __int16 k; // [esp+18h] [ebp-Ch]
    __int16 j; // [esp+1Ch] [ebp-8h]
    __int16 v8; // [esp+20h] [ebp-4h]

    v8 = 0;
    for (i = 0; i < g_taikoCount; ++i) {
        v4 = 0;
        for (j = 0; j < 12; ++j) {
            for (k = 0; k < 32; ++k) {
                if (g_taiko[i].field_668[j][k] > 0) {
                    v8 = 1;
                    g_taiko[i].field_668[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_taiko[i].field_668[j][k] <= 0) {
                        v2 = g_taiko[i].field_998[j][k];
                        v3 = ((double)v2 * 0.75 + 32.0 + (double)v2 * 0.75 + 32.0) / 128.0;
                        g_taiko[i].recoilOffset = v3 + g_taiko[i].recoilOffset;
                        if (g_taiko[i].recoilOffset > (double)MAX_RECOIL)
                            g_taiko[i].recoilOffset = MAX_RECOIL;
                        g_taiko[i].field_668[j][k] = 0;
                        g_taiko[i].field_968[j] = (double)g_taiko[i].field_998[j][k] * 8.0 / 128.0 + 1.75;
                        g_taiko[i].field_8[j] = g_taiko[i].field_68[j][k];
                        g_taiko[i].field_38[j] = g_taiko[i].field_68[j][k];
                        g_taiko[i].field_68[j][k] = 0;
                    }
                    g_taiko[i].isActive = 1;
                    g_taiko[i].timeToLive = 1000;
                    v4 = 1;
                }
            }
            if (g_taiko[i].field_8[j] <= 0) {
                if (g_taiko[i].field_968[j] > 0.0) {
                    v8 = 1;
                    g_taiko[i].field_968[j] = g_taiko[i].field_968[j] - 0.30000001;
                    if (g_taiko[i].field_968[j] > 0.0) {
                        g_taiko[i].isActive = 1;
                        g_taiko[i].timeToLive = 1000;
                        v4 = 1;
                    }
                    else {
                        g_taiko[i].field_968[j] = 0.0;
                    }
                }
            }
            else {
                g_taiko[i].field_8[j] -= pmtNow - g_currentGlobalTime;
                if (g_taiko[i].field_8[j] > 0) {
                    g_taiko[i].isActive = 1;
                    g_taiko[i].timeToLive = 1000;
                    v4 = 1;
                    v8 = 1;
                }
                else {
                    g_taiko[i].field_8[j] = 0;
                }
                g_taiko[i].field_968[j] = g_taiko[i].field_968[j] - 0.15000001;
                if (g_taiko[i].field_968[j] > 0.0) {
                    g_taiko[i].isActive = 1;
                    g_taiko[i].timeToLive = 1000;
                    v4 = 1;
                }
                else {
                    g_taiko[i].field_968[j] = 0.0;
                }
            }
        }
        if (!v4 && g_taiko[i].isActive == 1 && --g_taiko[i].timeToLive <= 0) {
            g_taiko[i].timeToLive = 0;
            g_taiko[i].isActive = 0;
        }
    }
    return v8 == 1;
}
