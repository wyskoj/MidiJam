//
// Created by Jacob on Saturday, April 11, 2026.
//


#include "Cello.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

#include "strings.h"

DEF_INST(cello, CelloState)

__int16 CELLO_NOTES[28][4] = {};

extern MUSIC_TIME g_currentGlobalTime;
extern int g_vibratingString_frame;

void RenderCello() {
    GLfloat x;
    GLfloat z;
    GLfloat y;
    GLfloat ya;
    GLfloat yb;
    GLfloat yc;
    GLfloat yd;
    GLfloat ye;
    GLfloat yf;
    GLfloat yg;
    __int16 j;
    __int16 i;
    __int16 v11;
    GLboolean v12;

    v11 = 0;
    v12 = g_cello_ms3d->ApplyMaterial();
    for (i = 0; i < g_celloCount; ++i) {
        if (g_cello[i].isActive == 1) {
            glPushMatrix();
            glTranslatef(-75.0f, 6.0f, -70.0f);
            glRotatef(20.0f, 0.0f, 1.0f, 0.0f);
            z = static_cast<double>(v11) * -7.5;
            x = static_cast<double>(v11) * -15.0;
            glTranslatef(x, 0.0f, z);
            glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.0f, 21.0f, 0.0f);
            glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, -21.0f, 0.0f);
            glScalef(1.88f, 1.88f, 1.88f);
            g_cello_ms3d->Render();
            glPushMatrix();
            glTranslatef(0.0, -3.9000001f, 1.675f);
            glRotatef(g_cello[i].bowX, 0.0, 1.0, 0.0);
            glRotatef(-200.0f, 0.0f, 0.0f, 1.0f);
            glScalef(0.60000002f, 0.75f, 0.75f);
            glTranslatef(g_cello[i].bowX, 0.0, g_cello[i].bowZ);
            g_violinBow_ms3d->Render();
            glPopMatrix();
            for (j = 0; j < 4; ++j) {
                if (g_cello[i].frets[j]) {
                    const int fret = g_cello[i].frets[j] > 17 ? 17 : g_cello[i].frets[j];
                    g_stringInstrument_stringScale[j] = dword_45E7E4[fret];
                }
                else {
                    g_stringInstrument_stringScale[j] = 1.0;
                }
            }
            if (g_stringInstrument_stringScale[0] > 0.0) {
                glPushMatrix();
                glTranslatef(-0.37200001f, 8.8369999f, 0.60000002f);
                glRotatef(-4.04f, 1.0, 0.0, 0.0);
                glRotatef(-1.64f, 0.0, 0.0, 1.0);
                glScalef(1.0, g_stringInstrument_stringScale[0], 1.0);
                g_violinString_ms3d->Render();
                glPopMatrix();
            }
            if (g_stringInstrument_stringScale[1] > 0.0) {
                glPushMatrix();
                glTranslatef(-0.123f, 8.8369999f, 0.60000002f);
                glRotatef(-4.5999999f, 1.0, 0.0, 0.0);
                glRotatef(-0.67199999f, 0.0, 0.0, 1.0);
                glScalef(1.0, g_stringInstrument_stringScale[1], 1.0);
                g_violinString_ms3d->Render();
                glPopMatrix();
            }
            if (g_stringInstrument_stringScale[2] > 0.0) {
                glPushMatrix();
                glTranslatef(0.123f, 8.8369999f, 0.60000002f);
                glRotatef(-4.5999999f, 1.0, 0.0, 0.0);
                glRotatef(0.67199999f, 0.0, 0.0, 1.0);
                glScalef(1.0, g_stringInstrument_stringScale[2], 1.0);
                g_violinString_ms3d->Render();
                glPopMatrix();
            }
            if (g_stringInstrument_stringScale[3] > 0.0) {
                glPushMatrix();
                glTranslatef(0.36399999f, 8.8369999f, 0.60000002f);
                glRotatef(-4.04f, 1.0, 0.0, 0.0);
                glRotatef(1.64f, 0.0, 0.0, 1.0);
                glScalef(1.0, g_stringInstrument_stringScale[3], 1.0);
                g_violinString_ms3d->Render();
                glPopMatrix();
            }
            if (g_stringInstrument_stringScale[0] < 1.0) {
                glPushMatrix();
                glTranslatef(-0.80000001f, -6.1620002f, 1.6619999f);
                glRotatef(-4.04f, 1.0, 0.0, 0.0);
                glRotatef(-1.64f, 0.0, 0.0, 1.0);
                if (g_stringInstrument_stringScale[0] > 0.0 && g_stringInstrument_stringScale[0] < 1.0) {
                    glPushMatrix();
                    y = (1.0f - g_stringInstrument_stringScale[0]) * 15.0f;
                    glTranslatef(0.0, y, 0.0);
                    g_violinFinger_ms3d->Render();
                    glPopMatrix();
                }
                ya = 1.0f - g_stringInstrument_stringScale[0];
                glScalef(1.0, ya, 1.0);
                g_violinStringPlayedX_ms3d[g_vibratingString_frame]->Render();
                glPopMatrix();
            }
            if (g_stringInstrument_stringScale[1] < 1.0) {
                glPushMatrix();
                glTranslatef(-0.30000001f, -6.1620002f, 1.8150001f);
                glRotatef(-4.5999999f, 1.0, 0.0, 0.0);
                glRotatef(-0.67199999f, 0.0, 0.0, 1.0);
                if (g_stringInstrument_stringScale[1] > 0.0 && g_stringInstrument_stringScale[1] < 1.0) {
                    glPushMatrix();
                    yb = (1.0f - g_stringInstrument_stringScale[1]) * 15.0f;
                    glTranslatef(0.0, yb, 0.0);
                    g_violinFinger_ms3d->Render();
                    glPopMatrix();
                }
                yc = 1.0f - g_stringInstrument_stringScale[1];
                glScalef(1.0, yc, 1.0);
                g_violinStringPlayedX_ms3d[g_vibratingString_frame]->Render();
                glPopMatrix();
            }
            if (g_stringInstrument_stringScale[2] < 1.0) {
                glPushMatrix();
                glTranslatef(0.30000001f, -6.1620002f, 1.8150001f);
                glRotatef(-4.5999999f, 1.0, 0.0, 0.0);
                glRotatef(0.67199999f, 0.0, 0.0, 1.0);
                if (g_stringInstrument_stringScale[2] > 0.0 && g_stringInstrument_stringScale[2] < 1.0) {
                    glPushMatrix();
                    yd = (1.0f - g_stringInstrument_stringScale[2]) * 15.0f;
                    glTranslatef(0.0, yd, 0.0);
                    g_violinFinger_ms3d->Render();
                    glPopMatrix();
                }
                ye = 1.0f - g_stringInstrument_stringScale[2];
                glScalef(1.0, ye, 1.0);
                g_violinStringPlayedX_ms3d[g_vibratingString_frame]->Render();
                glPopMatrix();
            }
            if (g_stringInstrument_stringScale[3] < 1.0) {
                glPushMatrix();
                glTranslatef(0.80000001f, -6.1620002f, 1.6619999f);
                glRotatef(-4.04f, 1.0, 0.0, 0.0);
                glRotatef(1.64f, 0.0, 0.0, 1.0);
                if (g_stringInstrument_stringScale[3] > 0.0 && g_stringInstrument_stringScale[3] < 1.0) {
                    glPushMatrix();
                    yf = (1.0f - g_stringInstrument_stringScale[3]) * 15.0f;
                    glTranslatef(0.0, yf, 0.0);
                    g_violinFinger_ms3d->Render();
                    glPopMatrix();
                }
                yg = 1.0f - g_stringInstrument_stringScale[3];
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

bool UpdateCello(MUSIC_TIME pmtNow) {
    __int16 jj;
    __int16 v3;
    __int16 v4;
    __int16 v5;
    __int16 v6;
    __int16 i;
    __int16 ii;
    __int16 j;
    __int16 k;
    __int16 m;
    __int16 n;

    for (i = 0; i < g_celloCount; ++i) {
        v3 = 0;
        v6 = 0;
        for (j = 0; j < 88; ++j) {
            if (g_cello[i].durations[j] > 0) {
                v6 = 1;
                g_cello[i].durations[j] -= pmtNow - g_currentGlobalTime;
                if (g_cello[i].durations[j] > 0) {
                    g_cello[i].isActive = 1;
                    g_cello[i].timeToLive = 1000;
                    v3 = 1;
                }
                else {
                    g_cello[i].durations[j] = 0;
                    g_cello[i].frets[g_cello[i].stringAssignment[j] - 1] = 0;
                    g_cello[i].stringAssignment[j] = 0;
                }
            }
        }
        if (v6 == 1) {
            g_cello[i].bowZ = 0.0;
            g_cello[i].bowSpeed = g_cello[i].bowSpeed + 0.0024999999f;
            if (g_cello[i].bowSpeed > 0.0175f)
                g_cello[i].bowSpeed = 0.0175f;
        }
        else {
            g_cello[i].bowZ = g_cello[i].bowZ + 0.0049999999f;
            if (g_cello[i].bowZ > 1.0f)
                g_cello[i].bowZ = 1.0f;
            g_cello[i].bowSpeed = g_cello[i].bowSpeed - 0.00075000001f;
            if (g_cello[i].bowSpeed < 0.0f)
                g_cello[i].bowSpeed = 0.0f;
        }
        if (g_cello[i].bowSpeed > 0.0f) {
            if (g_cello[i].bowDirection) {
                if (g_cello[i].bowDirection == 1) {
                    g_cello[i].bowX = g_cello[i].bowX - g_cello[i].bowSpeed;
                    if (-7.5f > g_cello[i].bowX) {
                        g_cello[i].bowX = -7.5f;
                        g_cello[i].bowDirection = 0;
                    }
                }
            }
            else {
                g_cello[i].bowX = g_cello[i].bowX + g_cello[i].bowSpeed;
                if (g_cello[i].bowX > 7.5f) {
                    g_cello[i].bowX = 7.5f;
                    g_cello[i].bowDirection = 1;
                }
            }
        }
        for (k = 0; k < 4; ++k) {
            if (g_cello[i].frets[k] > 0 && g_cello[i].durations[CELLO_NOTES[g_cello[i].frets[k]][k]] <= 0) {
                g_cello[i].durations[CELLO_NOTES[g_cello[i].frets[k]][k]] = 0;
                g_cello[i].stringAssignment[CELLO_NOTES[g_cello[i].frets[k]][k]] = 0;
                g_cello[i].frets[k] = 0;
            }
        }
        v4 = 0;
        for (m = 0; m < 4; ++m) {
            if (g_cello[i].frets[m] > 0)
                ++v4;
        }
        v5 = 0;
        for (n = 0; n < 88; ++n) {
            for (ii = 0; ii < 16; ++ii) {
                if (g_cello[i].timeDeltas[n][ii] > 0) {
                    g_cello[i].timeDeltas[n][ii] -= pmtNow - g_currentGlobalTime;
                    if (g_cello[i].timeDeltas[n][ii] > 0) {
                        g_cello[i].isActive = 1;
                        g_cello[i].timeToLive = 1000;
                        v3 = 1;
                    }
                    else {
                        g_cello[i].timeDeltas[n][ii] = 0;
                        g_cello[i].durations[n] = g_cello[i].queue[n][ii];
                        g_cello[i].queue[n][ii] = 0;
                        if (!v4)
                            v5 = 1;
                        for (jj = 0; jj < 28; ++jj) {
                            if (CELLO_NOTES[jj + 1][0] == n && !g_cello[i].frets[0]) {
                                g_cello[i].frets[0] = jj + 1;
                                g_cello[i].stringAssignment[n] = 1;
                                break;
                            }
                            if (CELLO_NOTES[jj + 1][1] == n && !g_cello[i].frets[1]) {
                                g_cello[i].frets[1] = jj + 1;
                                g_cello[i].stringAssignment[n] = 2;
                                break;
                            }
                            if (CELLO_NOTES[jj + 1][2] == n && !g_cello[i].frets[2]) {
                                g_cello[i].frets[2] = jj + 1;
                                g_cello[i].stringAssignment[n] = 3;
                                break;
                            }
                            if (CELLO_NOTES[jj + 1][3] == n && !g_cello[i].frets[3]) {
                                g_cello[i].frets[3] = jj + 1;
                                g_cello[i].stringAssignment[n] = 4;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (v5 == 1) {
            if (g_cello[i].bowDirection)
                g_cello[i].bowDirection = 0;
            else
                g_cello[i].bowDirection = 1;
            if (g_cello[i].bowDirection) {
                if (g_cello[i].bowDirection == 1 && -7.5f * 0.75f > g_cello[i].bowX)
                    g_cello[i].bowDirection = 0;
            }
            else if (7.5f * 0.75f < g_cello[i].bowX) {
                g_cello[i].bowDirection = 1;
            }
        }
        if (!v3 && g_cello[i].isActive == 1 && --g_cello[i].timeToLive <= 0) {
            g_cello[i].timeToLive = 0;
            g_cello[i].isActive = 0;
        }
    }
    return 0;
}

