//
// Created by Jacob on 11/15/2025.
//

#ifndef MIDIJAM_TEXTURES_H
#define MIDIJAM_TEXTURES_H
#include <windows.h>
#include <cstdio>
#include <GL/gl.h>

GLuint __cdecl CreateTextureFromBitmapData(char *bitmapData);

GLuint __cdecl LoadTextureFromHwf(const char *texture_name, FILE *hwfStream, void *hwfAppendix,
                                  unsigned int hwfAppendixEntriesCount);

void __stdcall SetTextureState(bool useTextures);

#endif //MIDIJAM_TEXTURES_H