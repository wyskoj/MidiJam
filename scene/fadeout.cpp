//
// Created by Jacob on 3/18/2026.
//

#include "fadeout.h"

extern GLfloat g_fadeFactor;

// FUNCTION: MIDIJAM 0x438B80
// MATCH: EXACT
void RenderFadeout()
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(0, GL_ONE_MINUS_SRC_ALPHA);
    RenderFadeoutImpl(0.0, 0.0, 0.0, g_fadeFactor);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}

// FUNCTION: MIDIJAM 0x438BF0
// MATCH: EXACT
void RenderFadeoutImpl(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a)
{
    GLfloat params[4];
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
