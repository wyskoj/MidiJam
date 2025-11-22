//
// Created by Jacob on 11/21/2025.
//

#include "windows.h"
#include "GL/gl.h"
#include "Bass.h"
#include "../globals.h"
#include "../Ms3dBundle.h"
#include "../textures.h"

DEF_INST(bass, I_DS_Bass)
__int16 g_bass_assignment[300];

GLfloat BASS_FRET_HEIGHTS[24] = {
    0.0,
    0.0,
    -1.47,
    -3.97,
    -6.4429998,
    -8.5819998,
    -10.734,
    -12.693,
    -14.609,
    -16.415001,
    -18.054001,
    -19.622999,
    -21.150999,
    -22.497999,
    -23.844999,
    -25.136999,
    -26.290001,
    -27.457001,
    -28.497999,
    -29.511999,
    -30.443001,
    -31.290001,
    -32.179001,
    -32.943001,
};
GLfloat flt_468BF4[24];
__int16 BASS_NOTES[24][4];

void I_Bass() {
    GLfloat x; // [esp+0h] [ebp-20h]
    GLfloat y; // [esp+4h] [ebp-1Ch]
    GLfloat ya; // [esp+4h] [ebp-1Ch]
    GLfloat yb; // [esp+4h] [ebp-1Ch]
    GLfloat yc; // [esp+4h] [ebp-1Ch]
    GLfloat z; // [esp+8h] [ebp-18h]
    __int16 i; // [esp+14h] [ebp-Ch]
    __int16 v7; // [esp+18h] [ebp-8h]
    GLboolean v8; // [esp+1Fh] [ebp-1h]

    v7 = 0;
    v8 = g_bass_ms3d->ApplyMaterial();
    for (i = 0; i < g_ialloc_bass; ++i) {
        if (g_ds_bass[i].isActive == 1) {
            glPushMatrix();
            z = v7 * -5.0;
            x = v7 * 5.0;
            glTranslatef(x, 0.0, z);
            g_bass_ms3d->Render();
            glPushMatrix();
            glTranslatef(-0.829, 19.492001, 0.0);
            glRotatef(-1.28, 0.0, 0.0, 1.0);
            if (g_ds_bass[i].frets[0] <= 1 || g_ds_bass[i].frets[0] > 23) {
                if (!g_ds_bass[i].frets[0])
                    g_bassString_ms3d->Render();
            } else {
                glPushMatrix();
                glTranslatef(0.0, BASS_FRET_HEIGHTS[g_ds_bass[i].frets[0]], 0.0);
                g_bassNoteFinger_ms3d->Render();
                glPopMatrix();
                glScalef(1.0, flt_468BF4[g_ds_bass[i].frets[0]], 1.0);
                g_bassString_ms3d->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(-0.30599999, 19.492001, 0.0);
            glRotatef(-0.68000001, 0.0, 0.0, 1.0);
            if (g_ds_bass[i].frets[1] <= 1 || g_ds_bass[i].frets[1] > 23) {
                if (!g_ds_bass[i].frets[1])
                    g_bassString_ms3d->Render();
            } else {
                glPushMatrix();
                glTranslatef(0.0, BASS_FRET_HEIGHTS[g_ds_bass[i].frets[1]], 0.0);
                g_bassNoteFinger_ms3d->Render();
                glPopMatrix();
                glScalef(1.0, flt_468BF4[g_ds_bass[i].frets[1]], 1.0);
                g_bassString_ms3d->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(0.20299999, 19.492001, 0.0);
            glRotatef(0.17200001, 0.0, 0.0, 1.0);
            if (g_ds_bass[i].frets[2] <= 1 || g_ds_bass[i].frets[2] > 23) {
                if (!g_ds_bass[i].frets[2])
                    g_bassString_ms3d->Render();
            } else {
                glPushMatrix();
                glTranslatef(0.0, BASS_FRET_HEIGHTS[g_ds_bass[i].frets[2]], 0.0);
                g_bassNoteFinger_ms3d->Render();
                glPopMatrix();
                glScalef(1.0, flt_468BF4[g_ds_bass[i].frets[2]], 1.0);
                g_bassString_ms3d->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(0.71399999, 19.492001, 0.0);
            glRotatef(0.81, 0.0, 0.0, 1.0);
            if (g_ds_bass[i].frets[3] <= 1 || g_ds_bass[i].frets[3] > 23) {
                if (!g_ds_bass[i].frets[3])
                    g_bassString_ms3d->Render();
            } else {
                glPushMatrix();
                glTranslatef(0.0, BASS_FRET_HEIGHTS[g_ds_bass[i].frets[3]], 0.0);
                g_bassNoteFinger_ms3d->Render();
                glPopMatrix();
                glScalef(1.0, flt_468BF4[g_ds_bass[i].frets[3]], 1.0);
                g_bassString_ms3d->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(-1.87, -26.6, 0.0);
            glRotatef(-1.28, 0.0, 0.0, 1.0);
            if (g_ds_bass[i].frets[0] <= 1 || g_ds_bass[i].frets[0] > 23) {
                if (g_ds_bass[i].frets[0] == 1)
                    g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            } else {
                y = 1.0 - flt_468BF4[g_ds_bass[i].frets[0]];
                glScalef(1.0, y, 1.0);
                g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(-0.85500002, -26.6, 0.0);
            glRotatef(-0.68000001, 0.0, 0.0, 1.0);
            if (g_ds_bass[i].frets[1] <= 1 || g_ds_bass[i].frets[1] > 23) {
                if (g_ds_bass[i].frets[1] == 1)
                    g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            } else {
                ya = 1.0 - flt_468BF4[g_ds_bass[i].frets[1]];
                glScalef(1.0, ya, 1.0);
                g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(0.34200001, -26.6, 0.0);
            glRotatef(0.17200001, 0.0, 0.0, 1.0);
            if (g_ds_bass[i].frets[2] <= 1 || g_ds_bass[i].frets[2] > 23) {
                if (g_ds_bass[i].frets[2] == 1)
                    g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            } else {
                yb = 1.0 - flt_468BF4[g_ds_bass[i].frets[2]];
                glScalef(1.0, yb, 1.0);
                g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(1.365, -26.6, 0.0);
            glRotatef(0.81, 0.0, 0.0, 1.0);
            if (g_ds_bass[i].frets[3] <= 1 || g_ds_bass[i].frets[3] > 23) {
                if (g_ds_bass[i].frets[3] == 1)
                    g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            } else {
                yc = 1.0 - flt_468BF4[g_ds_bass[i].frets[3]];
                glScalef(1.0, yc, 1.0);
                g_bassStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            glPopMatrix();
            glPopMatrix();
            ++v7;
        }
    }
    SetTextureState(v8);
    g_inst_visible_bass = 0;
    if (v7 > 0)
        g_inst_visible_bass = v7;
}

bool I_Bass_MM(int mtNow) {
    __int16 fret; // [esp+4h] [ebp-18h]
    __int16 v3; // [esp+8h] [ebp-14h]
    __int16 i; // [esp+Ch] [ebp-10h]
    __int16 slot; // [esp+10h] [ebp-Ch]
    __int16 noteA; // [esp+14h] [ebp-8h]
    __int16 string; // [esp+14h] [ebp-8h]
    __int16 noteB; // [esp+14h] [ebp-8h]
    __int16 v9; // [esp+18h] [ebp-4h]

    v9 = 0;
    for (i = 0; i < g_ialloc_bass; ++i) {
        v3 = 0;
        for (noteA = 0; noteA < 88; ++noteA) {
            if (g_ds_bass[i].field_4[noteA] > 0) {
                g_ds_bass[i].field_4[noteA] -= mtNow - g_time_global_current;
                if (g_ds_bass[i].field_4[noteA] > 0) {
                    g_ds_bass[i].isActive = 1;
                    g_ds_bass[i].timeToLive = 1000;
                    v3 = 1;
                } else {
                    g_ds_bass[i].field_4[noteA] = 0;
                    g_ds_bass[i].frets[g_ds_bass[i].field_164[noteA] + -1] = 0;
                    g_ds_bass[i].field_164[noteA] = 0;
                }
            }
        }
        for ( string = 0; string < 4; ++string )
        {
            if ( g_ds_bass[i].frets[string] > 0
              && g_ds_bass[i].field_4[BASS_NOTES[g_ds_bass[i].frets[string] - 1][string]] <= 0 )
            {
                g_ds_bass[i].field_4[BASS_NOTES[g_ds_bass[i].frets[string] - 1][string]] = 0;
                g_ds_bass[i].field_164[BASS_NOTES[g_ds_bass[i].frets[string] - 1][string]] = 0;
                g_ds_bass[i].frets[string] = 0;
            }
        }
        for (noteB = 0; noteB < 88; ++noteB) {
            for (slot = 0; slot < 16; ++slot) {
                if (g_ds_bass[i].field_18CC[noteB][slot] > 0) {
                    v9 = 1;
                    g_ds_bass[i].field_18CC[noteB][slot] -= mtNow - g_time_global_current;
                    if (g_ds_bass[i].field_18CC[noteB][slot] > 0) {
                        g_ds_bass[i].isActive = 1;
                        g_ds_bass[i].timeToLive = 1000;
                        v3 = 1;
                    } else {
                        g_ds_bass[i].field_18CC[noteB][slot] = 0;
                        g_ds_bass[i].field_4[noteB] = g_ds_bass[i].field_2CC[noteB][slot];
                        g_ds_bass[i].field_2CC[noteB][slot] = 0;
                        for (fret = 0; fret < 23; ++fret) {
                            if (BASS_NOTES[fret][0] == noteB && !g_ds_bass[i].frets[0]) {
                                g_ds_bass[i].frets[0] = fret + 1;
                                g_ds_bass[i].field_164[noteB] = 1;
                                break;
                            }
                            if (BASS_NOTES[fret][1] == noteB && !g_ds_bass[i].frets[1]) {
                                g_ds_bass[i].frets[1] = fret + 1;
                                g_ds_bass[i].field_164[noteB] = 2;
                                break;
                            }
                            if (BASS_NOTES[fret][2] == noteB && !g_ds_bass[i].frets[2]) {
                                g_ds_bass[i].frets[2] = fret + 1;
                                g_ds_bass[i].field_164[noteB] = 3;
                                break;
                            }
                            if (BASS_NOTES[fret][3] == noteB && !g_ds_bass[i].frets[3]) {
                                g_ds_bass[i].frets[3] = fret + 1;
                                g_ds_bass[i].field_164[noteB] = 4;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (!v3 && g_ds_bass[i].isActive == 1 && --g_ds_bass[i].timeToLive <= 0) {
            g_ds_bass[i].timeToLive = 0;
            g_ds_bass[i].isActive = 0;
        }
    }
    return v9 == 1;
}
