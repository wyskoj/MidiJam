//
// Created by Jacob on Saturday, April 11, 2026.
//


#include "MusicBox.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(musicBox, MusicBoxState)

Ms3dBundle* g_musicBoxSpindle_ms3d = nullptr;
Ms3dBundle* g_musicBoxTopBlade_ms3d = nullptr;
Ms3dBundle* g_musicBoxCase_ms3d = nullptr;
Ms3dBundle* g_musicBoxPoint_ms3d = nullptr;
Ms3dBundle* g_musicBoxKey_ms3d = nullptr;

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern float MAX_RECOIL;

// FUNCTION: MIDIJAM 0x40CB20
void RenderMusicBox() {
    GLfloat angle; // [esp+0h] [ebp-68h]
    GLfloat anglea; // [esp+0h] [ebp-68h]
    GLfloat angleb; // [esp+0h] [ebp-68h]
    GLfloat anglec; // [esp+0h] [ebp-68h]
    GLfloat angled; // [esp+0h] [ebp-68h]
    GLfloat x; // [esp+4h] [ebp-64h]
    GLfloat xa; // [esp+4h] [ebp-64h]
    GLfloat xb; // [esp+4h] [ebp-64h]
    GLfloat xc; // [esp+4h] [ebp-64h]
    GLfloat z; // [esp+Ch] [ebp-5Ch]
    __int16 k; // [esp+28h] [ebp-40h]
    float v11[12]; // [esp+2Ch] [ebp-3Ch]
    __int16 i; // [esp+5Ch] [ebp-Ch]
    __int16 v13; // [esp+60h] [ebp-8h]
    __int16 j; // [esp+64h] [ebp-4h]

    v13 = 0;
    glPushMatrix();
    glTranslatef(0.0, 0.0, -14.0);
    for (i = 0; i < g_musicBoxCount; ++i) {
        if (g_musicBox[i].isActive == 1) {
            glPushMatrix();
            z = (double)v13 * -18.0 + 0.0;
            glTranslatef(36.0, -24.0, z);
            glPushMatrix();
            glRotatef(g_musicBox[i].field_8D8, 1.0, 0.0, 0.0);
            g_musicBoxSpindle_ms3d->RenderModelShiny();
            glPopMatrix();
            g_musicBoxTopBlade_ms3d->RenderModelShiny();
            g_musicBoxCase_ms3d->RenderModel();
            for (j = 0; j < 12; ++j) {
                v11[j] = 90.0;
                for (k = 0; k < 16; ++k) {
                    if (g_musicBox[i].field_368[j][k] > 0) {
                        glPushMatrix();
                        x = (double)j * 1.0 + -5.5;
                        glTranslatef(x, 0.0, 0.0);
                        angle = (double)g_musicBox[i].field_368[j][k] * -0.125;
                        glRotatef(angle, 1.0, 0.0, 0.0);
                        g_musicBoxPoint_ms3d->RenderModelShiny();
                        glPopMatrix();
                    }
                    if (g_musicBox[i].field_4E8[j][k] < 0.0) {
                        glPushMatrix();
                        xa = (double)j * 1.0 + -5.5;
                        glTranslatef(xa, 0.0, 0.0);
                        anglea = g_musicBox[i].field_4E8[j][k] * -0.125;
                        glRotatef(anglea, 1.0, 0.0, 0.0);
                        g_musicBoxPoint_ms3d->RenderModelShiny();
                        glPopMatrix();
                        if (g_musicBox[i].field_4E8[j][k] * -0.125 < v11[j])
                            v11[j] = g_musicBox[i].field_4E8[j][k] * -0.125;
                    }
                }
                glPushMatrix();
                xb = (double)j * 1.0 + -5.5;
                glTranslatef(xb, 7.0, 0.0);
                if (v11[j] >= 14.0) {
                    if (v11[j] >= 29.0) {
                        if (v11[j] < 34.0) {
                            angled = (5.0 - (v11[j] - 29.0)) / 5.0 * 5.0;
                            glRotatef(angled, 1.0, 0.0, 0.0);
                        }
                    }
                    else {
                        anglec = (10.0 - (v11[j] - 14.0)) / 10.0 * -13.5;
                        glRotatef(anglec, 1.0, 0.0, 0.0);
                    }
                }
                else {
                    angleb = v11[j] / 14.0 * -13.5;
                    glRotatef(angleb, 1.0, 0.0, 0.0);
                }
                xc = (double)(12 - j) / 12.0 * 0.5 + 0.5;
                glScalef(xc, 1.0, 1.0);
                g_musicBoxKey_ms3d->RenderModelShiny();
                glPopMatrix();
            }
            glPopMatrix();
            ++v13;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x433550
bool UpdateMusicBox(MUSIC_TIME pmtNow) {
    double v1; // st7
    int v3; // [esp+8h] [ebp-30h]
    float v4; // [esp+20h] [ebp-18h]
    __int16 v5; // [esp+24h] [ebp-14h]
    __int16 i; // [esp+28h] [ebp-10h]
    __int16 k; // [esp+2Ch] [ebp-Ch]
    __int16 j; // [esp+30h] [ebp-8h]
    __int16 v9; // [esp+34h] [ebp-4h]

    v9 = 0;
    for (i = 0; i < g_musicBoxCount; ++i) {
        v5 = 0;
        g_musicBox[i].field_8D8 = (double)(pmtNow - g_currentGlobalTime) * 0.125 + g_musicBox[i].field_8D8;
        if (g_musicBox[i].field_8D8 > 360.0)
            g_musicBox[i].field_8D8 = g_musicBox[i].field_8D8 - 360.0;
        for (j = 0; j < 12; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_musicBox[i].field_4E8[j][k] < 0.0) {
                    g_musicBox[i].field_4E8[j][k] = g_musicBox[i].field_4E8[j][k]
                        - (double)(__int16)(pmtNow - g_currentGlobalTime);
                    if (g_musicBox[i].field_4E8[j][k] < -900.0)
                        g_musicBox[i].field_4E8[j][k] = 0.0;
                }
                if (g_musicBox[i].field_368[j][k] > 0) {
                    v9 = 1;
                    g_musicBox[i].field_368[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_musicBox[i].field_368[j][k] <= 0) {
                        if (0.0 == (double)g_musicBox[i].field_368[j][k])
                            v1 = (double)g_musicBox[i].field_368[j][k] - 0.001;
                        else
                            v1 = (double)g_musicBox[i].field_368[j][k];
                        g_musicBox[i].field_4E8[j][k] = v1;
                        v3 = g_musicBox[i].field_818[j][k];
                        v4 = ((double)v3 * 0.75 + 32.0 + (double)v3 * 0.75 + 32.0) / 128.0;
                        g_musicBox[i].field_4 = v4 + g_musicBox[i].field_4;
                        if (g_musicBox[i].field_4 > (double)MAX_RECOIL)
                            g_musicBox[i].field_4 = MAX_RECOIL;
                        g_musicBox[i].field_368[j][k] = 0;
                        g_musicBox[i].field_7E8[j] = (double)g_musicBox[i].field_818[j][k] * 8.0 / 128.0 + 1.75;
                        g_musicBox[i].field_8[j] = g_musicBox[i].field_68[j][k];
                        g_musicBox[i].field_38[j] = g_musicBox[i].field_68[j][k];
                        g_musicBox[i].field_68[j][k] = 0;
                    }
                    g_musicBox[i].isActive = 1;
                    g_musicBox[i].timeToLive = 1000;
                    v5 = 1;
                }
            }
            if (g_musicBox[i].field_8[j] <= 0) {
                if (g_musicBox[i].field_7E8[j] > 0.0) {
                    v9 = 1;
                    g_musicBox[i].field_7E8[j] = g_musicBox[i].field_7E8[j] - 0.30000001;
                    if (g_musicBox[i].field_7E8[j] > 0.0) {
                        g_musicBox[i].isActive = 1;
                        g_musicBox[i].timeToLive = 1000;
                        v5 = 1;
                    }
                    else {
                        g_musicBox[i].field_7E8[j] = 0.0;
                    }
                }
            }
            else {
                g_musicBox[i].field_8[j] -= pmtNow - g_currentGlobalTime;
                if (g_musicBox[i].field_8[j] > 0) {
                    g_musicBox[i].isActive = 1;
                    g_musicBox[i].timeToLive = 1000;
                    v5 = 1;
                    v9 = 1;
                }
                else {
                    g_musicBox[i].field_8[j] = 0;
                }
                g_musicBox[i].field_7E8[j] = g_musicBox[i].field_7E8[j] - 0.15000001;
                if (g_musicBox[i].field_7E8[j] > 0.0) {
                    g_musicBox[i].isActive = 1;
                    g_musicBox[i].timeToLive = 1000;
                    v5 = 1;
                }
                else {
                    g_musicBox[i].field_7E8[j] = 0.0;
                }
            }
        }
        if (!v5 && g_musicBox[i].isActive == 1 && --g_musicBox[i].timeToLive <= 0) {
            g_musicBox[i].timeToLive = 0;
            g_musicBox[i].isActive = 0;
        }
    }
    return v9 == 1;
}
