//
// Created by Jacob on 3/18/2026.
//

#ifndef MIDIJAM_CAMERA_H
#define MIDIJAM_CAMERA_H

struct CameraPosition {
    float cameraX;
    float cameraY;
    float cameraZ;
    float lookAtX;
    float lookAtY;
    float lookAtZ;
};

#if _MSC_VER < 1400
enum CameraAngle
#else
enum CameraAngle : short
#endif
{
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

#if _MSC_VER < 1400
enum RotatingCameraDirection
#else
enum RotatingCameraDirection : short
#endif
{
    CLOCKWISE = 0,
    ANTI_CLOCKWISE = 1,
};

void TriggerAutoCam();
void MoveCameraToAngle(CameraAngle angle, short isAutocamEnabled);

bool IsCameraAngleViable_2();
bool IsCameraAngleViable_3();
bool IsCameraAngleViable_4();
bool IsCameraAngleViable_5();
bool IsCameraAngleViable_6();

#endif //MIDIJAM_CAMERA_H
