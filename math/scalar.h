//
// Created by Jacob on 3/16/2026.
//

#ifndef MIDIJAM_SCALAR_H
#define MIDIJAM_SCALAR_H

/**
 * Returns the cosine of value (in radians).
 * Wrapper around the CRT cos function operating on floats.
 */
float cosine(float value);

/**
 * Returns the sine of value (in radians).
 * Wrapper around the CRT sin function operating on floats.
 */
float sine(float value);

/**
 * Returns the arctangent of value (in radians).
 * Wrapper around the CRT atan function operating on floats.
 */
float arctan(float value);

/**
 * Returns the square root of value.
 * Wrapper around the CRT sqrt function operating on floats.
 */
float square_root(float value);

/**
 * Returns the absolute value of value.
 * Wrapper around the CRT fabs function operating on floats.
 */
float fAbs(float value);

#endif // MIDIJAM_SCALAR_H
