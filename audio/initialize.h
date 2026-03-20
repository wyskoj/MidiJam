//
// Created by Jacob on 3/19/2026.
//

#ifndef MIDIJAM_INITIALIZE_H
#define MIDIJAM_INITIALIZE_H

#include <windows.h>
#include <cstdio>

enum WINDOW_CREATE_STATUS
{
    SUCCESS = 0x00,
    FAILURE = 0x2B,
};

WINDOW_CREATE_STATUS MidiJamMain(
    LPCSTR       lpWindowName,
    HINSTANCE    hInstance,
    FILE*        hwfStream,
    void*        pHwfAppendix,
    unsigned int nHwfAppendixItems);

void MidiJamInitialize();

#endif // MIDIJAM_INITIALIZE_H