//
// Created by Jacob on 4/12/2026.
//

#include "SnareDrum.h"

#include "instruments/instrument_ids.h"

#include <windows.h>
#include <GL/gl.h>

#include "model/Ms3dBundle.h"

extern int g_percussion_time_queue[88][32];
extern float g_recoil_snare;
extern Ms3dBundle* g_drumSet_Stick_ms3d;
extern float MAX_RECOIL;

__int16 g_snareStickActive = 0;

void UpdateSnareDrum() {
    GLfloat angle; // [esp+0h] [ebp-3Ch]
    GLfloat anglea; // [esp+0h] [ebp-3Ch]
    GLfloat angleb; // [esp+0h] [ebp-3Ch]
    GLfloat y; // [esp+8h] [ebp-34h]
    GLfloat ya; // [esp+8h] [ebp-34h]
    GLfloat yb; // [esp+8h] [ebp-34h]
    __int16 queue; // [esp+1Ch] [ebp-20h]
    __int16 earliestHitTime; // [esp+28h] [ebp-14h]
    __int16 activeHitCount; // [esp+2Ch] [ebp-10h]
    GM_PERCUSSION patch; // [esp+34h] [ebp-8h]
    __int16 i; // [esp+38h] [ebp-4h]

    earliestHitTime = 10000;
    activeHitCount = 0;
    for (i = 0; i < 2; ++i) {
        if (i)
            patch = ELECTRIC_SNARE;
        else
            patch = ACOUSTIC_SNARE;
        for (queue = 0; queue < 32; ++queue) {
            if (g_percussion_time_queue[patch][queue] > 0 && g_percussion_time_queue[patch][queue] < earliestHitTime) {
                earliestHitTime = g_percussion_time_queue[patch][queue];
                ++activeHitCount;
            }
        }
    }
    if (earliestHitTime <= 0 || (double)earliestHitTime >= 200.0) {
        if (g_recoil_snare <= 0.0) {
            if (activeHitCount <= 0) {
                g_snareStickActive = 0;
            }
            else if (g_snareStickActive == 1) {
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
            g_snareStickActive = 1;
            glPushMatrix();
            ya = (MAX_RECOIL - g_recoil_snare) / MAX_RECOIL * 1.5 + 3.0;
            glTranslatef(0.0, ya, 13.75);
            anglea = (MAX_RECOIL - g_recoil_snare) / MAX_RECOIL * 45.0 + -14.75;
            glRotatef(anglea, 1.0, 0.0, 0.0);
            g_drumSet_Stick_ms3d->RenderModel();
            glPopMatrix();
        }
    }
    else {
        g_snareStickActive = 1;
        glPushMatrix();
        y = (double)earliestHitTime / 200.0 * 1.5 + 3.0;
        glTranslatef(0.0, y, 13.75);
        angle = (double)earliestHitTime / 200.0 * 45.0 + -14.75;
        glRotatef(angle, 1.0, 0.0, 0.0);
        g_drumSet_Stick_ms3d->RenderModel();
        glPopMatrix();
    }
}

void UpdateSideStick() {
}
