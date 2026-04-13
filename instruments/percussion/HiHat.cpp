//
// Created by Jacob on 4/12/2026.
//

#include "HiHat.h"

#include <windows.h>
#include <GL/gl.h>

#include "model/Ms3dBundle.h"

extern int g_percussion_time_queue[88][32];
extern float g_recoil_hihat;
extern Ms3dBundle* g_drumSet_Stick_ms3d;
extern float MAX_RECOIL;

short word_4691A4;

// FUNCTION: MIDIJAM 0x4057B0
void UpdateHiHat() {
    GLfloat angle; // [esp+0h] [ebp-3Ch]
    GLfloat anglea; // [esp+0h] [ebp-3Ch]
    GLfloat angleb; // [esp+0h] [ebp-3Ch]
    GLfloat y; // [esp+8h] [ebp-34h]
    GLfloat ya; // [esp+8h] [ebp-34h]
    GLfloat yb; // [esp+8h] [ebp-34h]
    __int16 j; // [esp+1Ch] [ebp-20h]
    __int16 v7; // [esp+28h] [ebp-14h]
    __int16 v8; // [esp+2Ch] [ebp-10h]
    __int16 v9; // [esp+34h] [ebp-8h]
    __int16 i; // [esp+38h] [ebp-4h]

    v7 = 10000;
    v8 = 0;
    for (i = 0; i < 2; ++i) {
        if (i)
            v9 = 46;
        else
            v9 = 42;
        for (j = 0; j < 32; ++j) {
            if (g_percussion_time_queue[v9][j] > 0 && g_percussion_time_queue[v9][j] < v7) {
                v7 = g_percussion_time_queue[v9][j];
                ++v8;
            }
        }
    }
    if (v7 <= 0 || (double)v7 >= 200.0) {
        if (g_recoil_hihat <= 0.0) {
            if (v8 <= 0) {
                word_4691A4 = 0;
            }
            else if (word_4691A4 == 1) {
                glPushMatrix();
                yb = 1.5 + 3.0;
                glTranslatef(0.0, yb, 13.75);
                angleb = 45.0 + -14.75;
                glRotatef(angleb, 1.0, 0.0, 0.0);
                g_drumSet_Stick_ms3d->RenderModel();
                glPopMatrix();
            }
        }
        else {
            word_4691A4 = 1;
            glPushMatrix();
            ya = (MAX_RECOIL - g_recoil_hihat) / MAX_RECOIL * 1.5 + 3.0;
            glTranslatef(0.0, ya, 13.75);
            anglea = (MAX_RECOIL - g_recoil_hihat) / MAX_RECOIL * 45.0 + -14.75;
            glRotatef(anglea, 1.0, 0.0, 0.0);
            g_drumSet_Stick_ms3d->RenderModel();
            glPopMatrix();
        }
    }
    else {
        word_4691A4 = 1;
        glPushMatrix();
        y = (double)v7 / 200.0 * 1.5 + 3.0;
        glTranslatef(0.0, y, 13.75);
        angle = (double)v7 / 200.0 * 45.0 + -14.75;
        glRotatef(angle, 1.0, 0.0, 0.0);
        g_drumSet_Stick_ms3d->RenderModel();
        glPopMatrix();
    }
}
