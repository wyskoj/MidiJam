//
// Created by Jacob on 3/17/2026.
//

#include "gl.h"

#include <GL/glu.h>

// ---------------------------------------------------------------------------
// Light parameters — read-only constants matching original data segment layout.
// ---------------------------------------------------------------------------

static constexpr GLfloat LIGHT_PARAMS_0_AMBIENT[4] = {0.25f, 0.25f, 0.25f, 1.0f};
static constexpr GLfloat LIGHT_PARAMS_0_DIFFUSE[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static constexpr GLfloat LIGHT_PARAMS_0_POSITION[4] = {250.0f, 175.0f, 150.0f, 1.0f};
static constexpr GLfloat LIGHT_PARAMS_0_SPECULAR[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static constexpr GLfloat LIGHT_PARAMS_1_AMBIENT[4] = {0.025f, 0.025f, 0.025f, 1.0f};
static constexpr GLfloat LIGHT_PARAMS_1_DIFFUSE[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static constexpr GLfloat LIGHT_PARAMS_1_POSITION[4] = {-250.0f, 100.0f, -125.0f, 1.0f};

// ---------------------------------------------------------------------------
// Globals referenced here — defined in globals.cpp.
// ---------------------------------------------------------------------------

extern HDC g_hdc;
extern char g_leftButtonDown;
extern char g_middleButtonDown;
extern char g_rightButtonDown;
extern char g_leftButtonClicked;
extern char g_middleButtonClicked;
extern char g_rightButtonClicked;
extern char g_mouseWheelMoved;
extern char g_mouseMoved;
extern short g_mouseX;
extern short g_mouseY;
extern short g_mouseWheelDelta;

// FUNCTION: MIDIJAM 0x447E20
// MATCH: EXACT
void SetPerspectiveProjection(const int windowWidth, int windowHeight) {
    if (!windowHeight) {
        windowHeight = 1;
    }

    const float aspectRatio = windowWidth * 1.0f / windowHeight; // NOLINT(*-narrowing-conversions)

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, windowWidth, windowHeight);
    gluPerspective(45.0, aspectRatio, 0.1000000014901161, 4000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// FUNCTION: MIDIJAM 0x447EB0
// MATCH: EXACT
BOOL SwapBuffers_0() {
    return SwapBuffers(g_hdc);
}

// FUNCTION: MIDIJAM 0x447330
// MATCH: EXACT
BOOL InitLightsCullingAndInputs() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0);
    glClearStencil(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glEnable(GL_NORMALIZE);
    glLightfv(GL_LIGHT0, GL_AMBIENT, LIGHT_PARAMS_0_AMBIENT);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LIGHT_PARAMS_0_DIFFUSE);
    glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_PARAMS_0_POSITION);
    glLightfv(GL_LIGHT0, GL_SPECULAR, LIGHT_PARAMS_0_SPECULAR);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT1, GL_AMBIENT, LIGHT_PARAMS_1_AMBIENT);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LIGHT_PARAMS_1_DIFFUSE);
    glLightfv(GL_LIGHT1, GL_POSITION, LIGHT_PARAMS_1_POSITION);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    g_leftButtonDown = 0;
    g_middleButtonDown = 0;
    g_rightButtonDown = 0;
    g_leftButtonClicked = 0;
    g_middleButtonClicked = 0;
    g_rightButtonClicked = 0;
    g_mouseWheelMoved = 0;
    g_mouseWheelDelta = 0;
    g_mouseMoved = 0;
    g_mouseX = 0;
    g_mouseY = 0;
    return TRUE;
}
