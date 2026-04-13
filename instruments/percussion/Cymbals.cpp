//
// Created by Jacob on 4/12/2026.
//

#include "Cymbals.h"

#include "instruments/instrument_ids.h"
#include "model/Ms3dBundle.h"

extern Ms3dBundle* g_drumSet_Stick_ms3d;
extern int g_percussion_time_queue[88][32];
extern float g_cymbalMassFactor[7];
extern int g_lastRideCymbal;
extern float g_recoil_cymbals[7];
extern short g_stick_visible[37];

GLfloat CYMBAL_LOC_X[7] = {-2.0999999f, -19.629999f, 13.0f, 24.360001f, -26.35f, 31.754f, 31.754f};
GLfloat CYMBAL_LOC_Y[7] = {47.5f, 47.0f, 46.5f, 40.0f, 39.0f, 33.0f, 33.0f};
GLfloat CYMBAL_LOC_Z[7] = {-13.86f, -12.672f, -14.533f, -3.5999999f, 0.086000003f, 9.1199999f, 9.1199999f};

short g_cymbal_patches[7] = {
    SPLASH_CYMBAL, CRASH_CYMBAL_1, CRASH_CYMBAL_2, RIDE_CYMBAL_1, RIDE_CYMBAL_2, CHINESE_CYMBAL, OPEN_HI_HAT
};

float CYMBAL_SCALE[7] = {
    1.0f, 2.0f, 1.6f, 2.2f, 2.0f, 2.0f, 2.0f
};

float CYMBAL_MAX_WOBBLE[7] = {
    67.5f, 67.5f, 67.5f, 11.25f, 11.25f, 67.5f, 3.0f
};

__int16 word_469F48[32] = {};

// FUNCTION: MIDIJAM 0x406230
void UpdateCymbals(short a1) {
    double v1; // st7
    GLfloat angle; // [esp+0h] [ebp-34h]
    GLfloat anglea; // [esp+0h] [ebp-34h]
    GLfloat angleb; // [esp+0h] [ebp-34h]
    GLfloat y; // [esp+8h] [ebp-2Ch]
    GLfloat ya; // [esp+8h] [ebp-2Ch]
    GLfloat yb; // [esp+8h] [ebp-2Ch]
    __int16 i; // [esp+1Ch] [ebp-18h]
    __int16 v9; // [esp+28h] [ebp-Ch]
    __int16 v10; // [esp+2Ch] [ebp-8h]

    v9 = 10000;
    v10 = 0;
    for (i = 0; i < 32; ++i) {
        if (g_percussion_time_queue[g_cymbal_patches[a1]][i] > 0 && g_percussion_time_queue[g_cymbal_patches[a1]][i] <
            v9) {
            v9 = g_percussion_time_queue[g_cymbal_patches[a1]][i];
            ++v10;
            v1 = CYMBAL_SCALE[a1];
            g_cymbalMassFactor[a1] = v1 + v1 + 16.75;
        }
        if (word_469F48[i] > 0
            && word_469F48[i] < v9
            && (g_cymbal_patches[a1] == RIDE_CYMBAL_1 && g_lastRideCymbal != 2
                || g_cymbal_patches[a1] == RIDE_CYMBAL_2 && g_lastRideCymbal == 2)) {
            v9 = word_469F48[i];
            ++v10;
            g_cymbalMassFactor[a1] = CYMBAL_SCALE[a1] + 13.75;
        }
    }
    if (v9 <= 0 || (double)v9 >= 200.0) {
        if (g_recoil_cymbals[a1] <= 0.0) {
            if (v10 <= 0) {
                g_stick_visible[g_cymbal_patches[a1]] = 0;
            }
            else if (g_stick_visible[g_cymbal_patches[a1]] == 1) {
                glPushMatrix();
                yb = 1.5 + 3.0 - CYMBAL_SCALE[a1] * 0.5;
                glTranslatef(0.0, yb, g_cymbalMassFactor[a1]);
                angleb = 45.0 + -14.75;
                glRotatef(angleb, 1.0, 0.0, 0.0);
                g_drumSet_Stick_ms3d->RenderModel();
                glPopMatrix();
            }
        }
        else {
            g_stick_visible[g_cymbal_patches[a1]] = 1;
            glPushMatrix();
            ya = (CYMBAL_MAX_WOBBLE[a1] - g_recoil_cymbals[a1]) / CYMBAL_MAX_WOBBLE[a1] * 1.5 + 3.0 - CYMBAL_SCALE[a1] *
                0.5;
            glTranslatef(0.0, ya, g_cymbalMassFactor[a1]);
            anglea = (CYMBAL_MAX_WOBBLE[a1] - g_recoil_cymbals[a1]) / CYMBAL_MAX_WOBBLE[a1] * 45.0 + -14.75;
            glRotatef(anglea, 1.0, 0.0, 0.0);
            g_drumSet_Stick_ms3d->RenderModel();
            glPopMatrix();
        }
    }
    else {
        g_stick_visible[g_cymbal_patches[a1]] = 1;
        glPushMatrix();
        y = (double)v9 / 200.0 * 1.5 + 3.0 - CYMBAL_SCALE[a1] * 0.5;
        glTranslatef(0.0, y, g_cymbalMassFactor[a1]);
        angle = (double)v9 / 200.0 * 45.0 + -14.75;
        glRotatef(angle, 1.0, 0.0, 0.0);
        g_drumSet_Stick_ms3d->RenderModel();
        glPopMatrix();
    }
}
