//
// Created by Jacob on 3/16/2026.
//

#include "time.h"

#include <windows.h>

// Number of 100-nanosecond intervals between the Windows FILETIME epoch
// (January 1, 1601) and the Unix epoch (January 1, 1970).
static constexpr unsigned long long FILETIME_TO_UNIX_EPOCH_OFFSET = 116444736000000000ULL;

// 100-nanosecond intervals per second.
static constexpr unsigned long long FILETIME_TICKS_PER_SECOND = 10000000ULL;

/**
 * Returns the current time as a Unix epoch timestamp (seconds since January 1, 1970).
 * Converts from the Windows FILETIME epoch (January 1, 1601) using the standard
 * 100-nanosecond interval offset. Optionally writes the result to outputTime if non-null.
 *
 * Note: the original used a direct 64-bit reinterpret of the FILETIME struct (*&systemTime),
 * which is undefined behavior. Replaced here with an equivalent explicit shift-and-or.
 *
 * @param outputTime  Optional output pointer to receive the timestamp. May be null.
 * @return            Current Unix timestamp.
 */
// FUNCTION: MIDIJAM 0x44989C
time_t GetUnixEpochTime(time_t* outputTime)
{
    FILETIME systemTime;
    GetSystemTimeAsFileTime(&systemTime);

    const unsigned long long fileTime = (static_cast<unsigned long long>(systemTime.dwHighDateTime) << 32)
        | systemTime.dwLowDateTime;
    const time_t unixTime = static_cast<time_t>((fileTime - FILETIME_TO_UNIX_EPOCH_OFFSET) / FILETIME_TICKS_PER_SECOND);

    if (outputTime)
        *outputTime = unixTime;

    return unixTime;
}

// FUNCTION: MIDIJAM 0x449918 (TODO: verify address)
void setseed(long seed)
{
    srand(static_cast<unsigned int>(seed));
}
