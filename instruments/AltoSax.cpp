//
// Created by Jacob on Friday, April 10, 2026.
//


#include "AltoSax.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

#include "sax.h"

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(altoSax, AltoSaxState)

Ms3dBundle* g_altoSaxBody_ms3d = nullptr;
Ms3dBundle* g_altoSaxHorn_ms3d = nullptr;
Ms3dBundle* g_altoSaxKeyX_ms3d[20][2] = {};

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern int g_currentTempo_scaleFactor1_15;

// FUNCTION: MIDIJAM 0xXXXXXXXX
void RenderAltoSax() {
    GLfloat angle; // [esp+0h] [ebp-34h]
    GLfloat anglea; // [esp+0h] [ebp-34h]
    GLfloat angleb; // [esp+0h] [ebp-34h]
    GLfloat y; // [esp+8h] [ebp-2Ch]
    GLfloat ya; // [esp+8h] [ebp-2Ch]
    GLfloat yb; // [esp+8h] [ebp-2Ch]
    __int16 k; // [esp+20h] [ebp-14h]
    __int16 m; // [esp+20h] [ebp-14h]
    __int16 i; // [esp+24h] [ebp-10h]
    __int16 v9; // [esp+28h] [ebp-Ch]
    __int16 v10; // [esp+2Ch] [ebp-8h]
    __int16 j; // [esp+30h] [ebp-4h]

    v10 = 0;
    glPushMatrix();
    glTranslatef(-30.5, -40.0, -64.5); // alto sax positioning
    for (i = 0; i < g_altoSaxCount; ++i) {
        if (g_altoSax[i].isActive == 1) {
            v9 = 0;
            for (j = 0; j < 33; ++j) {
                if (g_altoSax[i].field_4[j] > 0) {
                    glPushMatrix();
                    angle = (double)v9 * 25.0 + 70.0;
                    glRotatef(angle, 0.0, 1.0, 0.0);
                    y = (double)v10 * 35.0 + 50.0;
                    glTranslatef(0.0, y, 18.0);
                    anglea = 15.0 - (double)g_altoSax[i].field_4[j] / (double)g_altoSax[i].field_88[j] * 3.0;
                    glRotatef(anglea, 1.0, 0.0, 0.0);
                    g_altoSaxBody_ms3d->RenderModelShiny();
                    for (k = 0; k < 20; ++k)
                        g_altoSaxKeyX_ms3d[k][SAXOPHONE_KEY_INDEX[j][k]]->RenderModelShiny();
                    glTranslatef(0.0, -22.0, 0.0);
                    ya = (double)g_altoSax[i].field_4[j] / (double)g_altoSax[i].field_88[j] * 0.5 + 1.0;
                    glScalef(1.0, ya, 1.0);
                    g_altoSaxHorn_ms3d->RenderModelShiny();
                    glPopMatrix();
                    ++v9;
                }
            }
            if (!v9) {
                glPushMatrix();
                angleb = (double)0 * 25.0 + 70.0;
                glRotatef(angleb, 0.0, 1.0, 0.0);
                yb = (double)v10 * 35.0 + 50.0;
                glTranslatef(0.0, yb, 18.0);
                glRotatef(15.0, 1.0, 0.0, 0.0);
                g_altoSaxBody_ms3d->RenderModelShiny();
                for (m = 0; m < 20; ++m)
                    g_altoSaxKeyX_ms3d[m][SAXOPHONE_KEY_INDEX[g_altoSax[i].field_D6C][m]]->RenderModelShiny();
                glTranslatef(0.0, -22.0, 0.0);
                g_altoSaxHorn_ms3d->RenderModelShiny();
                glPopMatrix();
            }
            ++v10;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0xXXXXXXXX
bool UpdateAltoSax(MUSIC_TIME pmtNow) {
    __int16 v2; // [esp+4h] [ebp-1Ch]
    __int16 v3; // [esp+8h] [ebp-18h]
    __int16 v4; // [esp+Ch] [ebp-14h]
    __int16 i; // [esp+10h] [ebp-10h]
    __int16 k; // [esp+14h] [ebp-Ch]
    __int16 j; // [esp+18h] [ebp-8h]
    __int16 m; // [esp+18h] [ebp-8h]
    __int16 v9; // [esp+1Ch] [ebp-4h]

    v9 = 0;
    for (i = 0; i < g_altoSaxCount; ++i) {
        v2 = 0;
        v4 = 0;
        v3 = 0;
        for (j = 0; j < 33; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_altoSax[i].field_94C[j][k] > 0) {
                    v9 = 1;
                    g_altoSax[i].field_94C[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_altoSax[i].field_94C[j][k] <= 0) {
                        ++v4;
                        g_altoSax[i].field_94C[j][k] = 0;
                        g_altoSax[i].field_4[j] = g_altoSax[i].field_10C[j][k];
                        g_altoSax[i].field_88[j] = g_altoSax[i].field_10C[j][k];
                        g_altoSax[i].field_10C[j][k] = 0;
                    }
                    g_altoSax[i].isActive = 1;
                    g_altoSax[i].timeToLive = 1000;
                    v2 = 1;
                }
            }
            if (g_altoSax[i].field_4[j] > 0) {
                g_altoSax[i].field_D6C = j;
                g_altoSax[i].field_4[j] -= pmtNow - g_currentGlobalTime;
                if (g_altoSax[i].field_4[j] > 0) {
                    ++v3;
                    g_altoSax[i].isActive = 1;
                    g_altoSax[i].timeToLive = 1000;
                    v2 = 1;
                    v9 = 1;
                }
                else {
                    g_altoSax[i].field_4[j] = 0;
                }
            }
        }
        if (v4 > 0 && v3 > 1) {
            for (m = 0; m < 33; ++m) {
                if (g_altoSax[i].field_4[m] > 0 && g_altoSax[i].field_4[m] < g_currentTempo_scaleFactor1_15)
                    g_altoSax[i].field_4[m] = 0;
            }
        }
        if (!v2 && g_altoSax[i].isActive == 1 && --g_altoSax[i].timeToLive <= 0) {
            g_altoSax[i].timeToLive = 0;
            g_altoSax[i].isActive = 0;
        }
    }
    return v9 == 1;
}
