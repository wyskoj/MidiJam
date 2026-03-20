//
// Created by Jacob on 3/19/2026.
//

#include "initialize.h"
#include "MidiJamTool.h"
#include "DirectMusicSystem.h"
#include "../render/window.h"

#include <windows.h>
#include <shellapi.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <GL/gl.h>

// ---------------------------------------------------------------------------
// GUIDs
// ---------------------------------------------------------------------------

static const GUID NULL_GUID = {};

static const IID IID_I_DIRECT_MUSIC_GRAPH8 = {
    0x2BEFC277, 0x5497, 0x11D2,
    {0xBC, 0xCB, 0x00, 0xA0, 0xC9, 0x22, 0xE6, 0xEB}
};

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------

extern char g_useSound;
extern GLsizei g_windowWidth;
extern GLsizei g_windowHeight;
extern GLfloat g_refreshRate;
extern int g_bitDepth;
extern bool g_fullscreen;
extern char g_workingDirectory[1000];
extern int g_windowCenter_X;
extern int g_windowCenter_Y;
extern __int16 g_windowWidth_0;
extern __int16 g_windowHeight_0;
extern HANDLE g_hMidiEvent;
extern HWND g_hWnd;
extern DirectMusicSystem* g_DirectMusicSystem;
extern IDirectMusicPerformance8* g_DirectMusicPerformance;
extern IDirectMusicGraph8* g_DirectMusicGraph;
extern MidiJamTool* g_midiJamTool;

// ---------------------------------------------------------------------------
// FUNCTION: MIDIJAM 0x421610 (called from WinMain after HWF load)
// ---------------------------------------------------------------------------
WINDOW_CREATE_STATUS MidiJamMain(
    LPCSTR lpWindowName,
    HINSTANCE hInstance,
    FILE* hwfStream,
    void* pHwfAppendix,
    unsigned int nHwfAppendixItems)
{
    // --- Defaults ---
    g_useSound = 1;
    g_windowWidth = 1024;
    g_windowHeight = 768;
    g_bitDepth = 32;
    g_refreshRate = 75.0f;

    // --- Read HWF.cfg ---
    char configFileBuffer[1000];
    sprintf(configFileBuffer, "%s\\HWF.cfg", g_workingDirectory);
    FILE* configFileStream = fopen(configFileBuffer, "rb");

    if (!configFileStream)
    {
        // Config file missing — launch config.exe and wait
        char configExecutablePath[1000];
        SHELLEXECUTEINFOA configInfo = {};
        configInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
        sprintf(configExecutablePath, "%s\\config.exe", g_workingDirectory);
        configInfo.lpFile = configExecutablePath;
        configInfo.lpVerb = "open";
        configInfo.nShow = SW_SHOWNORMAL;
        configInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

        if (ShellExecuteExA(&configInfo))
        {
            if (WaitForSingleObject(configInfo.hProcess, INFINITE))
            {
                MessageBoxA(0, "Error waiting for program to start", "WAIT FOR CONFIG ERROR", MB_ICONASTERISK);
                return FAILURE;
            }
        }
        else
        {
            MessageBoxA(0, "Error executing config program", "WAIT FOR CONFIG ERROR", MB_ICONASTERISK);
        }

        configFileStream = fopen(configFileBuffer, "rb");
    }

    if (configFileStream)
    {
        char configUseSound = 0;
        char configFullscreen = 0;
        int configResolution = 0;
        int configBitDepth = 0;
        int configRefreshRate = 0;

        fread(&configUseSound, 1, 1, configFileStream);
        fread(&configFullscreen, 1, 1, configFileStream);
        fread(&configResolution, 4, 1, configFileStream);
        fread(&configBitDepth, 4, 1, configFileStream);
        fread(&configRefreshRate, 4, 1, configFileStream);
        fclose(configFileStream);

        g_useSound = (configUseSound == 1);
        g_fullscreen = (configFullscreen == 1);

        switch (configResolution)
        {
        case 0: g_windowWidth = 640;
            g_windowHeight = 480;
            break;
        case 1: g_windowWidth = 800;
            g_windowHeight = 600;
            break;
        case 2: g_windowWidth = 1024;
            g_windowHeight = 768;
            break;
        case 3: g_windowWidth = 1152;
            g_windowHeight = 864;
            break;
        case 4: g_windowWidth = 1280;
            g_windowHeight = 1024;
            break;
        case 5: g_windowWidth = 1600;
            g_windowHeight = 1200;
            break;
        }

        if (configBitDepth == 0)
            g_bitDepth = 16;
        else if (configBitDepth == 1)
            g_bitDepth = 32;

        switch (configRefreshRate)
        {
        case 0: g_refreshRate = 60.0f;
            break;
        case 1: g_refreshRate = 70.0f;
            break;
        case 2: g_refreshRate = 72.0f;
            break;
        case 3: g_refreshRate = 75.0f;
            break;
        case 4: g_refreshRate = 85.0f;
            break;
        case 5: g_refreshRate = 100.0f;
            break;
        case 6: g_refreshRate = 120.0f;
            break;
        case 7: g_refreshRate = 0.0f;
            break;
        }
    }

    g_windowCenter_X = g_windowWidth / 2;
    g_windowCenter_Y = g_windowHeight / 2;

    if (!CreateMidijamWindow(
        lpWindowName,
        hInstance,
        g_windowWidth,
        g_windowHeight,
        g_bitDepth,
        static_cast<DWORD>(g_refreshRate),
        g_fullscreen,
        hwfStream,
        static_cast<char*>(pHwfAppendix),
        nHwfAppendixItems))
        return FAILURE;

    glClear(0x4500u);
    glFlush();
    glClear(0x4500u);
    glFlush();

    g_windowWidth_0 = static_cast<__int16>(g_windowWidth);
    g_windowHeight_0 = static_cast<__int16>(g_windowHeight);

    g_hMidiEvent = CreateEventA(0, 0, 0, 0);

    // --- DirectMusic system ---
    DirectMusicSystem* directMusicSystem = new DirectMusicSystem();
    g_DirectMusicSystem = directMusicSystem;

    HRESULT hResult = directMusicSystem->Init(g_hWnd, 128, 8);
    g_DirectMusicPerformance = directMusicSystem->GetPerformance();
    IDirectMusicAudioPath* pDefaultAudioPath = directMusicSystem->GetDefaultAudioPath();

    hResult = pDefaultAudioPath->GetObjectInPath(
        0,
        DMUS_PATH_PERFORMANCE_GRAPH,
        0,
        NULL_GUID,
        0,
        IID_I_DIRECT_MUSIC_GRAPH8,
        reinterpret_cast<void**>(&g_DirectMusicGraph));
    if (hResult < 0)
        return FAILURE;

    // --- Notification setup (GUID_NOTIFICATION_SEGMENT) ---
    GUID guidNotificationSegment;
    guidNotificationSegment.Data1 = 0xD2AC2899;
    guidNotificationSegment.Data2 = 0x11D1;
    guidNotificationSegment.Data3 = 0xB39B;
    guidNotificationSegment.Data4[0] = 0x60;
    guidNotificationSegment.Data4[1] = 0x00;
    guidNotificationSegment.Data4[2] = 0x04;
    guidNotificationSegment.Data4[3] = 0x87;
    guidNotificationSegment.Data4[4] = 0xBD;
    guidNotificationSegment.Data4[5] = 0xB1;
    guidNotificationSegment.Data4[6] = 0x93;
    guidNotificationSegment.Data4[7] = 0x08;

    hResult = g_DirectMusicPerformance->AddNotificationType(guidNotificationSegment);
    if (hResult < 0)
        return FAILURE;

    hResult = g_DirectMusicPerformance->SetNotificationHandle(g_hMidiEvent, 0);
    if (hResult < 0)
        return FAILURE;

    // --- MidiJam tool ---
    const auto midiJamTool = new MidiJamTool();
    g_midiJamTool = midiJamTool;
    hResult = (g_DirectMusicGraph->InsertTool)(midiJamTool, 0, 0, 0);

    if (hResult < 0)
        return FAILURE;

    return SUCCESS;
}

// ---------------------------------------------------------------------------
// FUNCTION: MIDIJAM 0x43C2C0 — no-op constructor stub
// ---------------------------------------------------------------------------
void MidiJamInitialize()
{
    // TODO: transcribe when pseudocode is available
}
