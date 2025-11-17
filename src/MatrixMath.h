//
// Created by Jacob on 11/15/2025.
//

#ifndef MIDIJAM_MATRIXMATH_H
#define MIDIJAM_MATRIXMATH_H

class MatrixMath {
public:
    float matrix[4][4];


    MatrixMath* Copy(const MatrixMath* other);
    MatrixMath* CreateRotationMatrixEulerZYX(const float *eulerAngles);
    MatrixMath* CreateRotationMatrixXYZ(const float *eulerAngles);
    float GetElement(int index) const;
    MatrixMath* Multiply(const MatrixMath* other);
    MatrixMath* PopulateIdentity();
    void RemoveTranslationFromVector(float (*vector)[3]) const;
    MatrixMath* SetTranslation(const float* translation);
    void TransformVectorByMatrixRotation(float (*vector)[3]) const;
};

#endif //MIDIJAM_MATRIXMATH_H