//
// Created by Jacob on 3/21/2026.
//

#ifndef MIDIJAM_VS2003_H
#define MIDIJAM_VS2003_H

// VS2003 (MSVC 13.10) does not support C++11 features.
// Provide compatibility shims when building with the original compiler.

#if _MSC_VER < 1400

// static_assert shim — silently disabled under VS2003.
// Layout verification is done via runtime checks or modern compiler builds.
template<bool> struct _static_assert_vs2003 {};
template<> struct _static_assert_vs2003<true> { typedef int type; };
#define static_assert(cond, msg) \
typedef _static_assert_vs2003<(cond)>::type _sa_##__LINE__

// nullptr shim
const class {
public:
    template<class T> operator T*() const { return 0; }
    template<class C, class T> operator T C::*() const { return 0; }
private:
    void operator&() const;
} nullptr = {};

// override keyword shim
#define override

#include <stddef.h>
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef __int64            int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned __int64   uint64_t;

#define constexpr const

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
#ifndef WINVER
#define WINVER 0x0500
#endif

#if _MSC_VER < 1400
#include <time.h>
#endif

#endif // _MSC_VER < 1400

#endif // MIDIJAM_VS2003_H