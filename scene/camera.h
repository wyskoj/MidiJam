//
// Created by Jacob on 3/18/2026.
//

#ifndef MIDIJAM_CAMERA_H
#define MIDIJAM_CAMERA_H

enum CAMERA_ANGLE : short
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


enum ROTATING_CAMERA_DIRECTION : short
{
    CLOCKWISE = 0,
    ANTI_CLOCKWISE = 1,
};

#endif //MIDIJAM_CAMERA_H
