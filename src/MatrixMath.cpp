//
// Created by Jacob on 11/15/2025.
//

#include "MatrixMath.h"

#include <cmath>
#include <cstring>

MatrixMath *MatrixMath::Copy(const MatrixMath *other) {
    return static_cast<MatrixMath *>(memcpy(this, other, sizeof(MatrixMath)));
}

MatrixMath *MatrixMath::CreateRotationMatrixEulerZYX(const float *eulerAngles) {
    double sinZ; // st7
    double cosZ; // [esp+4h] [ebp-40h]
    double cosX_sinY; // [esp+Ch] [ebp-38h]
    double sinX_sinY; // [esp+14h] [ebp-30h]
    double sinX; // [esp+1Ch] [ebp-28h]
    double cosX; // [esp+24h] [ebp-20h]
    double sinY; // [esp+2Ch] [ebp-18h]
    double cosY; // [esp+34h] [ebp-10h]

    cosX = cos(eulerAngles[0]);
    sinX = sin(eulerAngles[0]);
    cosY = cos(eulerAngles[1]);
    sinY = sin(eulerAngles[1]);
    cosZ = cos(eulerAngles[2]);
    sinZ = sin(eulerAngles[2]);
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

MatrixMath *MatrixMath::CreateRotationMatrixXYZ(const float *eulerAngles) {
    double v2; // st7
    double v5; // [esp+4h] [ebp-40h]
    double v6; // [esp+Ch] [ebp-38h]
    double v7; // [esp+14h] [ebp-30h]
    double v8; // [esp+1Ch] [ebp-28h]
    double v9; // [esp+24h] [ebp-20h]
    double v10; // [esp+2Ch] [ebp-18h]
    double v11; // [esp+34h] [ebp-10h]

    v9 = cos(*eulerAngles);
    v8 = sin(*eulerAngles);
    v11 = cos(eulerAngles[1]);
    v10 = sin(eulerAngles[1]);
    v5 = cos(eulerAngles[2]);
    v2 = sin(eulerAngles[2]);
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

float MatrixMath::GetElement(const int index) const {
    return this->matrix[0][index];
}

MatrixMath *MatrixMath::Multiply(const MatrixMath *other) {
    MatrixMath temp; // [esp+8h] [ebp-48h] BYREF
    MatrixMath *v4; // [esp+4Ch] [ebp-4h]

    v4 = this;
    temp.matrix[0][0] = this->matrix[0][0] * other->matrix[0][0]
                 + this->matrix[1][0] * other->matrix[0][1]
                 + this->matrix[2][0] * other->matrix[0][2];
    temp.matrix[0][1] = this->matrix[0][1] * other->matrix[0][0]
                 + this->matrix[1][1] * other->matrix[0][1]
                 + this->matrix[2][1] * other->matrix[0][2];
    temp.matrix[0][2] = this->matrix[0][2] * other->matrix[0][0]
                 + this->matrix[1][2] * other->matrix[0][1]
                 + this->matrix[2][2] * other->matrix[0][2];
    temp.matrix[0][3] = 0.0;
    temp.matrix[1][0] = this->matrix[0][0] * other->matrix[1][0]
                 + this->matrix[1][0] * other->matrix[1][1]
                 + this->matrix[2][0] * other->matrix[1][2];
    temp.matrix[1][1] = this->matrix[0][1] * other->matrix[1][0]
                 + this->matrix[1][1] * other->matrix[1][1]
                 + this->matrix[2][1] * other->matrix[1][2];
    temp.matrix[1][2] = this->matrix[0][2] * other->matrix[1][0]
                 + this->matrix[1][2] * other->matrix[1][1]
                 + this->matrix[2][2] * other->matrix[1][2];
    temp.matrix[1][3] = 0.0;
    temp.matrix[2][0] = this->matrix[0][0] * other->matrix[2][0]
                 + this->matrix[1][0] * other->matrix[2][1]
                 + this->matrix[2][0] * other->matrix[2][2];
    temp.matrix[2][1] = this->matrix[0][1] * other->matrix[2][0]
                 + this->matrix[1][1] * other->matrix[2][1]
                 + this->matrix[2][1] * other->matrix[2][2];
    temp.matrix[2][2] = this->matrix[0][2] * other->matrix[2][0]
                 + this->matrix[1][2] * other->matrix[2][1]
                 + this->matrix[2][2] * other->matrix[2][2];
    temp.matrix[2][3] = 0.0;
    temp.matrix[3][0] = this->matrix[0][0] * other->matrix[3][0]
                 + this->matrix[1][0] * other->matrix[3][1]
                 + this->matrix[2][0] * other->matrix[3][2]
                 + this->matrix[3][0];
    temp.matrix[3][1] = this->matrix[0][1] * other->matrix[3][0]
                 + this->matrix[1][1] * other->matrix[3][1]
                 + this->matrix[2][1] * other->matrix[3][2]
                 + this->matrix[3][1];
    temp.matrix[3][2] = this->matrix[0][2] * other->matrix[3][0]
                 + this->matrix[1][2] * other->matrix[3][1]
                 + this->matrix[2][2] * other->matrix[3][2]
                 + this->matrix[3][2];
    temp.matrix[3][3] = 1.0;
    return Copy(&temp);
}

MatrixMath *MatrixMath::PopulateIdentity() {
    memset(this, 0, sizeof(MatrixMath));
    matrix[3][3] = 1.0;
    matrix[2][2] = 1.0;
    matrix[1][1] = 1.0;
    matrix[0][0] = 1.0;
    return this;
}

void MatrixMath::RemoveTranslationFromVector(float (*vector)[3]) const {
    (*vector)[0] = (*vector)[0] - this->matrix[3][0];
    (*vector)[1] = (*vector)[1] - this->matrix[3][1];
    (*vector)[2] = (*vector)[2] - this->matrix[3][2];
}

MatrixMath *MatrixMath::SetTranslation(const float *translation) {
    matrix[3][0] = translation[0];
    matrix[3][1] = translation[1];
    matrix[3][2] = translation[2];
    return this;
}

void MatrixMath::TransformVectorByMatrixRotation(float (*vector)[3]) const {
    float temp[3]; // [esp+4h] [ebp-Ch] BYREF

    temp[0] = (*vector)[0] * this->matrix[0][0] + (*vector)[1] * this->matrix[0][1] + (*vector)[2] * this->matrix[0][2];
    temp[1] = (*vector)[0] * this->matrix[1][0] + (*vector)[1] * this->matrix[1][1] + (*vector)[2] * this->matrix[1][2];
    temp[2] = (*vector)[0] * this->matrix[2][0] + (*vector)[1] * this->matrix[2][1] + (*vector)[2] * this->matrix[2][2];
    memcpy(vector, temp, sizeof(float[3]));
}
