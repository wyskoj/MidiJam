//
// Created by Jacob on 3/17/2026.
//

#include "text.h"

#include "texture.h"

#include <cstdio>
#include <cstring>

extern int g_fontCellWidth;
extern int g_charWidthProportional[];
extern GLuint g_fontTextureArray[];
extern GLuint g_fontDisplayListBase[];

// FUNCTION: MIDIJAM 0x446A50
BOOL __cdecl LoadHwfFonts(FILE* hwfStream, char* hwfAppendix, const unsigned int hwfAppendixCount) {
    char Buffer[20];
    for (__int16 i = 0; i < 2; ++i) {
        sprintf(Buffer, "Font%d.bmp", i);
        fseek(hwfStream, 0, 0);
        for (unsigned int j = 0; j < hwfAppendixCount; ++j) {
            if (!_stricmp(Buffer, (const char*)(hwfAppendix + 264 * j))) {
                void* Src = operator new(*reinterpret_cast<unsigned int*>(hwfAppendix + 264 * j + 260));
                fread(Src, 1u, *reinterpret_cast<unsigned int*>(hwfAppendix + 264 * j + 260), hwfStream);
                ProcessBitmapFont(static_cast<char*>(Src), static_cast<BitmapFontType>(i));
                free(Src);
                break;
            }
            fseek(hwfStream, *reinterpret_cast<unsigned int*>(hwfAppendix + 264 * j + 260), 1);
        }
    }
    return 1;
}

// FUNCTION: MIDIJAM 0x446BA0
void CreateFontDisplayLists() {
    for (short fontIndex = 0; fontIndex < 2; ++fontIndex) {
        g_fontDisplayListBase[fontIndex] = glGenLists(256);
        glBindTexture(GL_TEXTURE_2D, g_fontTextureArray[fontIndex]);

        for (short charIndex = 0; charIndex < 256; ++charIndex) {
            const GLfloat texCoordLeft = (charIndex % 16) / 16.0f;
            const float texCoordTopRow = (charIndex / 16) / 16.0f;

            glNewList(g_fontDisplayListBase[fontIndex] + charIndex, GL_COMPILE);
            glBegin(GL_QUADS);

            const GLfloat texCoordBottom = 1.0f - texCoordTopRow - 0.0625f;
            glTexCoord2f(texCoordLeft, texCoordBottom);
            glVertex2i(0, 0);

            glTexCoord2f(texCoordLeft + 0.0625f, texCoordBottom);
            glVertex2i(32, 0);

            const GLfloat texCoordTop = 1.0f - texCoordTopRow;
            glTexCoord2f(texCoordLeft + 0.0625f, texCoordTop);
            glVertex2i(32, 32);

            glTexCoord2f(texCoordLeft, texCoordTop);
            glVertex2i(0, 32);

            glEnd();

            glTranslated(
                g_charWidthProportional[charIndex % 128]
                / static_cast<double>(g_fontCellWidth) * 35.0,
                0.0, 0.0);

            glEndList();
        }
    }
}

// FUNCTION: MIDIJAM 0x446DA0
// MATCH: EXACT
void DeleteFontDisplayLists() {
    for (short fontIndex = 0; fontIndex < 2; ++fontIndex)
        glDeleteLists(g_fontDisplayListBase[fontIndex], 256);
}

// FUNCTION: MIDIJAM 0x446E80
// MATCH: APPROXIMATE - expression evaluation order differs (shift before array vs after), binary equivalent
void RenderTextShadow(const int positionX, const int positionY, const char* text, const int fontType, int fontStyle,
                      const GLfloat scaleX, const GLfloat scaleY) {
    GLfloat shadowColor[4];

    shadowColor[0] = 1.0;
    shadowColor[1] = 1.0;
    shadowColor[2] = 1.0;
    shadowColor[3] = 1.0;
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, shadowColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, shadowColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shadowColor);
    glMaterialfv(GL_FRONT, GL_EMISSION, shadowColor);
    if (fontStyle > 1)
        fontStyle = 1;
    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR, GL_POINTS);
    glBindTexture(GL_TEXTURE_2D, g_fontTextureArray[fontType]);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, 640.0, 0.0, 480.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(positionX, (480 - positionY), 0.0);
    glScalef(scaleX, scaleY, 0.0);
    glListBase(g_fontDisplayListBase[fontType] + (fontStyle << 7) - 32);
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_VERSION_1_1);
}

// FUNCTION: MIDIJAM 0x446DE0
// MATCH: EXACT
void RenderTextWithShadow(const int positionX, const int positionY, const short shadowShiftX, const short shadowShiftY,
                          const char* text, const int fontIndex, const GLfloat scaleX, const GLfloat scaleY) {
    if (text) {
        if (shadowShiftX == 0 && shadowShiftY == 0)
            goto render_main;

        // Shadow render
        RenderTextShadow(positionX + shadowShiftX, positionY + shadowShiftY,
                         text, 0, 1, scaleX, scaleY);

    render_main:
        RenderTextShadow(positionX, positionY, text, 0, 0, scaleX, scaleY);

        if (fontIndex) {
            RenderText(positionX, positionY, text, fontIndex, 0, scaleX, scaleY);
        }
    }
}

// FUNCTION: MIDIJAM 0x447080
void RenderText(const int positionX, const int positionY, const char* text, const int fontIndex, int fontStyle,
                const GLfloat scaleX, const GLfloat scaleY) {
    if (fontStyle > 1) {
        fontStyle = 1;
    }
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, g_fontTextureArray[fontIndex]);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(positionX, (480 - positionY), 0.0f);
    glScalef(scaleX, scaleY, 0.0f);
    glListBase(g_fontDisplayListBase[fontIndex] + (fontStyle << 7) - 32);
    const size_t textLength = strlen(text);
    glCallLists(textLength, GL_UNSIGNED_BYTE, text);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}
