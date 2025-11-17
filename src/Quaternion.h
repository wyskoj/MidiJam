//
// Created by Jacob on 11/15/2025.
//

#ifndef MIDIJAM_QUATERNION_H
#define MIDIJAM_QUATERNION_H
#include "MatrixMath.h"


class Quaternion {
public:
    float x;
    float y;
    float z;
    float w;

    void CopyFromVector(const float *source);

    void FromVector(const float *vector);

    void Identity();

    float Length() const;

    void Normalize();

    void TransformByMatrix(const MatrixMath *pOther);

    void TransformByMatrixRotation(const MatrixMath *pMatrix);
};


#endif //MIDIJAM_QUATERNION_H
