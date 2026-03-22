//
// Created by Jacob on 3/17/2026.
//

#include "texture.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>

// Controls whether error message boxes are shown to the user.
extern int SHOW_ERRORS;

// FUNCTION: MIDIJAM 0x4486A1
short CalculateColorTableSize(const char* pDibHeader)
{
    short bitCount;

    if (*pDibHeader == 12)
    {
        // BITMAPCOREHEADER (OS/2 format) — bit count is at byte offset 5.
        bitCount = *(reinterpret_cast<const short*>(pDibHeader) + 5 / 2);
        // TODO: verify byte offset — IDA shows *(pDibHeader + 5) as __int16 access.
        bitCount = *reinterpret_cast<const short*>(pDibHeader + 5);
    }
    else
    {
        // BITMAPINFOHEADER — biClrUsed is at DWORD offset 8 (byte offset 32).
        short colorCount = *reinterpret_cast<const short*>(pDibHeader + 8 * sizeof(int));
        if (colorCount)
            return colorCount;
        bitCount = *reinterpret_cast<const short*>(pDibHeader + 7 * sizeof(int) / 2);
        // TODO: IDA shows *(pDibHeader + 7) as __int16 — verify these are DWORD-indexed fields.
        bitCount = *reinterpret_cast<const short*>(pDibHeader + 14); // biBitCount offset in BITMAPINFOHEADER
    }

    switch (bitCount)
    {
    case 1: return 2;
    case 4: return 16;
    case 8: return 256;
    default: return 0;
    }
}

// FUNCTION: MIDIJAM 0x4486F7
TextureData* LoadDibFileInternal(const void* pszFileName, BOOL isUnicodePath)
{
    TextureData* pTextureResult = nullptr;
    void* pPixelBuffer = nullptr;
    LPCVOID pMappedData = nullptr;
    HBITMAP hBitmap = nullptr;
    BITMAPINFO* pTempBitmapInfo = nullptr;
    char errorText[256];

    // Open the file.
    HANDLE hFile;
    if (isUnicodePath)
        hFile = CreateFileW(static_cast<LPCWSTR>(pszFileName), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0,
        nullptr);
    else
        hFile = CreateFileA(static_cast<LPCSTR>(pszFileName), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0,
        nullptr);

    HANDLE hFileHandle = hFile;

    if (hFile == INVALID_HANDLE_VALUE)
        goto LOAD_ERROR;

    {
        HANDLE hFileMapping = CreateFileMappingA(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
        HANDLE hMapping = hFileMapping;

        if (!hFileMapping)
            goto CLOSE_HANDLES;

        char* pDibData = static_cast<char*>(MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0));
        pMappedData = pDibData;

        if (!pDibData)
        {
            CloseHandle(hFileMapping);
            goto CLOSE_HANDLES;
        }

        char* pDibHeader = nullptr;
        char* pSourcePixelData = nullptr;

        // Check BMP file signature ('BM' = 0x424D, stored little-endian as 'MB' in IDA).
        if (*reinterpret_cast<const short*>(pDibData) == 0x4D42) // 'BM'
        {
            pDibHeader = pDibData + 14;
            pSourcePixelData = pDibData + *reinterpret_cast<int*>(pDibData + 10);
        }
        else
        {
            pDibHeader = pDibData;
        }

        short colorTableSize = CalculateColorTableSize(pDibHeader);

        BITMAPINFO* pBitmapInfo = static_cast<BITMAPINFO*>(
            LocalAlloc(LMEM_FIXED, 4 * colorTableSize + sizeof(BITMAPINFOHEADER)));

        if (!pBitmapInfo)
        {
            if (SHOW_ERRORS)
            {
                HWND hWnd = GetFocus();
                MessageBoxA(hWnd, "Out of memory.", "Error", 0);
            }
            UnmapViewOfFile(pMappedData);
            CloseHandle(hMapping);
            goto CLOSE_HANDLES;
        }

        if (*reinterpret_cast<int*>(pDibHeader) == 12)
        {
            // BITMAPCOREHEADER (OS/2 format) — convert to BITMAPINFOHEADER.
            pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            pBitmapInfo->bmiHeader.biWidth = *reinterpret_cast<short*>(pDibHeader + 2);
            pBitmapInfo->bmiHeader.biHeight = *reinterpret_cast<short*>(pDibHeader + 3 * sizeof(short));
            pBitmapInfo->bmiHeader.biPlanes = *reinterpret_cast<short*>(pDibHeader + 4 * sizeof(short));
            pBitmapInfo->bmiHeader.biBitCount = *reinterpret_cast<short*>(pDibHeader + 5 * sizeof(short));
            pBitmapInfo->bmiHeader.biCompression = 0;
            pBitmapInfo->bmiHeader.biSizeImage = 0;
            pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
            pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
            pBitmapInfo->bmiHeader.biClrUsed = colorTableSize;
            pBitmapInfo->bmiHeader.biClrImportant = colorTableSize;

            // Convert 3-byte OS/2 palette entries to 4-byte RGBQUAD entries.
            BYTE* bmiColors = reinterpret_cast<BYTE*>(pBitmapInfo->bmiColors);
            BYTE* pSourceColors = reinterpret_cast<BYTE*>(pDibHeader + 12);
            for (int c = 0; c < colorTableSize; ++c)
            {
                bmiColors[2] = pSourceColors[2];
                bmiColors[1] = pSourceColors[1];
                bmiColors[0] = pSourceColors[0];
                bmiColors[3] = 0;
                bmiColors += 4;
                pSourceColors += 3;
            }

            if (!pSourcePixelData)
                pSourcePixelData = pDibHeader + 12 + 3 * colorTableSize;
        }
        else if (*reinterpret_cast<int*>(pDibHeader) == sizeof(BITMAPINFOHEADER))
        {
            // Standard BITMAPINFOHEADER.
            BITMAPINFOHEADER* pSrcHeader =
                reinterpret_cast<BITMAPINFOHEADER*>(pDibHeader);
            pBitmapInfo->bmiHeader = *pSrcHeader;
            memcpy(pBitmapInfo->bmiColors,
                   pDibHeader + sizeof(BITMAPINFOHEADER),
                   4 * colorTableSize);

            if (!pSourcePixelData)
                pSourcePixelData = pDibHeader + sizeof(BITMAPINFOHEADER) + 4 * colorTableSize;
        }
        else
        {
            if (SHOW_ERRORS)
            {
                HWND hWnd = GetFocus();
                MessageBoxA(hWnd, "Unknown DIB file format.", "Error", 0);
            }
            LocalFree(pBitmapInfo);
            UnmapViewOfFile(pMappedData);
            CloseHandle(hMapping);
            goto CLOSE_HANDLES;
        }

        if (!pBitmapInfo->bmiHeader.biSizeImage)
            pBitmapInfo->bmiHeader.biSizeImage =
                pBitmapInfo->bmiHeader.biHeight *
                ((pBitmapInfo->bmiHeader.biWidth * pBitmapInfo->bmiHeader.biBitCount + 7) >> 3);

        if (!pBitmapInfo->bmiHeader.biClrUsed)
            pBitmapInfo->bmiHeader.biClrUsed = colorTableSize;

        HDC hMemDC = CreateCompatibleDC(nullptr);
        HDC hDC = hMemDC;

        if (hMemDC)
        {
            BITMAPINFO* pConvertedBitmapInfo =
                static_cast<BITMAPINFO*>(LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)));
            pTempBitmapInfo = pConvertedBitmapInfo;

            if (pConvertedBitmapInfo)
            {
                pConvertedBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                pConvertedBitmapInfo->bmiHeader.biWidth = pBitmapInfo->bmiHeader.biWidth;
                pConvertedBitmapInfo->bmiHeader.biHeight = pBitmapInfo->bmiHeader.biHeight;
                pConvertedBitmapInfo->bmiHeader.biSizeImage =
                    3 * pConvertedBitmapInfo->bmiHeader.biWidth
                    * abs(pConvertedBitmapInfo->bmiHeader.biHeight);
                pConvertedBitmapInfo->bmiHeader.biPlanes = 1;
                pConvertedBitmapInfo->bmiHeader.biBitCount = 24;
                pConvertedBitmapInfo->bmiHeader.biCompression = 0;

                void* pDibSectionBits = nullptr;
                HBITMAP hDibSection = CreateDIBSection(
                    hMemDC, pConvertedBitmapInfo, DIB_RGB_COLORS,
                    &pDibSectionBits, nullptr, 0);
                hBitmap = hDibSection;

                if (hDibSection)
                {
                    if (SelectObject(hDC, hDibSection))
                    {
                        if (SetDIBits(hDC, hDibSection, 0,
                                      pBitmapInfo->bmiHeader.biHeight,
                                      pSourcePixelData, pBitmapInfo,
                                      DIB_RGB_COLORS))
                        {
                            GdiFlush();

                            void* pConvertedPixels =
                                malloc(pConvertedBitmapInfo->bmiHeader.biSizeImage);
                            pPixelBuffer = pConvertedPixels;

                            if (pConvertedPixels)
                            {
                                // Convert BGR to RGB and strip row padding.
                                BYTE* pSourcePixels = static_cast<BYTE*>(pDibSectionBits);
                                int sourceRowPadding =
                                    pBitmapInfo->bmiHeader.biWidth & 3;
                                BYTE* pDestPixel =
                                    static_cast<BYTE*>(pConvertedPixels);

                                for (int row = 0;
                                     row < pBitmapInfo->bmiHeader.biHeight;
                                     ++row)
                                {
                                    for (LONG x = 0;
                                         x < pBitmapInfo->bmiHeader.biWidth;
                                         ++x)
                                    {
                                        pDestPixel[0] = pSourcePixels[2]; // R
                                        pDestPixel[1] = pSourcePixels[1]; // G
                                        pDestPixel[2] = pSourcePixels[0]; // B
                                        pDestPixel += 3;
                                        pSourcePixels += 3;
                                    }
                                    pSourcePixels += sourceRowPadding;
                                }

                                pTextureResult =
                                    static_cast<TextureData*>(malloc(sizeof(TextureData)));
                                if (pTextureResult)
                                {
                                    pTextureResult->width = pBitmapInfo->bmiHeader.biWidth;
                                    pTextureResult->height = pBitmapInfo->bmiHeader.biHeight;
                                    pTextureResult->pPixelData = pConvertedPixels;
                                }
                            }
                        }
                        else if (SHOW_ERRORS)
                        {
                            HWND hErrorWnd = GetFocus();
                            MessageBoxA(hErrorWnd, "Image file conversion error.", "Error", 0);
                        }
                    }
                }
            }
        }

        if (hDC) DeleteDC(hDC);
        if (hBitmap) DeleteObject(hBitmap);
        if (pTempBitmapInfo) LocalFree(pTempBitmapInfo);

        LocalFree(pBitmapInfo);
        UnmapViewOfFile(pMappedData);
        CloseHandle(hMapping);
        goto CLOSE_HANDLES;
    }

LOAD_ERROR:
    if (isUnicodePath)
        wsprintfA(errorText, "Failed to open DIB file %ws.\n", pszFileName);
    else
        wsprintfA(errorText, "Failed to open DIB file %s.\n", pszFileName);

    if (SHOW_ERRORS)
    {
        HWND hFocusWnd = GetFocus();
        MessageBoxA(hFocusWnd, errorText, "Error", 0);
    }

    if (!pTextureResult)
    {
        if (pPixelBuffer)
            free(pPixelBuffer);
    }

CLOSE_HANDLES:
    CloseHandle(hFileHandle);
    return pTextureResult;
}

// FUNCTION: MIDIJAM 0x448693
TextureData* LoadDibFileW(const char* lpFileName)
{
    return LoadDibFileInternal(lpFileName, FALSE);
}

// FUNCTION: MIDIJAM 0x447500
TextureData* LoadDibFileSafe(char* fileName)
{
    if (!fileName)
        return nullptr;

    // Check file exists before attempting to load.
    FILE* stream = fopen(fileName, "r");
    if (!stream)
        return nullptr;
    fclose(stream);

    return LoadDibFileW(fileName);
}

// FUNCTION: MIDIJAM 0x447630
GLuint LoadTexture(const char* textureName)
{
    size_t filenameLength = strlen(textureName);
    char* fullPath = static_cast<char*>(malloc(filenameLength + 8));
    sprintf(fullPath, "data/%s", textureName);

    TextureData* textureData = LoadDibFileSafe(fullPath);
    free(fullPath);

    GLuint textureId = 0;
    if (textureData && textureData->pPixelData)
    {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(
            GL_TEXTURE_2D, 0, 3,
            textureData->width, textureData->height,
            0, GL_RGB, GL_UNSIGNED_BYTE,
            textureData->pPixelData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        free(textureData->pPixelData);
        free(textureData);
    }

    return textureId;
}

// FUNCTION: MIDIJAM 0x447740
GLuint LoadTextureFromHwf(
    const char* textureName,
    FILE* hwfStream,
    void* hwfAppendix,
    unsigned int hwfAppendixEntriesCount)
{
    fseek(hwfStream, 0, 0);
    for (unsigned int i = 0; i < hwfAppendixEntriesCount; ++i) {
        if (!_stricmp(textureName, static_cast<const char *>(hwfAppendix) + 264 * i)) // search for texture with name
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

// FUNCTION: MIDIJAM 0x447550
// MATCH: EXACT
char* ReadHwfAppendix(char* pHwfAppendix, FILE* hwfStream, unsigned int* nHwfAppendixItems)
{
    fseek(hwfStream, 0, SEEK_END);
    fseek(hwfStream, -4, SEEK_CUR);
    fread(nHwfAppendixItems, 4, 1, hwfStream);
    pHwfAppendix = static_cast<char*>(realloc(pHwfAppendix, 264 * *nHwfAppendixItems));
    fseek(hwfStream, 0, SEEK_END);
    fseek(hwfStream, -4, SEEK_CUR);
    fseek(hwfStream, (int)*nHwfAppendixItems * 264 * -1, SEEK_CUR);
    for (unsigned int i = 0; i < *nHwfAppendixItems; ++i)
        fread(&pHwfAppendix[264 * i], 264, 1, hwfStream);
    return pHwfAppendix;
}

// FUNCTION: MIDIJAM 0x447840
GLuint CreateTextureFromBitmapData(char* bitmapData)
{
    GLuint textureId = 0;

    // Read the BITMAPFILEHEADER (14 bytes).
    BITMAPFILEHEADER fileHeader;
    memcpy(&fileHeader, bitmapData, sizeof(BITMAPFILEHEADER));

    // Validate BMP signature ('BM' = 0x4D42).
    if (fileHeader.bfType != 0x4D42)
        return 0;

    // Copy the info header and pixel data.
    size_t infoHeaderSize = fileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
    BITMAPINFOHEADER* infoHeader =
        static_cast<BITMAPINFOHEADER*>(malloc(infoHeaderSize));
    memcpy(infoHeader, bitmapData + sizeof(BITMAPFILEHEADER), infoHeaderSize);

    // Calculate padded row stride and total pixel data size.
    long long rowBits = static_cast<long long>(infoHeader->biWidth)
        * infoHeader->biBitCount + 7;
    size_t pixelDataSize = abs(infoHeader->biHeight)
        * static_cast<size_t>(rowBits >> 3);

    BYTE* pixelData = static_cast<BYTE*>(malloc(pixelDataSize));
    memcpy(pixelData,
           bitmapData + sizeof(BITMAPFILEHEADER) + infoHeaderSize,
           pixelDataSize);

    // Swap BGR to RGB.
    unsigned int totalPixels = infoHeader->biHeight * infoHeader->biWidth;
    for (unsigned int i = 0; i < totalPixels; ++i)
    {
        BYTE temp = pixelData[3 * i];
        pixelData[3 * i] = pixelData[3 * i + 2];
        pixelData[3 * i + 2] = temp;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(
        GL_TEXTURE_2D, 0, 3,
        infoHeader->biWidth, infoHeader->biHeight,
        0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(pixelData);
    free(infoHeader);
    return textureId;
}

// FUNCTION: MIDIJAM 0x447A00
BOOL ProcessBitmapFont(char* bitmapFontData, BitmapFontType bitmapFontType)
{
    extern int g_fontCellWidth;
    extern int g_charWidthProportional[];
    extern float g_scaledCharacterWidths[];
    extern GLuint g_fontTextureArray[];

    BOOL result = FALSE;

    // Parse BMP file header.
    BITMAPFILEHEADER fileHeader;
    memcpy(&fileHeader, bitmapFontData, sizeof(fileHeader));

    // Validate BMP signature.
    if (fileHeader.bfType != 0x4D42)
        return FALSE;

    size_t infoHeaderSize = fileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
    BITMAPINFOHEADER* infoHeader =
        static_cast<BITMAPINFOHEADER*>(malloc(infoHeaderSize));
    memcpy(infoHeader,
           bitmapFontData + sizeof(BITMAPFILEHEADER),
           infoHeaderSize);

    // Calculate pixel data size with row padding.
    long long rowBits = static_cast<long long>(infoHeader->biWidth)
        * infoHeader->biBitCount + 7;
    size_t pixelDataSize = abs(infoHeader->biHeight)
        * static_cast<size_t>(rowBits >> 3);

    BYTE* pixelData = static_cast<BYTE*>(malloc(pixelDataSize));
    memcpy(pixelData,
           bitmapFontData + sizeof(BITMAPFILEHEADER) + infoHeaderSize,
           pixelDataSize);

    // Swap BGR to RGB.
    unsigned int pixelCount = infoHeader->biHeight * infoHeader->biWidth;
    for (unsigned int i = 0; i < pixelCount; ++i)
    {
        BYTE temp = pixelData[3 * i];
        pixelData[3 * i] = pixelData[3 * i + 2];
        pixelData[3 * i + 2] = temp;
    }

    if (bitmapFontType == COLOR)
    {
        // Analyze per-character pixel widths for proportional spacing.
        // The font bitmap is laid out as a 16x8 grid of characters
        // (16 columns, 8 rows in the lower half; upper half is the mask).
        g_fontCellWidth = infoHeader->biWidth / 16;

        for (short charRow = 0; charRow < 8; ++charRow)
        {
            for (short charCol = 0; charCol < 16; ++charCol)
            {
                int charIndex = 16 * (7 - charRow) + charCol;
                g_charWidthProportional[charIndex] = 0;

                for (short pixelY = 0; pixelY < infoHeader->biHeight / 16; ++pixelY)
                {
                    for (short pixelX = 0; pixelX < infoHeader->biWidth / 16; ++pixelX)
                    {
                        int pixelOffset = 3 * infoHeader->biWidth * pixelY
                            + 3 * infoHeader->biWidth / 16 * charCol
                            + 3 * pixelX
                            + infoHeader->biHeight / 16
                            * 3 * infoHeader->biWidth
                            * (charRow + 8);

                        if (pixelData[pixelOffset] != 255
                            && pixelX > g_charWidthProportional[charIndex])
                        {
                            g_charWidthProportional[charIndex] = pixelX;
                        }
                    }
                }

                if (!g_charWidthProportional[charIndex])
                    g_charWidthProportional[charIndex] = infoHeader->biWidth / 16 / 2;

                g_scaledCharacterWidths[charIndex] =
                    static_cast<float>(g_charWidthProportional[charIndex])
                    / static_cast<float>(g_fontCellWidth)
                    * 35.0f;
            }
        }
    }

    glGenTextures(1, &g_fontTextureArray[bitmapFontType]);
    glBindTexture(GL_TEXTURE_2D, g_fontTextureArray[bitmapFontType]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D, 0, 3,
        infoHeader->biWidth, infoHeader->biHeight,
        0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);

    free(pixelData);
    free(infoHeader);
    return result;
}

void SetTextureState(const GLboolean wasEnabled)
{
    if (wasEnabled)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);
}
