//
// Created by Jacob on 11/16/2025.
//

#ifndef MIDIJAM_CAMERA_H
#define MIDIJAM_CAMERA_H
#include <windows.h>
#include <GL/GL.h>

enum ROTATING_CAMERA_DIRECTION : __int16 {
    CLOCKWISE = 0x0,
    ANTI_CLOCKWISE = 0x1,
};

enum CameraAngle : __int16 {
    CAMERA_1A = 0x0,
    CAMERA_1B = 0x1,
    CAMERA_1C = 0x2,
    CAMERA_2A = 0x3,
    CAMERA_4A = 0x4,
    CAMERA_3A = 0x5,
    CAMERA_5 = 0x6,
    CAMERA_2B = 0x7,
    CAMERA_4B = 0x8,
    CAMERA_3B = 0x9,
    CAMERA_6 = 0xA,
};

struct CameraPosition {
    float cameraX;
    float cameraY;
    float cameraZ;
    float lookAtX;
    float lookAtY;
    float lookAtZ;
};

extern CameraPosition CAMERA_POSITIONS[11];

extern int g_autoCamIdleTime;
extern CameraAngle g_targetCameraAngle;
extern float g_cameraLocation[6];
extern __int16 g_rotatingCameraIsActive;
extern GLfloat g_rotatingCameraAngle;
extern ROTATING_CAMERA_DIRECTION g_rotatingCameraDirection;
extern __int16 g_rotatingCameraIdleTime;

#endif //MIDIJAM_CAMERA_H
