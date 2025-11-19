//
// Created by Jacob on 11/16/2025.
//
#include "MidiJam.h"

#include <iostream>
#include <GL/GLU.h>

#include "camera.h"
#include "dmusici.h"
#include "fade.h"
#include "fonts.h"
#include "globals.h"
#include "Ms3dBundle.h"
#include "text.h"

bool CreateMidijamWindow(LPCSTR lpWindowName, HINSTANCE hInstance, GLsizei windowWidth, GLsizei windowHeight,
                         int bitDepth, DWORD refreshRate, bool isFullscreen, FILE *hwfStream, char *hwfAppendix_ptr,
                         unsigned int hwfAppendixEntriesCount) {
    DEVMODEA DevMode; // [esp+0h] [ebp-E8h] BYREF
    WNDCLASSA WndClass; // [esp+A4h] [ebp-44h] BYREF
    DWORD dwExStyle; // [esp+CCh] [ebp-1Ch]
    DWORD dwStyle; // [esp+D0h] [ebp-18h]
    RECT windowRect; // [esp+D4h] [ebp-14h] BYREF
    int pixelFormat; // [esp+E4h] [ebp-4h]

    windowRect.left = 0;
    windowRect.right = windowWidth;
    windowRect.top = 0;
    windowRect.bottom = windowHeight;
    g_hInstance = hInstance;
    g_fullscreen = isFullscreen;
    g_hInstance = GetModuleHandleA(0);
    WndClass.style = 35; // CS_HREDRAW | CS_VREDRAW | CS_OWNDC
    WndClass.lpfnWndProc = HandleWindowBehavior;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = g_hInstance;
    WndClass.hIcon = LoadIconA(0, IDI_WINLOGO);
    WndClass.hCursor = LoadCursorA(0, IDC_ARROW);
    WndClass.hbrBackground = 0;
    WndClass.lpszMenuName = 0;
    WndClass.lpszClassName = "OpenGL";
    if (RegisterClassA(&WndClass)) {
        if (!g_fullscreen)
            goto LABEL_8;
        memset(&DevMode, 0, sizeof(DevMode));
        DevMode.dmSize = sizeof(DEVMODEA);
        DevMode.dmPelsWidth = windowWidth;
        DevMode.dmPelsHeight = windowHeight;
        DevMode.dmBitsPerPel = bitDepth;
        if (refreshRate)
            DevMode.dmDisplayFrequency = refreshRate;
        DevMode.dmFields = 0x5C0000; // AI says DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY
        // Set fullscreen
        if (ChangeDisplaySettingsA(&DevMode, CDS_FULLSCREEN)) {
            PostQuitMessage(0);
            return 0;
        } else {
        LABEL_8:
            if (g_fullscreen) {
                dwExStyle = WS_EX_APPWINDOW;
                dwStyle = WS_POPUP;
            } else {
                dwExStyle = 0x40100; // AI says: WS_EX_APPWINDOW | WS_EX_WINDOWEDGE
                dwStyle = WS_OVERLAPPEDWINDOW;
            }
            ShowCursor(FALSE); // hide cursor
            AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);
            g_hWnd = CreateWindowExA(
                dwExStyle,
                "OpenGL",
                lpWindowName,
                dwStyle | 0x6000000, // AI says: dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
                0,
                0,
                windowRect.right - windowRect.left,
                windowRect.bottom - windowRect.top,
                0,
                0,
                g_hInstance,
                0);
            if (!g_hWnd)
                goto CLEANUP_AND_FAIL;
            if ((g_pixelFormatInitialized & 1) == 0) {
                g_pixelFormatInitialized |= 1u;
                g_pixelFormatDescriptor.cColorBits = bitDepth;
                g_pixelFormatDescriptor.cRedBits = 0;
                g_pixelFormatDescriptor.cRedShift = 0;
                g_pixelFormatDescriptor.cGreenBits = 0;
                g_pixelFormatDescriptor.cGreenShift = 0;
                g_pixelFormatDescriptor.cBlueBits = 0;
                g_pixelFormatDescriptor.cBlueShift = 0;
                g_pixelFormatDescriptor.cAlphaBits = 0;
                g_pixelFormatDescriptor.cAlphaShift = 0;
                g_pixelFormatDescriptor.cAccumBits = 0;
                g_pixelFormatDescriptor.cAccumRedBits = 0;
                g_pixelFormatDescriptor.cAccumGreenBits = 0;
                g_pixelFormatDescriptor.cAccumBlueBits = 0;
                g_pixelFormatDescriptor.cAccumAlphaBits = 0;
                g_pixelFormatDescriptor.cDepthBits = 16;
                g_pixelFormatDescriptor.cStencilBits = 1;
                g_pixelFormatDescriptor.cAuxBuffers = 0;
                g_pixelFormatDescriptor.iLayerType = 0;
                g_pixelFormatDescriptor.bReserved = 0;
                g_pixelFormatDescriptor.dwLayerMask = 0;
                g_pixelFormatDescriptor.dwVisibleMask = 0;
                g_pixelFormatDescriptor.dwDamageMask = 0;
            }
            g_hdc = GetDC(g_hWnd);
            if (!g_hdc)
                goto CLEANUP_AND_FAIL;
            pixelFormat = ChoosePixelFormat(g_hdc, &g_pixelFormatDescriptor);
            if (pixelFormat
                && SetPixelFormat(g_hdc, pixelFormat, &g_pixelFormatDescriptor)
                && (g_openglRenderingContext = wglCreateContext(g_hdc)) != 0
                && wglMakeCurrent(g_hdc, g_openglRenderingContext)
                && (ShowWindow(g_hWnd, 5),
                    SetForegroundWindow(g_hWnd),
                    SetFocus(g_hWnd),
                    SetPerspectiveProjection(windowWidth, windowHeight),
                    LoadHwfFonts(hwfStream, hwfAppendix_ptr, hwfAppendixEntriesCount))
                && (CreateFontDisplayLists(), InitLightsCullingAndInputs())) {
                return TRUE;
            } else {
            CLEANUP_AND_FAIL:
                MidiJamWindowCleanup();
                return FALSE;
            }
        }
    } else {
        PostQuitMessage(0);
        return FALSE;
    }
}

int MidiJamMain(LPCSTR lpWindowName,
                HINSTANCE hInstance,
                FILE *hwfStream,
                char *pHwfAppendix,
                unsigned int nHwfAppendixItems) {
    IMidiJamTool *MidiJamTool; // [esp+0h] [ebp-86Ch]
    DirectMusicSystem *directMusicSystem; // [esp+4h] [ebp-868h]
    void *pMidiJamTool; // [esp+8h] [ebp-864h]
    void *pDirectMusicSystem; // [esp+10h] [ebp-85Ch]
    int configBitDepth; // [esp+18h] [ebp-854h] BYREF
    int configResolution; // [esp+1Ch] [ebp-850h] BYREF
    int configRefreshRate; // [esp+20h] [ebp-84Ch] BYREF
    char configUseSound; // [esp+26h] [ebp-846h] BYREF
    char configFullscreen; // [esp+27h] [ebp-845h] BYREF
    HANDLE hConfigProcess; // [esp+28h] [ebp-844h]
    SHELLEXECUTEINFOA configInfo; // [esp+2Ch] [ebp-840h] BYREF
    char configExecutablePath[1000]; // [esp+68h] [ebp-804h] BYREF
    char configFileBuffer[1000]; // [esp+450h] [ebp-41Ch] BYREF
    HRESULT hResult; // [esp+838h] [ebp-34h]
    DWORD v20[4]; // [esp+83Ch] [ebp-30h] BYREF
    FILE *configFileStream; // [esp+840h] [ebp-2Ch]
    IDirectMusicAudioPath *DefaultAudioPath; // [esp+844h] [ebp-28h]
    int rguidNotificationType[4]; // [esp+848h] [ebp-24h] BYREF
    REFERENCE_TIME v24[8]; // [esp+858h] [ebp-14h] BYREF
    int _; // [esp+868h] [ebp-4h]

    g_useSound = 1;
    g_windowWidth = 1024;
    g_windowHeight = 768;
    g_bitDepth = 32;
    g_refreshRate = 75.0f;
    sprintf_s(configFileBuffer, "%s\\HWF.cfg", g_workingDirectory);
    configFileStream = fopen(configFileBuffer, "rb"); // rb = read binary
    if (!configFileStream) {
        // Create executable info for config.exe
        memset(&configInfo, 0, sizeof(configInfo));
        configInfo.cbSize = 60;
        sprintf(configExecutablePath, "%s\\config.exe", g_workingDirectory);
        configInfo.lpFile = configExecutablePath;
        configInfo.lpVerb = "open";
        configInfo.nShow = 1;
        configInfo.fMask = 64;
        if (ShellExecuteExA(&configInfo)) // Attempt to open config.exe
        {
            hConfigProcess = configInfo.hProcess;
            if (WaitForSingleObject(configInfo.hProcess, 0xFFFFFFFF)) // Wait for config.exe to start
            {
                MessageBoxA(nullptr, "Error waiting for program to start", "WAIT FOR CONFIG ERROR", MB_ICONASTERISK);
                return 0x2B; // FAILURE
            }
        } else {
            // Config executable info did not start
            MessageBoxA(nullptr, "Error executing config program", "WAIT FOR CONFIG ERROR", MB_ICONASTERISK);
        }
        configFileStream = fopen(configFileBuffer, "rb");
    }
    if (configFileStream) {
        fread(&configUseSound, 1u, 1u, configFileStream);
        fread(&configFullscreen, 1u, 1u, configFileStream);
        fread(&configResolution, 4u, 1u, configFileStream);
        fread(&configBitDepth, 4u, 1u, configFileStream);
        fread(&configRefreshRate, 4u, 1u, configFileStream);
        fclose(configFileStream);
        g_useSound = configUseSound == 1;
        g_fullscreen = configFullscreen == 1;
        if (configResolution) {
            switch (configResolution) {
                case 1:
                    g_windowWidth = 800;
                    g_windowHeight = 600;
                    break;
                case 2:
                    g_windowWidth = 1024;
                    g_windowHeight = 768;
                    break;
                case 3:
                    g_windowWidth = 1152;
                    g_windowHeight = 864;
                    break;
                case 4:
                    g_windowWidth = 1280;
                    g_windowHeight = 1024;
                    break;
                case 5:
                    g_windowWidth = 1600;
                    g_windowHeight = 1200;
                    break;
            }
        } else {
            g_windowWidth = 640;
            g_windowHeight = 480;
        }
        if (configBitDepth) {
            if (configBitDepth == 1)
                g_bitDepth = 32;
        } else {
            g_bitDepth = 16;
        }
        if (configRefreshRate) {
            switch (configRefreshRate) {
                case 1:
                    g_refreshRate = 70;
                    break;
                case 2:
                    g_refreshRate = 72;
                    break;
                case 3:
                    g_refreshRate = 75;
                    break;
                case 4:
                    g_refreshRate = 85;
                    break;
                case 5:
                    g_refreshRate = 100;
                    break;
                case 6:
                    g_refreshRate = 120;
                    break;
                case 7:
                    g_refreshRate = 0.0;
                    break;
            }
        } else {
            g_refreshRate = 60;
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
        g_refreshRate,
        g_fullscreen,
        hwfStream,
        pHwfAppendix,
        nHwfAppendixItems))
        return 0x2B; // FAILURE
    glClear(0x4500u);
    glFlush();
    glClear(0x4500u);
    glFlush();
    g_windowWidth_0 = g_windowWidth;
    g_windowHeight_0 = g_windowHeight;
    g_hMidiEvent = CreateEventA(nullptr, 0, 0, nullptr);
    directMusicSystem = new DirectMusicSystem();
    g_DirectMusicSystem = directMusicSystem;
    hResult = directMusicSystem->Init(g_hWnd, 128, 8);
    g_DirectMusicPerformance = g_DirectMusicSystem->GetPerformance();
    DefaultAudioPath = g_DirectMusicSystem->GetDefaultAudioPath();

    hResult = DefaultAudioPath->GetObjectInPath(
        0, // dwPChannel
        0x3200, // dwStage = DMUS_PATH_PERFORMANCE_GRAPH
        0, // dwBuffer
        GUID_NULL, // guidObject
        0, // dwIndex
        IID_IDirectMusicGraph8, // iidInterface
        reinterpret_cast<void **>(&g_DirectMusicGraph)
    );


    if (hResult < 0)
        return hResult;
    // Magic (GUID_NOTIFICATION_SEGMENT)
    rguidNotificationType[0] = 0xD2AC2899;
    rguidNotificationType[1] = 0x11D1B39B;
    rguidNotificationType[2] = 0x60000487;
    rguidNotificationType[3] = 0xBDB19308;
    hResult = (g_DirectMusicPerformance->AddNotificationType)(GUID_NOTIFICATION_SEGMENT);
    if (hResult < 0)
        return hResult;
    hResult = (g_DirectMusicPerformance->SetNotificationHandle)(g_hMidiEvent, 0);
    if (hResult < 0)
        return hResult;
    MidiJamTool = new IMidiJamTool();
    g_midiJamTool = MidiJamTool;
    hResult = (g_DirectMusicGraph->InsertTool)(MidiJamTool, 0, 0, 0);
    if (hResult < 0)
        return hResult;
    (g_DirectMusicPerformance->GetBumperLength)(v20); // unsure
    (g_DirectMusicPerformance->GetLatencyTime)(v24); // unsure
    return 0;
}

BOOL UpdateMidiJam() {
    GLfloat x_bassShadow; // [esp+3Ch] [ebp-A0h]
    GLfloat x_guitarShadow; // [esp+3Ch] [ebp-A0h]
    GLfloat x_harpShadow; // [esp+3Ch] [ebp-A0h]
    GLfloat z_bassShadow; // [esp+44h] [ebp-98h]
    GLfloat z_guitarShadow; // [esp+44h] [ebp-98h]
    __int16 m; // [esp+60h] [ebp-7Ch]
    __int16 k; // [esp+64h] [ebp-78h]
    __int16 j; // [esp+68h] [ebp-74h]
    __int16 i; // [esp+6Ch] [ebp-70h]
    GLfloat z; // [esp+70h] [ebp-6Ch]
    char headsUpDisplayText[104]; // [esp+74h] [ebp-68h] BYREF

    if (g_worldReady == 1) {
        glClear(0x4500u);
        glLoadIdentity();
        // -- SCREEN GRADIENT --
        glDisable(GL_DEPTH_TEST);
        g_screenGradient_ms3d->RenderModel();
        glEnable(GL_DEPTH_TEST);
        // -- APPLY CAMERA TRANSFORM --
        gluLookAt(
            g_cameraLocation[0],
            g_cameraLocation[1],
            g_cameraLocation[2],
            g_cameraLocation[3],
            g_cameraLocation[4],
            g_cameraLocation[5],
            0.0,
            1.0,
            0.0);
        // rotating camera
        if (g_rotatingCameraIsActive == 1) {
            glRotatef(g_rotatingCameraAngle, 0.0, 1.0, 0.0);
            if (g_rotatingCameraDirection) {
                g_rotatingCameraAngle = g_rotatingCameraAngle - 0.025f;
                if (g_rotatingCameraAngle < -20.0) {
                    g_rotatingCameraAngle = -20.0;
                    if (++g_rotatingCameraIdleTime > 500) {
                        g_rotatingCameraIdleTime = 0;
                        g_rotatingCameraDirection = CLOCKWISE;
                    }
                }
            } else {
                g_rotatingCameraAngle = g_rotatingCameraAngle + 0.025f;
                if (g_rotatingCameraAngle > 20.0) {
                    g_rotatingCameraAngle = 20.0;
                    if (++g_rotatingCameraIdleTime > 500) {
                        g_rotatingCameraIdleTime = 0;
                        g_rotatingCameraDirection = ANTI_CLOCKWISE;
                    }
                }
            }
        }
        // -- STAGE --
        glPushMatrix();
        glTranslatef(0.0, -32.0, -14.0);
        g_stage_ms3d->RenderModel();
        glPopMatrix();
        // TODO
        // // -- PIANO SHADOW --
        // if ( g_inst_visible_piano > 0 )
        // {
        //   glPushMatrix();
        //   glTranslatef(-50.0, -32.0, -20.0);
        //   glRotatef(45.0, 0.0, 1.0, 0.0);
        //   glTranslatef(0.0, 0.0, 5.0);
        //   if ( g_inst_visible_piano > 1 )
        //   {
        //     z = (g_inst_visible_piano - 1) * 0.5 + 1.0;
        //     glScalef(1.0, 1.0, z);
        //   }
        //   Ms3dBundle::RenderModel(g_pianoShadow_ms3d);
        //   glPopMatrix();
        // }
        // // -- XYLOPHONE SHADOW --
        // if ( g_inst_visible_xylophone > 0 )
        // {
        //   glPushMatrix();
        //   glTranslatef(-22.0, -32.0, 10.0);
        //   glRotatef(32.0, 0.0, 1.0, 0.0);
        //   glScalef(0.64999998, 0.64999998, 0.64999998);
        //   for ( i = 0; i < g_inst_visible_xylophone; ++i )
        //   {
        //     if ( i > 0 )
        //     {
        //       glTranslatef(-2.0, 0.0, 0.0);
        //       glRotatef(-18.0, 0.0, 1.0, 0.0);
        //       glTranslatef(0.0, 3.0, -23.0);
        //     }
        //     Ms3dBundle::RenderModel(g_xylophoneShadow_ms3d);
        //   }
        //   glPopMatrix();
        // }
        // // -- BASS SHADOW --
        // if ( g_inst_visible_bass > 0 )
        // {
        //   for ( j = 0; j < g_inst_visible_bass; ++j )
        //   {
        //     glPushMatrix();
        //     z_bassShadow = j * -5.0 + -25.0;
        //     x_bassShadow = j * 5.0 + 50.0;
        //     glTranslatef(x_bassShadow, -32.0, z_bassShadow);
        //     glRotatef(-45.0, 0.0, 1.0, 0.0);
        //     Ms3dBundle::RenderModel(g_bassShadow_ms3d);
        //     glPopMatrix();
        //   }
        // }
        // // -- GUITAR SHADOW --
        // if ( g_inst_visible_guitar > 0 )
        // {
        //   for ( k = 0; k < g_inst_visible_guitar; ++k )
        //   {
        //     glPushMatrix();
        //     z_guitarShadow = k * -5.0 + -5.0;
        //     x_guitarShadow = k * 5.0 + 43.0;
        //     glTranslatef(x_guitarShadow, -32.0, z_guitarShadow);
        //     glRotatef(-50.0, 0.0, 1.0, 0.0);
        //     Ms3dBundle::RenderModel(g_guitarShadow_ms3d);
        //     glPopMatrix();
        //   }
        // }
        // // -- DRUM SHADOW --
        // if ( g_inst_visible_drumset > 0 )
        // {
        //   glPushMatrix();
        //   glTranslatef(0.0, -32.0, -95.0);
        //   Ms3dBundle::RenderModel(g_drumShadow_ms3d);
        //   glPopMatrix();
        // }
        // // -- HARP SHADOW --
        // if ( g_inst_visible_harp > 0 )
        // {
        //   for ( m = 0; m < g_inst_visible_harp; ++m )
        //   {
        //     glPushMatrix();
        //     x_harpShadow = m * 16.0 + 7.0;
        //     glTranslatef(x_harpShadow, -32.0, 5.0);
        //     glRotatef(-33.0, 0.0, 1.0, 0.0);
        //     Ms3dBundle::RenderModel(g_harpShadow_ms3d);
        //     glPopMatrix();
        //   }
        // }
        // if ( g_ds_harp )
        // {
        //   glPushMatrix();
        //   glTranslatef(7.0, -28.4, 5.0);
        //   glRotatef(-33.0, 0.0, 1.0, 0.0);
        //   I_Harp();
        //   glPopMatrix();
        // }
        // if ( g_ds_trombone )
        //   I_Trombone();
        // if ( g_ds_trumpet )
        //   I_Trumpet();
        // if ( g_ds_frenchHorn )
        //   I_FrenchHorn();
        // if ( g_ds_baritoneSax )
        //   I_BaritoneSax();
        // if ( g_ds_tenorSax )
        //   I_TenorSax();
        // if ( g_ds_sapranoSax )
        //   I_SapranoSax();
        // if ( g_ds_altoSax )
        //   I_AltoSax();
        // if ( g_ds_recorder )
        //   I_Recorder();
        // if ( g_ds_piccolo )
        //   I_Piccolo();
        // if ( g_ds_flute )
        //   I_Flute();
        // if ( g_ds_tuba )
        //   I_Tuba();
        // if ( g_ds_stageHorn )
        //   I_StageHorn();
        // if ( g_ds_whistles )
        //   I_Whistles();
        // if ( g_ds_panPipe )
        //   I_PanPipe();
        // if ( g_ds_harmonica )
        //   I_Harmonica();
        // if ( g_ds_violin )
        //   I_Violin();
        // if ( g_ds_viola )
        //   I_Viola();
        // if ( g_ds_cello )
        //   I_Cello();
        // if ( g_ds_doubleBass )
        //   I_DoubleBass();
        // if ( g_ds_popBottle )
        //   I_PopBottle();
        // if ( g_ds_agogos )
        //   I_Agogo();
        // if ( g_ds_woodblocks )
        //   I_Woodblock();
        // if ( g_ds_stageChoir )
        //   I_StageChoir();
        // if ( g_ds_accordion )
        //   I_Accordion();
        // if ( g_ds_stageStrings )
        //   I_StageStrings();
        // if ( g_ds_pizzicatoStrings )
        //   I_PizzicatoStrings();
        // if ( g_ds_ocarina )
        //   I_Ocarina();
        // if ( g_ds_xylophone )
        // {
        //   glPushMatrix();
        //   glTranslatef(-22.0, -10.0, 10.0);
        //   glRotatef(32.0, 0.0, 1.0, 0.0);
        //   glScalef(0.64999998, 0.64999998, 0.64999998);
        //   I_Xylophone();
        //   glPopMatrix();
        // }
        // if ( g_ds_musicBox )
        //   I_MusicBox();
        // if ( g_ds_piano )
        // {
        //   glPushMatrix();
        //   glTranslatef(-50.0, 0.0, -20.0);
        //   glRotatef(45.0, 0.0, 1.0, 0.0);
        //   I_Piano();
        //   glPopMatrix();
        // }
        // if ( g_show_percussion == 1 )
        // {
        //   glPushMatrix();
        //   glTranslatef(0.0, -32.0, -95.0);
        //   RenderPercussion();
        //   glPopMatrix();
        // }
        // if ( g_ds_melodicTom )
        //   I_MelodicTom();
        // if ( g_ds_synthDrum )
        //   I_SynthDrum();
        // if ( g_ds_steelDrum )
        //   I_SteelDrum();
        // if ( g_ds_timpani )
        //   I_Timpani();
        // if ( g_ds_taiko )
        //   I_Taiko();
        // if ( g_ds_telephone )
        //   I_Telephone();
        // if ( g_ds_tubularBells )
        //   I_TubularBells();
        // if ( g_ds_guitar )
        // {
        //   glPushMatrix();
        //   glTranslatef(43.0, 4.0, -5.0);
        //   glRotatef(-50.0, 0.0, 1.0, 0.0);
        //   glRotatef(-60.0, 0.0, 0.0, 1.0);
        //   I_Guitar();
        //   glPopMatrix();
        // }
        // if ( g_ds_bass )
        // {
        //   glPushMatrix();
        //   glTranslatef(50.0, 24.0, -25.0);
        //   glRotatef(-45.0, 0.0, 1.0, 0.0);
        //   glRotatef(-30.0, 0.0, 0.0, 1.0);
        //   I_Bass();
        //   glPopMatrix();
        // }
        //  -- HEADS UP DISPLAY --
        sprintf_s(headsUpDisplayText, "%s fps:%0.2f", g_midiFileNameDisp, g_framesPerSecond);
        // -- SONG FILLBAR BOX --
        glPushMatrix();
        glLoadIdentity();
        g_songFillbarBox_ms3d->RenderModel();
        // -- SONG FILLBAR --
        glTranslatef(-122.087, 0.0, 0.0);
        glScalef(g_songFillbarScale, 1.0, 1.0);
        g_songFillbar_ms3d->RenderModel();
        glPopMatrix();
        //  -- HUD TEXT --
        RenderTextWithShadow(75, 460, 3, 3, headsUpDisplayText, 1, 0.5, 0.5);
        if (g_fadeFactor != 0.0) {
            glPushMatrix();
            glLoadIdentity();
            gluLookAt(0.0, 0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            // RenderFadeout();
            glPopMatrix();
        }
        glFlush();
    }
    return TRUE;
}

void SetPerspectiveProjection(int windowWidth, int windowHeight) {
    // Prevent division by 0
    if (!windowHeight)
        windowHeight = 1;

    const float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, windowWidth, windowHeight);
    gluPerspective(45.0, aspectRatio, 0.1000000014901161, 4000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MidiJamWindowCleanup() {
    if (g_fullscreen) {
        ChangeDisplaySettingsA(NULL, 0);
        ShowCursor(TRUE);
    }
    if (g_openglRenderingContext) {
        if (!wglMakeCurrent(NULL, NULL))
            MessageBoxA(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        if (!wglDeleteContext(g_openglRenderingContext))
            MessageBoxA(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        g_openglRenderingContext = NULL;
    }
    if (g_hdc && !ReleaseDC(g_hWnd, g_hdc)) {
        MessageBoxA(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        g_hdc = NULL;
    }
    if (g_hWnd && !DestroyWindow(g_hWnd)) {
        MessageBoxA(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        g_hWnd = NULL;
    }
    if (!UnregisterClassA("OpenGL", g_hInstance)) {
        MessageBoxA(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        g_hInstance = NULL;
    }
    DeleteFontDisplayLists();
    PostQuitMessage(0);
}

BOOL InitLightsCullingAndInputs() {
    glClearColor(0.0, 0.0, 0.0, 0.5);
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

void __stdcall UpdateMidiJamMM(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) {
    /* TODO */
    double v5; // st7
    int velocity; // [esp+24h] [ebp-44h]
    __int16 cymbalIndex; // [esp+30h] [ebp-38h]
    float velocityFactor; // [esp+34h] [ebp-34h]
    __int16 i; // [esp+38h] [ebp-30h]
    float currentTimeMs; // [esp+3Ch] [ebp-2Ch]
    REFERENCE_TIME prtNow; // [esp+40h] [ebp-28h] BYREF
    __int16 anyPercussionActive; // [esp+4Ch] [ebp-1Ch]
    MUSIC_TIME pmtNow; // [esp+50h] [ebp-18h] BYREF
    DWORD globalTimeMs; // [esp+54h] [ebp-14h]
    __int16 patch; // [esp+58h] [ebp-10h]
    float recoil_scale_factor; // [esp+5Ch] [ebp-Ch]
    __int16 anyInstrumentActive; // [esp+60h] [ebp-8h]
    __int16 cameraTransformComponentsAtTarget; // [esp+64h] [ebp-4h]

    if (g_worldReady == 1) // this function no-ops if world not ready
    {
        //     recoil_scale_factor = RECOIL_SCALE_FACTOR * 0.25;
        prtNow = 0;
        pmtNow = 0;
        //     if ( DirectMusicSegmentWrapper::IsPlaying(g_DirectMusicSegmentWrapper) )
        //     {
        //       (g_DirectMusicPerformance->vtable->GetTime)(g_DirectMusicPerformance, &prtNow, &pmtNow);
        //       if ( g_songFillbarScale < 1.0 )
        //       {
        //         g_songFillbarScale = (pmtNow - g_mtStart) / g_midiFile_duration;
        //         if ( g_songFillbarScale > 1.0 )
        //           g_songFillbarScale = 1.0;             // limit scale to 100%
        //       }
        //     }
        //     if ( ++g_isEvenFrame > 1 )
        //     {
        //       g_isEvenFrame = 0;
        //       if ( ++g_vibratingString_frameIndex >= 8 )
        //         g_vibratingString_frameIndex = 0;
        //       g_vibratingString_frame = VIBRATING_STRING_ANIM_SEQUENCE[g_vibratingString_frameIndex];
        //     }
        //     if ( !g_time_global_current )
        //       g_time_global_current = pmtNow;
        //     if ( g_isShuttingDown == 1 )
        //       pmtNow = g_time_global_current;
        anyInstrumentActive = 0;
        //     I_Accordion_MM(pmtNow);
        //     if ( g_ds_harp && I_Harp_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_piano && I_Piano_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_xylophone && I_Xylophone_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_violin && I_Violin_MM(pmtNow) == 1 )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_viola && I_Viola_MM(pmtNow) == 1 )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_cello && I_Cello_MM(pmtNow) == 1 )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_doubleBass && I_DoubleBass_MM(pmtNow) == 1 )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_bass && I_Bass_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_guitar && I_Guitar_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_stageHorn && I_StageHorn_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_whistles && I_Whistles_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     UpdateSteamPuffers();
        //     if ( g_ds_panPipe && I_PanPipe_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_harmonica && I_Harmonica_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_popBottle && I_PopBottle_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_agogos && I_Agogo_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_woodblocks && I_Woodblocks_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_trombone && I_Trombone_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_tuba && I_Tuba_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_frenchHorn && I_FrenchHorn_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_ocarina && I_Ocarina_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_baritoneSax && I_BaritoneSax_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_tenorSax && I_TenorSax_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_sapranoSax && I_SapranoSax_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_altoSax && I_AltoSax_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_flute && I_Flute_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_piccolo && I_Piccolo_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_recorder && I_Recorder_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_trumpet && I_Trumpet_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_stageChoir && I_StageChoir_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_musicBox && I_MusicBox_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_melodicTom && I_MelodicTom_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_synthDrum && I_SynthDrum_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_steelDrum && I_SteelDrum_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_timpani && I_Timpani_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_taiko && I_Taiko_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_telephone && I_Telephone_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_tubularBells && I_TubularBells_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_stageStrings && I_StageStrings_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_ds_pizzicatoStrings && I_PizzicatoStrings_MM(pmtNow) )
        //       anyInstrumentActive = 1;
        //     UpdateAllRecoils(recoil_scale_factor);
        //     anyPercussionActive = 0;
        //     if ( g_show_percussion == 1 )
        //     {
        //       for ( patch = 0; patch < 88; ++patch )
        //       {
        //         for ( i = 0; i < 32; ++i )
        //         {
        //           if ( g_percussion_time_queue[patch][i] > 0 )
        //           {
        //             anyInstrumentActive = 1;
        //             anyPercussionActive = 1;
        //             g_percussion_framesWithEmptyQueue = 0;
        //             g_percussion_time_queue[patch][i] -= pmtNow - g_time_global_current;
        //             if ( g_percussion_time_queue[patch][i] <= 0 )
        //             {
        //               g_percussion_time_queue[patch][i] = 0;
        //               velocity = g_percussion_velocity_queue[patch][i];
        //               velocityFactor = (velocity * 0.75 + 32.0 + velocity * 0.75 + 32.0) / 128.0;
        //               switch ( patch )
        //               {
        //                 case ACOUSTIC_SNARE:
        //                 case ELECTRIC_SNARE:
        //                   g_recoil_snaredrum = g_recoil_snaredrum + velocityFactor;
        //                   if ( g_recoil_snaredrum > MAX_RECOIL )
        //                     g_recoil_snaredrum = MAX_RECOIL;
        //                   g_recoil_snare = g_recoil_snare + velocityFactor;
        //                   if ( g_recoil_snare > MAX_RECOIL )
        //                     g_recoil_snare = MAX_RECOIL;
        //                   break;
        //                 case SIDE_STICK:
        //                   g_recoil_snaredrum = velocityFactor * 0.25 + g_recoil_snaredrum;
        //                   if ( g_recoil_snaredrum > MAX_RECOIL )
        //                     g_recoil_snaredrum = MAX_RECOIL;
        //                   g_recoil_sideStick = g_recoil_sideStick + velocityFactor;
        //                   if ( g_recoil_sideStick > MAX_RECOIL )
        //                     g_recoil_sideStick = MAX_RECOIL;
        //                   break;
        //                 case HIGH_TOM:
        //                   g_recoil_tom[0] = g_recoil_tom[0] + velocityFactor;
        //                   if ( g_recoil_tom[0] > MAX_RECOIL )
        //                     g_recoil_tom[0] = MAX_RECOIL;
        //                   break;
        //                 case HIGH_MID_TOM:
        //                   g_recoil_tom[1] = g_recoil_tom[1] + velocityFactor;
        //                   if ( g_recoil_tom[1] > MAX_RECOIL )
        //                     g_recoil_tom[1] = MAX_RECOIL;
        //                   break;
        //                 case LOW_MID_TOM:
        //                   g_recoil_tom[2] = g_recoil_tom[2] + velocityFactor;
        //                   if ( g_recoil_tom[2] > MAX_RECOIL )
        //                     g_recoil_tom[2] = MAX_RECOIL;
        //                   break;
        //                 case LOW_TOM:
        //                   g_recoil_tom[3] = g_recoil_tom[3] + velocityFactor;
        //                   if ( g_recoil_tom[3] > MAX_RECOIL )
        //                     g_recoil_tom[3] = MAX_RECOIL;
        //                   break;
        //                 case HIGH_FLOOR_TOM:
        //                   g_recoil_tom[4] = g_recoil_tom[4] + velocityFactor;
        //                   if ( g_recoil_tom[4] > MAX_RECOIL )
        //                     g_recoil_tom[4] = MAX_RECOIL;
        //                   break;
        //                 case LOW_FLOOR_TOM:
        //                   g_recoil_tom[5] = g_recoil_tom[5] + velocityFactor;
        //                   if ( g_recoil_tom[5] > MAX_RECOIL )
        //                     g_recoil_tom[5] = MAX_RECOIL;
        //                   break;
        //                 case ACOUSTIC_BASS_DRUM:
        //                 case ELECTRIC_BASS_DRUM:
        //                   g_recoil_bassDrumArm = 0.0;
        //                   g_recoil_bassdrum = g_recoil_bassdrum + velocityFactor;
        //                   if ( g_recoil_bassdrum > MAX_RECOIL )
        //                     g_recoil_bassdrum = MAX_RECOIL;
        //                   break;
        //                 case COWBELL:
        //                   g_recoil_cowbell = g_recoil_cowbell + velocityFactor;
        //                   if ( g_recoil_cowbell > MAX_RECOIL )
        //                     g_recoil_cowbell = MAX_RECOIL;
        //                   break;
        //                 case HAND_CLAP:
        //                   g_recoil_clap_hand_r = g_recoil_clap_hand_r + velocityFactor;
        //                   if ( g_recoil_clap_hand_r > MAX_RECOIL )
        //                     g_recoil_clap_hand_r = MAX_RECOIL;
        //                   g_recoil_clap_hand_l = g_recoil_clap_hand_l + velocityFactor;
        //                   if ( g_recoil_clap_hand_l > MAX_RECOIL )
        //                     g_recoil_clap_hand_l = MAX_RECOIL;
        //                   break;
        //                 case TAMBOURINE:
        //                   g_recoil_tambourine_hand = g_recoil_tambourine_hand + velocityFactor;
        //                   if ( g_recoil_tambourine_hand > MAX_RECOIL )
        //                     g_recoil_tambourine_hand = MAX_RECOIL;
        //                   g_recoil_tambourine = g_recoil_tambourine + velocityFactor;
        //                   if ( g_recoil_tambourine > MAX_RECOIL )
        //                     g_recoil_tambourine = MAX_RECOIL;
        //                   break;
        //                 case STICKS:
        //                   g_recoil_sticks_2 = g_recoil_sticks_2 + velocityFactor;
        //                   if ( g_recoil_sticks_2 > MAX_RECOIL )
        //                     g_recoil_sticks_2 = MAX_RECOIL;
        //                   g_recoil_sticks_1 = g_recoil_sticks_1 + velocityFactor;
        //                   if ( g_recoil_sticks_1 > MAX_RECOIL )
        //                     g_recoil_sticks_1 = MAX_RECOIL;
        //                   break;
        //                 case CLAVES:
        //                   g_recoil = g_recoil + velocityFactor;
        //                   if ( g_recoil > MAX_RECOIL )
        //                     g_recoil = MAX_RECOIL;
        //                   g_recoil_clave_l = g_recoil_clave_l + velocityFactor;
        //                   if ( g_recoil_clave_l > MAX_RECOIL )
        //                     g_recoil_clave_l = MAX_RECOIL;
        //                   break;
        //                 case JINGLE_BELL:
        //                   g_recoil_jingleBell = g_recoil_jingleBell + velocityFactor;
        //                   if ( g_recoil_jingleBell > MAX_RECOIL )
        //                     g_recoil_jingleBell = MAX_RECOIL;
        //                   break;
        //                 case CASTANETS:
        //                   g_recoil_castanets = g_recoil_castanets + velocityFactor;
        //                   if ( g_recoil_castanets > MAX_RECOIL )
        //                     g_recoil_castanets = MAX_RECOIL;
        //                   break;
        //                 case SHAKER:
        //                   g_recoil_shaker = g_recoil_shaker + velocityFactor;
        //                   if ( g_recoil_shaker > MAX_RECOIL )
        //                     g_recoil_shaker = MAX_RECOIL;
        //                   break;
        //                 case HIGH_Q:
        //                   g_recoil_highQ = g_recoil_highQ + velocityFactor;
        //                   if ( g_recoil_highQ > MAX_RECOIL )
        //                     g_recoil_highQ = MAX_RECOIL;
        //                   break;
        //                 case SQUARE_CLICK:
        //                   g_recoil_squareClick = g_recoil_squareClick + velocityFactor;
        //                   if ( g_recoil_squareClick > MAX_RECOIL )
        //                     g_recoil_squareClick = MAX_RECOIL;
        //                   break;
        //                 case METRONOME_CLICK:
        //                   g_recoil_metronomeClick = g_recoil_metronomeClick + velocityFactor;
        //                   if ( g_recoil_metronomeClick > MAX_RECOIL )
        //                     g_recoil_metronomeClick = MAX_RECOIL;
        //                   break;
        //                 case METRONOME_BELL:
        //                   g_recoil_metronomeBell = g_recoil_metronomeBell + velocityFactor;
        //                   if ( g_recoil_metronomeBell > MAX_RECOIL )
        //                     g_recoil_metronomeBell = MAX_RECOIL;
        //                   break;
        //                 case MARACAS:
        //                   g_recoil_Maracas = g_recoil_Maracas + velocityFactor;
        //                   if ( g_recoil_Maracas > MAX_RECOIL )
        //                     g_recoil_Maracas = MAX_RECOIL;
        //                   break;
        //                 case CABASA:
        //                   g_recoil_cabasa = g_recoil_cabasa + velocityFactor;
        //                   if ( g_recoil_cabasa > MAX_RECOIL )
        //                     g_recoil_cabasa = MAX_RECOIL;
        //                   break;
        //                 case HIGH_AGOGO:
        //                   g_recoil_agogo_hi = g_recoil_agogo_hi + velocityFactor;
        //                   if ( g_recoil_agogo_hi > MAX_RECOIL )
        //                     g_recoil_agogo_hi = MAX_RECOIL;
        //                   break;
        //                 case LOW_AGOGO:
        //                   g_recoil_agogo_lo = g_recoil_agogo_lo + velocityFactor;
        //                   if ( g_recoil_agogo_lo > MAX_RECOIL )
        //                     g_recoil_agogo_lo = MAX_RECOIL;
        //                   break;
        //                 case HIGH_WOODBLOCK:
        //                   g_recoil_woodblock_hi = g_recoil_woodblock_hi + velocityFactor;
        //                   if ( g_recoil_woodblock_hi > MAX_RECOIL )
        //                     g_recoil_woodblock_hi = MAX_RECOIL;
        //                   break;
        //                 case OPEN_TRIANGLE:
        //                   g_recoil_triangle_open = g_recoil_triangle_open + velocityFactor;
        //                   if ( g_recoil_triangle_open > MAX_RECOIL )
        //                     g_recoil_triangle_open = MAX_RECOIL;
        //                   break;
        //                 case MUTE_TRIANGLE:
        //                   g_recoil_triangle_muted = g_recoil_triangle_muted + velocityFactor;
        //                   if ( g_recoil_triangle_muted > MAX_RECOIL )
        //                     g_recoil_triangle_muted = MAX_RECOIL;
        //                   break;
        //                 case LONG_WHISTLE:
        //                   g_recoil_whistle_long = MAX_RECOIL;
        //                   break;
        //                 case SHORT_WHISTLE:
        //                   g_recoil_whistle_short = MAX_RECOIL;
        //                   break;
        //                 case LOW_WOODBLOCK:
        //                   g_recoil_woodblock_lo = g_recoil_woodblock_lo + velocityFactor;
        //                   if ( g_recoil_woodblock_lo > MAX_RECOIL )
        //                     g_recoil_woodblock_lo = MAX_RECOIL;
        //                   break;
        //                 case HIGH_TIMBALE:
        //                   g_recoil_timbale_hi = g_recoil_timbale_hi + velocityFactor;
        //                   if ( g_recoil_timbale_hi > MAX_RECOIL )
        //                     g_recoil_timbale_hi = MAX_RECOIL;
        //                   break;
        //                 case LOW_TIMBALE:
        //                   g_recoil_timbale_lo = g_recoil_timbale_lo + velocityFactor;
        //                   if ( g_recoil_timbale_lo > MAX_RECOIL )
        //                     g_recoil_timbale_lo = MAX_RECOIL;
        //                   break;
        //                 case HIGH_BONGO:
        //                   g_recoil_bongo_hi = g_recoil_bongo_hi + velocityFactor;
        //                   if ( g_recoil_bongo_hi > MAX_RECOIL )
        //                     g_recoil_bongo_hi = MAX_RECOIL;
        //                   break;
        //                 case LOW_BONGO:
        //                   g_recoil_bongo_lo = g_recoil_bongo_lo + velocityFactor;
        //                   if ( g_recoil_bongo_lo > MAX_RECOIL )
        //                     g_recoil_bongo_lo = MAX_RECOIL;
        //                   break;
        //                 case MUTE_HIGH_CONGA:
        //                   g_recoil_conga_high_mute = g_recoil_conga_high_mute + velocityFactor;
        //                   if ( g_recoil_conga_high_mute > MAX_RECOIL )
        //                     g_recoil_conga_high_mute = MAX_RECOIL;
        //                   break;
        //                 case OPEN_HIGH_CONGA:
        //                   g_recoil_conga_high_open = g_recoil_conga_high_open + velocityFactor;
        //                   if ( g_recoil_conga_high_open > MAX_RECOIL )
        //                     g_recoil_conga_high_open = MAX_RECOIL;
        //                   break;
        //                 case LOW_CONGA:
        //                   g_recoil_conga_low = g_recoil_conga_low + velocityFactor;
        //                   if ( g_recoil_conga_low > MAX_RECOIL )
        //                     g_recoil_conga_low = MAX_RECOIL;
        //                   break;
        //                 case PEDAL_HI_HAT:
        //                   g_hihat_isOpen = 0;
        //                   g_recoil_hihat = g_recoil_hihat + velocityFactor;
        //                   if ( g_recoil_hihat > MAX_RECOIL )
        //                     g_recoil_hihat = MAX_RECOIL;
        //                   break;
        //                 case CLOSED_HI_HAT:
        //                   g_hihat_isOpen = 0;
        //                   g_recoil_hihat = g_recoil_hihat + velocityFactor;
        //                   if ( g_recoil_hihat > MAX_RECOIL )
        //                     g_recoil_hihat = MAX_RECOIL;
        //                   break;
        //                 default:
        //                   for ( cymbalIndex = 0; cymbalIndex < 7; ++cymbalIndex )
        //                   {
        //                     if ( patch == g_cymbal_patches[cymbalIndex]
        //                       || patch == RIDE_BELL
        //                       && (g_cymbal_patches[cymbalIndex] == RIDE_CYMBAL_1 && g_lastRideCymbal != 2
        //                        || g_cymbal_patches[cymbalIndex] == RIDE_CYMBAL_2 && g_lastRideCymbal == 2) )
        //                     {
        //                       switch ( patch )
        //                       {
        //                         case RIDE_CYMBAL_1:
        //                           g_lastRideCymbal = 1;
        //                           break;
        //                         case RIDE_CYMBAL_2:
        //                           g_lastRideCymbal = 2;
        //                           break;
        //                         case OPEN_HI_HAT:
        //                           g_hihat_isOpen = 1;
        //                           g_recoil_hihat = g_recoil_hihat + velocityFactor;
        //                           if ( g_recoil_hihat > MAX_RECOIL )
        //                             g_recoil_hihat = MAX_RECOIL;
        //                           break;
        //                       }
        //                       if ( patch == RIDE_BELL )
        //                       {
        //                         g_recoil_cymbals[cymbalIndex] = g_recoil_cymbals[cymbalIndex] + CYMBAL_MASS_FACTOR[cymbalIndex];
        //                         g_cymbalMassFactor[cymbalIndex] = CYMBAL_SCALE[cymbalIndex] + 13.75;
        //                         if ( g_cymbalCurrentWobble[cymbalIndex] < 0.0 )
        //                           g_cymbalCurrentWobble[cymbalIndex] = 0.0;
        //                         g_cymbalCurrentWobble[cymbalIndex] = g_percussion_velocity_queue[patch][i]
        //                                                            * CYMBAL_MASS_FACTOR[cymbalIndex]
        //                                                            / 128.0
        //                                                            * 0.75
        //                                                            + g_cymbalCurrentWobble[cymbalIndex];
        //                         g_cymbalAngularVelocity[cymbalIndex] = g_percussion_velocity_queue[patch][i]
        //                                                              * CYMBAL_ANGULAR_VELOCITY_FACTOR[cymbalIndex]
        //                                                              / 128.0
        //                                                              * 0.75;
        //                       }
        //                       else
        //                       {
        //                         g_recoil_cymbals[cymbalIndex] = g_recoil_cymbals[cymbalIndex] + CYMBAL_MASS_FACTOR[cymbalIndex];
        //                         v5 = CYMBAL_SCALE[cymbalIndex];
        //                         g_cymbalMassFactor[cymbalIndex] = v5 + v5 + 16.75;
        //                         if ( g_cymbalCurrentWobble[cymbalIndex] < 0.0 )
        //                           g_cymbalCurrentWobble[cymbalIndex] = 0.0;
        //                         // cymbal was hit, applying full wobble
        //                         g_cymbalCurrentWobble[cymbalIndex] = g_percussion_velocity_queue[patch][i]
        //                                                            * CYMBAL_MASS_FACTOR[cymbalIndex]
        //                                                            / 128.0
        //                                                            + g_cymbalCurrentWobble[cymbalIndex];
        //                         g_cymbalAngularVelocity[cymbalIndex] = g_percussion_velocity_queue[patch][i]
        //                                                              * CYMBAL_ANGULAR_VELOCITY_FACTOR[cymbalIndex]
        //                                                              / 128.0;
        //                       }
        //                       if ( g_cymbalAngularVelocity[cymbalIndex] < 0.0 )
        //                         g_cymbalAngularVelocity[cymbalIndex] = CYMBAL_ANGULAR_VELOCITY_FACTOR[cymbalIndex] * 0.25;
        //                       if ( g_cymbalAngularVelocity[cymbalIndex] > CYMBAL_ANGULAR_VELOCITY_FACTOR[cymbalIndex] )
        //                         g_cymbalAngularVelocity[cymbalIndex] = CYMBAL_ANGULAR_VELOCITY_FACTOR[cymbalIndex];
        //                       if ( g_cymbalCurrentWobble[cymbalIndex] > CYMBAL_MAX_WOBBLE[cymbalIndex] )
        //                         g_cymbalCurrentWobble[cymbalIndex] = CYMBAL_MAX_WOBBLE[cymbalIndex];
        //                       if ( g_recoil_cymbals[cymbalIndex] > CYMBAL_MAX_WOBBLE[cymbalIndex] )
        //                         g_recoil_cymbals[cymbalIndex] = CYMBAL_MAX_WOBBLE[cymbalIndex];
        //                       g_cymbalWobbleAmplitude[cymbalIndex] = CYMBAL_WOBBLE_AMPLITUDE[cymbalIndex];
        //                       goto LABEL_135;           // Break for-loop
        //                     }
        //                   }
        //                   break;
        //               }
        //             }
        //           }
        // LABEL_135:
        //           ;
        //         }
        //       }
        //       if ( !anyPercussionActive && ++g_percussion_framesWithEmptyQueue > 1000 )
        //       {
        //         g_show_percussion = 0;
        //         g_inst_visible_drumset = 0;
        //       }
        //     }
        if (g_songFillbarScale >= 1.0 && /* TODO !anyInstrumentActive*/ true && !g_isShuttingDown) {
            g_fadeFactor = 0.0;
            g_isShuttingDown = 1;
        }
        g_time_global_current = pmtNow;
        ++g_framesAlive;
        // HandleKeyPresses();
        globalTimeMs = timeGetTime();
        currentTimeMs = (globalTimeMs - g_applicationStartTime);
        recoil_scale_factor = currentTimeMs / RECOIL_SCALE_FACTOR; // wtf?
        g_applicationStartTime = globalTimeMs;
        // FADE IN
        if (g_isFadingIn == 1) {
            g_fadeFactor = g_fadeFactor - RECOIL_SCALE_FACTOR * 0.0024999999;
            if (g_fadeFactor <= 0.0) {
                g_fadeFactor = 0.0;
                g_isFadingIn = 0;
                g_isShuttingDown = 0;
                PlaySegment();
            }
        }
        // FADE OUT
        else if (g_isShuttingDown == 1) {
            g_fadeFactor = RECOIL_SCALE_FACTOR * 0.0024999999 + g_fadeFactor;
            if (g_fadeFactor >= 1.0) {
                g_fadeFactor = 1.0;
                g_isFadingIn = 0;
                g_isShuttingDown = 0;
                g_killApplication = 1;
            }
        }
        // if ( g_killApplication == 1 )
        //   g_killApplication_0 = 1;
        //     cameraTransformComponentsAtTarget = 0;
        //     // check if auto cam is at target transform
        //     for ( patch = 0; patch < 6; ++patch )
        //     {
        //       if ( g_cameraLocation[patch] >= *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch) )
        //       {
        //         if ( g_cameraLocation[patch] <= *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch) )
        //         {
        //           if ( g_cameraLocation[patch] == *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch) )
        //             ++cameraTransformComponentsAtTarget;
        //         }
        //         else
        //         {
        //           // adjust camera transform to reach target
        //           g_cameraLocation[patch] = g_cameraLocation[patch] + g_autoCamDeltaTransform[patch];
        //           // check for equality
        //           if ( g_cameraLocation[patch] <= *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch) )
        //           {
        //             g_cameraLocation[patch] = *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch);
        //             ++cameraTransformComponentsAtTarget;
        //           }
        //         }
        //       }
        //       else
        //       {
        //         // adjust camera transform to reach target
        //         g_cameraLocation[patch] = g_cameraLocation[patch] + g_autoCamDeltaTransform[patch];
        //         // check for equality
        //         if ( g_cameraLocation[patch] >= *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch) )
        //         {
        //           g_cameraLocation[patch] = *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch);
        //           ++cameraTransformComponentsAtTarget;
        //         }
        //       }
        //     }
        //     if ( cameraTransformComponentsAtTarget == 6 )
        //       g_autoCamIsIdle = 1;
        //     ++g_framesSinceStart;
        //     if ( !(++g_autoCamIdleTime % 1000)
        //       || !(g_autoCamIdleTime % 200)
        //       && ((g_targetCameraAngle == CAMERA_2A || g_targetCameraAngle == CAMERA_2B) && !IsCameraAngleViable_2()
        //        || (g_targetCameraAngle == CAMERA_3A || g_targetCameraAngle == CAMERA_3B) && !IsCameraAngleViable_3()
        //        || (g_targetCameraAngle == CAMERA_4A || g_targetCameraAngle == CAMERA_4B) && !IsCameraAngleViable_4()
        //        || g_targetCameraAngle == CAMERA_6 && !IsCameraAngleViable_6()) )
        //     {
        //       TriggerAutoCam();
        //     }
    }
}

__time32_t GetUnixEpochTime(__time32_t *const Time) {
    // rax
    _FILETIME SystemTimeAsFileTime; // [esp+0h] [ebp-8h] BYREF

    GetSystemTimeAsFileTime(&SystemTimeAsFileTime);

    // Convert FILETIME to 64-bit integer
    const unsigned __int64 fileTime64 = (static_cast<unsigned __int64>(SystemTimeAsFileTime.dwHighDateTime) << 32)
                                        | SystemTimeAsFileTime.dwLowDateTime;

    const unsigned __int64 v1 = (fileTime64 - 116444736000000000ULL) / 0x989680ULL;
    if (Time)
        *Time = v1;
    return v1;
}

HRESULT PlaySegment() {
    HRESULT hrSetRepeats; // [esp+0h] [ebp-8h]
    int hrPlaySegment; // [esp+0h] [ebp-8h]

    hrSetRepeats = g_DirectMusicSegmentWrapper->SetRepeats(0);
    if (hrSetRepeats < 0)
        return hrSetRepeats;
    g_DirectMusicPerformance->GetTime(&g_prtStart, &g_mtStart);
    hrPlaySegment = g_DirectMusicSegmentWrapper->PlaySegment(0x800000, 0);
    if (hrPlaySegment >= 0)
        return 0;
    return hrPlaySegment;
}

void MidiJamInitialize() {
    /* TODO */
    double cymbalScale; // st7
    __int16 queue; // [esp+10h] [ebp-8h]
    __int16 i; // [esp+14h] [ebp-4h]
    __int16 j; // [esp+14h] [ebp-4h]
    __int16 k; // [esp+14h] [ebp-4h]
    __int16 m; // [esp+14h] [ebp-4h]
    __int16 n; // [esp+14h] [ebp-4h]
    __int16 patch; // [esp+14h] [ebp-4h]
    __int16 slot; // [esp+14h] [ebp-4h]

    g_fadeFactor = 1.0;
    g_isFadingIn = 1;
    g_targetCameraAngle = CAMERA_1A;
    g_rotatingCameraIsActive = 0;
    for (i = 0; i < 6; ++i)
        g_cameraLocation[i] = *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + i);
    // g_percussion_framesWithEmptyQueue = 0;
    // // define harp string colors
    // for (j = 0; j < 47; ++j) {
    //     if (j % 7) {
    //         if (j % 7 == 3)
    //             g_harpStringColors[j] = Blue;
    //         else
    //             g_harpStringColors[j] = White;
    //     } else {
    //         g_harpStringColors[j] = Red;
    //     }
    // }
    // // define harp string scales
    // for (k = 0; k < 47; ++k) {
    //     HARP_STRING_SCALE[k] = HARP_STRING_SCALE[k] - (k / 47.0 * 4.5 + 1.0);
    //     *&g_harp_string_y[k] = k / 47.0 * 42.0 + 4.7379999;
    //     g_harp_string_z[k] = -k * 0.75 - 4.0;
    //     g_harp_string_scale[k] = ((1.0 - k / 47.0) * 42.0 + HARP_STRING_SCALE[k] - 42.0) / 72.0;
    // }
    // for (m = 0; m < 7; ++m) {
    //     cymbalScale = CYMBAL_SCALE[m];
    //     g_cymbalMassFactor[m] = cymbalScale + cymbalScale + 16.75;
    //     g_recoil_cymbals[m] = 0.0;
    //     g_cymbalCurrentWobble[m] = 0.0;
    //     g_cymbalAngularVelocity[m] = 0.0;
    //     g_cymbalWobbleAmplitude[m] = 0.0;
    // }
    // g_lastRideCymbal = 1;
    // g_show_percussion = 0;
    // g_ialloc_piano = 0;
    // g_ialloc_bass = 0;
    // g_ialloc_guitar = 0;
    // dword_464700 = 0;
    // dword_468EC0 = 0;
    // g_ialloc_harp = 0;
    // g_ialloc_xylophone = 0;
    // g_ialloc_stageChoir = 0;
    // g_ialloc_stageHorn = 0;
    // g_ialloc_whistles = 0;
    // g_ialloc_panPipe = 0;
    // g_ialloc_harmonica = 0;
    // g_ialloc_popBottle = 0;
    // g_ialloc_agogo = 0;
    // g_ialloc_woodblocks = 0;
    // g_ialloc_stageStrings = 0;
    // g_ialloc_pizzicatoStrings = 0;
    // g_ialloc_musicBox = 0;
    // g_ialloc_melodicTom = 0;
    // g_ialloc_synthDrum = 0;
    // g_ialloc_steelDrum = 0;
    // g_ialloc_timpani = 0;
    // g_ialloc_taiko = 0;
    // g_ialloc_tubularBells = 0;
    // g_ialloc_telephone = 0;
    // g_ialloc_trombone = 0;
    // g_ialloc_tuba = 0;
    // g_ialloc_ocarina = 0;
    // g_ialloc_frenchHorn = 0;
    // g_ialloc_baritoneSax = 0;
    // g_ialloc_tenorSax = 0;
    // g_ialloc_sapranoSax = 0;
    // g_ialloc_altoSax = 0;
    // g_ialloc_recorder = 0;
    // g_ialloc_piccolo = 0;
    // g_ialloc_flute = 0;
    // g_ialloc_trumpet = 0;
    // g_recoil_cowbell = 0.0;
    // g_recoil_clap_hand_l = 0.0;
    // g_recoil_clap_hand_r = 0.0;
    // g_recoil_tambourine = 0.0;
    // g_recoil_tambourine_hand = 0;
    // g_recoil_sticks_1 = 0.0;
    // g_recoil_sticks_2 = 0.0;
    // g_recoil_clave_l = 0.0;
    // g_recoil = 0.0;
    // g_recoil_castanets = 0.0;
    // g_recoil_jingleBell = 0.0;
    // g_recoil_shaker = 0.0;
    // g_recoil_highQ = 0.0;
    // g_recoil_squareClick = 0.0;
    // g_recoil_metronomeClick = 0.0;
    // g_recoil_metronomeBell = 0.0;
    // g_recoil_Maracas = 0.0;
    // g_recoil_cabasa = 0.0;
    // g_recoil_agogo_hi = 0.0;
    // g_recoil_agogo_lo = 0.0;
    // g_recoil_woodblock_hi = 0.0;
    // g_recoil_woodblock_lo = 0.0;
    // g_recoil_triangle_open = 0.0;
    // g_recoil_triangle_muted = 0.0;
    // g_recoil_whistle_long = 0.0;
    // g_recoil_whistle_short = 0.0;
    // g_recoil_timbale_hi = 0.0;
    // g_recoil_timbale_lo = 0.0;
    // g_recoil_bongo_hi = 0.0;
    // g_recoil_bongo_lo = 0.0;
    // g_recoil_conga_high_mute = 0.0;
    // g_recoil_conga_high_open = 0.0;
    // g_recoil_conga_low = 0.0;
    // g_recoil_snaredrum = 0.0;
    // g_recoil_snare = 0.0;
    // g_recoil_sideStick = 0.0;
    // for (n = 0; n < 6; ++n)
    //     g_recoil_tom[n] = 0.0;
    // g_recoil_bassdrum = 0.0;
    // g_recoil_hihat = 0.0;
    // g_hihat_isOpen = 0;
    // g_recoil_bassDrumArm = 45.0;
    g_songFillbarScale = 0.0;
    // Zero-out percussion queues
    // for (patch = 0; patch < 88; ++patch) {
    //     g_stick_visible[patch] = 0;
    //     for (queue = 0; queue < 32; ++queue) {
    //         g_percussion_time_queue[patch][queue] = 0;
    //         g_percussion_velocity_queue[patch][queue] = 0;
    //     }
    // }
    // for (slot = 0; slot < 300; ++slot) {
    //     g_violin_assignment[slot] = 0;
    //     g_viola_assignment[slot] = 0;
    //     g_cello_assignment[slot] = 0;
    //     g_doubleBass_assignment[slot] = 0;
    //     g_doubleBass_playingStyle[slot] = -1;
    //     g_midiJamInstrumentIds[slot] = 0;
    //     g_xylophone_types[slot] = 0;
    //     g_piano_assignment[slot] = -1;
    //     g_bass_assignment[slot] = -1;
    //     g_guitar_assignment[slot] = -1;
    //     g_xylophone_assignment[slot] = -1;
    //     g_stateChoir_assignment[slot] = -1;
    //     g_stageHorn_assignment[slot] = -1;
    //     g_whistles_assignment[slot] = -1;
    //     g_panPipe_assignment[slot] = -1;
    //     g_isPanPipeCalliope[slot] = -1;
    //     g_harmonica_assignment[slot] = -1;
    //     g_popBottle_assignment[slot] = -1;
    //     g_agogos_assignment[slot] = -1;
    //     g_woodblocks_assignment[slot] = -1;
    //     g_stageStrings_assignment[slot] = -1;
    //     g_pizzicatoStrings_assignment[slot] = -1;
    //     g_musicBox_assignment[slot] = -1;
    //     g_melodicTom_assignment[slot] = -1;
    //     g_synthDrum_assignment[slot] = -1;
    //     g_steelDrum_assignment[slot] = -1;
    //     g_timpani_assignment[slot] = -1;
    //     g_taiko_assignment[slot] = -1;
    //     g_tubularBells_assignment[slot] = -1;
    //     g_telephone_assignment[slot] = -1;
    //     g_trombone_assignment[slot] = -1;
    //     g_tuba_assignment[slot] = -1;
    //     g_ocarina_assignment[slot] = -1;
    //     g_frenchHorn_assignment[slot] = -1;
    //     g_baritoneSax_assignment[slot] = -1;
    //     g_tenorSax_assignment[slot] = -1;
    //     g_sapranoSax_assignment[slot] = -1;
    //     g_altoSax_assignment[slot] = -1;
    //     g_recorder_assignment[slot] = -1;
    //     g_piccolo_assignment[slot] = -1;
    //     g_flute_assignment[slot] = -1;
    //     g_trumpet_assignment[slot] = -1;
    //     g_harp_assignment[slot] = -1;
    // }
    // g_framesKeyLeftPressed = 0;
    // g_framesKeyRightPressed = 0;
    // g_framesKeyUpPressed = 0;
    // g_framesKeyDownPressed = 0;
    // word_468974 = 0;
    // word_4614AC = 0;
    // word_461C90 = 0;
    // g_isAKeyPressed = 0;
    // g_isSpacePressed = 0;
    // word_46AF68 = 0;
    // word_463228 = 0;
    // g_shiftPressed = 0;
    // dword_46ACF4 = 0;
    // word_464710 = 0;
}

LRESULT __stdcall HandleWindowBehavior(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    LRESULT result; // eax

    if (Msg > WM_LBUTTONDOWN) {
        switch (Msg) {
            case WM_LBUTTONUP:
                g_leftButtonDown = 0;
                result = 0;
                break;
            case WM_RBUTTONDOWN:
                if (!g_rightButtonDown)
                    g_rightButtonClicked = 1;
                g_rightButtonDown = 1;
                result = 0;
                break;
            case WM_RBUTTONUP:
                g_rightButtonDown = 0;
                result = 0;
                break;
            case WM_MBUTTONDOWN:
                if (!g_middleButtonDown)
                    g_middleButtonClicked = 1;
                g_middleButtonDown = 1;
                result = 0;
                break;
            case WM_MBUTTONUP:
                g_middleButtonDown = 0;
                result = 0;
                break;
            case WM_MOUSEWHEEL:
                g_mouseWheelMoved = 1;
                g_mouseWheelDelta += HIWORD(wParam);
                result = 0;
                break;
            default:
                return DefWindowProcA(hWnd, Msg, wParam, lParam);
        }
    } else if (Msg == WM_LBUTTONDOWN) {
        if (!g_leftButtonDown)
            g_leftButtonClicked = 1;
        g_leftButtonDown = 1;
        return 0;
    } else {
        if (Msg > WM_KEYDOWN) {
            switch (Msg) {
                case WM_KEYUP:
                    g_keyStateArray[wParam] = 0;
                    return 0;
                case WM_SYSCOMMAND:
                    if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER)
                        return 0; // Prevent screensaver or monitor power-off while game is running?
                    break;
                case WM_MOUSEMOVE:
                    g_mouseMoved = 1;
                    g_mouseY = HIWORD(wParam);
                    g_mouseX = wParam;
                    return 0;
            }
        } else {
            switch (Msg) {
                case WM_KEYDOWN:
                    g_keyStateArray[wParam] = 1;
                    g_lastKeyPressed = wParam;
                    return 0;
                case WM_SIZE:
                    return 0;
                case WM_ACTIVATE:
                    g_isWindowActive = HIWORD(wParam) == 0;
                    return 0;
                case WM_CLOSE: // User clicked the X button on the window
                    PostQuitMessage(0); // Kill app
                    return 0;
            }
        }
        return DefWindowProcA(hWnd, Msg, wParam, lParam);
    }
    return result;
}
