//
// Created by Jacob on 4/12/2026.
//

#ifndef MIDIJAM_TOMS_H
#define MIDIJAM_TOMS_H

#include <windows.h>
#include <GL/gl.h>

void UpdateToms(short tomIndex);

extern GLfloat DRUMSET_TOM_LOC_X[6];
extern GLfloat DRUMSET_TOM_LOC_Y[6];
extern GLfloat DRUMSET_TOM_LOC_Z[6];
extern GLfloat DRUMSET_TOM_ROT_X[6];
extern GLfloat DRUMSET_TOM_ROT_Y[6];
extern GLfloat DRUMSET_TOM_SCALE_Y[6];
extern GLfloat DRUMSET_TOM_SCALE_XZ[6];
extern float g_recoil_tom[6];

#endif //MIDIJAM_TOMS_H
