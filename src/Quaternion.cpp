//
// Created by Jacob on 11/15/2025.
//

#include "Quaternion.h"

#include <cmath>

#include "ObjectConstructor.h"

void Quaternion::CopyFromVector(const float *source) {
    this->x = source[0];
    this->y = source[1];
    this->z = source[2];
}

void Quaternion::FromVector(const float *vector) {
    this->CopyFromVector(vector);
    this->w = 1.0;
}

void Quaternion::Identity() {
    this->z = 0.0;
    this->y = 0.0;
    this->x = 0.0;
    this->w = 1.0;
}

float Quaternion::Length() const {
    const float squaredLength = this->x * this->x + this->y * this->y + this->z * this->z;
    return sqrt(squaredLength);
}

void Quaternion::Normalize() {
    const float length = this->Length();
    this->x = this->x / length;
    this->y = this->y / length;
    this->z = this->z / length;
}

void Quaternion::TransformByMatrix(const MatrixMath *pOther) {
    const auto matrix = static_cast<const MatrixMath *>(ObjectConstructor(pOther));
    const double newY = this->x * matrix->matrix[0][1]
                  + this->y * matrix->matrix[1][1]
                  + this->z * matrix->matrix[2][1]
                  + matrix->matrix[3][1];
    const double newZ = this->x * matrix->matrix[0][2]
                  + this->y * matrix->matrix[1][2]
                  + this->z * matrix->matrix[2][2]
                  + matrix->matrix[3][2];
    const double newW = this->x * matrix->matrix[0][3]
                  + this->y * matrix->matrix[1][3]
                  + this->z * matrix->matrix[2][3]
                  + matrix->matrix[3][3];
    this->x = this->x * matrix->matrix[0][0]
              + this->y * matrix->matrix[1][0]
              + this->z * matrix->matrix[2][0]
              + matrix->matrix[3][0];
    this->y = newY;
    this->z = newZ;
    this->w = newW;
}

void Quaternion::TransformByMatrixRotation(const MatrixMath *pMatrix) {
    const auto matrix = static_cast<const MatrixMath *>(ObjectConstructor(pMatrix));
    const double newY = this->x * matrix->matrix[0][1] + this->y * matrix->matrix[1][1] + this->z * matrix->matrix[2][1];
    const double newZ = this->x * matrix->matrix[0][2] + this->y * matrix->matrix[1][2] + this->z * matrix->matrix[2][2];
    this->x = this->x * matrix->matrix[0][0] + this->y * matrix->matrix[1][0] + this->z * matrix->matrix[2][0];
    this->y = newY;
    this->z = newZ;
    this->w = 1.0;
}
