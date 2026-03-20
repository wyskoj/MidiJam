//
// Created by Jacob on 3/17/2026.
//

#ifndef MIDIJAM_WINDOW_H
#define MIDIJAM_WINDOW_H

#include <windows.h>
#include <cstdio>

LRESULT CALLBACK HandleWindowBehavior(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

bool CreateMidijamWindow(
    LPCSTR       lpWindowName,
    HINSTANCE    hInstance,
    int          windowWidth,
    int          windowHeight,
    int          bitDepth,
    DWORD        refreshRate,
    bool         isFullscreen,
    FILE*        hwfStream,
    char*        hwfAppendixPtr,
    unsigned int hwfAppendixEntriesCount);

void MidiJamWindowCleanup();

#endif // MIDIJAM_WINDOW_H