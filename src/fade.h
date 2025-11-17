//
// Created by Jacob on 11/16/2025.
//

#ifndef MIDIJAM_FADE_H
#define MIDIJAM_FADE_H
#include <windows.h>
#include <GL/GL.h>

extern GLfloat g_fadeFactor;

void RenderFadeout();

void RenderFadeoutImpl(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

#endif //MIDIJAM_FADE_H
