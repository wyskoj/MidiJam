//
// Created by Jacob on 3/17/2026.
//

#ifndef MIDIJAM_GL_H
#define MIDIJAM_GL_H

#include <windows.h>
#include <GL/gl.h>

void SetPerspectiveProjection(int windowWidth, int windowHeight);
BOOL SwapBuffers_0();
BOOL InitLightsCullingAndInputs();

#endif // MIDIJAM_GL_H