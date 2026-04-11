//
// Created by Jacob on Saturday, April 11, 2026.
//


#include "PizzicatoStrings.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

#include "StageString.h"
#include "strings.h"
#include "scene/scene.h"

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(pizzicatoStrings, PizzicatoStringsState)

Ms3dBundle* g_pizzicatoStringHolder_ms3d = nullptr;

__int16 word_45E980[24] = {
    2, 1, 0, 3, 4, 3, 0, 1, 2, 1, 0, 3, 4, 3, 0, 1, 0, 3, 0, 1, 0, 3, 0, 1
};

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;

// FUNCTION: MIDIJAM 0x40BEF0
void RenderPizzicatoStrings() {
    GLfloat angle; // [esp+0h] [ebp-34h]
    GLfloat y; // [esp+8h] [ebp-2Ch]
    GLfloat ya; // [esp+8h] [ebp-2Ch]
    GLfloat yb; // [esp+8h] [ebp-2Ch]
    GLfloat yc; // [esp+8h] [ebp-2Ch]
    GLfloat z; // [esp+Ch] [ebp-28h]
    GLfloat za; // [esp+Ch] [ebp-28h]
    __int16 i; // [esp+28h] [ebp-Ch]
    __int16 v8; // [esp+2Ch] [ebp-8h]
    __int16 j; // [esp+30h] [ebp-4h]

    v8 = 0;
    glPushMatrix();
    glTranslatef(0.0, 8.0, -14.0);
    for (i = 0; i < g_pizzicatoStringsCount; ++i) {
        if (g_pizzicatoStrings[i].isActive == 1) {
            for (j = 0; j < 12; ++j) {
                glPushMatrix();
                angle = (double)v8 * flt_46E3AC + flt_46E3C0 - (double)j * flt_46E398;
                glRotatef(angle, 0.0, 1.0, 0.0);
                if (g_pizzicatoStrings[i].field_364[j] <= 0) {
                    za = STAGE_BASE_Z + 7.0;
                    yb = (double)j * 1.0 + -38.0;
                    glTranslatef(0.0, yb, za);
                    yc = ((double)(12 - j) / 12.0 * 24.0 + 16.0) / 72.0;
                    glScalef(1.0, yc, 1.0);
                    g_pizzicatoStringHolder_ms3d->RenderModel();
                    g_stageString_ms3d->RenderModel();
                }
                else {
                    z = STAGE_BASE_Z + 9.0;
                    y = (double)j * 1.0 + -38.0;
                    glTranslatef(0.0, y, z);
                    ya = ((double)(12 - j) / 12.0 * 24.0 + 16.0) / 72.0;
                    glScalef(1.0, ya, 1.0);
                    g_pizzicatoStringHolder_ms3d->RenderModel();
                    g_stageStringBottomX_ms3d[word_45E980[24 - g_pizzicatoStrings[i].field_364[j]]]->RenderModel();
                }
                glPopMatrix();
            }
            ++v8;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x437080
bool UpdatePizzicatoStrings(MUSIC_TIME pmtNow) {
    __int16 v2; // [esp+4h] [ebp-14h]
    __int16 i; // [esp+8h] [ebp-10h]
    __int16 k; // [esp+Ch] [ebp-Ch]
    __int16 j; // [esp+10h] [ebp-8h]
    __int16 v6; // [esp+14h] [ebp-4h]

    v6 = 0;
    for (i = 0; i < g_pizzicatoStringsCount; ++i) {
        v2 = 0;
        for (j = 0; j < 12; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_pizzicatoStrings[i].field_37C[j][k] > 0) {
                    v6 = 1;
                    g_pizzicatoStrings[i].field_37C[j][k] -= pmtNow - g_currentGlobalTime;
                    if (g_pizzicatoStrings[i].field_37C[j][k] <= 0) {
                        g_pizzicatoStrings[i].field_37C[j][k] = 0;
                        g_pizzicatoStrings[i].field_4[j] = g_pizzicatoStrings[i].field_64[0][16 * j + k];
                        g_pizzicatoStrings[i].field_34[j] = g_pizzicatoStrings[i].field_64[0][16 * j + k];
                        g_pizzicatoStrings[i].field_64[0][16 * j + k] = 0;
                        g_pizzicatoStrings[i].field_364[j] = 24;
                    }
                    g_pizzicatoStrings[i].isActive = 1;
                    g_pizzicatoStrings[i].timeToLive = 1000;
                    v2 = 1;
                }
            }
            if (g_pizzicatoStrings[i].field_4[j] > 0) {
                g_pizzicatoStrings[i].field_4[j] -= pmtNow - g_currentGlobalTime;
                if (g_pizzicatoStrings[i].field_4[j] > 0) {
                    g_pizzicatoStrings[i].isActive = 1;
                    g_pizzicatoStrings[i].timeToLive = 1000;
                    v2 = 1;
                    v6 = 1;
                }
                else {
                    g_pizzicatoStrings[i].field_4[j] = 0;
                }
            }
            if (g_pizzicatoStrings[i].field_364[j] > 0 && --g_pizzicatoStrings[i].field_364[j] <= 0)
                g_pizzicatoStrings[i].field_364[j] = 0;
        }
        if (!v2 && g_pizzicatoStrings[i].isActive == 1 && --g_pizzicatoStrings[i].timeToLive <= 0) {
            g_pizzicatoStrings[i].timeToLive = 0;
            g_pizzicatoStrings[i].isActive = 0;
        }
    }
    return v6 == 1;
}
