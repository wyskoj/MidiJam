//
// Created by Jacob on 11/19/2025.
//

#include "input.h"

#include "camera.h"
#include "globals.h"
#include "MidiJam.h"

__int16 g_isSpacePressed;
__int16 g_isAKeyPressed;
__int16 g_shiftPressed;
__int16 g_framesKeyRightPressed;
__int16 g_framesKeyLeftPressed;
__int16 g_framesKeyUpPressed;
__int16 g_framesKeyDownPressed;

void HandleKeyPresses() {
    g_shiftPressed = g_keyStateArray[VK_SHIFT] != 0;
    //  -- Space key --
    if (g_keyStateArray[VK_SPACE]) {
        if (!g_isSpacePressed) {
            g_isSpacePressed = 1;
            if (g_DirectMusicSegmentWrapper->IsPlaying())
                g_DirectMusicSegmentWrapper->Stop(4096);
            else
                PlaySegment();
        }
    } else {
        g_isSpacePressed = 0;
    }
    //  -- KEY A (unused) --
    if (g_keyStateArray['A']) {
        if (!g_isAKeyPressed)
            g_isAKeyPressed = 1;
    } else {
        g_isAKeyPressed = 0;
    }
    //  -- KEY RIGHT (SPEED INCREASE) --
    if (g_keyStateArray[VK_RIGHT]) {
        if (!g_framesKeyRightPressed) {
            g_playbackSpeed = g_playbackSpeed + 1.0;
            if (g_playbackSpeed > 10.0)
                g_playbackSpeed = 10.0;
            g_DirectMusicPerformance->SetGlobalParam(
                GUID_PerfMasterTempo,
                &g_playbackSpeed,
                4);
            g_framesKeyRightPressed = 1;
        }
    } else {
        g_framesKeyRightPressed = 0;
    }
    //  -- KEY LEFT (SPEED DECREASE) --
    if (g_keyStateArray[VK_LEFT]) {
        if (!g_framesKeyLeftPressed) {
            g_playbackSpeed = g_playbackSpeed - 1.0;
            if (g_playbackSpeed < 1.0)
                g_playbackSpeed = 1.0;
            g_DirectMusicPerformance->SetGlobalParam(
                GUID_PerfMasterTempo,
                &g_playbackSpeed,
                4);
            g_framesKeyLeftPressed = 1;
        }
    } else {
        g_framesKeyLeftPressed = 0;
    }
    //  -- KEY UP --
    if (g_keyStateArray[VK_UP]) {
        if (!g_framesKeyUpPressed)
            g_framesKeyUpPressed = 1;
    } else {
        g_framesKeyUpPressed = 0;
    }
    //  -- KEY DOWN --
    if (g_keyStateArray[VK_DOWN]) {
        if (!g_framesKeyDownPressed)
            g_framesKeyDownPressed = 1;
    } else {
        g_framesKeyDownPressed = 0;
    }
    if (g_framesKeyRightPressed > 0 && ++g_framesKeyRightPressed > 50)
        g_framesKeyRightPressed = 0;
    if (g_framesKeyLeftPressed > 0 && ++g_framesKeyLeftPressed > 50)
        g_framesKeyLeftPressed = 0;
    if (g_framesKeyUpPressed > 0 && ++g_framesKeyUpPressed > 50)
        g_framesKeyUpPressed = 0;
    if (g_framesKeyDownPressed > 0 && ++g_framesKeyDownPressed > 50)
        g_framesKeyDownPressed = 0;
    if (g_keyStateArray[VK_ESCAPE]) {
        g_DirectMusicSegmentWrapper->Stop(4096);
        g_isShuttingDown = 1;
    }
    //  -- CAMERA 1A, 1B, 1C --
    if (g_keyStateArray['1']) {
        g_targetCameraAngle = static_cast<CameraAngle>(static_cast<__int16>(g_targetCameraAngle) + 1);
        g_rotatingCameraIsActive = 0;
        // wrap to 0..2
        if (g_targetCameraAngle > CAMERA_1C)
            g_targetCameraAngle = CAMERA_1A;
        MoveCameraToAngle(g_targetCameraAngle, 0);
        g_keyStateArray['1'] = 0;
    }
    //  -- CAMERA 2A, 2B --
    if (g_keyStateArray['2']) {
        // toggle between 3 and 7
        if (g_targetCameraAngle == CAMERA_2A)
            g_targetCameraAngle = CAMERA_2B;
        else
            g_targetCameraAngle = CAMERA_2A;
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(g_targetCameraAngle, 0);
        g_keyStateArray['2'] = 0;
    }
    //  -- CAMERA 3A, 3B --
    if (g_keyStateArray['3']) {
        if (g_targetCameraAngle == CAMERA_3A)
            g_targetCameraAngle = CAMERA_3B;
        else
            g_targetCameraAngle = CAMERA_3A;
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(g_targetCameraAngle, 0);
        g_keyStateArray['3'] = 0;
    }
    //  -- CAMERA 4A, 4B --
    if (g_keyStateArray['4']) {
        if (g_targetCameraAngle == CAMERA_4A)
            g_targetCameraAngle = CAMERA_4B;
        else
            g_targetCameraAngle = CAMERA_4A;
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(g_targetCameraAngle, 0);
        g_keyStateArray['4'] = 0;
    }
    //  -- CAMERA 5 --
    if (g_keyStateArray['5']) {
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(CAMERA_5, 0);
        g_keyStateArray['5'] = 0;
    }
    //  -- CAMERA 6 --
    if (g_keyStateArray['6']) {
        g_targetCameraAngle = CAMERA_6;
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(CAMERA_6, 0);
        g_keyStateArray['6'] = 0;
    }
    //  -- CAMERA 9 (ROTATING) --
    if (g_keyStateArray['9']) {
        g_targetCameraAngle = CAMERA_1A;
        g_rotatingCameraIsActive = 1;
        g_rotatingCameraAngle = 0.0;
        g_rotatingCameraDirection = CLOCKWISE;
        g_rotatingCameraIdleTime = 0;
        MoveCameraToAngle(CAMERA_1A, 0);
        g_keyStateArray['9'] = 0;
    }
    //  -- CAMERA 0 (AUTO) --
    if (g_keyStateArray['0']) {
        g_autoCameraIsActive = 1;
        g_rotatingCameraIsActive = 0;
        TriggerAutoCam();
        g_keyStateArray['0'] = 0;
    }
    if (g_mouseWheelMoved) {
        g_mouseWheelDelta = 0;
        g_mouseWheelMoved = 0;
    }
}
