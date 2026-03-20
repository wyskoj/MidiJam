//
// Created by Jacob on 3/16/2026.
//

#ifndef MIDIJAM_QUATERNIOND_H
#define MIDIJAM_QUATERNIOND_H

/**
 * A proper unit quaternion using double precision, used for skeletal animation.
 *
 * Unlike the float-based Quaternion class, all four components participate in
 * Length, Normalize, and Multiply, making this a standard Hamilton quaternion.
 *
 * Field layout (confirmed from QuaternionDIdentity):
 *   x — scalar (real) component; set to 1.0 by Identity
 *   y — vector i component
 *   z — vector j component
 *   w — vector k component
 *
 * Note: this is an unconventional layout. Most quaternion libraries place the
 * scalar in w. Verify against call sites before assuming standard conventions.
 */
struct QuaternionD
{
    double x;
    double y;
    double z;
    double w;
};

static_assert(sizeof(QuaternionD) == 32, "QuaternionD layout mismatch");

/**
 * Sets quat to the identity quaternion: x=1, y=0, z=0, w=0.
 */
void QuaternionDIdentity(QuaternionD* quat);

/**
 * Returns the length (magnitude) of all four components of a.
 */
double QuaternionDLength(QuaternionD* a);

/**
 * Normalizes quat in place by dividing all four components by their combined length.
 * Undefined behavior if length is zero.
 */
void QuaternionDNormalize(QuaternionD* quat);

/**
 * Constructs a quaternion from Euler angles (pitch, yaw, roll) in degrees.
 * Angles are converted to radians internally (multiplied by 0.017453292).
 * Result is normalized after construction.
 *
 * TODO: verify z and w component formulas against a reference implementation —
 * the decompiled output may have swapped components relative to standard convention.
 *
 * @param result  Output quaternion.
 * @param pitch   Rotation around X axis in degrees.
 * @param yaw     Rotation around Y axis in degrees.
 * @param roll    Rotation around Z axis in degrees.
 */
void QuaternionDFromEuler(QuaternionD* result, float pitch, float yaw, float roll);

/**
 * Multiplies q1 and q2 using the Hamilton product and stores the result in result.
 * Uses a temp buffer to allow result to alias q1 or q2 safely.
 * @param result  Output quaternion.
 * @param q1      Left operand.
 * @param q2      Right operand.
 */
void QuaternionDMultiply(QuaternionD* result, QuaternionD* q1, QuaternionD* q2);

/**
 * Converts a unit quaternion to axis-angle representation.
 * If the vector length is below 1e-12, returns the default axis (1, 0, 0) and angle 0.
 * @param quat    Input quaternion.
 * @param axisX   Output X component of the rotation axis.
 * @param axisY   Output Y component of the rotation axis.
 * @param axisZ   Output Z component of the rotation axis.
 * @param angle   Output rotation angle in radians.
 */
void QuaternionDToAxisAngle(QuaternionD* quat, float* axisX, float* axisY, float* axisZ, float* angle);

#endif // MIDIJAM_QUATERNIOND_H