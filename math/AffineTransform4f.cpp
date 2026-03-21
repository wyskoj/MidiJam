#include "AffineTransform4f.h"

#include "scalar.h"

#include <cstring>

// FUNCTION: MIDIJAM 0x401FD0
AffineTransform4f::AffineTransform4f()
{
    SetIdentity();
}

// FUNCTION: MIDIJAM 0x402960
void AffineTransform4f::SetIdentity()
{
    memset(this, 0, sizeof(AffineTransform4f));
    matrix[0][0] = 1.0f;
    matrix[1][1] = 1.0f;
    matrix[2][2] = 1.0f;
    matrix[3][3] = 1.0f;
}

// FUNCTION: MIDIJAM 0x402940
AffineTransform4f* AffineTransform4f::CopyFromBytes(const void* src)
{
    return static_cast<AffineTransform4f*>(memcpy(this, src, sizeof(AffineTransform4f)));
}

// FUNCTION: MIDIJAM 0x402000
float AffineTransform4f::GetElement(const int index) const
{
    return matrix[0][index];
}

// FUNCTION: MIDIJAM 0x402000
AffineTransform4f* AffineTransform4f::PostMultiply(const AffineTransform4f* other)
{
    float temp[4][4];

    temp[0][0] = matrix[0][0] * other->matrix[0][0] + matrix[1][0] * other->matrix[0][1] + matrix[2][0] * other->matrix[0][2];
    temp[0][1] = matrix[0][1] * other->matrix[0][0] + matrix[1][1] * other->matrix[0][1] + matrix[2][1] * other->matrix[0][2];
    temp[0][2] = matrix[0][2] * other->matrix[0][0] + matrix[1][2] * other->matrix[0][1] + matrix[2][2] * other->matrix[0][2];
    temp[0][3] = 0.0f;

    temp[1][0] = matrix[0][0] * other->matrix[1][0] + matrix[1][0] * other->matrix[1][1] + matrix[2][0] * other->matrix[1][2];
    temp[1][1] = matrix[0][1] * other->matrix[1][0] + matrix[1][1] * other->matrix[1][1] + matrix[2][1] * other->matrix[1][2];
    temp[1][2] = matrix[0][2] * other->matrix[1][0] + matrix[1][2] * other->matrix[1][1] + matrix[2][2] * other->matrix[1][2];
    temp[1][3] = 0.0f;

    temp[2][0] = matrix[0][0] * other->matrix[2][0] + matrix[1][0] * other->matrix[2][1] + matrix[2][0] * other->matrix[2][2];
    temp[2][1] = matrix[0][1] * other->matrix[2][0] + matrix[1][1] * other->matrix[2][1] + matrix[2][1] * other->matrix[2][2];
    temp[2][2] = matrix[0][2] * other->matrix[2][0] + matrix[1][2] * other->matrix[2][1] + matrix[2][2] * other->matrix[2][2];
    temp[2][3] = 0.0f;

    temp[3][0] = matrix[0][0] * other->matrix[3][0] + matrix[1][0] * other->matrix[3][1] + matrix[2][0] * other->matrix[3][2] + matrix[3][0];
    temp[3][1] = matrix[0][1] * other->matrix[3][0] + matrix[1][1] * other->matrix[3][1] + matrix[2][1] * other->matrix[3][2] + matrix[3][1];
    temp[3][2] = matrix[0][2] * other->matrix[3][0] + matrix[1][2] * other->matrix[3][1] + matrix[2][2] * other->matrix[3][2] + matrix[3][2];
    temp[3][3] = 1.0f;

    return CopyFromBytes(temp);
}

// FUNCTION: MIDIJAM 0x402260
AffineTransform4f* AffineTransform4f::SetTranslation(const float* translation)
{
    // IDA collapsed this to a single pointer copy — the correct form sets all three components.
    matrix[3][0] = translation[0];
    matrix[3][1] = translation[1];
    matrix[3][2] = translation[2];
    return this;
}

// FUNCTION: MIDIJAM 0x402390
AffineTransform4f* AffineTransform4f::SetRotationFromEulerZYX(const float (*eulerAngles)[3])
{
    const float cosX = cosine((*eulerAngles)[0]);
    const float sinX = sine((*eulerAngles)[0]);
    const float cosY = cosine((*eulerAngles)[1]);
    const float sinY = sine((*eulerAngles)[1]);
    const float cosZ = cosine((*eulerAngles)[2]);
    const float sinZ = sine((*eulerAngles)[2]);

    const float sinX_sinY = sinX * sinY;
    const float cosX_sinY = cosX * sinY;

    matrix[0][0] = cosY * cosZ;
    matrix[0][1] = cosY * sinZ;
    matrix[0][2] = -sinY;

    matrix[1][0] = sinX_sinY * cosZ - cosX * sinZ;
    matrix[1][1] = sinX_sinY * sinZ + cosX * cosZ;
    matrix[1][2] = sinX * cosY;

    matrix[2][0] = cosX_sinY * cosZ + sinX * sinZ;
    matrix[2][1] = cosX_sinY * sinZ - sinX * cosZ;
    matrix[2][2] = cosX * cosY;

    return this;
}

// FUNCTION: MIDIJAM 0x4024B0
AffineTransform4f* AffineTransform4f::SetRotationFromEulerXYZ(const float* eulerAngles)
{
    const float cosX = cosine(eulerAngles[0]);
    const float sinX = sine(eulerAngles[0]);
    const float cosY = cosine(eulerAngles[1]);
    const float sinY = sine(eulerAngles[1]);
    const float cosZ = cosine(eulerAngles[2]);
    const float sinZ = sine(eulerAngles[2]);

    const float sinX_sinY = sinX * sinY;
    const float cosX_sinY = cosX * sinY;

    matrix[0][0] = cosY * cosZ;
    matrix[1][0] = cosY * sinZ;
    matrix[2][0] = -sinY;

    matrix[0][1] = sinX_sinY * cosZ - cosX * sinZ;
    matrix[1][1] = sinX_sinY * sinZ + cosX * cosZ;
    matrix[2][1] = sinX * cosY;

    matrix[0][2] = cosX_sinY * cosZ + sinX * sinZ;
    matrix[1][2] = cosX_sinY * sinZ - sinX * cosZ;
    matrix[2][2] = cosX * cosY;

    return this;
}

// FUNCTION: MIDIJAM 0x43C1D0
void AffineTransform4f::RotateVectorInPlace(float (*vector)[3]) const
{
    float temp[3];
    temp[0] = (*vector)[0] * matrix[0][0] + (*vector)[1] * matrix[0][1] + (*vector)[2] * matrix[0][2];
    temp[1] = (*vector)[0] * matrix[1][0] + (*vector)[1] * matrix[1][1] + (*vector)[2] * matrix[1][2];
    temp[2] = (*vector)[0] * matrix[2][0] + (*vector)[1] * matrix[2][1] + (*vector)[2] * matrix[2][2];
    memcpy(vector, temp, sizeof(float[3]));
}

// FUNCTION: MIDIJAM 0x43C270
void AffineTransform4f::SubtractTranslationInPlace(float (*vector)[3]) const
{
    (*vector)[0] -= matrix[3][0];
    (*vector)[1] -= matrix[3][1];
    (*vector)[2] -= matrix[3][2];
}