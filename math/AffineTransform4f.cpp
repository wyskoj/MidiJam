#include "AffineTransform4f.h"

#include "scalar.h"

#include <cstring>

// FUNCTION: MIDIJAM 0x401FD0
// MATCH: EXACT
AffineTransform4f::AffineTransform4f() {
    SetIdentity();
}

// FUNCTION: MIDIJAM 0x402960
// MATCH: EXACT
void AffineTransform4f::SetIdentity() {
    memset(this, 0, sizeof(AffineTransform4f));
    matrix[3][3] = 1.0f;
    matrix[2][2] = 1.0f;
    matrix[1][1] = 1.0f;
    matrix[0][0] = 1.0f;
}

// FUNCTION: MIDIJAM 0x402940
// MATCH: EXACT
AffineTransform4f* AffineTransform4f::CopyFromBytes(const void* src) {
    return static_cast<AffineTransform4f*>(memcpy(this, src, sizeof(AffineTransform4f)));
}

// FUNCTION: MIDIJAM 0x402920
// MATCH: EXACT
float AffineTransform4f::GetElement(const int index) const {
    return matrix[0][index];
}

// FUNCTION: MIDIJAM 0x402000
AffineTransform4f* AffineTransform4f::PostMultiply(const AffineTransform4f* other) {
    float temp[4][4];
    const AffineTransform4f* v5;
    AffineTransform4f* v4;

    v4 = this;
    v5 = other;
    temp[0][0] = v4->matrix[0][0] * v5->matrix[0][0]
               + v4->matrix[1][0] * v5->matrix[0][1]
               + v4->matrix[2][0] * v5->matrix[0][2];
    temp[0][1] = v4->matrix[0][1] * v5->matrix[0][0]
               + v4->matrix[1][1] * v5->matrix[0][1]
               + v4->matrix[2][1] * v5->matrix[0][2];
    temp[0][2] = v4->matrix[0][2] * v5->matrix[0][0]
               + v4->matrix[1][2] * v5->matrix[0][1]
               + v4->matrix[2][2] * v5->matrix[0][2];
    temp[0][3] = 0.0;
    temp[1][0] = v4->matrix[0][0] * v5->matrix[1][0]
               + v4->matrix[1][0] * v5->matrix[1][1]
               + v4->matrix[2][0] * v5->matrix[1][2];
    temp[1][1] = v4->matrix[0][1] * v5->matrix[1][0]
               + v4->matrix[1][1] * v5->matrix[1][1]
               + v4->matrix[2][1] * v5->matrix[1][2];
    temp[1][2] = v4->matrix[0][2] * v5->matrix[1][0]
               + v4->matrix[1][2] * v5->matrix[1][1]
               + v4->matrix[2][2] * v5->matrix[1][2];
    temp[1][3] = 0.0;
    temp[2][0] = v4->matrix[0][0] * v5->matrix[2][0]
               + v4->matrix[1][0] * v5->matrix[2][1]
               + v4->matrix[2][0] * v5->matrix[2][2];
    temp[2][1] = v4->matrix[0][1] * v5->matrix[2][0]
               + v4->matrix[1][1] * v5->matrix[2][1]
               + v4->matrix[2][1] * v5->matrix[2][2];
    temp[2][2] = v4->matrix[0][2] * v5->matrix[2][0]
               + v4->matrix[1][2] * v5->matrix[2][1]
               + v4->matrix[2][2] * v5->matrix[2][2];
    temp[2][3] = 0.0;
    temp[3][0] = v4->matrix[0][0] * v5->matrix[3][0]
               + v4->matrix[1][0] * v5->matrix[3][1]
               + v4->matrix[2][0] * v5->matrix[3][2]
               + v4->matrix[3][0];
    temp[3][1] = v4->matrix[0][1] * v5->matrix[3][0]
               + v4->matrix[1][1] * v5->matrix[3][1]
               + v4->matrix[2][1] * v5->matrix[3][2]
               + v4->matrix[3][1];
    temp[3][2] = v4->matrix[0][2] * v5->matrix[3][0]
               + v4->matrix[1][2] * v5->matrix[3][1]
               + v4->matrix[2][2] * v5->matrix[3][2]
               + v4->matrix[3][2];
    temp[3][3] = 1.0;
    return v4->CopyFromBytes(temp);
}

// FUNCTION: MIDIJAM 0x402260
// MATCH: Exact
void AffineTransform4f::SetTranslation(const float* translation) {
    matrix[3][0] = translation[0];
    matrix[3][1] = translation[1];
    matrix[3][2] = translation[2];
}

// FUNCTION: MIDIJAM 0x402390
AffineTransform4f* AffineTransform4f::SetRotationFromEulerZYX(const float (*eulerAngles)[3]) {
    double sinZ; // st7
    double cosZ; // [esp+4h] [ebp-40h]
    double cosX_sinY; // [esp+Ch] [ebp-38h]
    double sinX_sinY; // [esp+14h] [ebp-30h]
    double sinX; // [esp+1Ch] [ebp-28h]
    double cosX; // [esp+24h] [ebp-20h]
    double sinY; // [esp+2Ch] [ebp-18h]
    double cosY; // [esp+34h] [ebp-10h]

    cosX = cosine((*eulerAngles)[0]);
    sinX = sine((*eulerAngles)[0]);
    cosY = cosine((*eulerAngles)[1]);
    sinY = sine((*eulerAngles)[1]);
    cosZ = cosine((*eulerAngles)[2]);
    sinZ = sine((*eulerAngles)[2]);
    this->matrix[0][0] = cosY * cosZ;
    this->matrix[0][1] = cosY * sinZ;
    this->matrix[0][2] = -sinY;
    sinX_sinY = sinX * sinY;
    cosX_sinY = cosX * sinY;
    this->matrix[1][0] = sinX_sinY * cosZ - cosX * sinZ;
    this->matrix[1][1] = sinX_sinY * sinZ + cosX * cosZ;
    this->matrix[1][2] = sinX * cosY;
    this->matrix[2][0] = cosX_sinY * cosZ + sinX * sinZ;
    this->matrix[2][1] = cosX_sinY * sinZ - sinX * cosZ;
    this->matrix[2][2] = cosX * cosY;
    return this;
}

// FUNCTION: MIDIJAM 0x4024B0
AffineTransform4f* AffineTransform4f::SetRotationFromEulerXYZ(const float* eulerAngles) {
    double v2; // st7
    double v5; // [esp+4h] [ebp-40h]
    double v6; // [esp+Ch] [ebp-38h]
    double v7; // [esp+14h] [ebp-30h]
    double v8; // [esp+1Ch] [ebp-28h]
    double v9; // [esp+24h] [ebp-20h]
    double v10; // [esp+2Ch] [ebp-18h]
    double v11; // [esp+34h] [ebp-10h]

    v9 = cosine(*eulerAngles);
    v8 = sine(*eulerAngles);
    v11 = cosine(eulerAngles[1]);
    v10 = sine(eulerAngles[1]);
    v5 = cosine(eulerAngles[2]);
    v2 = sine(eulerAngles[2]);
    this->matrix[0][0] = v11 * v5;
    this->matrix[1][0] = v11 * v2;
    this->matrix[2][0] = -v10;
    v7 = v8 * v10;
    v6 = v9 * v10;
    this->matrix[0][1] = v7 * v5 - v9 * v2;
    this->matrix[1][1] = v7 * v2 + v9 * v5;
    this->matrix[2][1] = v8 * v11;
    this->matrix[0][2] = v6 * v5 + v8 * v2;
    this->matrix[1][2] = v6 * v2 - v8 * v5;
    this->matrix[2][2] = v9 * v11;
    return this;
}

// FUNCTION: MIDIJAM 0x43C1D0
// MATCH: EXACT
void AffineTransform4f::RotateVectorInPlace(float (*vector)[3]) const {
    float temp[3];
    temp[0] = (*vector)[0] * matrix[0][0] + (*vector)[1] * matrix[0][1] + (*vector)[2] * matrix[0][2];
    temp[1] = (*vector)[0] * matrix[1][0] + (*vector)[1] * matrix[1][1] + (*vector)[2] * matrix[1][2];
    temp[2] = (*vector)[0] * matrix[2][0] + (*vector)[1] * matrix[2][1] + (*vector)[2] * matrix[2][2];
    memcpy(vector, temp, sizeof(float[3]));
}

// FUNCTION: MIDIJAM 0x43C270
// MATCH: EXACT
void AffineTransform4f::SubtractTranslationInPlace(float (*vector)[3]) const {
    (*vector)[0] -= matrix[3][0];
    (*vector)[1] -= matrix[3][1];
    (*vector)[2] -= matrix[3][2];
}
