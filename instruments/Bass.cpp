//
// Created by Jacob on 03/20/2026.
//

#include "Bass.h"

#include "macros.h"
#include "render/texture.h"
#include "scene/scene.h"

#include <GL/gl.h>


// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(bass, BassState)

Ms3dBundle* g_bass_ms3d;
Ms3dBundle* g_bassString_ms3d;
Ms3dBundle* g_bassShadow_ms3d;
Ms3dBundle* g_bassNoteFinger_ms3d;
Ms3dBundle* g_bassStringBottomX_ms3d[5];

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern int g_vibratingString_frame;

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------
GLfloat BASS_FRET_HEIGHTS[24] = {
    0.0f, 0.0f, -1.47f, -3.97f, -6.443f, -8.582f, -10.734f, -12.693f, -14.609f, -16.415f, -18.054f,
    -19.623f, -21.151f, -22.498f, -23.845f, -25.137f, -26.29f, -27.457f, -28.498f, -29.512f,
    -30.443f, -31.29f, -32.179f, -32.943f,
};
GLfloat flt_468BF4[24];
__int16 g_bassNotes[24][4];

// ---------------------------------------------------------------------------
// FUNCTION: MIDIJAM 0x40F3C0
// ---------------------------------------------------------------------------
void RenderBass() {
    __int16 v7 = 0;
    const GLboolean v8 = g_bass_ms3d->ApplyMaterial();
    for (__int16 i = 0; i < g_bassCount; ++i) {
        if (g_bass[i].isActive == 1) {
            glPushMatrix();
            const GLfloat z = v7 * -5.0f;
            const GLfloat x = v7 * 5.0f;
            glTranslatef(x, 0.0f, z);
            g_bass_ms3d->Render();
            glPushMatrix();
            glTranslatef(-0.829f, 19.492001f, 0.0f);
            glRotatef(-1.28f, 0.0f, 0.0f, 1.0f);
            if (g_bass[i].frets[0] <= 1 || g_bass[i].frets[0] > 23) {
                if (!g_bass[i].frets[0])
                    g_bassString_ms3d->Render();
            }
            else {
                glPushMatrix();
                glTranslatef(0.0f, BASS_FRET_HEIGHTS[g_bass[i].frets[0]], 0.0f);
                g_bassNoteFinger_ms3d->Render();
                glPopMatrix();
                glScalef(1.0f, flt_468BF4[g_bass[i].frets[0]], 1.0f);
                g_bassString_ms3d->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(-0.30599999f, 19.492001f, 0.0f);
            glRotatef(-0.68000001f, 0.0f, 0.0f, 1.0f);
            if (g_bass[i].frets[1] <= 1 || g_bass[i].frets[1] > 23) {
                if (!g_bass[i].frets[1])
                    g_bassString_ms3d->Render();
            }
            else {
                glPushMatrix();
                glTranslatef(0.0f, BASS_FRET_HEIGHTS[g_bass[i].frets[1]], 0.0f);
                g_bassNoteFinger_ms3d->Render();
                glPopMatrix();
                glScalef(1.0f, flt_468BF4[g_bass[i].frets[1]], 1.0f);
                g_bassString_ms3d->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(0.20299999f, 19.492001f, 0.0f);
            glRotatef(0.17200001f, 0.0f, 0.0f, 1.0f);
            if (g_bass[i].frets[2] <= 1 || g_bass[i].frets[2] > 23) {
                if (!g_bass[i].frets[2])
                    g_bassString_ms3d->Render();
            }
            else {
                glPushMatrix();
                glTranslatef(0.0f, BASS_FRET_HEIGHTS[g_bass[i].frets[2]], 0.0f);
                g_bassNoteFinger_ms3d->Render();
                glPopMatrix();
                glScalef(1.0f, flt_468BF4[g_bass[i].frets[2]], 1.0f);
                g_bassString_ms3d->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(0.71399999f, 19.492001f, 0.0f);
            glRotatef(0.81f, 0.0f, 0.0f, 1.0f);
            if (g_bass[i].frets[3] <= 1 || g_bass[i].frets[3] > 23) {
                if (!g_bass[i].frets[3])
                    g_bassString_ms3d->Render();
            }
            else {
                glPushMatrix();
                glTranslatef(0.0f, BASS_FRET_HEIGHTS[g_bass[i].frets[3]], 0.0f);
                g_bassNoteFinger_ms3d->Render();
                glPopMatrix();
                glScalef(1.0f, flt_468BF4[g_bass[i].frets[3]], 1.0f);
                g_bassString_ms3d->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(-1.87f, -26.6f, 0.0f);
            glRotatef(-1.28f, 0.0f, 0.0f, 1.0f);
            if (g_bass[i].frets[0] <= 1 || g_bass[i].frets[0] > 23) {
                if (g_bass[i].frets[0] == 1)
                    g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            else {
                const GLfloat y = 1.0f - flt_468BF4[g_bass[i].frets[0]];
                glScalef(1.0f, y, 1.0f);
                g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(-0.85500002f, -26.6f, 0.0f);
            glRotatef(-0.68000001f, 0.0f, 0.0f, 1.0f);
            if (g_bass[i].frets[1] <= 1 || g_bass[i].frets[1] > 23) {
                if (g_bass[i].frets[1] == 1)
                    g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            else {
                const GLfloat ya = 1.0f - flt_468BF4[g_bass[i].frets[1]];
                glScalef(1.0f, ya, 1.0f);
                g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(0.34200001f, -26.6f, 0.0f);
            glRotatef(0.17200001f, 0.0f, 0.0f, 1.0f);
            if (g_bass[i].frets[2] <= 1 || g_bass[i].frets[2] > 23) {
                if (g_bass[i].frets[2] == 1)
                    g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            else {
                const GLfloat yb = 1.0f - flt_468BF4[g_bass[i].frets[2]];
                glScalef(1.0f, yb, 1.0f);
                g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(1.365f, -26.6f, 0.0f);
            glRotatef(0.81f, 0.0f, 0.0f, 1.0f);
            if (g_bass[i].frets[3] <= 1 || g_bass[i].frets[3] > 23) {
                if (g_bass[i].frets[3] == 1)
                    g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            else {
                const GLfloat yc = 1.0f - flt_468BF4[g_bass[i].frets[3]];
                glScalef(1.0f, yc, 1.0f);
                g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            glPopMatrix();
            glPopMatrix();
            ++v7;
        }
    }
    SetTextureState(v8);
    g_bassVisible = 0;
    if (v7 > 0)
        g_bassVisible = v7;
}

// ---------------------------------------------------------------------------
// FUNCTION: MIDIJAM 0x428110
// ---------------------------------------------------------------------------
bool UpdateBass(const MUSIC_TIME pmtNow) {
    __int16 v9 = 0;
    for (__int16 i = 0; i < g_bassCount; ++i) {
        __int16 v3 = 0;
        for (__int16 noteA = 0; noteA < 88; ++noteA) {
            if (g_bass[i].field_4[noteA] > 0) {
                g_bass[i].field_4[noteA] -= pmtNow - g_currentGlobalTime;
                if (g_bass[i].field_4[noteA] > 0) {
                    g_bass[i].isActive = 1;
                    g_bass[i].timeToLive = 1000;
                    v3 = 1;
                }
                else {
                    g_bass[i].field_4[noteA] = 0;
                    g_bass[i].frets[g_bass[i].field_164[noteA] + -1] = 0;
                    g_bass[i].field_164[noteA] = 0;
                }
            }
        }
        for (__int16 string = 0; string < 4; ++string) {
            if (g_bass[i].frets[string] > 0
                && g_bass[i].field_4[g_bassNotes[g_bass[i].frets[string] - 1][string]] <= 0) {
                g_bass[i].field_4[g_bassNotes[g_bass[i].frets[string] - 1][string]] = 0;
                g_bass[i].field_164[g_bassNotes[g_bass[i].frets[string] - 1][string]] = 0;
                g_bass[i].frets[string] = 0;
            }
        }
        for (__int16 noteB = 0; noteB < 88; ++noteB) {
            for (__int16 slot = 0; slot < 16; ++slot) {
                if (g_bass[i].field_18CC[noteB][slot] > 0) {
                    v9 = 1;
                    g_bass[i].field_18CC[noteB][slot] -= pmtNow - g_currentGlobalTime;
                    if (g_bass[i].field_18CC[noteB][slot] > 0) {
                        g_bass[i].isActive = 1;
                        g_bass[i].timeToLive = 1000;
                        v3 = 1;
                    }
                    else {
                        g_bass[i].field_18CC[noteB][slot] = 0;
                        g_bass[i].field_4[noteB] = g_bass[i].queue[noteB][slot];
                        g_bass[i].queue[noteB][slot] = 0;
                        for (__int16 fret = 0; fret < 23; ++fret) {
                            if (g_bassNotes[fret][0] == noteB && !g_bass[i].frets[0]) {
                                g_bass[i].frets[0] = fret + 1;
                                g_bass[i].field_164[noteB] = 1;
                                break;
                            }
                            if (g_bassNotes[fret][1] == noteB && !g_bass[i].frets[1]) {
                                g_bass[i].frets[1] = fret + 1;
                                g_bass[i].field_164[noteB] = 2;
                                break;
                            }
                            if (g_bassNotes[fret][2] == noteB && !g_bass[i].frets[2]) {
                                g_bass[i].frets[2] = fret + 1;
                                g_bass[i].field_164[noteB] = 3;
                                break;
                            }
                            if (g_bassNotes[fret][3] == noteB && !g_bass[i].frets[3]) {
                                g_bass[i].frets[3] = fret + 1;
                                g_bass[i].field_164[noteB] = 4;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (!v3 && g_bass[i].isActive == 1 && --g_bass[i].timeToLive <= 0) {
            g_bass[i].timeToLive = 0;
            g_bass[i].isActive = 0;
        }
    }
    return v9 == 1;
}
