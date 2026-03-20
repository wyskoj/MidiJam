//
// Created by Jacob on 3/16/2026.
//

#include "Quaternion.h"

#include "scalar.h"

// FUNCTION: MIDIJAM 0x448390
void Quaternion::CopyFromVector(const float* source)
{
    x = source[0];
    y = source[1];
    z = source[2];
}

// FUNCTION: MIDIJAM 0x448150
void Quaternion::FromVector(const float* vector)
{
    CopyFromVector(vector);
    w = 1.0f;
}

// FUNCTION: MIDIJAM 0x448350
void Quaternion::Identity()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 1.0f;
}

// FUNCTION: MIDIJAM 0x43C320
float Quaternion::Length() const
{
    const float squaredLength = x * x + y * y + z * z;
    return square_root(squaredLength);
}

// FUNCTION: MIDIJAM 0x43C2D0
void Quaternion::Normalize()
{
    const float length = Length();
    x = x / length;
    y = y / length;
    z = z / length;
}

// FUNCTION: MIDIJAM 0x448190
void Quaternion::TransformByMatrix(const MatrixMath* pOther)
{
    // ObjectConstructor at 0x43C2C0 is a 14-byte no-op pass-through — elided.
    const float newY = x * pOther->matrix[0][1]
                     + y * pOther->matrix[1][1]
                     + z * pOther->matrix[2][1]
                     + pOther->matrix[3][1];
    const float newZ = x * pOther->matrix[0][2]
                     + y * pOther->matrix[1][2]
                     + z * pOther->matrix[2][2]
                     + pOther->matrix[3][2];
    const float newW = x * pOther->matrix[0][3]
                     + y * pOther->matrix[1][3]
                     + z * pOther->matrix[2][3]
                     + pOther->matrix[3][3];
    x = x * pOther->matrix[0][0]
      + y * pOther->matrix[1][0]
      + z * pOther->matrix[2][0]
      + pOther->matrix[3][0];
    y = newY;
    z = newZ;
    w = newW;
}

// FUNCTION: MIDIJAM 0x448290
void Quaternion::TransformByMatrixRotation(const MatrixMath* pMatrix)
{
    // ObjectConstructor at 0x43C2C0 is a 14-byte no-op pass-through — elided.
    const float newY = x * pMatrix->matrix[0][1]
                     + y * pMatrix->matrix[1][1]
                     + z * pMatrix->matrix[2][1];
    const float newZ = x * pMatrix->matrix[0][2]
                     + y * pMatrix->matrix[1][2]
                     + z * pMatrix->matrix[2][2];
    x = x * pMatrix->matrix[0][0]
      + y * pMatrix->matrix[1][0]
      + z * pMatrix->matrix[2][0];
    y = newY;
    z = newZ;
    w = 1.0f;
}