//
// Created by Jacob on 11/20/2025.
//

#include <windows.h>
#include <GL/gl.h>
#include "Accordion.h"
#include "../globals.h"
#include "../Ms3dBundle.h"

I_DS_Accordion *g_ds_accordion;
__int16 g_inst_visible_accordion;
__int16 g_ialloc_accordion;
__int16 g_accordion_assignment[300];

__int16 ACCORDION_KEY_SHAPE[26] = {
    8,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    8,
};

void I_Accordion() {
    GLfloat angle; // [esp+0h] [ebp-20h]
    GLfloat y; // [esp+8h] [ebp-18h]
    GLfloat ya; // [esp+8h] [ebp-18h]
    GLfloat yb; // [esp+8h] [ebp-18h]
    GLfloat yc; // [esp+8h] [ebp-18h]
    GLfloat yd; // [esp+8h] [ebp-18h]
    __int16 i; // [esp+14h] [ebp-Ch]
    __int16 index; // [esp+18h] [ebp-8h]
    __int16 j; // [esp+1Ch] [ebp-4h]
    __int16 key; // [esp+1Ch] [ebp-4h]

    index = 0;
    glPushMatrix();
    glTranslatef(-75.0, -24.0, -75.0);
    glRotatef(45.0, 0.0, 1.0, 0.0);
    for (i = 0; i < g_ialloc_accordion; ++i) {
        if (g_ds_accordion[i].isActive == 1) {
            glPushMatrix();
            y = index * 24.0;
            glTranslatef(0.0, y, 0.0);
            glRotatef(7.5, 1.0, 0.0, 0.0);
            angle = g_ds_accordion[i].squeezeAngle * -9.5;
            glRotatef(angle, 0.0, 0.0, 1.0);
            g_accordionLeftHand_ms3d->RenderModel();
            g_accordionFold_ms3d->RenderModel();
            for (j = 0; j < 12; ++j) {
                glRotatef(g_ds_accordion[i].squeezeAngle, 0.0, 0.0, 1.0);
                g_accordionFold_ms3d->RenderModel();
            }
            glRotatef(g_ds_accordion[i].squeezeAngle, 0.0, 0.0, 1.0);
            g_accordionFold_ms3d->RenderModel();
            g_accordionRightHand_ms3d->RenderModel();
            glTranslatef(-3.5999999, 29.0, -0.75);
            for (key = 0; key < 26; ++key) {
                glPushMatrix();
                glRotatef(g_ds_accordion[i].keyAngles[key], 0.0, -1.0, 0.0);
                if (ACCORDION_KEY_SHAPE[key] < 1 || ACCORDION_KEY_SHAPE[key] > 7) {
                    if (ACCORDION_KEY_SHAPE[key] == 8) {
                        // Full-width key
                        if (g_ds_accordion[i].keyAngles[key] <= 0.0) {
                            g_accordionKeyWhiteFront_ms3d->RenderModel();
                            g_accordionKeyWhiteBack_ms3d->RenderModel();
                        } else {
                            g_accordionKeyWhiteFront_Down_ms3d->RenderModel();
                            g_accordionKeyWhiteBack_Down_ms3d->RenderModel();
                        }
                    } else if (ACCORDION_KEY_SHAPE[key] >= 9 && ACCORDION_KEY_SHAPE[key] <= 13) {
                        yc = -g_pianokey_translation_x[ACCORDION_KEY_SHAPE[key]];
                        glTranslatef(0.0, yc, 0.0);
                        if (g_ds_accordion[i].keyAngles[key] <= 0.0)
                            g_accordionKeyBlack_ms3d->RenderModel();
                        else
                            g_accordionKeyBlack_Down_ms3d->RenderModel();
                    }
                } else {
                    if (g_ds_accordion[i].keyAngles[key] <= 0.0)
                        g_accordionKeyWhiteFront_ms3d->RenderModel();
                    else
                        g_accordionKeyWhiteFront_Down_ms3d->RenderModel();
                    ya = -g_pianokey_translation_x[ACCORDION_KEY_SHAPE[key]];
                    glTranslatef(0.0, ya, 0.0);
                    yb = g_pianokey_whiteBack_scale[PIANO_KEY_SHAPE[key]] * 0.89999998;
                    glScalef(1.0, yb, 1.0);
                    if (g_ds_accordion[i].keyAngles[key] <= 0.0)
                        g_accordionKeyWhiteBack_ms3d->RenderModel();
                    else
                        g_accordionKeyWhiteBack_Down_ms3d->RenderModel();
                }
                glPopMatrix();
                yd = -KEY_WIDTHS[ACCORDION_KEY_SHAPE[key]];
                glTranslatef(0.0, yd, 0.0);
            }
            glPopMatrix();
            ++index;
        }
    }
    glPopMatrix();
}

bool I_Accordion_MM(MUSIC_TIME mtNow) {
    __int16 v2; // [esp+Ch] [ebp-18h]
    __int16 v3; // [esp+10h] [ebp-14h]
    __int16 i; // [esp+14h] [ebp-10h]
    __int16 k; // [esp+18h] [ebp-Ch]
    __int16 j; // [esp+1Ch] [ebp-8h]
    __int16 v7; // [esp+20h] [ebp-4h]

    v7 = 0;
    for (i = 0; i < g_ialloc_accordion; ++i) {
        v2 = 0;
        v3 = 0;
        for (j = 0; j < 26; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_ds_accordion[i].timeDeltas[j][k] > 0) {
                    v7 = 1;
                    g_ds_accordion[i].timeDeltas[j][k] -= mtNow - g_time_global_current;
                    if (g_ds_accordion[i].timeDeltas[j][k] <= 0) {
                        g_ds_accordion[i].timeDeltas[j][k] = 0;
                        g_ds_accordion[i].keyAngles[j] = 5.0;
                        g_ds_accordion[i].field_4[j] = g_ds_accordion[i].queueDurations[j][k];
                        g_ds_accordion[i].queueDurations[j][k] = 0;
                    }
                    g_ds_accordion[i].isActive = 1;
                    g_ds_accordion[i].timeToLive = 1000;
                    v2 = 1;
                }
            }
            if (g_ds_accordion[i].field_4[j] > 0) {
                g_ds_accordion[i].field_4[j] -= mtNow - g_time_global_current;
                if (g_ds_accordion[i].field_4[j] <= 0)
                    g_ds_accordion[i].field_4[j] = 0;
                g_ds_accordion[i].isActive = 1;
                g_ds_accordion[i].timeToLive = 1000;
                v2 = 1;
                if (v3 < 3)
                    v3 = 3;
            }
            if (g_ds_accordion[i].keyAngles[j] > 0.0 && !g_ds_accordion[i].field_4[j]) {
                v7 = 1;
                g_ds_accordion[i].keyAngles[j] = g_ds_accordion[i].keyAngles[j] - 0.25;
                if (g_ds_accordion[i].keyAngles[j] > 0.0) {
                    g_ds_accordion[i].isActive = 1;
                    g_ds_accordion[i].timeToLive = 1000;
                    v2 = 1;
                } else {
                    g_ds_accordion[i].keyAngles[j] = 0.0;
                }
                if (v3 < 2)
                    v3 = 2;
            }
        }
        if (v3 <= 0) {
            g_ds_accordion[i].field_B02 = g_ds_accordion[i].field_B02 - 0.0099999998;
            if (g_ds_accordion[i].field_B02 < 0.0)
                g_ds_accordion[i].field_B02 = 0.0;
        } else {
            g_ds_accordion[i].field_B02 = g_ds_accordion[i].field_B02 + 0.1;
            if (g_ds_accordion[i].field_B02 > 1.0)
                g_ds_accordion[i].field_B02 = 1.0;
        }
        if (g_ds_accordion[i].field_B02 > 0.0) {
            if (g_ds_accordion[i].direction) {
                if (g_ds_accordion[i].direction == EXPANDING) {
                    g_ds_accordion[i].squeezeAngle =
                            g_ds_accordion[i].field_B02 * 0.001 * (mtNow - g_time_global_current)
                            + g_ds_accordion[i].squeezeAngle;
                    if (g_ds_accordion[i].squeezeAngle >= 4.0) {
                        g_ds_accordion[i].squeezeAngle = 4.0;
                        g_ds_accordion[i].direction = CONTRACTING;
                    }
                }
            } else {
                g_ds_accordion[i].squeezeAngle = g_ds_accordion[i].squeezeAngle
                                                 - g_ds_accordion[i].field_B02 * 0.001 * (
                                                     mtNow - g_time_global_current);
                if (g_ds_accordion[i].squeezeAngle <= 1.0) {
                    g_ds_accordion[i].squeezeAngle = 1.0;
                    g_ds_accordion[i].direction = EXPANDING;
                }
            }
        }
        if (!v2 && g_ds_accordion[i].isActive == 1 && --g_ds_accordion[i].timeToLive <= 0) {
            g_ds_accordion[i].timeToLive = 0;
            g_ds_accordion[i].isActive = 0;
        }
    }
    return v7 == 1;
}
