//
// Created by Jacob on 3/17/2026.
//

#ifndef MIDIJAM_TEXT_H
#define MIDIJAM_TEXT_H

#include <windows.h>
#include <GL/gl.h>
#include <cstdio>

BOOL LoadHwfFonts(FILE* hwfStream, char* hwfAppendix, unsigned int hwfAppendixCount);
void CreateFontDisplayLists();
void DeleteFontDisplayLists();

void RenderText(int positionX, int positionY, const char* text, int fontIndex, int fontStyle, GLfloat scaleX, GLfloat scaleY);
void RenderTextShadow(int positionX, int positionY, const char* text, int fontType, int fontStyle, GLfloat scaleX, GLfloat scaleY);
void RenderTextWithShadow(int positionX, int positionY, short shadowShiftX, short shadowShiftY, const char* text, int fontIndex, GLfloat scaleX, GLfloat scaleY);

#endif // MIDIJAM_TEXT_H