//
// Created by Jacob on 11/19/2025.
//
#include "camera.h"

#include "instrument/Piano.h"

bool IsCameraAngleViable_2() {
    __int16 i; // [esp+0h] [ebp-10h]
    __int16 k; // [esp+4h] [ebp-Ch]
    __int16 j; // [esp+8h] [ebp-8h]

    if (g_ds_piano) {
        for (i = 0; i < g_ialloc_piano; ++i) {
            if (g_ds_piano[i].isActive == 1) {
                for (j = 0; j < 88; ++j) {
                    for (k = 0; k < 16; ++k) {
                        if (g_ds_piano[i].timeDeltas[j][k] > 0)
                            return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}

bool IsCameraAngleViable_3() {
    // TODO
    return FALSE;
}

bool IsCameraAngleViable_4() {
    // TODO
    return FALSE;
}

bool IsCameraAngleViable_6() {
    // TODO
    return FALSE;
}

void TriggerAutoCam() {
    int rand; // eax
    __int16 x; // [esp+0h] [ebp-20h]
    __int16 i; // [esp+4h] [ebp-1Ch]
    __int16 viableAngles[12]; // [esp+8h] [ebp-18h]

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

void MoveCameraToAngle(CameraAngle angle, short isAutocamEnabled) {
    __int16 i; // [esp+0h] [ebp-4h]
    __int16 j; // [esp+0h] [ebp-4h]

    g_autoCameraIsActive = isAutocamEnabled;
    g_lastCameraAngle = g_targetCameraAngle;
    g_targetCameraAngle = angle;
    // Jump 40 percent of the time
    if (!isAutocamEnabled || isAutocamEnabled == 1 && 100 * rand() / 32768 < 40) {
        // Jump to new camera location
        for (i = 0; i < 6; ++i)
            g_cameraLocation[i] = *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + i);
    }
    for (j = 0; j < 6; ++j)
        // Set delta transforms: Move 1/500th the distance on each frame
        g_autoCamDeltaTransform[j] = (*(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + j) - g_cameraLocation[j]) /
                                     500.0;
    g_autoCamIdleTime = 0;
    g_autoCamIsIdle = 0;
}
