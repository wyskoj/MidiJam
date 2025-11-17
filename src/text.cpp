//
// Created by Jacob on 11/16/2025.
//

#include "text.h"

GLuint g_fontTextureArray[2];
GLuint g_fontDisplayListBase[2];

void RenderText(int positionX, int positionY, const char *text, int fontIndex, int fontStyle, GLfloat scaleX,
                GLfloat scaleY) {
    size_t textLength; // eax

    if (fontStyle > 1)
        fontStyle = 1;
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, g_fontTextureArray[fontIndex]);
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
    glListBase(g_fontDisplayListBase[fontIndex] + (fontStyle << 7) - 32);
    textLength = strlen(text);
    glCallLists(textLength, GL_UNSIGNED_BYTE, text);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void RenderTextShadow(int positionX, int positionY, const char *text, int fontType, int fontStyle, GLfloat scaleX,
                      GLfloat scaleY) {
    size_t textLength; // eax
    GLfloat shadowColor[4]; // [esp+34h] [ebp-10h] BYREF

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
    textLength = strlen(text);
    glCallLists(textLength, GL_UNSIGNED_BYTE, text);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_VERSION_1_1);
}

void RenderTextWithShadow(int positionX, int positionY, short shadowShiftX, short shadowShiftY, char *text,
                          int fontIndex, GLfloat scaleX, GLfloat scaleY) {
    if (text) {
        if (shadowShiftX || shadowShiftY)
            RenderTextShadow(positionX + shadowShiftX, positionY + shadowShiftY, text, 0, 1, scaleX, scaleY);
        RenderTextShadow(positionX, positionY, text, 0, 0, scaleX, scaleY);
        if (fontIndex)
            RenderText(positionX, positionY, text, fontIndex, 0, scaleX, scaleY);
    }
}
