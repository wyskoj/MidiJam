//
// Created by Jacob on 03/22/2026.
//


#include "Trombone.h"
#include "scene/scene.h"

DEF_INST(trombone, TromboneState)

float flt_46E3A0 = 32.0625;
float flt_46E3B8 = 4.05;

Ms3dBundle* g_trombone_ms3d;
Ms3dBundle* g_tromboneSlide_ms3d;

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern int g_currentTempo_scaleFactor1_15;

// FUNCTION: MIDIJAM 0x4082F0
void RenderTrombone() {
    __int16 v8 = 0;
    glPushMatrix();
    glTranslatef(0.0, 8.0, -14.0);
    for (__int16 t = 0; t < g_tromboneCount; ++t) {
        if (g_trombone[t].isActive == 1) {
            __int16 v9 = 0;
            for (__int16 j = 0; j < 12; ++j) {
                if (g_trombone[t].field_4[j] > 0) {
                    glPushMatrix();
                    const GLfloat angle = flt_46E3A0 - static_cast<double>(v9) * flt_46E3B8;
                    glRotatef(angle, 0.0, 1.0, 0.0);
                    const double v0 = static_cast<double>(g_trombone[t].field_4[j]) / static_cast<double>(g_trombone[t].field_34[j]);
                    const GLfloat z = v0 + v0 + STAGE_BASE_Z;
                    const GLfloat y = static_cast<double>(v9) * 1.25 + 30.0 + static_cast<double>(v8) * 10.0;
                    glTranslatef(0.0, y, z);
                    const GLfloat anglea = -10.0 - static_cast<double>(g_trombone[t].field_4[j]) / static_cast<double>(g_trombone[t].field_34[j])
                        * 3.0;
                    glRotatef(anglea, 1.0, 0.0, 0.0);
                    g_trombone_ms3d->RenderModelShiny();
                    *(float*)&g_trombone[t].field_34[300] = static_cast<double>(j) / 12.0 * 23.0;
                    glTranslatef(0.0, 0.0, *(GLfloat*)&g_trombone[t].field_34[300]);
                    g_tromboneSlide_ms3d->RenderModelShiny();
                    glPopMatrix();
                    ++v9;
                }
            }
            if (!v9) {
                glPushMatrix();
                const GLfloat angleb = flt_46E3A0 - static_cast<double>(0) * flt_46E3B8;
                glRotatef(angleb, 0.0, 1.0, 0.0);
                const GLfloat ya = static_cast<double>(0) * 1.25 + 30.0 + static_cast<double>(v8) * 10.0;
                glTranslatef(0.0, ya, STAGE_BASE_Z);
                glRotatef(-10.0, 1.0, 0.0, 0.0);
                g_trombone_ms3d->RenderModelShiny();
                glTranslatef(0.0, 0.0, *reinterpret_cast<GLfloat*>(&g_trombone[t].field_34[300]));
                g_tromboneSlide_ms3d->RenderModelShiny();
                glPopMatrix();
            }
            ++v8;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x42DF70
bool UpdateTrombone(MUSIC_TIME pmtNow) {
    float v2; // [esp+10h] [ebp-2Ch]
    __int16 v3; // [esp+14h] [ebp-28h]
    __int16 v4; // [esp+18h] [ebp-24h]
    __int16 v5; // [esp+1Ch] [ebp-20h]
    __int16 v6; // [esp+20h] [ebp-1Ch]
    __int16 v7; // [esp+24h] [ebp-18h]
    __int16 v8; // [esp+28h] [ebp-14h]
    __int16 i; // [esp+2Ch] [ebp-10h]
    __int16 k; // [esp+30h] [ebp-Ch]
    __int16 j; // [esp+34h] [ebp-8h]
    __int16 m; // [esp+34h] [ebp-8h]
    __int16 v13; // [esp+38h] [ebp-4h]

    v13 = 0;
    for (i = 0; i < g_tromboneCount; ++i) {
        v3 = 0;
        v4 = 0;
        v5 = 30000;
        v8 = 0;
        v7 = 0;
        v6 = 0;
        for (j = 0; j < 12; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_trombone[i].field_364[j][k] > 0) {
                    v13 = 1;
                    g_trombone[i].field_364[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_trombone[i].field_364[j][k] > 0) {
                        if (g_trombone[i].field_364[j][k] < v5) {
                            v5 = g_trombone[i].field_364[j][k];
                            v8 = j;
                            v4 = 1;
                        }
                    }
                    else {
                        ++v7;
                        g_trombone[i].field_364[j][k] = 0;
                        g_trombone[i].field_4[j] = g_trombone[i].field_64[j][k];
                        g_trombone[i].field_34[j] = g_trombone[i].field_64[j][k];
                        g_trombone[i].field_64[j][k] = 0;
                    }
                    g_trombone[i].isActive = 1;
                    g_trombone[i].timeToLive = 1000;
                    v3 = 1;
                }
            }
            if (g_trombone[i].field_4[j] > 0) {
                g_trombone[i].field_4[j] -= pmtNow - g_currentGlobalTime;
                v4 = 0;
                if (g_trombone[i].field_4[j] > 0) {
                    ++v6;
                    g_trombone[i].isActive = 1;
                    g_trombone[i].timeToLive = 1000;
                    v3 = 1;
                    v13 = 1;
                }
                else {
                    g_trombone[i].field_4[j] = 0;
                }
            }
        }
        if (v7 > 0 && v6 > 1) {
            for (m = 0; m < 12; ++m) {
                if (g_trombone[i].field_4[m] > 0 && g_trombone[i].field_4[m] < g_currentTempo_scaleFactor1_15)
                    g_trombone[i].field_4[m] = 0;
            }
        }
        if (v4 == 1) {
            v2 = (double)v8 / 12.0 * 23.0;
            if (g_trombone[i].slidePosition <= (double)v2) {
                if (g_trombone[i].slidePosition < (double)v2) {
                    g_trombone[i].slidePosition = g_trombone[i].slidePosition - (g_trombone[i].slidePosition - v2) / (
                            double)v5 *
                        7.0;
                    if (g_trombone[i].slidePosition >= (double)v2)
                        g_trombone[i].slidePosition = v2;
                }
            }
            else {
                g_trombone[i].slidePosition = g_trombone[i].slidePosition - (g_trombone[i].slidePosition - v2) / (
                    double)v5 * 7.0;
                if (g_trombone[i].slidePosition <= (double)v2)
                    g_trombone[i].slidePosition = v2;
            }
        }
        if (!v3 && g_trombone[i].isActive == 1 && --g_trombone[i].timeToLive <= 0) {
            g_trombone[i].timeToLive = 0;
            g_trombone[i].isActive = 0;
        }
    }
    return v13 == 1;
}
