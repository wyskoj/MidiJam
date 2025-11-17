//
// Created by Jacob on 11/16/2025.
//

#ifndef MIDIJAM_FONTS_H
#define MIDIJAM_FONTS_H
#include <cstdio>
#include <windows.h>
#include <GL/gl.h>

enum BITMAP_FONT_TYPE : __int16 {
    COLOR = 0x0,
    MASK = 0x1
};

extern char g_charWidthProportional[128];
extern __int16 g_fontCellWidth;
extern __int16 g_scaledCharacterWidths[128];

void CreateFontDisplayLists();

void DeleteFontDisplayLists();

BOOL __cdecl LoadHwfFonts(FILE *hwfStream, char *hwfAppendix_ptr, unsigned int hwfAppendixEntriesCount);

BOOL __cdecl ProcessBitmapFont(char *bitmapFontData_ptr, BITMAP_FONT_TYPE bitmapFontType);

#endif //MIDIJAM_FONTS_H
