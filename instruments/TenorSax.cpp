//
// Created by Jacob on Friday, April 10, 2026.
//

#include "TenorSax.h"

#include "AltoSax.h"
#include "../render/texture.h"
#include "../macros.h"
#include "sax.h"

#include <GL/gl.h>

DEF_INST(tenorSax, TenorSaxState)

Ms3dBundle* g_tenorSaxBody_ms3d = nullptr;
Ms3dBundle* g_tenorSaxHorn_ms3d = nullptr;

extern MUSIC_TIME g_currentGlobalTime;
extern int g_currentTempo_scaleFactor1_15;

// FUNCTION: MIDIJAM 0x409D10
void RenderTenorSax() {
    GLfloat angle;
    GLfloat anglea;
    GLfloat angleb;
    GLfloat y;
    GLfloat ya;
    GLfloat yb;
    __int16 k;
    __int16 m;
    __int16 i;
    __int16 v9;
    __int16 v10;
    __int16 j;

    v10 = 0;
    glPushMatrix();
    glTranslatef(-10.5f, -60.0f, -34.5f);
    glScalef(1.2f, 1.2f, 1.2f);
    for (i = 0; i < g_tenorSaxCount; ++i) {
        if (g_tenorSax[i].isActive == 1) {
            v9 = 0;
            for (j = 0; j < 33; ++j) {
                if (g_tenorSax[i].field_4[j] > 0) {
                    glPushMatrix();
                    angle = (double)v9 * 25.0 + 30.0;
                    glRotatef(angle, 0.0f, 1.0f, 0.0f);
                    y = (double)v10 * 35.0 + 50.0;
                    glTranslatef(0.0f, y, 18.0f);
                    anglea = 12.0 - (double)g_tenorSax[i].field_4[j] / (double)g_tenorSax[i].field_88[j] * 3.0;
                    glRotatef(anglea, 1.0f, 0.0f, 0.0f);
                    g_tenorSaxBody_ms3d->RenderModelShiny();
                    for (k = 0; k < 20; ++k)
                        g_altoSaxKeyX_ms3d[k][SAXOPHONE_KEY_INDEX[j][k]]->RenderModelShiny();
                    glTranslatef(0.0f, -22.0f, 0.0f);
                    ya = (double)g_tenorSax[i].field_4[j] / (double)g_tenorSax[i].field_88[j] * 0.75 + 1.0;
                    glScalef(1.0f, ya, 1.0f);
                    g_tenorSaxHorn_ms3d->RenderModelShiny();
                    glPopMatrix();
                    ++v9;
                }
            }
            if (!v9) {
                glPushMatrix();
                angleb = (double)0 * 25.0 + 30.0;
                glRotatef(angleb, 0.0f, 1.0f, 0.0f);
                yb = (double)v10 * 35.0 + 50.0;
                glTranslatef(0.0f, yb, 18.0f);
                glRotatef(12.0f, 1.0f, 0.0f, 0.0f);
                g_tenorSaxBody_ms3d->RenderModelShiny();
                for (m = 0; m < 20; ++m)
                    g_altoSaxKeyX_ms3d[m][SAXOPHONE_KEY_INDEX[g_tenorSax[i].field_D6C][m]]->RenderModelShiny();
                glTranslatef(0.0f, -22.0f, 0.0f);
                g_tenorSaxHorn_ms3d->RenderModelShiny();
                glPopMatrix();
            }
            ++v10;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x42F820
bool UpdateTenorSax(MUSIC_TIME pmtNow) {
    __int16 v2;
    __int16 v3;
    __int16 v4;
    __int16 i;
    __int16 k;
    __int16 j;
    __int16 m;
    __int16 v9;

    v9 = 0;
    for (i = 0; i < g_tenorSaxCount; ++i) {
        v2 = 0;
        v4 = 0;
        v3 = 0;
        for (j = 0; j < 33; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_tenorSax[i].field_94C[j][k] > 0) {
                    v9 = 1;
                    g_tenorSax[i].field_94C[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_tenorSax[i].field_94C[j][k] <= 0) {
                        ++v4;
                        g_tenorSax[i].field_94C[j][k] = 0;
                        g_tenorSax[i].field_4[j] = g_tenorSax[i].field_10C[j][k];
                        g_tenorSax[i].field_88[j] = g_tenorSax[i].field_10C[j][k];
                        g_tenorSax[i].field_10C[j][k] = 0;
                    }
                    g_tenorSax[i].isActive = 1;
                    g_tenorSax[i].timeToLive = 1000;
                    v2 = 1;
                }
            }
            if (g_tenorSax[i].field_4[j] > 0) {
                g_tenorSax[i].field_D6C = j;
                g_tenorSax[i].field_4[j] -= pmtNow - g_currentGlobalTime;
                if (g_tenorSax[i].field_4[j] > 0) {
                    ++v3;
                    g_tenorSax[i].isActive = 1;
                    g_tenorSax[i].timeToLive = 1000;
                    v2 = 1;
                    v9 = 1;
                }
                else {
                    g_tenorSax[i].field_4[j] = 0;
                }
            }
        }
        if (v4 > 0 && v3 > 1) {
            for (m = 0; m < 33; ++m) {
                if (g_tenorSax[i].field_4[m] > 0 && g_tenorSax[i].field_4[m] < g_currentTempo_scaleFactor1_15)
                    g_tenorSax[i].field_4[m] = 0;
            }
        }
        if (!v2 && g_tenorSax[i].isActive == 1 && --g_tenorSax[i].timeToLive <= 0) {
            g_tenorSax[i].timeToLive = 0;
            g_tenorSax[i].isActive = 0;
        }
    }
    return v9 == 1;
}

