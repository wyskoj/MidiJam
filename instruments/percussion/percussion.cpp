//
// Created by Jacob on 4/12/2026.
//

#include "percussion.h"

extern int g_percussion_time_queue[88][32];
extern float MAX_RECOIL;
extern short g_stick_visible[43];

// FUNCTION: MIDIJAM 0x405CC0
bool IsShowPercussionInstrument(GM_PERCUSSION percussionPatch, float recoil) {
    __int16 i; // [esp+4h] [ebp-18h]
    __int16 time_until_next_hit; // [esp+10h] [ebp-Ch]
    __int16 num_positive_values; // [esp+14h] [ebp-8h]

    time_until_next_hit = 10000;
    num_positive_values = 0;
    for (i = 0; i < 32; ++i) {
        if (g_percussion_time_queue[percussionPatch][i] > 0
            && g_percussion_time_queue[percussionPatch][i] < time_until_next_hit) {
            time_until_next_hit = g_percussion_time_queue[percussionPatch][i];
            ++num_positive_values;
        }
    }
    if (time_until_next_hit > 0 && (double)time_until_next_hit < 200.0)
        return true;
    if (recoil <= 0.0)
        return num_positive_values > 0;
    return true;
}

// FUNCTION: MIDIJAM 0x405F00
bool UpdatePercussionStriker(GM_PERCUSSION percussionPatch, float recoilDistance, float a3,
                             Ms3dBundle* stick_model_ms3d, bool isShiny, GLfloat a6) {
    GLfloat angle; // [esp+0h] [ebp-3Ch]
    GLfloat anglea; // [esp+0h] [ebp-3Ch]
    GLfloat angleb; // [esp+0h] [ebp-3Ch]
    GLfloat y; // [esp+8h] [ebp-34h]
    GLfloat ya; // [esp+8h] [ebp-34h]
    GLfloat yb; // [esp+8h] [ebp-34h]
    __int16 i; // [esp+20h] [ebp-1Ch]
    __int16 v14; // [esp+2Ch] [ebp-10h]
    __int16 v15; // [esp+30h] [ebp-Ch]
    bool hitting; // [esp+37h] [ebp-5h]

    v14 = 10000;
    hitting = 0;
    v15 = 0;
    for (i = 0; i < 32; ++i) {
        if (g_percussion_time_queue[percussionPatch][i] > 0 && g_percussion_time_queue[percussionPatch][i] < v14) {
            v14 = g_percussion_time_queue[percussionPatch][i];
            ++v15;
        }
    }
    if (v14 > 0 && (double)v14 < 200.0) {
        g_stick_visible[percussionPatch] = 1;
        glPushMatrix();
        y = (double)v14 / 200.0 * 1.5 + a3;
        glTranslatef(0.0, y, 13.75);
        angle = (double)v14 / 200.0 * 45.0 + -14.75;
        glRotatef(angle, 1.0, 0.0, 0.0);
        if ((double)v14 / 200.0 < 0.375)
            hitting = 1;
        if (a6 != 0.0)
            glRotatef(a6, 0.0, 1.0, 0.0);
        if (isShiny) {
        LABEL_14:
            stick_model_ms3d->RenderModelShiny();
        LABEL_16:
            glPopMatrix();
            return hitting;
        }
    LABEL_15:
        stick_model_ms3d->RenderModel();
        goto LABEL_16;
    }
    if (recoilDistance > 0.0) {
        g_stick_visible[percussionPatch] = 1;
        glPushMatrix();
        ya = (MAX_RECOIL - recoilDistance) / MAX_RECOIL * 1.5 + a3;
        glTranslatef(0.0, ya, 13.75);
        anglea = (MAX_RECOIL - recoilDistance) / MAX_RECOIL * 45.0 + -14.75;
        glRotatef(anglea, 1.0, 0.0, 0.0);
        if ((MAX_RECOIL - recoilDistance) / MAX_RECOIL < 0.25)
            hitting = 1;
        if (a6 != 0.0)
            glRotatef(a6, 0.0, 1.0, 0.0);
        if (isShiny)
            goto LABEL_14;
        goto LABEL_15;
    }
    if (v15 <= 0) {
        g_stick_visible[percussionPatch] = 0;
    }
    else if (g_stick_visible[percussionPatch] == 1) {
        glPushMatrix();
        yb = a3 + 1.5;
        glTranslatef(0.0, yb, 13.75);
        angleb = 45.0 + -14.75;
        glRotatef(angleb, 1.0, 0.0, 0.0);
        if (a6 != 0.0)
            glRotatef(a6, 0.0, 1.0, 0.0);
        if (isShiny)
            stick_model_ms3d->RenderModelShiny();
        else
            stick_model_ms3d->RenderModel();
        glPopMatrix();
    }
    return hitting;
}
