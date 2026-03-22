//
// Created by Jacob on 3/16/2026.
//

#include "Quaternion.h"

#include "scalar.h"

#include <cmath>
#include <cstring>

// FUNCTION: MIDIJAM 0x446210
// MATCH: EXACT
void SetIdentityQuaternion(Quaternion* quat)
{
    quat->x = 1.0;
    quat->y = 0.0;
    quat->z = 0.0;
    quat->w = 0.0;
}

// FUNCTION: MIDIJAM 0x4462C0
double QuaternionLength(Quaternion* a)
{
    return sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
}

// FUNCTION: MIDIJAM 0x445AF0
void QuaternionNormalize(Quaternion* quat)
{
    const float length = quat->x * quat->x
                       + quat->y * quat->y
                       + quat->z * quat->z
                       + quat->w * quat->w;
    const double inverseLength = 1.0 / square_root(length);
    quat->x = inverseLength * quat->x;
    quat->y = inverseLength * quat->y;
    quat->z = inverseLength * quat->z;
    quat->w = inverseLength * quat->w;
}

// FUNCTION: MIDIJAM 0x445C00
void QuaternionFromEulerAngles(Quaternion* result, const float pitch, const float yaw, const float roll)
{
    const double halfPitch = 0.017453292 * pitch / 2.0;
    const double halfYaw   = 0.017453292 * yaw   / 2.0;
    const double halfRoll  = 0.017453292 * roll  / 2.0;

    const double cosPitch = cos(halfPitch);
    const double cosYaw   = cos(halfYaw);
    const double cosRoll  = cos(halfRoll);
    const double sinPitch = sin(halfPitch);
    const double sinYaw   = sin(halfYaw);
    const double sinRoll  = sin(halfRoll);

    const double cy_cr = cosYaw * cosRoll;
    const double sy_sr = sinYaw * sinRoll;

    // TODO: verify z and w component formulas — may be swapped vs standard convention.
    result->x = cosPitch * cy_cr + sinPitch * sy_sr;
    result->y = sinPitch * cy_cr - cosPitch * sy_sr;
    result->z = cosPitch * sinYaw * sinRoll + sinPitch * cosYaw * sinRoll;
    result->w = cosPitch * cosYaw * sinRoll - sinPitch * sinYaw * cosRoll;

    QuaternionNormalize(result);
}

// FUNCTION: MIDIJAM 0x446040
// MATCH: APPROXIMATE - stack over-allocation (0x80 vs 0x20) and rep movsd vs memcpy call, MSVC version difference
void QuaternionMultiply(Quaternion* result, Quaternion* q1, Quaternion* q2)
{
    Quaternion temp;

    // Hamilton product
    temp.x = q1->x * q2->x - q1->y * q2->y - q1->z * q2->z - q1->w * q2->w;
    temp.y = q1->x * q2->y + q1->y * q2->x + q1->z * q2->w - q1->w * q2->z;
    temp.z = q1->x * q2->z + q1->z * q2->x + q1->w * q2->y - q1->y * q2->w;
    temp.w = q1->x * q2->w + q1->w * q2->x + q1->y * q2->z - q1->z * q2->y;

    // IDA reported qmemcpy size as 128 — incorrect. QuaternionD is 4 doubles = 32 bytes.
    memcpy(result, &temp, sizeof(Quaternion));
}

// FUNCTION: MIDIJAM 0x446140
void QuaternionToAxisAngles(Quaternion* quat, float* axisX, float* axisY, float* axisZ, float* angle)
{
    const float vectorLengthSquared = quat->y * quat->y
                                    + quat->z * quat->z
                                    + quat->w * quat->w;
    if (vectorLengthSquared >= 1.0e-12f)
    {
        const double inverseLength = 1.0 / square_root(vectorLengthSquared);
        *axisX = inverseLength * quat->y;
        *axisY = inverseLength * quat->z;
        *axisZ = inverseLength * quat->w;
        const double acosX = acos(quat->x);
        *angle = acosX + acosX;
    }
    else
    {
        *axisX = 1.0f;
        *axisY = 0.0f;
        *axisZ = 0.0f;
        *angle = 0.0f;
    }
}