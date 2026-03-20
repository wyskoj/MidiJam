//
// Created by Jacob on 3/16/2026.
//

#ifndef MIDIJAM_QUATERNION_H
#define MIDIJAM_QUATERNION_H

#include "MatrixMath.h"

/**
 * A 4-component structure used throughout MidiJam for 3D position and orientation.
 *
 * Despite the name, this is not used as a standard unit quaternion in all contexts.
 * In many cases it functions as a 3D vector with w as a tag or homogeneous coordinate.
 * Notably, Length() and Normalize() operate only on x, y, z — w is excluded.
 *
 * Field layout (confirmed from CopyFromVector and Identity):
 *   x, y, z — spatial components
 *   w       — homogeneous/tag component; set to 1.0 by Identity and FromVector
 */
class Quaternion
{
public:
    float x;
    float y;
    float z;
    float w;

    /**
     * Copies x, y, z from source. w is left unmodified.
     * @param source  Pointer to a float[3]: { x, y, z }.
     */
    void CopyFromVector(const float* source);

    /**
     * Copies x, y, z from vector and sets w to 1.0.
     * @param vector  Pointer to a float[3]: { x, y, z }.
     */
    void FromVector(const float* vector);

    /**
     * Sets x, y, z to 0.0 and w to 1.0.
     * Equivalent to the identity position/orientation.
     */
    void Identity();

    /**
     * Returns the length of the x, y, z components only. w is not included.
     */
    float Length() const;

    /**
     * Normalizes x, y, z by dividing each by Length(). w is not modified.
     * Undefined behavior if Length() is zero.
     */
    void Normalize();

    /**
     * Transforms this quaternion as a 4-component vector by the full matrix,
     * including translation (row 3). All four output components are written.
     * @param pOther  The matrix to transform by.
     */
    void TransformByMatrix(const MatrixMath* pOther);

    /**
     * Transforms x, y, z by the rotation portion of the matrix only (upper-left 3x3).
     * Translation is ignored. w is set to 1.0 after the transform.
     * @param pMatrix  The matrix to transform by.
     */
    void TransformByMatrixRotation(const MatrixMath* pMatrix);
};

static_assert(sizeof(Quaternion) == 16, "Quaternion layout mismatch");

#endif // MIDIJAM_QUATERNION_H