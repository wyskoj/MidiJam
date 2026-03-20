//
// Created by Jacob on 3/16/2026.
//

#include <windows.h>

#include <cstring>

/**
 * Internal implementation of ANSI to Unicode conversion.
 * Converts lpMultiByteStr to a wide character string in lpWideCharStr.
 * If a3 is -1, the length is determined from lpMultiByteStr automatically.
 * The output is always null-terminated within the buffer size.
 */
// FUNCTION: MIDIJAM 0x4011D0
// ReSharper disable CppParameterMayBeConst
static void AnsiToUnicodeImpl(LPWSTR lpWideCharStr, LPCCH lpMultiByteStr, int a3)
// ReSharper restore CppParameterMayBeConst
{
    // Null checks are redundant when called from AnsiToUnicode, but retained
    // from the original — AnsiToUnicodeImpl may have had other call sites.
    // ReSharper disable CppDFAConstantConditions
    if (lpWideCharStr && lpMultiByteStr) {
        // ReSharper restore CppDFAConstantConditions
        if (a3 == -1)
            a3 = static_cast<int>(strlen(lpMultiByteStr)) + 1;
        MultiByteToWideChar(0, 0, lpMultiByteStr, -1, lpWideCharStr, a3 - 1);
        lpWideCharStr[a3 - 1] = 0;
    }
}

/**
 * Converts a multibyte ANSI string to a wide character (Unicode) string.
 * Null-checks both pointers before delegating to AnsiToUnicodeImpl.
 * @param lpWideCharStr   Output wide string buffer.
 * @param lpMultiByteStr  Input ANSI string.
 * @param a3              Output buffer size in characters, or -1 to auto-detect from input length.
 */
// FUNCTION: MIDIJAM 0x401230
// ReSharper disable CppParameterMayBeConst
void AnsiToUnicode(LPWSTR lpWideCharStr, LPCCH lpMultiByteStr, const int a3)
// ReSharper restore CppParameterMayBeConst
{
    if (lpWideCharStr && lpMultiByteStr)
        AnsiToUnicodeImpl(lpWideCharStr, lpMultiByteStr, a3);
}
