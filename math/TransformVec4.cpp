//
// Created by Jacob on 3/16/2026.
//

#include "TransformVec4.h"

#include "scalar.h"

// FUNCTION: MIDIJAM 0x448390
// MATCH: EXACT
void TransformVec4::SetXYZ(const float* xyz3)
{
    x = xyz3[0];
    y = xyz3[1];
    z = xyz3[2];
}

// FUNCTION: MIDIJAM 0x448150
// MATCH: EXACT
TransformVec4* TransformVec4::SetFromXYZ1(const float* xyz3)
{
    SetXYZ(xyz3);
    // Store 1.0f as integer constant to match original codegen (mov dword ptr, 0x3F800000)
    *reinterpret_cast<int*>(&this->w) = 0x3F800000;
    return this;
}

// FUNCTION: MIDIJAM 0x448350
// MATCH: EXACT
void TransformVec4::SetZeroXYZOneW()
{
    z = 0.0f;
    y = 0.0f;
    x = 0.0f;
    w = 1.0f;
}

// FUNCTION: MIDIJAM 0x43C320
// MATCH: EXACT
float TransformVec4::Length3() const
{
    return square_root(x * x + y * y + z * z);
}

// FUNCTION: MIDIJAM 0x43C2D0
// MATCH: EXACT
void TransformVec4::Normalize3()
{
    const float length = Length3();
    x = x / length;
    y = y / length;
    z = z / length;
}

// FUNCTION: MIDIJAM 0x448190
void TransformVec4::TransformPointByAffine(const AffineTransform4f* pAffineTransform4f)
{
    // ObjectConstructor at 0x43C2C0 is a 14-byte no-op pass-through — elided.
    const float newY = x * pAffineTransform4f->matrix[0][1]
                     + y * pAffineTransform4f->matrix[1][1]
                     + z * pAffineTransform4f->matrix[2][1]
                     + pAffineTransform4f->matrix[3][1];
    const float newZ = x * pAffineTransform4f->matrix[0][2]
                     + y * pAffineTransform4f->matrix[1][2]
                     + z * pAffineTransform4f->matrix[2][2]
                     + pAffineTransform4f->matrix[3][2];
    const float newW = x * pAffineTransform4f->matrix[0][3]
                     + y * pAffineTransform4f->matrix[1][3]
                     + z * pAffineTransform4f->matrix[2][3]
                     + pAffineTransform4f->matrix[3][3];
    x = x * pAffineTransform4f->matrix[0][0]
      + y * pAffineTransform4f->matrix[1][0]
      + z * pAffineTransform4f->matrix[2][0]
      + pAffineTransform4f->matrix[3][0];
    y = newY;
    z = newZ;
    w = newW;
}

// FUNCTION: MIDIJAM 0x448290
void TransformVec4::TransformDirectionByRotation3x3(const AffineTransform4f* pAffineTransform4f)
{
    // ObjectConstructor at 0x43C2C0 is a 14-byte no-op pass-through — elided.
    const float newY = x * pAffineTransform4f->matrix[0][1]
                     + y * pAffineTransform4f->matrix[1][1]
                     + z * pAffineTransform4f->matrix[2][1];
    const float newZ = x * pAffineTransform4f->matrix[0][2]
                     + y * pAffineTransform4f->matrix[1][2]
                     + z * pAffineTransform4f->matrix[2][2];
    x = x * pAffineTransform4f->matrix[0][0]
      + y * pAffineTransform4f->matrix[1][0]
      + z * pAffineTransform4f->matrix[2][0];
    y = newY;
    z = newZ;
    w = 1.0f;
}