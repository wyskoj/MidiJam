//
// Created by Jacob on Saturday, April 11, 2026.
//


#include "Violin.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

#include "strings.h"

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(violin, ViolinState)

Ms3dBundle* g_violin_ms3d = nullptr;
__int16 VIOLIN_NOTES[18][4] = {};

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern int g_vibratingString_frame;

// FUNCTION: MIDIJAM 0xXXXXXXXX
void RenderViolin() {
    GLfloat x; // [esp+0h] [ebp-20h]
    GLfloat y; // [esp+4h] [ebp-1Ch]
    GLfloat ya; // [esp+4h] [ebp-1Ch]
    GLfloat yb; // [esp+4h] [ebp-1Ch]
    GLfloat yc; // [esp+4h] [ebp-1Ch]
    GLfloat yd; // [esp+4h] [ebp-1Ch]
    GLfloat ye; // [esp+4h] [ebp-1Ch]
    GLfloat yf; // [esp+4h] [ebp-1Ch]
    GLfloat yg; // [esp+4h] [ebp-1Ch]
    __int16 j; // [esp+10h] [ebp-10h]
    __int16 i; // [esp+14h] [ebp-Ch]
    __int16 v11; // [esp+18h] [ebp-8h]
    GLboolean v12; // [esp+1Fh] [ebp-1h]

    v11 = 0;
    v12 = g_violin_ms3d->ApplyMaterial();
    for (i = 0; i < g_violinCount; ++i) {
        if (g_violin[i].isActive == 1) {
            glPushMatrix();
            x = (double)v11 * 15.0 + 12.0;
            glTranslatef(x, 24.0, -30.0);
            glRotatef(210.0, 0.0, 0.0, 1.0);
            glRotatef(60.0, 1.0, 0.0, 0.0);
            g_violin_ms3d->Render();
            glPushMatrix();
            glTranslatef(0.0, -3.9000001, 1.675);
            glRotatef(g_violin[i].bowX, 0.0, 1.0, 0.0);
            glRotatef(15.0, 0.0, 0.0, 1.0);
            glTranslatef(g_violin[i].bowX, 0.0, g_violin[i].bowZ);
            g_violinBow_ms3d->Render();
            glPopMatrix();
            for (j = 0; j < 4; ++j) {
                if (g_violin[i].frets[j])
                    g_stringInstrument_stringScale[j] = dword_45E7E4[g_violin[i].frets[j]];
                else
                    g_stringInstrument_stringScale[j] = 1.0;
            }
            if (g_stringInstrument_stringScale[0] > 0.0) {
                glPushMatrix();
                glTranslatef(-0.37200001, 8.8369999, 0.60000002);
                glRotatef(-4.04, 1.0, 0.0, 0.0);
                glRotatef(-1.64, 0.0, 0.0, 1.0);
                glScalef(1.0, g_stringInstrument_stringScale[0], 1.0);
                g_violinString_ms3d->Render();
                glPopMatrix();
            }
            if (g_stringInstrument_stringScale[1] > 0.0) {
                glPushMatrix();
                glTranslatef(-0.123, 8.8369999, 0.60000002);
                glRotatef(-4.5999999, 1.0, 0.0, 0.0);
                glRotatef(-0.67199999, 0.0, 0.0, 1.0);
                glScalef(1.0, g_stringInstrument_stringScale[1], 1.0);
                g_violinString_ms3d->Render();
                glPopMatrix();
            }
            if (g_stringInstrument_stringScale[2] > 0.0) {
                glPushMatrix();
                glTranslatef(0.123, 8.8369999, 0.60000002);
                glRotatef(-4.5999999, 1.0, 0.0, 0.0);
                glRotatef(0.67199999, 0.0, 0.0, 1.0);
                glScalef(1.0, g_stringInstrument_stringScale[2], 1.0);
                g_violinString_ms3d->Render();
                glPopMatrix();
            }
            if (g_stringInstrument_stringScale[3] > 0.0) {
                glPushMatrix();
                glTranslatef(0.36399999, 8.8369999, 0.60000002);
                glRotatef(-4.04, 1.0, 0.0, 0.0);
                glRotatef(1.64, 0.0, 0.0, 1.0);
                glScalef(1.0, g_stringInstrument_stringScale[3], 1.0);
                g_violinString_ms3d->Render();
                glPopMatrix();
            }
            if (g_stringInstrument_stringScale[0] < 1.0) {
                glPushMatrix();
                glTranslatef(-0.80000001, -6.1620002, 1.6619999);
                glRotatef(-4.04, 1.0, 0.0, 0.0);
                glRotatef(-1.64, 0.0, 0.0, 1.0);
                if (g_stringInstrument_stringScale[0] > 0.0 && g_stringInstrument_stringScale[0] < 1.0) {
                    glPushMatrix();
                    y = (1.0 - g_stringInstrument_stringScale[0]) * 15.0;
                    glTranslatef(0.0, y, 0.0);
                    g_violinFinger_ms3d->Render();
                    glPopMatrix();
                }
                ya = 1.0 - g_stringInstrument_stringScale[0];
                glScalef(1.0, ya, 1.0);
                g_violinStringPlayedX_ms3d[g_vibratingString_frame]->Render();
                glPopMatrix();
            }
            if (g_stringInstrument_stringScale[1] < 1.0) {
                glPushMatrix();
                glTranslatef(-0.30000001, -6.1620002, 1.8150001);
                glRotatef(-4.5999999, 1.0, 0.0, 0.0);
                glRotatef(-0.67199999, 0.0, 0.0, 1.0);
                if (g_stringInstrument_stringScale[1] > 0.0 && g_stringInstrument_stringScale[1] < 1.0) {
                    glPushMatrix();
                    yb = (1.0 - g_stringInstrument_stringScale[1]) * 15.0;
                    glTranslatef(0.0, yb, 0.0);
                    g_violinFinger_ms3d->Render();
                    glPopMatrix();
                }
                yc = 1.0 - g_stringInstrument_stringScale[1];
                glScalef(1.0, yc, 1.0);
                g_violinStringPlayedX_ms3d[g_vibratingString_frame]->Render();
                glPopMatrix();
            }
            if (g_stringInstrument_stringScale[2] < 1.0) {
                glPushMatrix();
                glTranslatef(0.30000001, -6.1620002, 1.8150001);
                glRotatef(-4.5999999, 1.0, 0.0, 0.0);
                glRotatef(0.67199999, 0.0, 0.0, 1.0);
                if (g_stringInstrument_stringScale[2] > 0.0 && g_stringInstrument_stringScale[2] < 1.0) {
                    glPushMatrix();
                    yd = (1.0 - g_stringInstrument_stringScale[2]) * 15.0;
                    glTranslatef(0.0, yd, 0.0);
                    g_violinFinger_ms3d->Render();
                    glPopMatrix();
                }
                ye = 1.0 - g_stringInstrument_stringScale[2];
                glScalef(1.0, ye, 1.0);
                g_violinStringPlayedX_ms3d[g_vibratingString_frame]->Render();
                glPopMatrix();
            }
            if (g_stringInstrument_stringScale[3] < 1.0) {
                glPushMatrix();
                glTranslatef(0.80000001, -6.1620002, 1.6619999);
                glRotatef(-4.04, 1.0, 0.0, 0.0);
                glRotatef(1.64, 0.0, 0.0, 1.0);
                if (g_stringInstrument_stringScale[3] > 0.0 && g_stringInstrument_stringScale[3] < 1.0) {
                    glPushMatrix();
                    yf = (1.0 - g_stringInstrument_stringScale[3]) * 15.0;
                    glTranslatef(0.0, yf, 0.0);
                    g_violinFinger_ms3d->Render();
                    glPopMatrix();
                }
                yg = 1.0 - g_stringInstrument_stringScale[3];
                glScalef(1.0, yg, 1.0);
                g_violinStringPlayedX_ms3d[g_vibratingString_frame]->Render();
                glPopMatrix();
            }
            glPopMatrix();
            ++v11;
        }
    }
    SetTextureState(v12);
}

// FUNCTION: MIDIJAM 0xXXXXXXXX
bool UpdateViolin(MUSIC_TIME pmtNow) {
    __int16 jj; // [esp+4h] [ebp-28h]
    __int16 v3; // [esp+8h] [ebp-24h]
    __int16 v4; // [esp+Ch] [ebp-20h]
    __int16 v5; // [esp+14h] [ebp-18h]
    __int16 v6; // [esp+18h] [ebp-14h]
    __int16 i; // [esp+1Ch] [ebp-10h]
    __int16 ii; // [esp+20h] [ebp-Ch]
    __int16 j; // [esp+24h] [ebp-8h]
    __int16 k; // [esp+24h] [ebp-8h]
    __int16 m; // [esp+24h] [ebp-8h]
    __int16 n; // [esp+24h] [ebp-8h]

    for (i = 0; i < g_violinCount; ++i) {
        v3 = 0;
        v6 = 0;
        for (j = 0; j < 88; ++j) {
            if (g_violin[i].durations[j] > 0) {
                v6 = 1;
                g_violin[i].durations[j] -= pmtNow - g_currentGlobalTime;
                if (g_violin[i].durations[j] > 0) {
                    g_violin[i].isActive = 1;
                    g_violin[i].timeToLive = 1000;
                    v3 = 1;
                }
                else {
                    g_violin[i].durations[j] = 0;
                    g_violin[i].frets[g_violin[i].stringAssignment[j] - 1] = 0;
                    g_violin[i].stringAssignment[j] = 0;
                }
            }
        }
        if (v6 == 1) {
            g_violin[i].bowZ = 0.0;
            g_violin[i].bowSpeed = g_violin[i].bowSpeed + 0.0024999999;
            if (g_violin[i].bowSpeed > 0.0175)
                g_violin[i].bowSpeed = 0.0175;
        }
        else {
            g_violin[i].bowZ = g_violin[i].bowZ + 0.0049999999;
            if (g_violin[i].bowZ > 1.0)
                g_violin[i].bowZ = 1.0;
            g_violin[i].bowSpeed = g_violin[i].bowSpeed - 0.00075000001;
            if (g_violin[i].bowSpeed < 0.0)
                g_violin[i].bowSpeed = 0.0;
        }
        if (g_violin[i].bowSpeed > 0.0) {
            if (g_violin[i].bowDirection) {
                if (g_violin[i].bowDirection == 1) {
                    g_violin[i].bowX = g_violin[i].bowX - g_violin[i].bowSpeed;
                    if (-7.5 > g_violin[i].bowX) {
                        g_violin[i].bowX = -7.5;
                        g_violin[i].bowDirection = 0;
                    }
                }
            }
            else {
                g_violin[i].bowX = g_violin[i].bowX + g_violin[i].bowSpeed;
                if (g_violin[i].bowX > 7.5) {
                    g_violin[i].bowX = 7.5;
                    g_violin[i].bowDirection = 1;
                }
            }
        }
        for (k = 0; k < 4; ++k) {
            if (g_violin[i].frets[k] > 0 && g_violin[i].durations[VIOLIN_NOTES[g_violin[i].frets[k]][k]] <= 0) {
                g_violin[i].durations[VIOLIN_NOTES[g_violin[i].frets[k]][k]] = 0;
                g_violin[i].stringAssignment[VIOLIN_NOTES[g_violin[i].frets[k]][k]] = 0;
                g_violin[i].frets[k] = 0;
            }
        }
        v4 = 0;
        for (m = 0; m < 4; ++m) {
            if (g_violin[i].frets[m] > 0)
                ++v4;
        }
        v5 = 0;
        for (n = 0; n < 88; ++n) {
            for (ii = 0; ii < 16; ++ii) {
                if (g_violin[i].timeDeltas[n][ii] > 0) {
                    g_violin[i].timeDeltas[n][ii] -= pmtNow - g_currentGlobalTime;
                    if (g_violin[i].timeDeltas[n][ii] > 0) {
                        g_violin[i].isActive = 1;
                        g_violin[i].timeToLive = 1000;
                        v3 = 1;
                    }
                    else {
                        g_violin[i].timeDeltas[n][ii] = 0;
                        g_violin[i].durations[n] = g_violin[i].queue[n][ii];
                        g_violin[i].queue[n][ii] = 0;
                        if (!v4)
                            v5 = 1;
                        for (jj = 0; jj < 18; ++jj) {
                            if (VIOLIN_NOTES[jj + 1][0] == n && !g_violin[i].frets[0]) {
                                g_violin[i].frets[0] = jj + 1;
                                g_violin[i].stringAssignment[n] = 1;
                                break;
                            }
                            if (VIOLIN_NOTES[jj + 1][1] == n && !g_violin[i].frets[1]) {
                                g_violin[i].frets[1] = jj + 1;
                                g_violin[i].stringAssignment[n] = 2;
                                break;
                            }
                            if (VIOLIN_NOTES[jj + 1][2] == n && !g_violin[i].frets[2]) {
                                g_violin[i].frets[2] = jj + 1;
                                g_violin[i].stringAssignment[n] = 3;
                                break;
                            }
                            if (VIOLIN_NOTES[jj + 1][3] == n && !g_violin[i].frets[3]) {
                                g_violin[i].frets[3] = jj + 1;
                                g_violin[i].stringAssignment[n] = 4;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (v5 == 1) {
            if (g_violin[i].bowDirection)
                g_violin[i].bowDirection = 0;
            else
                g_violin[i].bowDirection = 1;
            if (g_violin[i].bowDirection) {
                if (g_violin[i].bowDirection == 1 && -7.5 * 0.75 > g_violin[i].bowX)
                    g_violin[i].bowDirection = 0;
            }
            else if (7.5 * 0.75 < g_violin[i].bowX) {
                g_violin[i].bowDirection = 1;
            }
        }
        if (!v3 && g_violin[i].isActive == 1 && --g_violin[i].timeToLive <= 0) {
            g_violin[i].timeToLive = 0;
            g_violin[i].isActive = 0;
        }
    }
    return 0;
}
