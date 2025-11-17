//
// Created by Jacob on 11/16/2025.
//

#ifndef MIDIJAM_HWF_H
#define MIDIJAM_HWF_H
#include <cstdio>

char *ReadHwfAppendix(char *pHwfAppendix, std::FILE *hwfStream, unsigned int *nHwfAppendixItems);
#endif //MIDIJAM_HWF_H