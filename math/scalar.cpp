//
// Created by Jacob on 3/16/2026.
//

#include "scalar.h"

#include <cmath>

// FUNCTION: MIDIJAM 0x4028C0
static float cosImpl(const float value) {
    return cosf(value);
}

// FUNCTION: MIDIJAM 0x4028A0
// MATCH: EXACT
float cosine(const float value) {
    return cosImpl(value);
}

// FUNCTION: MIDIJAM 0x402900
static float sinImpl(const float value) {
    return sinf(value);
}

// FUNCTION: MIDIJAM 0x4028E0
// MATCH: EXACT
float sine(const float value) {
    return sinImpl(value);
}

// FUNCTION: MIDIJAM 0x438E80
static float atan_0(const float value) {
    return atanf(value);
}

// FUNCTION: MIDIJAM 0x438E60
// MATCH: EXACT
float arctan(const float value) {
    return atan_0(value);
}

// FUNCTION: MIDIJAM 0x43C380
static float sqrt_0(const float value) {
    return sqrtf(value);
}

// FUNCTION: MIDIJAM 0x43C360
// MATCH: EXACT
float square_root(const float value) {
    return sqrt_0(value);
}

// FUNCTION: MIDIJAM 0x438E40
static float fabs_0(const float value) {
    return fabsf(value);
}

// FUNCTION: MIDIJAM 0x438E20
// MATCH: EXACT
float fAbs(const float value) {
    return fabs_0(value);
}
