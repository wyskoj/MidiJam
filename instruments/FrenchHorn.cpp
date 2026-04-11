//
// Created by Jacob on Saturday, April 11, 2026.
//


#include "FrenchHorn.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(frenchHorn, FrenchHornState)

Ms3dBundle* g_frenchHornBody_ms3d = nullptr;
Ms3dBundle* g_frenchHornHorn_ms3d = nullptr;
Ms3dBundle* g_frenchHornKey1_ms3d = nullptr;
Ms3dBundle* g_frenchHornKey2_ms3d = nullptr;
Ms3dBundle* g_frenchHornKey3_ms3d = nullptr;
Ms3dBundle* g_frenchHornTrigger_ms3d = nullptr;

char FRENCH_HORN_KEY_1_INDEX[36] = {
    1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0
};

char FRENCH_HORN_KEY_2_INDEX[36] = {
    0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1
};

char FRENCH_HORN_KEY_3_INDEX[36] = {
    1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0
};

char FRENCH_HORN_KEY_TRIGGER_INDEX[36] = {
    1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1
};

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern int g_currentTempo_scaleFactor1_15;

// FUNCTION: MIDIJAM 0xXXXXXXXX
void RenderFrenchHorn() {
    GLfloat angle; // [esp+0h] [ebp-30h]
    GLfloat anglea; // [esp+0h] [ebp-30h]
    GLfloat angleb; // [esp+0h] [ebp-30h]
    GLfloat y; // [esp+8h] [ebp-28h]
    GLfloat ya; // [esp+8h] [ebp-28h]
    GLfloat yb; // [esp+8h] [ebp-28h]
    __int16 i; // [esp+20h] [ebp-10h]
    __int16 v7; // [esp+24h] [ebp-Ch]
    __int16 v8; // [esp+28h] [ebp-8h]
    __int16 j; // [esp+2Ch] [ebp-4h]

    v8 = 0;
    glPushMatrix();
    glTranslatef(-80.5, -40.0, -75.5);
    for (i = 0; i < g_frenchHornCount; ++i) {
        if (g_frenchHorn[i].isActive == 1) {
            v7 = 0;
            for (j = 0; j < 36; ++j) {
                if (g_frenchHorn[i].field_4[j] > 0) {
                    glPushMatrix();
                    angle = (double)v7 * 45.0 + 90.0;
                    glRotatef(angle, 0.0, 1.0, 0.0);
                    y = (double)v8 * 25.0 + 50.0;
                    glTranslatef(0.0, y, 18.0);
                    anglea = 20.0 - (double)g_frenchHorn[i].field_4[j] / (double)g_frenchHorn[i].field_94[j] *
                        3.0;
                    glRotatef(anglea, 1.0, 0.0, 0.0);
                    g_frenchHornBody_ms3d->RenderModelShiny();
                    glPushMatrix();
                    glTranslatef(0.0, 0.14, 0.0);
                    if (FRENCH_HORN_KEY_1_INDEX[j] == 1) {
                        glPushMatrix();
                        glRotatef(-45.0, 0.0, 0.0, 1.0);
                    }
                    g_frenchHornKey1_ms3d->RenderModelShiny();
                    if (FRENCH_HORN_KEY_1_INDEX[j] == 1)
                        glPopMatrix();
                    if (FRENCH_HORN_KEY_2_INDEX[j] == 1) {
                        glPushMatrix();
                        glRotatef(-45.0, 0.0, 0.0, 1.0);
                    }
                    g_frenchHornKey2_ms3d->RenderModelShiny();
                    if (FRENCH_HORN_KEY_2_INDEX[j] == 1)
                        glPopMatrix();
                    if (FRENCH_HORN_KEY_3_INDEX[j] == 1) {
                        glPushMatrix();
                        glRotatef(-45.0, 0.0, 0.0, 1.0);
                    }
                    g_frenchHornKey3_ms3d->RenderModelShiny();
                    if (FRENCH_HORN_KEY_3_INDEX[j] == 1)
                        glPopMatrix();
                    glTranslatef(0.0, 0.0, 1.1339999);
                    if (FRENCH_HORN_KEY_TRIGGER_INDEX[j] == 1) {
                        glPushMatrix();
                        glRotatef(32.0, 1.0, 0.0, 0.0);
                    }
                    g_frenchHornTrigger_ms3d->RenderModelShiny();
                    if (FRENCH_HORN_KEY_TRIGGER_INDEX[j] == 1)
                        glPopMatrix();
                    glPopMatrix();
                    glRotatef(22.0, 1.0, 0.0, 0.0);
                    glPushMatrix();
                    glTranslatef(0.0, -5.0, 0.0);
                    ya = (double)g_frenchHorn[i].field_4[j] / (double)g_frenchHorn[i].field_94[j] * 1.0 + 1.0;
                    glScalef(1.0, ya, 1.0);
                    g_frenchHornHorn_ms3d->RenderModelShiny();
                    glPopMatrix();
                    glPopMatrix();
                    ++v7;
                }
            }
            if (!v7) {
                glPushMatrix();
                angleb = (double)0 * 45.0 + 90.0;
                glRotatef(angleb, 0.0, 1.0, 0.0);
                yb = (double)v8 * 25.0 + 50.0;
                glTranslatef(0.0, yb, 18.0);
                glRotatef(20.0, 1.0, 0.0, 0.0);
                g_frenchHornBody_ms3d->RenderModelShiny();
                glPushMatrix();
                glTranslatef(0.0, 0.14, 0.0);
                if (FRENCH_HORN_KEY_1_INDEX[g_frenchHorn[i].field_EA4] == 1) {
                    glPushMatrix();
                    glRotatef(-45.0, 0.0, 0.0, 1.0);
                }
                g_frenchHornKey1_ms3d->RenderModelShiny();
                if (FRENCH_HORN_KEY_1_INDEX[g_frenchHorn[i].field_EA4] == 1)
                    glPopMatrix();
                if (FRENCH_HORN_KEY_2_INDEX[g_frenchHorn[i].field_EA4] == 1) {
                    glPushMatrix();
                    glRotatef(-45.0, 0.0, 0.0, 1.0);
                }
                g_frenchHornKey2_ms3d->RenderModelShiny();
                if (FRENCH_HORN_KEY_2_INDEX[g_frenchHorn[i].field_EA4] == 1)
                    glPopMatrix();
                if (FRENCH_HORN_KEY_3_INDEX[g_frenchHorn[i].field_EA4] == 1) {
                    glPushMatrix();
                    glRotatef(-45.0, 0.0, 0.0, 1.0);
                }
                g_frenchHornKey3_ms3d->RenderModelShiny();
                if (FRENCH_HORN_KEY_3_INDEX[g_frenchHorn[i].field_EA4] == 1)
                    glPopMatrix();
                glTranslatef(0.0, 0.0, 1.1339999);
                if (FRENCH_HORN_KEY_TRIGGER_INDEX[g_frenchHorn[i].field_EA4] == 1) {
                    glPushMatrix();
                    glRotatef(32.0, 1.0, 0.0, 0.0);
                }
                g_frenchHornTrigger_ms3d->RenderModelShiny();
                if (FRENCH_HORN_KEY_TRIGGER_INDEX[g_frenchHorn[i].field_EA4] == 1)
                    glPopMatrix();
                glPopMatrix();
                glRotatef(22.0, 1.0, 0.0, 0.0);
                glPushMatrix();
                glTranslatef(0.0, -5.0, 0.0);
                g_frenchHornHorn_ms3d->RenderModelShiny();
                glPopMatrix();
                glPopMatrix();
            }
            ++v8;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0xXXXXXXXX
bool UpdateFrenchHorn(MUSIC_TIME pmtNow) {
    __int16 v2; // [esp+4h] [ebp-1Ch]
    __int16 v3; // [esp+8h] [ebp-18h]
    __int16 v4; // [esp+Ch] [ebp-14h]
    __int16 i; // [esp+10h] [ebp-10h]
    __int16 k; // [esp+14h] [ebp-Ch]
    __int16 j; // [esp+18h] [ebp-8h]
    __int16 m; // [esp+18h] [ebp-8h]
    __int16 v9; // [esp+1Ch] [ebp-4h]

    v9 = 0;
    for (i = 0; i < g_frenchHornCount; ++i) {
        v2 = 0;
        v4 = 0;
        v3 = 0;
        for (j = 0; j < 36; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_frenchHorn[i].field_A24[j][k] > 0) {
                    v9 = 1;
                    g_frenchHorn[i].field_A24[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_frenchHorn[i].field_A24[j][k] <= 0) {
                        ++v4;
                        g_frenchHorn[i].field_A24[j][k] = 0;
                        g_frenchHorn[i].field_4[j] = g_frenchHorn[i].field_124[j][k];
                        g_frenchHorn[i].field_94[j] = g_frenchHorn[i].field_124[j][k];
                        g_frenchHorn[i].field_124[j][k] = 0;
                    }
                    g_frenchHorn[i].isActive = 1;
                    g_frenchHorn[i].timeToLive = 1000;
                    v2 = 1;
                }
            }
            if (g_frenchHorn[i].field_4[j] > 0) {
                g_frenchHorn[i].field_EA4 = j;
                g_frenchHorn[i].field_4[j] -= pmtNow - g_currentGlobalTime;
                if (g_frenchHorn[i].field_4[j] > 0) {
                    ++v3;
                    g_frenchHorn[i].isActive = 1;
                    g_frenchHorn[i].timeToLive = 1000;
                    v2 = 1;
                    v9 = 1;
                }
                else {
                    g_frenchHorn[i].field_4[j] = 0;
                }
            }
        }
        if (v4 > 0 && v3 > 1) {
            for (m = 0; m < 36; ++m) {
                if (g_frenchHorn[i].field_4[m] > 0 && g_frenchHorn[i].field_4[m] < g_currentTempo_scaleFactor1_15)
                    g_frenchHorn[i].field_4[m] = 0;
            }
        }
        if (!v2 && g_frenchHorn[i].isActive == 1 && --g_frenchHorn[i].timeToLive <= 0) {
            g_frenchHorn[i].timeToLive = 0;
            g_frenchHorn[i].isActive = 0;
        }
    }
    return v9 == 1;
}
