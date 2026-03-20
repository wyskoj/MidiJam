//
// Created by Jacob on 3/16/2026.
//

#include <windows.h>

/**
 * Internal implementation of GUID equality comparison.
 * Performs a 16-byte memcmp of the two GUIDs.
 * @param guid1  First GUID to compare.
 * @param guid2  Second GUID to compare.
 * @return       TRUE if the GUIDs are equal, FALSE otherwise.
 */
// FUNCTION: MIDIJAM 0x445AD0
static BOOL IsEqualGUIDImpl(const GUID *guid1, const GUID *guid2) {
    return memcmp(guid1, guid2, sizeof(GUID)) == 0;
}

/**
 * Returns TRUE if guid1 and guid2 are equal.
 * Delegates to IsEqualGUIDImpl for a 16-byte comparison.
 * @param guid1  First GUID to compare.
 * @param guid2  Second GUID to compare.
 * @return       TRUE if the GUIDs are equal, FALSE otherwise.
 */
// FUNCTION: MIDIJAM 0x445AB0
BOOL IsEqualGUID(const GUID *guid1, const GUID *guid2) {
    return IsEqualGUIDImpl(guid1, guid2);
}
