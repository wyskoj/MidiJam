//
// Created by Jacob on 11/16/2025.
//

#include "fonts.h"

#include "text.h"

char g_charWidthProportional[128];
__int16 g_fontCellWidth;
__int16 g_scaledCharacterWidths[128];

void CreateFontDisplayLists() {
    GLfloat texCoordRight_1; // [esp+10h] [ebp-2Ch]
    GLfloat texCoordRight; // [esp+10h] [ebp-2Ch]
    GLfloat texCoordBottom; // [esp+14h] [ebp-28h]
    GLfloat texCoordBottom_1; // [esp+14h] [ebp-28h]
    GLfloat texCoordV_Top; // [esp+14h] [ebp-28h]
    GLfloat texCoordTop; // [esp+14h] [ebp-28h]
    __int16 fontIndex; // [esp+2Ch] [ebp-10h]
    float texCoordTopRow; // [esp+30h] [ebp-Ch]
    GLfloat texCoordLeft; // [esp+34h] [ebp-8h]
    __int16 charIndex; // [esp+38h] [ebp-4h]

    for (fontIndex = 0; fontIndex < 2; ++fontIndex) {
        g_fontDisplayListBase[fontIndex] = glGenLists(256);
        glBindTexture(GL_TEXTURE_2D, g_fontTextureArray[fontIndex]);
        for (charIndex = 0; charIndex < 256; ++charIndex) {
            texCoordLeft = (charIndex % 16) / 16.0;
            texCoordTopRow = (charIndex / 16) / 16.0;
            glNewList(g_fontDisplayListBase[fontIndex] + charIndex, GL_COMPILE);
            glBegin(GL_QUADS);
            texCoordBottom = 1.0 - texCoordTopRow - 0.0625;
            glTexCoord2f(texCoordLeft, texCoordBottom);
            glVertex2i(0, 0);
            texCoordBottom_1 = 1.0 - texCoordTopRow - 0.0625;
            texCoordRight_1 = texCoordLeft + 0.0625;
            glTexCoord2f(texCoordRight_1, texCoordBottom_1);
            glVertex2i(32, 0);
            texCoordV_Top = 1.0 - texCoordTopRow;
            texCoordRight = texCoordLeft + 0.0625;
            glTexCoord2f(texCoordRight, texCoordV_Top);
            glVertex2i(32, 32);
            texCoordTop = 1.0 - texCoordTopRow;
            glTexCoord2f(texCoordLeft, texCoordTop);
            glVertex2i(0, 32);
            glEnd();
            glTranslated((g_charWidthProportional[charIndex % 128] / g_fontCellWidth * 35.0), 0.0, 0.0);
            glEndList();
        }
    }
}

void DeleteFontDisplayLists() {
    __int16 fontIndex; // [esp+0h] [ebp-4h]

    for (fontIndex = 0; fontIndex < 2; ++fontIndex)
        glDeleteLists(g_fontDisplayListBase[fontIndex], 256);
}

BOOL LoadHwfFonts(FILE *hwfStream, char *hwfAppendix_ptr, unsigned int hwfAppendixEntriesCount) {
    void *bitmapFontData_ptr; // [esp+8h] [ebp-28h]
    unsigned int i; // [esp+Ch] [ebp-24h]
    char bitmapFontName[20]; // [esp+10h] [ebp-20h] BYREF
    int _; // [esp+24h] [ebp-Ch]
    int __; // [esp+28h] [ebp-8h]
    short bitmapFontType; // [esp+2Ch] [ebp-4h]

    __ = 1;
    for (bitmapFontType = 0; bitmapFontType < 2; ++bitmapFontType) {
        sprintf_s(bitmapFontName, "Font%d.bmp", bitmapFontType); // make name of bitmap font
        _ = 0;
        fseek(hwfStream, 0, 0); // jump to start of file
        for (i = 0; i < hwfAppendixEntriesCount; ++i) {
            // Compare the current appendix entry's name to the font name we want.
            if (!_stricmp(bitmapFontName, &hwfAppendix_ptr[264 * i])) {
                // ~~ this is a bitmap font ~~
                bitmapFontData_ptr = operator new(*reinterpret_cast<int *>(&hwfAppendix_ptr[264 * i + 260]));
                // size is at index 260, alloc memory based on that value
                fread(bitmapFontData_ptr, 1u, *reinterpret_cast<int *>(&hwfAppendix_ptr[264 * i + 260]), hwfStream); // read bitmap font
                _ = ProcessBitmapFont(static_cast<char *>(bitmapFontData_ptr),
                                      static_cast<BITMAP_FONT_TYPE>(bitmapFontType));
                free(bitmapFontData_ptr); // free temp. buffer
                break;
            }
            // ~~ not a bitmap font ~~
            // but we read how many bytes large this asset is and skip by that many
            fseek(hwfStream, *reinterpret_cast<int *>(&hwfAppendix_ptr[264 * i + 260]), 1);
        }
    }
    return TRUE;
}

BOOL ProcessBitmapFont(char *bitmapFontData_ptr, BITMAP_FONT_TYPE bitmapFontType) {
    __int64 rowBitsWithPadding; // rax
    __int16 pixelX; // [esp+10h] [ebp-48h]
    __int16 charCol; // [esp+14h] [ebp-44h]
    __int16 charRow; // [esp+18h] [ebp-40h]
    __int16 pixelY; // [esp+1Ch] [ebp-3Ch]
    BITMAPFILEHEADER fileHeader; // [esp+28h] [ebp-30h] BYREF
    BITMAPINFOHEADER *infoHeader; // [esp+38h] [ebp-20h]
    unsigned int pixelCount; // [esp+3Ch] [ebp-1Ch]
    size_t infoHeaderSize; // [esp+40h] [ebp-18h]
    BYTE *pixelData; // [esp+44h] [ebp-14h]
    GLubyte temp; // [esp+4Bh] [ebp-Dh]
    size_t pixelDataSize; // [esp+4Ch] [ebp-Ch]
    unsigned int i; // [esp+50h] [ebp-8h]
    BOOL v16; // [esp+54h] [ebp-4h]

    v16 = 0;
    // 1. PARSE BMP HEADER
    memcpy(&fileHeader, bitmapFontData_ptr, sizeof(fileHeader)); // Read header
    if (fileHeader.bfType != 0x4D42) // Check to make sure this is a BMP ('BM' 0x4D42 = 19778)
        return FALSE; // If not, fail
    infoHeaderSize = fileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
    infoHeader = static_cast<BITMAPINFOHEADER *>(malloc(fileHeader.bfOffBits - sizeof(BITMAPFILEHEADER)));
    memcpy(infoHeader, bitmapFontData_ptr + sizeof(BITMAPFILEHEADER), infoHeaderSize);
    rowBitsWithPadding = infoHeader->biWidth * infoHeader->biBitCount + 7;
    pixelDataSize = abs(infoHeader->biHeight) * (((static_cast<int>(rowBitsWithPadding) & 7) + rowBitsWithPadding) >> 3);
    pixelData = static_cast<BYTE *>(malloc(pixelDataSize));
    memcpy(pixelData, &bitmapFontData_ptr[infoHeaderSize + sizeof(BITMAPFILEHEADER)], pixelDataSize);
    pixelCount = infoHeader->biHeight * infoHeader->biWidth;
    // Convert BGR to RGB
    for (i = 0; i < pixelCount; ++i) {
        temp = pixelData[3 * i];
        pixelData[3 * i] = pixelData[3 * i + 2];
        pixelData[3 * i + 2] = temp;
    }
    if (bitmapFontType == COLOR) {
        g_fontCellWidth = (infoHeader->biWidth / 16);
        for (charRow = 0; charRow < 8; ++charRow) // 8 rows of characters
        {
            for (charCol = 0; charCol < 16; ++charCol) // 16 cols of characters
            {
                // perform width analysis
                g_charWidthProportional[16 * (7 - charRow) + charCol] = 0;
                for (pixelY = 0; pixelY < infoHeader->biHeight / 16; ++pixelY) {
                    for (pixelX = 0; pixelX < infoHeader->biWidth / 16; ++pixelX) {
                        if (pixelData[3 * infoHeader->biWidth * pixelY
                                      + 3 * infoHeader->biWidth / 16 * charCol
                                      + 3 * pixelX
                                      + infoHeader->biHeight / 16 * 3 * infoHeader->biWidth * (charRow + 8)] != 255
                            && pixelX > g_charWidthProportional[16 * (7 - charRow) + charCol]) {
                            g_charWidthProportional[16 * (7 - charRow) + charCol] = pixelX;
                        }
                    }
                }
                if (!g_charWidthProportional[16 * (7 - charRow) + charCol])
                    g_charWidthProportional[16 * (7 - charRow) + charCol] = infoHeader->biWidth / 16 / 2;
                g_scaledCharacterWidths[16 * (7 - charRow) + charCol] = (
                    g_charWidthProportional[16 * (7 - charRow) + charCol]
                    / g_fontCellWidth
                    * 35.0);
            }
        }
    }
    glGenTextures(1, &g_fontTextureArray[bitmapFontType]);
    glBindTexture(GL_TEXTURE_2D, g_fontTextureArray[bitmapFontType]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, infoHeader->biWidth, infoHeader->biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 pixelData);
    free(pixelData);
    free(infoHeader);
    return v16;
}
