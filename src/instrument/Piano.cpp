//
// Created by Jacob on 11/18/2025.
//

#include "Piano.h"

#include <iostream>

#include "../globals.h"
#include "../Ms3dBundle.h"
#include "../textures.h"

__int16 g_inst_visible_piano = 0;
__int16 g_ialloc_piano = 0;
__int16 PIANO_KEY_SHAPE[88] = {
    1, 12, 3,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    8,
};
I_DS_Piano *g_ds_piano = nullptr;
GLfloat g_pianokey_whiteBack_scale[14];
__int16 g_piano_assignment[300];

void I_Piano() {
    GLfloat x; // [esp+0h] [ebp-24h]
    GLfloat y; // [esp+4h] [ebp-20h]
    GLfloat z; // [esp+8h] [ebp-1Ch]
    GLboolean v3; // [esp+17h] [ebp-Dh]
    __int16 i; // [esp+18h] [ebp-Ch]
    __int16 key; // [esp+1Ch] [ebp-8h]
    __int16 v6; // [esp+20h] [ebp-4h]

    v6 = 0;
    for (i = 0; i < g_ialloc_piano; ++i) {
        if (g_ds_piano[i].isActive == 1) {
            v3 = g_pianoModels_ms3d_arr[g_ds_piano[i].materialIndex]->pianoCase->ApplyMaterial();
            glPushMatrix();
            z = v6 * -5.75;
            y = v6 * 3.0;
            glTranslatef(0.0, y, z);
            g_pianoModels_ms3d_arr[g_ds_piano[i].materialIndex]->pianoCase->Render();
            x = -g_pianoXylophoneOffset;
            glTranslatef(x, 0.0, 0.0);
            for (key = 0; key < 88; ++key) {
                glPushMatrix();
                glRotatef(g_ds_piano[i].keyAngles[key], 1.0, 0.0, 0.0);
                if (PIANO_KEY_SHAPE[key] < 1 || PIANO_KEY_SHAPE[key] > 7) {
                    if (PIANO_KEY_SHAPE[key] == 8) {
                        // Full-width key (only the last key)
                        if (g_ds_piano[i].keyAngles[key] <= 0.0) {
                            g_pianoModels_ms3d_arr[g_ds_piano[i].materialIndex]->pianoKeyWhiteFront->Render();
                            g_pianoModels_ms3d_arr[g_ds_piano[i].materialIndex]->pianoKeyWhiteBack->Render();
                        } else {
                            g_pianoModels_ms3d_arr[g_ds_piano[i].materialIndex]->pianoKeyWhiteFrontDown->Render();
                            g_pianoModels_ms3d_arr[g_ds_piano[i].materialIndex]->pianoKeyWhiteBackDown->Render();
                        }
                    } else if (PIANO_KEY_SHAPE[key] >= 9 && PIANO_KEY_SHAPE[key] <= 13) {
                        // Black key
                        glTranslatef(g_pianokey_translation_x[PIANO_KEY_SHAPE[key]], 0.0, 0.0);
                        if (g_ds_piano[i].keyAngles[key] <= 0.0)
                            g_pianoModels_ms3d_arr[g_ds_piano[i].materialIndex]->pianoKeyBlack->Render();
                        else
                            g_pianoModels_ms3d_arr[g_ds_piano[i].materialIndex]->pianoKeyBlackDown->Render();
                    }
                } else {
                    // White key
                    if (g_ds_piano[i].keyAngles[key] <= 0.0)
                        g_pianoModels_ms3d_arr[g_ds_piano[i].materialIndex]->pianoKeyWhiteFront->Render();
                    else
                        g_pianoModels_ms3d_arr[g_ds_piano[i].materialIndex]->pianoKeyWhiteFrontDown->Render();
                    glTranslatef(g_pianokey_translation_x[PIANO_KEY_SHAPE[key]], 0.0, 0.0);
                    glScalef(g_pianokey_whiteBack_scale[PIANO_KEY_SHAPE[key]], 1.0, 1.0);
                    if (g_ds_piano[i].keyAngles[key] <= 0.0)
                        g_pianoModels_ms3d_arr[g_ds_piano[i].materialIndex]->pianoKeyWhiteBack->Render();
                    else
                        g_pianoModels_ms3d_arr[g_ds_piano[i].materialIndex]->pianoKeyWhiteBackDown->Render();
                }
                glPopMatrix();
                glTranslatef(KEY_WIDTHS[PIANO_KEY_SHAPE[key]], 0.0, 0.0);
            }
            glPopMatrix();
            ++v6;
            SetTextureState(v3);
        }
    }
    g_inst_visible_piano = 0;
    if (v6 > 0) {
        g_inst_visible_piano = v6;
        g_pianoStand_ms3d->RenderModel();
    }
}

bool I_Piano_MM(MUSIC_TIME mtNow) {
    __int16 bAnyKeyActive; // [esp+8h] [ebp-14h]
    __int16 piano; // [esp+Ch] [ebp-10h]
    __int16 slot; // [esp+10h] [ebp-Ch]
    __int16 key; // [esp+14h] [ebp-8h]
    __int16 bAnyPianoActive; // [esp+18h] [ebp-4h]

    bAnyPianoActive = 0;
    for (piano = 0; piano < g_ialloc_piano; ++piano) {
        bAnyKeyActive = 0;
        for (key = 0; key < 88; ++key) {
            for (slot = 0; slot < 16; ++slot) {
                if (g_ds_piano[piano].timeDeltas[key][slot] > 0) {
                    bAnyPianoActive = 1;
                    g_ds_piano[piano].timeDeltas[key][slot] -= mtNow - g_time_global_current;
                    if (g_ds_piano[piano].timeDeltas[key][slot] <= 0) {
                        g_ds_piano[piano].timeDeltas[key][slot] = 0;
                        g_ds_piano[piano].keyAngles[key] =
                                (double) g_ds_piano[piano].velocities[key][slot] * 8.0 / 128.0 + 1.75;
                        g_ds_piano[piano].durations[key] = g_ds_piano[piano].queueDurations[key][slot];
                        g_ds_piano[piano].queueDurations[key][slot] = 0;
                    }
                    g_ds_piano[piano].isActive = 1;
                    g_ds_piano[piano].timeToLive = 1000;
                    bAnyKeyActive = 1;
                }
            }
            if (g_ds_piano[piano].durations[key] > 0) {
                g_ds_piano[piano].durations[key] -= mtNow - g_time_global_current;
                if (g_ds_piano[piano].durations[key] <= 0)
                    g_ds_piano[piano].durations[key] = 0;
                g_ds_piano[piano].isActive = 1;
                g_ds_piano[piano].timeToLive = 1000;
                bAnyKeyActive = 1;
            }
            if (g_ds_piano[piano].keyAngles[key] > 0.0 && !g_ds_piano[piano].durations[key]) {
                bAnyPianoActive = 1;
                g_ds_piano[piano].keyAngles[key] = g_ds_piano[piano].keyAngles[key] - 0.5;
                if (g_ds_piano[piano].keyAngles[key] > 0.0) {
                    g_ds_piano[piano].isActive = 1;
                    g_ds_piano[piano].timeToLive = 1000;
                    bAnyKeyActive = 1;
                } else {
                    g_ds_piano[piano].keyAngles[key] = 0.0;
                }
            }
        }
        if (!bAnyKeyActive && g_ds_piano[piano].isActive == 1 && --g_ds_piano[piano].timeToLive <= 0) {
            g_ds_piano[piano].timeToLive = 0;
            g_ds_piano[piano].isActive = 0;
        }
    }
    return bAnyPianoActive == 1;
}
