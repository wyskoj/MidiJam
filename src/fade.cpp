//
// Created by Jacob on 11/16/2025.
//
#include "fade.h"

GLfloat g_fadeFactor;

void RenderFadeout() {
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(0, GL_ONE_MINUS_SRC_ALPHA);
    RenderFadeoutImpl(0.0, 0.0, 0.0, g_fadeFactor);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}

void RenderFadeoutImpl(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    GLfloat params[4]; // [esp+0h] [ebp-10h] BYREF

    params[0] = r;
    params[1] = g;
    params[2] = b;
    params[3] = a;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, params);
    glBegin(GL_QUADS);
    glVertex3f(-300.0, -200.0, 0.0);
    glVertex3f(300.0, -200.0, 0.0);
    glVertex3f(300.0, 200.0, 0.0);
    glVertex3f(-300.0, 200.0, 0.0);
    glEnd();
}
