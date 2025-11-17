//
// Created by Jacob on 11/15/2025.
//

#include "textures.h"

GLuint CreateTextureFromBitmapData(char *bitmapData) {
    __int64 strideWithPadding; // rax
    BITMAPFILEHEADER texture_header; // [esp+4h] [ebp-30h] BYREF
    BITMAPINFOHEADER *infoHeader; // [esp+14h] [ebp-20h]
    unsigned int totalPixels; // [esp+18h] [ebp-1Ch]
    BYTE *pixelData; // [esp+20h] [ebp-14h]
    BYTE temp; // [esp+27h] [ebp-Dh]
    size_t pixelDataSize; // [esp+28h] [ebp-Ch]
    unsigned int i; // [esp+2Ch] [ebp-8h]
    GLuint textureId; // [esp+30h] [ebp-4h] BYREF

    textureId = 0;
    memcpy(&texture_header, bitmapData, sizeof(BITMAPFILEHEADER));
    if (texture_header.bfType != 19778) // must start with "BM" for Windows Bitmap
        return 0;
    size_t infoHeaderSize = texture_header.bfOffBits - sizeof(BITMAPFILEHEADER); // size of info header + color table
    infoHeader = static_cast<BITMAPINFOHEADER *>(malloc(infoHeaderSize)); // allocate memory for bitmap data
    memcpy(infoHeader, bitmapData + sizeof(BITMAPFILEHEADER), infoHeaderSize); // copy bitmap data into buffer
    strideWithPadding = infoHeader->biWidth * infoHeader->biBitCount + 7;
    pixelDataSize = abs(infoHeader->biHeight) * ((((strideWithPadding >> 32) & 0xFF & 7) + strideWithPadding) >> 3);
    pixelData = static_cast<BYTE *>(malloc(pixelDataSize));
    memcpy(pixelData, bitmapData + texture_header.bfOffBits, pixelDataSize);
    totalPixels = infoHeader->biHeight * infoHeader->biWidth;
    // Swap R and B channels (BGR -> RGB)
    for (i = 0; i < totalPixels; ++i) {
        temp = pixelData[3 * i]; // blue
        pixelData[3 * i] = pixelData[3 * i + 2]; // blue <- red
        pixelData[3 * i + 2] = temp; // red -< blue
    }
    // OpenGL texture setup
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, infoHeader->biWidth, infoHeader->biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 pixelData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Cleanup
    free(pixelData);
    free(infoHeader);
    return textureId;
}

GLuint LoadTextureFromHwf(const char *texture_name, FILE *hwfStream, void *hwfAppendix,
                          const unsigned int hwfAppendixEntriesCount) {
    fseek(hwfStream, 0, 0);
    for (unsigned int i = 0; i < hwfAppendixEntriesCount; ++i) {
        if (!_stricmp(texture_name, static_cast<const char *>(hwfAppendix) + 264 * i)) // search for texture with name
        {
            char *texture_buffer = static_cast<char *>(operator new(*(static_cast<int *>(hwfAppendix) + 66 * i + 65)));
            // based on size in appendix
            fread(texture_buffer, 1u, *(static_cast<int *>(hwfAppendix) + 66 * i + 65), hwfStream);
            const GLuint texture_ptr = CreateTextureFromBitmapData(texture_buffer);
            free(texture_buffer);
            return texture_ptr;
        }
        fseek(hwfStream, *(static_cast<int *>(hwfAppendix) + 66 * i + 65), 1);
    }
    return 0;
}

void __stdcall SetTextureState(bool useTextures) {
    if (useTextures)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);
}
