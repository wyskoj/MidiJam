//
// Created by Jacob on 11/16/2025.
//

#ifndef MIDIJAM_TEXT_H
#define MIDIJAM_TEXT_H
#include <windows.h>
#include <GL/gl.h>

extern GLuint g_fontTextureArray[2];
extern GLuint g_fontDisplayListBase[2];

void __cdecl RenderText(
        int positionX,
        int positionY,
        const char *text,
        int fontIndex,
        int fontStyle,
        GLfloat scaleX,
        GLfloat scaleY);

void __cdecl RenderTextShadow(
        int positionX,
        int positionY,
        const char *text,
        int fontType,
        int fontStyle,
        GLfloat scaleX,
        GLfloat scaleY);

void __cdecl RenderTextWithShadow(
        int positionX,
        int positionY,
        __int16 shadowShiftX,
        __int16 shadowShiftY,
        char *text,
        int fontIndex,
        GLfloat scaleX,
        GLfloat scaleY);

#endif //MIDIJAM_TEXT_H