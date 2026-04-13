//
// Created by Jacob on 4/9/2026.
//

#include "camera.h"

#include <cstdlib>

#include "instruments/Bass.h"
#include "instruments/Guitar.h"
#include "instruments/Piano.h"

CameraPosition CAMERA_POSITIONS[11] = {
    {-2.0f, 60.0f, 120.0f, -2.0f, 20.0f, 0.0f}, // CAMERA_1A
    {60.0f, 60.0f, 110.0f, 10.0f, 20.0f, 0.0f}, // CAMERA_1B
    {-60.0f, 60.0f, 80.0f, -20.0f, 20.0f, 0.0f}, // CAMERA_1C
    {0.0f, 40.0f, 30.0f, -50.0f, 20.0f, -20.0f}, // CAMERA_2A
    {0.0f, 50.0f, 20.0f, 50.0f, 24.0f, -25.0f}, // CAMERA_4A
    {0.0f, 30.0f, 25.0f, 0.0f, 5.0f, -65.0f}, // CAMERA_3A
    {5.0f, 400.0f, 10.0f, 5.0f, 0.0f, -40.0f}, // CAMERA_5
    {-35.0f, 45.0f, 20.0f, -50.0f, -25.0f, -25.0f}, // CAMERA_2B
    {35.0f, -5.0f, -30.0f, 55.0f, 25.0f, -40.0f}, // CAMERA_4B
    {-20.0f, 45.0f, -10.0f, 5.0f, 0.0f, -95.0f}, // CAMERA_3B
    {17.0f, -1.0f, 30.0f, 42.0f, 4.0f, -5.0f}, // CAMERA_6
};

float g_cameraLocation[6] = {};
float g_autoCamDeltaTransform[6] = {};
int g_autoCamIsIdle = 0;
int g_autoCamIdleTime = 0;
int g_autoCameraIsActive = 1;
CameraAngle g_targetCameraAngle = CAMERA_1A;
CameraAngle g_lastCameraAngle;

extern int g_show_percussion;
extern int g_percussion_time_queue[88][32];

// FUNCTION: MIDIJAM 0x4236A0
void TriggerAutoCam() {
    int rand; // eax
    short x; // [esp+0h] [ebp-20h]
    __int16 i; // [esp+4h] [ebp-1Ch]
    short viableAngles[12]; // [esp+8h] [ebp-18h]

    if (g_autoCameraIsActive && g_autoCamIsIdle) {
        x = 0;
        for (i = 0; i < 11; ++i) {
            if (g_targetCameraAngle != i && g_lastCameraAngle != i) {
                switch (i) {
                    case CAMERA_2A:
                    case CAMERA_2B:
                        if (IsCameraAngleViable_2())
                            viableAngles[x++] = i;
                        break;
                    case CAMERA_3A:
                    case CAMERA_3B:
                        if (IsCameraAngleViable_3())
                            viableAngles[x++] = i;
                        break;
                    case CAMERA_4A:
                    case CAMERA_4B:
                        if (IsCameraAngleViable_4() == 1)
                            viableAngles[x++] = i;
                        break;
                    case CAMERA_6:
                        if (IsCameraAngleViable_6() == 1)
                            viableAngles[x++] = i;
                        break;
                    default:
                        // Angles 1A, 1B, 1C, and 5 are always viable
                        viableAngles[x++] = i;
                        break;
                }
            }
        }
        if (x > CAMERA_1A) {
            rand = ::rand();
            MoveCameraToAngle(static_cast<CameraAngle>(viableAngles[x * rand / 32768]), 1);
        }
    }
}

// FUNCTION: MIDIJAM 0x423850
void MoveCameraToAngle(const CameraAngle angle, const short isAutocamEnabled) {
    g_autoCameraIsActive = isAutocamEnabled;
    g_lastCameraAngle = g_targetCameraAngle;
    g_targetCameraAngle = angle;
    // Jump 40 percent of the time
    if (!isAutocamEnabled || isAutocamEnabled == 1 && 100 * rand() / 32768 < 40) {
        // Jump to new camera location
        for (__int16 i = 0; i < 6; ++i)
            g_cameraLocation[i] = *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + i);
    }
    for (__int16 j = 0; j < 6; ++j)
        // Set delta transforms: Move 1/500th the distance on each frame
        g_autoCamDeltaTransform[j] = (*(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + j) - g_cameraLocation[j]) /
            500.0;
    g_autoCamIdleTime = 0;
    g_autoCamIsIdle = 0;
}

// FUNCTION: MIDIJAM 0x4239D0
bool IsCameraAngleViable_2() {
    if (g_piano) {
        for (__int16 i = 0; i < g_pianoCount; ++i) {
            if (g_piano[i].isActive == 1) {
                for (__int16 j = 0; j < 88; ++j) {
                    for (__int16 k = 0; k < 16; ++k) {
                        if (g_piano[i].timeDeltas[j][k] > 0)
                            return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}

// FUNCTION: MIDIJAM 0x423950
bool IsCameraAngleViable_3() {
    __int16 j; // [esp+0h] [ebp-8h]
    __int16 i; // [esp+4h] [ebp-4h]

    if (g_show_percussion == 1) {
        for (i = 0; i < 88; ++i) {
            for (j = 0; j < 32; ++j) {
                if (g_percussion_time_queue[i][j] > 0)
                    return TRUE;
            }
        }
    }
    return FALSE;
}

// FUNCTION: MIDIJAM 0x423AA0
bool IsCameraAngleViable_4() {
    __int16 i; // [esp+0h] [ebp-10h]
    __int16 k; // [esp+4h] [ebp-Ch]
    __int16 j; // [esp+8h] [ebp-8h]

    if (g_bass) {
        for (i = 0; i < g_bassCount; ++i) {
            if (g_bass[i].isActive == 1) {
                for (j = 0; j < 88; ++j) {
                    for (k = 0; k < 16; ++k) {
                        if (g_bass[i].field_18CC[j][k] > 0)
                            return 1;
                    }
                }
            }
        }
    }
    return 0;
}

// FUNCTION: MIDIJAM 0x423B70
bool IsCameraAngleViable_6() {
    __int16 i; // [esp+0h] [ebp-10h]
    __int16 k; // [esp+4h] [ebp-Ch]
    __int16 j; // [esp+8h] [ebp-8h]

    if ( g_guitar )
    {
        for ( i = 0; i < g_guitarCount; ++i )
        {
            if ( g_guitar[i].isActive == 1 )
            {
                for ( j = 0; j < 88; ++j )
                {
                    for ( k = 0; k < 16; ++k )
                    {
                        if ( g_guitar[i].field_18D2[j][k] > 0 )
                            return 1;
                    }
                }
            }
        }
    }
    return 0;
}
