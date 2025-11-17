#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>

#include "camera.h"
#include "fade.h"
#include "globals.h"
#include "hwf.h"
#include "midi.h"
#include "MidiJam.h"
#include "Ms3dBundle.h"

int main() {
    extern char ** __argv;
    int i;
    int j;

    g_killApplication_0 = 0;
    g_worldReady = 0;
    g_applicationStartTime = timeGetTime();

    for (i = 0; i < 6; ++i) {
        for (j = 0; j < 6; ++j) {
            g_latinSquare[6 * i + j] = (i + j) % 6;
        }
    }
    g_pianoXylophoneOffset = 0.0;

    for (i = 0; i < 7; ++i) {
        const float cymbalAzimuth = CYMBAL_LOC_X[i] / (CYMBAL_LOC_Z[i] - 24.0);
        g_cymbal_rot_y[i] = std::atan(cymbalAzimuth) * 57.29578;
        g_cymbalRestingAngle[i] = 20.0;
    }

    for (i = 0; i < 23; ++i) {
        word_46B5D8[4 * i] = i + 7;
        word_46B5DA[4 * i] = i + 12;
        word_46B5DC[4 * i] = i + 17;
        word_46B5DE[4 * i] = i + 22;
        flt_468BF8[i] = flt_45EA70[i] / -46.081;
        word_46CEE0[6 * i] = i + 19;
        word_46CEE2[6 * i] = i + 24;
        word_46CEE4[6 * i] = i + 29;
        word_46CEE6[6 * i] = i + 34;
        word_46CEE8[6 * i] = i + 38;
        word_46CEEA[6 * i] = i + 43;
        flt_4654A0[i] = flt_45EAD0[i] / -34.700;
    }

    for (i = 0; i < 18; ++i) {
        word_46B2D0[4 * i] = i + 34;
        word_46B2D2[4 * i] = i + 41;
        word_46B2D4[4 * i] = i + 48;
        word_46B2D6[4 * i] = i + 55;
        word_4688C0[4 * i] = i + 27;
        word_4688C2[4 * i] = i + 34;
        word_4688C4[4 * i] = i + 41;
        word_4688C6[4 * i] = i + 48;
    }

    for (i = 0; i < 28; ++i) {
        word_46BBB0[4 * i] = i + 15;
        word_46BBB2[4 * i] = i + 22;
        word_46BBB4[4 * i] = i + 29;
        word_46BBB6[4 * i] = i + 36;
    }

    float v603 = 0.0;
    float v612 = 0.0525;
    for (i = 0; i < 49; ++i) {
        word_468258[4 * i] = i + 7;
        word_46825A[4 * i] = i + 12;
        word_46825C[4 * i] = i + 17;
        word_46825E[4 * i] = i + 22;
        flt_4679E0[i] = v603 * 0.8;
        v603 = v603 + v612;
        v612 = v612 * 0.95;
    }
    g_vibratingString_frame = VIBRATING_STRING_ANIM_SEQUENCE[0];
    g_vibratingString_frameIndex = 0;


    for (i = 0; i < 88; ++i) {
        // TODO There may be a bad array access here (out of bounds)
        g_pianoXylophoneOffset = g_pianoXylophoneOffset + dword_45EBDC[word_45EB30[i]];
    }
    g_pianoXylophoneOffset = g_pianoXylophoneOffset * 0.5;

    // WTF?
    dword_464BFC = -1101703851;
    dword_464F68 = 1058362709;
    dword_464C00 = 0;
    dword_464F6C = 1056964608;
    dword_464C04 = 1045779797;
    dword_464F70 = 1058362709;
    dword_464C08 = -1098907648;
    dword_464F74 = 1056964608;
    dword_464C0C = -1112888661;
    dword_464F78 = 1056964608;
    dword_464C10 = 1034594987;
    dword_464F7C = 1056964608;
    dword_464C14 = 1048576000;
    dword_464F80 = 1056964608;
    dword_464C18 = 0;
    dword_464F84 = 1065353216;
    dword_464C1C = -1107296256;
    dword_464F88 = 1065353216;
    dword_464C20 = 1040187392;
    dword_464F8C = 1065353216;
    dword_464C24 = -1098907648;
    dword_464F90 = 1065353216;
    dword_464C28 = 0;
    dword_464F94 = 1065353216;
    dword_464C2C = 1048576000;
    dword_464F98 = 1065353216;

    g_windowCenterX = 0;
    g_windowCenterY = 0;
    g_framesSinceStart = 0;
    g_autoCamIdleTime = 0;
    g_fadeFactor = 1.0;
    g_isFadingIn = 1;
    g_isShuttingDown = 0;
    g_killApplication = 0;

    GetModuleFileNameA(nullptr, g_workingDirectory, 1000u);
    while (strlen(g_workingDirectory) > 0 && g_workingDirectory[strlen(g_workingDirectory) - 1] != '\\') {
        g_workingDirectory[strlen(g_workingDirectory) - 1] = '\0';
    }
    if (strlen(g_workingDirectory) > 0 && g_workingDirectory[strlen(g_workingDirectory) - 1] == '\\') {
        g_workingDirectory[strlen(g_workingDirectory) - 1] = '\0';
    }

    MidiJamInitialize();

    g_lastUnixEpochTime = GetUnixEpochTime(nullptr);

    char hwfPath[1000];
    sprintf(hwfPath, "%s\\MidiJam.HWF", g_workingDirectory);
    g_hwfStream = fopen(hwfPath, "rb");
    g_pHwfAppendix = ReadHwfAppendix(g_pHwfAppendix, g_hwfStream, &g_nHwfAppendixItems);

    // TODO: Maybe bad array access here?
    // memset(g_cloudTTL_whistle_long, 0, 8808u);

    const HINSTANCE hInstance = nullptr;
    if (MidiJamMain("MidiJam", hInstance, g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems) == 0x2B) {
        MessageBoxA(nullptr, "Video mode not supported, try running CONFIG.EXE first", "SHUTDOWN ERROR",
                    MB_ICONASTERISK);
        return 0;
    }

    gluLookAt(0.0, 50.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    constexpr GLfloat light_params_0_ambient[4] = {0.25, 0.25, 0.25, 1.0};
    constexpr GLfloat light_params_0_diffuse[4] = {1.0, 1.0, 1.0, 1.0};
    constexpr GLfloat light_params_0_position[4] = {15.0, 75.0, 5.0, 1.0};
    constexpr GLfloat light_params_1_ambient[4] = {0.025, 0.025, 0.25, 1.0};
    constexpr GLfloat light_params_1_diffuse[4] = {0.5, 0.5, 1.0, 1.0};
    constexpr GLfloat light_params_1_position[4] = {-25.0, 5.0, 25.0, 1.0};
    constexpr GLfloat light_params_2_ambient[4] = {0.25, 0.25, 0.25, 1.0};
    constexpr GLfloat light_params_2_diffuse[4] = {1.0, 1.0, 1.0, 1.0};
    constexpr GLfloat light_params_2_position[4] = {-15.0, 45.0, 35.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_params_0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_params_0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_params_0_position);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_params_1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_params_1_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light_params_1_position);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light_params_2_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_params_2_diffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, light_params_2_position);
    glEnable(GL_LIGHT2);
    g_normalizedWindowScale = g_windowWidth / 1024.0;
    g_windowCenterX = g_windowCenter_X;
    g_windowCenterY = g_windowCenter_Y;

    g_drumSet_Stick_ms3d = new Ms3dBundle();
    g_drumSet_Stick_ms3d->LoadFromHWF("DrumSet_Stick.ms3d", g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems);
    g_drumSet_Stick_ms3d->ApplyTextures(g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems);

    g_screenGradient_ms3d = new Ms3dBundle();
    g_screenGradient_ms3d->LoadFromHWF("ScreenGradient.ms3d", g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems);
    g_screenGradient_ms3d->ApplyTextures(g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems);

    g_stage_ms3d = new Ms3dBundle();
    g_stage_ms3d->LoadFromHWF("Stage.ms3d", g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems);
    g_stage_ms3d->ApplyTextures(g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems);

    g_songFillbar_ms3d = new Ms3dBundle();
    g_songFillbar_ms3d->LoadFromHWF("SongFillbar.ms3d", g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems);
    g_songFillbar_ms3d->ApplyTextures(g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems);

    g_songFillbarBox_ms3d = new Ms3dBundle();
    g_songFillbarBox_ms3d->LoadFromHWF("SongFillbarBox.ms3d", g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems);
    g_songFillbarBox_ms3d->ApplyTextures(g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems);

    int v613 = 0;
    g_timerEventId = timeSetEvent(5u, 0, UpdateMidiJamMM, 0, TIME_PERIODIC);

    auto lpCmdLine = __argv[1];
    if (lpCmdLine && strlen(lpCmdLine) > 4) {
        int m = 0, n = 0;
        size_t len = strlen(lpCmdLine);

        // Copy MIDI file name up to but not including ".MID" or ".RMI"
        for (m = 0;
             m < (int) (len - 4) &&
             !((lpCmdLine[m] == '.' &&
                ((lpCmdLine[m + 1] == 'M' && lpCmdLine[m + 2] == 'I' && lpCmdLine[m + 3] == 'D') ||
                 (lpCmdLine[m + 1] == 'R' && lpCmdLine[m + 2] == 'M' && lpCmdLine[m + 3] == 'I'))));
             m++) {
            g_midiFileName[m] = lpCmdLine[m];
        }

        // Copy extension
        g_midiFileName[m] = lpCmdLine[m];
        g_midiFileName[m + 1] = lpCmdLine[m + 1];
        g_midiFileName[m + 2] = lpCmdLine[m + 2];
        g_midiFileName[m + 3] = lpCmdLine[m + 3];
        g_midiFileName[m + 4] = '\0';

        // Remove quotes if present
        if (g_midiFileName[0] == '"') {
            for (n = 0; g_midiFileName[n]; n++)
                g_midiFileName[n] = g_midiFileName[n + 1]; // shift left by 1

            if (g_midiFileName[n - 1] == '"')
                g_midiFileName[n - 1] = '\0';
        }
    }

    if (!lpCmdLine || strlen(lpCmdLine) <= 4) {
        // no midi file passed, load default song
        if (LoadAndPlayMidiFile(const_cast<char *>("rocky_1.mid")) == 1)
            g_killApplication_0 = 1;
    } else if (LoadAndPlayMidiFile(reinterpret_cast<char *>(&g_midiFileName)) == 1) {
        g_killApplication_0 = 1;
    }

    g_worldReady = 1;

    tagMSG Msg;
    while (!g_killApplication_0) {
        if (PeekMessageA(&Msg, 0, 0, 0, 1u)) {
            if (Msg.message == WM_QUIT) {
                g_killApplication_0 = 1;
            } else {
                TranslateMessage(&Msg);
                DispatchMessageA(&Msg);
            }
        } else if (!g_isWindowActive || UpdateMidiJam()) {
            // SwapBuffers_0();
            SwapBuffers(g_hdc);
            ++v613;
            // run once per second
            if (GetUnixEpochTime(0) > g_lastUnixEpochTime) {
                g_framesPerSecond = v613;
                v613 = 0;
                g_lastUnixEpochTime = GetUnixEpochTime(0);
            }
        } else {
            g_killApplication_0 = 1;
        }
    }

    /* TODO */
}
