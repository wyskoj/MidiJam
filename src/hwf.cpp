//
// Created by Jacob on 11/16/2025.
//

#include "hwf.h"

#include <cstdlib>

char * ReadHwfAppendix(char *pHwfAppendix, std::FILE *hwfStream, unsigned int *nHwfAppendixItems) {
    char *hwfAppendix_ptr;

    fseek(hwfStream, 0, 2); // jump to end of file
    fseek(hwfStream, -4, 1); // move back 4 bytes
    fread(nHwfAppendixItems, 4u, 1u, hwfStream); // read footer

    // allocate 264 * (value read in footer)
    // this is like 264 bytes per asset for the appendix
    hwfAppendix_ptr = static_cast<char *>(realloc(pHwfAppendix, 264 * *nHwfAppendixItems));

    fseek(hwfStream, 0, 2); // again jump to end of file
    fseek(hwfStream, -4, 1); // move back 4 bytes
    fseek(hwfStream, -264 * *nHwfAppendixItems, 1); // move back 264 bytes per asset

    for (unsigned int i = 0; i < *nHwfAppendixItems; ++i) // for the number of appendix items,
        fread(&hwfAppendix_ptr[264 * i], 264u, 1u, hwfStream); // read it into memory

    return hwfAppendix_ptr;
}
