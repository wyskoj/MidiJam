//
// Created by Jacob on Saturday, April 11, 2026.
//


#include "TubularBells.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

#include "scene/scene.h"

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(tubularBells, TubularBellsState)

Ms3dBundle* g_tubularBellDark_ms3d = nullptr;
Ms3dBundle* g_tubularBell_ms3d = nullptr;
Ms3dBundle* g_tubularBellMallet_ms3d = nullptr;

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern float MAX_RECOIL;

// FUNCTION: MIDIJAM 0x40C610
void RenderTubularBells() {
    double v0; // st7
    GLfloat angle; // [esp+0h] [ebp-58h]
    GLfloat anglea; // [esp+0h] [ebp-58h]
    GLfloat angleb; // [esp+0h] [ebp-58h]
    GLfloat z; // [esp+Ch] [ebp-4Ch]
    GLfloat za; // [esp+Ch] [ebp-4Ch]
    GLfloat zb; // [esp+Ch] [ebp-4Ch]
    GLfloat zc; // [esp+Ch] [ebp-4Ch]
    GLfloat zd; // [esp+Ch] [ebp-4Ch]
    __int16 k; // [esp+38h] [ebp-20h]
    __int16 v10; // [esp+44h] [ebp-14h]
    __int16 i; // [esp+4Ch] [ebp-Ch]
    __int16 v12; // [esp+50h] [ebp-8h]
    __int16 j; // [esp+54h] [ebp-4h]

    v12 = 0;
    glPushMatrix();
    glTranslatef(0.0, 8.0, -14.0);
    for (i = 0; i < g_tubularBellsCount; ++i) {
        if (g_tubularBells[i].isActive == 1) {
            for (j = 0; j < 12; ++j) {
                glPushMatrix();
                angle = STAGE_RIGHT_ROTATION_BASE - (double)(11 - j) * STAGE_RIGHT_ROTATION_DELTA;
                glRotatef(angle, 0.0, 1.0, 0.0);
                glPushMatrix();
                z = STAGE_BASE_Z - (double)v12 * 12.0;
                glTranslatef(0.0, 60.0, z);
                glRotatef(g_tubularBells[i].field_5D8[j], 1.0, 0.0, 0.0);
                za = (double)(12 - j) / 12.0 * 0.5 + 0.5;
                glScalef(za, za, za);
                if (g_tubularBells[i].field_5D8[j] == 0.0)
                    g_tubularBellDark_ms3d->RenderModelShiny();
                else
                    g_tubularBell_ms3d->RenderModelShiny();
                glPopMatrix();
                zb = STAGE_BASE_Z - (double)v12 * 12.0 + 4.0;
                glTranslatef(0.0, 40.0, zb);
                v10 = 10000;
                for (k = 0; k < 16; ++k) {
                    if (g_tubularBells[i].field_368[j][k] > 0 && g_tubularBells[i].field_368[j][k] < v10)
                        v10 = g_tubularBells[i].field_368[j][k];
                }
                if (v10 <= 0 || (double)v10 >= 300.0) {
                    if (g_tubularBells[i].field_4E8[j] > 0.0) {
                        v0 = g_tubularBells[i].field_4;
                        zd = 1.0 - (v0 + v0) + (10.0 - g_tubularBells[i].field_4E8[j]) / 10.0 * 7.0;
                        glTranslatef(0.0, 0.0, zd);
                        angleb = (10.0 - g_tubularBells[i].field_4E8[j]) / 10.0 * 65.0;
                        glRotatef(angleb, 1.0, 0.0, 0.0);
                        g_tubularBellMallet_ms3d->RenderModel();
                    }
                }
                else {
                    zc = 1.0 - (g_tubularBells[i].field_4 + g_tubularBells[i].field_4) + (double)v10 / 300.0 * 7.0;
                    glTranslatef(0.0, 0.0, zc);
                    anglea = (double)v10 / 300.0 * 65.0;
                    glRotatef(anglea, 1.0, 0.0, 0.0);
                    g_tubularBellMallet_ms3d->RenderModel();
                }
                glPopMatrix();
            }
            ++v12;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x436460
bool UpdateTubularBells(MUSIC_TIME pmtNow) {
    int v2; // [esp+8h] [ebp-24h]
    float v3; // [esp+Ch] [ebp-20h]
    float v4; // [esp+Ch] [ebp-20h]
    float v5; // [esp+10h] [ebp-1Ch]
    float v6; // [esp+14h] [ebp-18h]
    __int16 v7; // [esp+18h] [ebp-14h]
    __int16 i; // [esp+1Ch] [ebp-10h]
    __int16 k; // [esp+20h] [ebp-Ch]
    __int16 j; // [esp+24h] [ebp-8h]
    __int16 v11; // [esp+28h] [ebp-4h]

    v11 = 0;
    for (i = 0; i < g_tubularBellsCount; ++i) {
        v7 = 0;
        for (j = 0; j < 12; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_tubularBells[i].field_368[j][k] > 0) {
                    v11 = 1;
                    g_tubularBells[i].field_368[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_tubularBells[i].field_368[j][k] <= 0) {
                        v2 = g_tubularBells[i].field_518[j][k];
                        v6 = ((double)v2 * 0.75 + 32.0 + (double)v2 * 0.75 + 32.0) / 128.0;
                        g_tubularBells[i].field_4 = v6 + g_tubularBells[i].field_4;
                        if (g_tubularBells[i].field_4 > (double)MAX_RECOIL)
                            g_tubularBells[i].field_4 = MAX_RECOIL;
                        g_tubularBells[i].field_368[j][k] = 0;
                        g_tubularBells[i].field_4E8[j] = (double)g_tubularBells[i].field_518[j][k] * 8.0 / 128.0 + 1.75;
                        g_tubularBells[i].field_8[j] = g_tubularBells[i].field_68[j][k];
                        g_tubularBells[i].field_38[j] = g_tubularBells[i].field_68[j][k];
                        g_tubularBells[i].field_68[j][k] = 0;
                        g_tubularBells[i].field_668[j] = 0;
                        if (g_tubularBells[i].field_5D8[j] < 0.0)
                            g_tubularBells[i].field_5D8[j] = g_tubularBells[i].field_5D8[j] * 0.25;
                        if (g_tubularBells[i].field_5D8[j] > 0.0)
                            g_tubularBells[i].field_5D8[j] = g_tubularBells[i].field_5D8[j] * 0.5;
                        if (g_tubularBells[i].field_608[j] < 0.0)
                            g_tubularBells[i].field_608[j] = 0.0;
                        g_tubularBells[i].field_608[j] = 0.30000001;
                        g_tubularBells[i].field_638[j] = 0.0125;
                    }
                    g_tubularBells[i].isActive = 1;
                    g_tubularBells[i].timeToLive = 1000;
                    v7 = 1;
                }
            }
            if (g_tubularBells[i].field_8[j] <= 0) {
                if (g_tubularBells[i].field_4E8[j] > 0.0) {
                    v11 = 1;
                    g_tubularBells[i].field_4E8[j] = g_tubularBells[i].field_4E8[j] - 0.30000001;
                    if (g_tubularBells[i].field_4E8[j] > 0.0) {
                        g_tubularBells[i].isActive = 1;
                        g_tubularBells[i].timeToLive = 1000;
                        v7 = 1;
                    }
                    else {
                        g_tubularBells[i].field_4E8[j] = 0.0;
                    }
                }
            }
            else {
                g_tubularBells[i].field_8[j] -= pmtNow - g_currentGlobalTime;
                if (g_tubularBells[i].field_8[j] > 0) {
                    g_tubularBells[i].isActive = 1;
                    g_tubularBells[i].timeToLive = 1000;
                    v7 = 1;
                    v11 = 1;
                }
                else {
                    g_tubularBells[i].field_8[j] = 0;
                }
                g_tubularBells[i].field_4E8[j] = g_tubularBells[i].field_4E8[j] - 0.15000001;
                if (g_tubularBells[i].field_4E8[j] > 0.0) {
                    g_tubularBells[i].isActive = 1;
                    g_tubularBells[i].timeToLive = 1000;
                    v7 = 1;
                }
                else {
                    g_tubularBells[i].field_4E8[j] = 0.0;
                }
            }
            if (g_tubularBells[i].field_608[j] != 0.0) {
                g_tubularBells[i].isActive = 1;
                g_tubularBells[i].timeToLive = 1000;
                v7 = 1;
                v5 = g_tubularBells[i].field_5D8[j];
                g_tubularBells[i].field_5D8[j] = v5 + g_tubularBells[i].field_608[j];
                if ((v5 >= 0.0 && g_tubularBells[i].field_5D8[j] <= 0.0
                        || v5 <= 0.0 && g_tubularBells[i].field_5D8[j] >= 0.0)
                    && g_tubularBells[i].field_668[j] >= 7) {
                    g_tubularBells[i].field_5D8[j] = 0.0;
                    g_tubularBells[i].field_608[j] = 0.0;
                    g_tubularBells[i].field_638[j] = 0.0;
                    g_tubularBells[i].field_668[j] = 0;
                }
                if (g_tubularBells[i].field_5D8[j] <= 0.0) {
                    v4 = g_tubularBells[i].field_608[j];
                    g_tubularBells[i].field_608[j] = v4 + g_tubularBells[i].field_638[j];
                    if (v4 < 0.0 && g_tubularBells[i].field_608[j] >= 0.0) {
                        g_tubularBells[i].field_638[j] = g_tubularBells[i].field_638[j] * 0.75;
                        ++g_tubularBells[i].field_668[j];
                    }
                }
                else {
                    v3 = g_tubularBells[i].field_608[j];
                    g_tubularBells[i].field_608[j] = v3 - g_tubularBells[i].field_638[j];
                    if (v3 > 0.0 && g_tubularBells[i].field_608[j] <= 0.0) {
                        g_tubularBells[i].field_638[j] = g_tubularBells[i].field_638[j] * 0.75;
                        ++g_tubularBells[i].field_668[j];
                    }
                }
            }
        }
        if (!v7 && g_tubularBells[i].isActive == 1 && --g_tubularBells[i].timeToLive <= 0) {
            g_tubularBells[i].timeToLive = 0;
            g_tubularBells[i].isActive = 0;
        }
    }
    return v11 == 1;
}
