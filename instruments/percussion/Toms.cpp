//
// Created by Jacob on 4/12/2026.
//

#include "Toms.h"

#include "model/Ms3dBundle.h"

extern Ms3dBundle* g_drumSet_Stick_ms3d;
extern int g_percussion_time_queue[88][32];
extern short g_stick_visible[37];
extern float MAX_RECOIL;

GLfloat DRUMSET_TOM_LOC_X[6] = {-15.453f, -9.7229996f, 0.0f, 10.729f, 17.747999f, 20.75f};
GLfloat DRUMSET_TOM_LOC_Y[6] = {30.324007f, 31.233f, 32.0f, 30.002001f, 22.507023f, 20.257961f};
GLfloat DRUMSET_TOM_LOC_Z[6] = {1.2970001f, -4.0739999f, -6.0f, -1.17f, 7.796f, 22.746f};
GLfloat DRUMSET_TOM_ROT_X[6] = {45.0f, 48.5f, 59.0f, 55.5f, 17.0f, 13.5f};
GLfloat DRUMSET_TOM_ROT_Y[6] = {45.0f, 22.5f, -1.0f, -33.0f, -90.0f, -110.75f};
GLfloat DRUMSET_TOM_SCALE_Y[6] = {0.5f, 0.69999999f, 0.89999998f, 1.1f, 1.4f, 1.6f};
GLfloat DRUMSET_TOM_SCALE_XZ[6] = {0.69999999f, 0.89999998f, 1.1f, 1.3f, 1.4f, 1.6f};
short GM_TOM_PATCHES[6] = {
    50, 48, 47, 45, 43, 41
};

float g_recoil_tom[6] = {};

// FUNCTION: MIDIJAM 0x405A20
void UpdateToms(short tomIndex) {
    GLfloat angle; // [esp+0h] [ebp-34h]
    GLfloat anglea; // [esp+0h] [ebp-34h]
    GLfloat angleb; // [esp+0h] [ebp-34h]
    GLfloat y; // [esp+8h] [ebp-2Ch]
    GLfloat ya; // [esp+8h] [ebp-2Ch]
    GLfloat yb; // [esp+8h] [ebp-2Ch]
    __int16 i; // [esp+1Ch] [ebp-18h]
    __int16 v8; // [esp+28h] [ebp-Ch]
    __int16 v9; // [esp+2Ch] [ebp-8h]

    v8 = 10000;
    v9 = 0;
    for (i = 0; i < 32; ++i) {
        if (g_percussion_time_queue[GM_TOM_PATCHES[tomIndex]][i] > 0
            && g_percussion_time_queue[GM_TOM_PATCHES[tomIndex]][i] < v8) {
            v8 = g_percussion_time_queue[GM_TOM_PATCHES[tomIndex]][i];
            ++v9;
        }
    }
    if (v8 <= 0 || (double)v8 >= 200.0) {
        if (g_recoil_tom[tomIndex] <= 0.0) {
            if (v9 <= 0) {
                g_stick_visible[GM_TOM_PATCHES[tomIndex]] = 0;
            }
            else if (g_stick_visible[GM_TOM_PATCHES[tomIndex]] == 1) {
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
            g_stick_visible[GM_TOM_PATCHES[tomIndex]] = 1;
            glPushMatrix();
            ya = (MAX_RECOIL - g_recoil_tom[tomIndex]) / MAX_RECOIL * 1.5 + 3.0;
            glTranslatef(0.0, ya, 13.75);
            anglea = (MAX_RECOIL - g_recoil_tom[tomIndex]) / MAX_RECOIL * 45.0 + -14.75;
            glRotatef(anglea, 1.0, 0.0, 0.0);
            g_drumSet_Stick_ms3d->RenderModel();
            glPopMatrix();
        }
    }
    else {
        g_stick_visible[GM_TOM_PATCHES[tomIndex]] = 1;
        glPushMatrix();
        y = (double)v8 / 200.0 * 1.5 + 3.0;
        glTranslatef(0.0, y, 13.75);
        angle = (double)v8 / 200.0 * 45.0 + -14.75;
        glRotatef(angle, 1.0, 0.0, 0.0);
        g_drumSet_Stick_ms3d->RenderModel();
        glPopMatrix();
    }
}
