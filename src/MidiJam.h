//
// Created by Jacob on 11/16/2025.
//

#ifndef MIDIJAM_MIDIJAM_H
#define MIDIJAM_MIDIJAM_H
#include <cstdio>
#include <windows.h>
#include <GL/gl.h>

#include "DirectMusicSystem.h"
#include "IMidiJamTool.h"

// Config and window
extern bool g_useSound;
extern bool g_fullscreen;
extern GLsizei g_windowWidth;
extern GLsizei g_windowHeight;
extern __int16 g_worldReady;
extern int g_bitDepth;
extern GLfloat g_refreshRate;
extern int g_windowCenter_X;
extern int g_windowCenter_Y;
extern __int16 g_windowWidth_0;
extern __int16 g_windowHeight_0;
extern HANDLE g_hMidiEvent;
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern int g_pixelFormatInitialized;
extern PIXELFORMATDESCRIPTOR g_pixelFormatDescriptor;
extern HGLRC g_openglRenderingContext;
extern float g_normalizedWindowScale;

// Input
extern char g_leftButtonDown;
extern char g_leftButtonClicked;
extern char g_middleButtonDown;
extern char g_middleButtonClicked;
extern char g_rightButtonDown;
extern char g_rightButtonClicked;
extern char g_mouseWheelMoved;
extern int g_mouseWheelDelta;
extern char g_mouseMoved;
extern __int16 g_mouseY;
extern __int16 g_mouseX;
extern char g_keyStateArray[256];
extern char g_lastKeyPressed;
extern char g_isWindowActive;

// DirectMusic
extern DirectMusicSystem *g_DirectMusicSystem;
extern DirectMusicSegmentWrapper *g_DirectMusicSegmentWrapper;
extern IDirectMusicPerformance8 *g_DirectMusicPerformance;
extern IDirectMusicGraph8 *g_DirectMusicGraph;
extern IMidiJamTool *g_midiJamTool;
extern HDC g_hdc;

// Lighting
extern GLfloat LIGHT_PARAMS_0_AMBIENT[4];
extern GLfloat LIGHT_PARAMS_0_DIFFUSE[4];
extern GLfloat LIGHT_PARAMS_0_POSITION[4];
extern GLfloat LIGHT_PARAMS_0_SPECULAR[4];
extern GLfloat LIGHT_PARAMS_1_AMBIENT[4];
extern GLfloat LIGHT_PARAMS_1_DIFFUSE[4];
extern GLfloat LIGHT_PARAMS_1_POSITION[4];


bool CreateMidijamWindow(
    LPCSTR lpWindowName,
    HINSTANCE hInstance,
    GLsizei windowWidth,
    GLsizei windowHeight,
    int bitDepth,
    DWORD refreshRate,
    bool isFullscreen,
    FILE *hwfStream,
    char *hwfAppendix_ptr,
    unsigned int hwfAppendixEntriesCount);

int MidiJamMain(LPCSTR lpWindowName,
                HINSTANCE hInstance,
                FILE *hwfStream,
                char *pHwfAppendix,
                unsigned int nHwfAppendixItems);

BOOL UpdateMidiJam();

void SetPerspectiveProjection(int windowWidth, int windowHeight);

void MidiJamWindowCleanup();

LRESULT __stdcall HandleWindowBehavior(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

BOOL InitLightsCullingAndInputs();

void __stdcall UpdateMidiJamMM(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

__time32_t GetUnixEpochTime(__time32_t *const Time);

HRESULT PlaySegment();

void MidiJamInitialize();


#endif //MIDIJAM_MIDIJAM_H
