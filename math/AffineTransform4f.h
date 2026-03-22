//
// Created by Jacob on 3/16/2026.
//

#ifndef MIDIJAM_AFFINETRANSFORM4F_H
#define MIDIJAM_AFFINETRANSFORM4F_H

/**
 * A 4x4 row-major float matrix used for 3D transformations throughout MidiJam.
 *
 * Conventions:
 *   - matrix[row][col]
 *   - Translation is stored in matrix[3][0..2]
 *   - The 4th column (matrix[*][3]) is implicitly 0, 0, 0, 1 for affine transforms
 *   - Rotation functions only populate the upper-left 3x3; the rest is left as-is
 */
class AffineTransform4f {
public:
    float matrix[4][4];

    /**
     * Initializes the matrix to the identity matrix via PopulateIdentity.
     */
    AffineTransform4f();

    /**
     * Sets this matrix to the 4x4 identity matrix.
     * All elements are zeroed first, then the diagonal is set to 1.
     */
    void SetIdentity();

    /**
     * Copies 64 bytes from src into this matrix.
     * @param src  Pointer to a 4x4 float array or another AffineTransform4f instance.
     * @return     this
     */
    AffineTransform4f* CopyFromBytes(const void* src);

    /**
     * Returns a single element from the first row of the matrix.
     * @param index  Column index (0..3).
     * @return       matrix[0][index]
     */
    float GetElement(int index) const;

    /**
     * Multiplies this matrix by other and stores the result in this.
     * Uses a local temp buffer to avoid aliasing issues.
     * Only the upper-left 3x3 rotation and row 3 translation are fully computed;
     * the 4th column is hardcoded to (0, 0, 0, 1).
     * @param other  The right-hand matrix.
     * @return       this
     */
    AffineTransform4f* PostMultiply(const AffineTransform4f* other);

    /**
     * Sets the translation component of this matrix (matrix[3][0..2]).
     * @param translation  Pointer to an array of 3 floats: { x, y, z }.
     * @return             this
     */
    void SetTranslation(const float* translation);

    /**
     * Builds a rotation matrix from Euler angles applied in Z, Y, X order.
     * Only the upper-left 3x3 is written; translation and homogeneous row are untouched.
     * @param eulerAngles  Pointer to a float[3]: { angleX, angleY, angleZ } in radians.
     * @return             this
     */
    AffineTransform4f* SetRotationFromEulerZYX(const float (*eulerAngles)[3]);

    /**
     * Builds a rotation matrix from Euler angles applied in X, Y, Z order.
     * Note: populates matrix columns rather than rows — verify axis mapping at call sites.
     * Only the upper-left 3x3 is written.
     * @param eulerAngles  Pointer to a float[3]: { angleX, angleY, angleZ } in radians.
     * @return             this
     */
    AffineTransform4f* SetRotationFromEulerXYZ(const float* eulerAngles);

    /**
     * Transforms a 3-component vector by the rotation portion of this matrix (upper-left 3x3).
     * Translation is ignored. Result is written back to vector in place.
     * @param vector  Pointer to a float[3] to transform in place.
     */
    void RotateVectorInPlace(float (*vector)[3]) const;

    /**
     * Subtracts the translation component of this matrix (matrix[3][0..2]) from vector.
     * Effectively moves vector into local space relative to this matrix's origin.
     * @param vector  Pointer to a float[3] to modify in place.
     */
    void SubtractTranslationInPlace(float (*vector)[3]) const;
};

static_assert(sizeof(AffineTransform4f) == 64, "AffineTransform4f layout mismatch");

#endif // MIDIJAM_AFFINETRANSFORM4F_H
