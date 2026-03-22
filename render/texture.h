//
// Created by Jacob on 3/17/2026.
//

#ifndef MIDIJAM_TEXTURE_H
#define MIDIJAM_TEXTURE_H

#include <windows.h>
#include <GL/gl.h>
#include <cstdio>

/**
 * Raw pixel data loaded from a DIB/BMP file, converted to 24-bit RGB.
 *
 * Original struct layout (sizeof = 0xC):
 *   +0x00  int    width
 *   +0x04  int    height
 *   +0x08  void*  pPixelData
 */
struct TextureData {
    int width;
    int height;
    void* pPixelData;
};

static_assert(sizeof(TextureData) == 0xC, "TextureData layout mismatch");

TextureData* LoadDibFileSafe(char* fileName);
TextureData* LoadDibFileW(const char* lpFileName);
TextureData* LoadDibFileInternal(const void* pszFileName, BOOL isUnicodePath);
short CalculateColorTableSize(const char* pDibHeader);

GLuint LoadTexture(const char* textureName);
GLuint LoadTextureFromHwf(
    const char* textureName,
    FILE* hwfStream,
    void* hwfAppendix,
    unsigned int hwfAppendixEntriesCount);
GLuint CreateTextureFromBitmapData(char* bitmapData);

/**
 * Font texture type — COLOR is the full color font bitmap,
 * MASK is the alpha mask bitmap used for blending.
 */
#if _MSC_VER < 1400
enum BitmapFontType
#else
enum BitmapFontType : short
#endif
{
    COLOR = 0,
    MASK = 1
};

char* ReadHwfAppendix(char* pHwfAppendix, FILE* hwfStream, unsigned int* nHwfAppendixItems);
BOOL ProcessBitmapFont(char* bitmapFontData, BitmapFontType bitmapFontType);
void SetTextureState(GLboolean wasEnabled);

#endif // MIDIJAM_TEXTURE_H
