//
// Created by Jacob on 3/22/2026.
//

#include "Xylophone.h"

#include "keys.h"
#include "Piano.h"
#include "render/texture.h"
#include "scene/scene.h"

DEF_INST(xylophone, XylophoneState)

Ms3dBundle* g_xylophoneMalletWhite_ms3d;
Ms3dBundle* g_malletHitShadow_ms3d;
Ms3dBundle* g_xylophoneLegs_ms3d;
Ms3dBundle* g_xylophoneCase_ms3d;
XylophoneModels* g_xylophoneModels[4];
Ms3dBundle* g_xylophoneShadow_ms3d;
__int16 g_xylophone_types[300];

// FUNCTION: MIDIJAM 0x412170
void RenderXylophone() {
    int v0; // eax
    GLfloat angle; // [esp+0h] [ebp-58h]
    GLfloat anglea; // [esp+0h] [ebp-58h]
    GLfloat x; // [esp+4h] [ebp-54h]
    GLfloat xa; // [esp+4h] [ebp-54h]
    GLfloat xb; // [esp+4h] [ebp-54h]
    GLfloat y; // [esp+8h] [ebp-50h]
    GLfloat ya; // [esp+8h] [ebp-50h]
    GLfloat yb; // [esp+8h] [ebp-50h]
    GLfloat yc; // [esp+8h] [ebp-50h]
    GLfloat yd; // [esp+8h] [ebp-50h]
    GLfloat ye; // [esp+8h] [ebp-50h]
    GLfloat yf; // [esp+8h] [ebp-50h]
    GLfloat z; // [esp+Ch] [ebp-4Ch]
    GLfloat za; // [esp+Ch] [ebp-4Ch]
    GLfloat zb; // [esp+Ch] [ebp-4Ch]
    __int16 slot; // [esp+34h] [ebp-24h]
    __int16 v17; // [esp+40h] [ebp-18h]
    GLboolean v18; // [esp+4Bh] [ebp-Dh]
    __int16 i; // [esp+4Ch] [ebp-Ch]
    __int16 v20; // [esp+50h] [ebp-8h]
    __int16 note; // [esp+54h] [ebp-4h]

    v20 = 0;
    glPushMatrix();
    for (i = 0; i < g_xylophoneCount; ++i) {
        if (g_xylophone[i].field_0 != 1)
            continue;
        if (v20 > 0) {
            glTranslatef(-2.0, 0.0, 0.0);
            glRotatef(-18.0, 0.0, 1.0, 0.0);
            glTranslatef(0.0, 3.0, -23.0);
        }
        glPushMatrix();
        g_xylophoneCase_ms3d->RenderModel();
        v18 = g_xylophoneModels[g_xylophone_types[i]]->xylophoneWhiteBar->ApplyMaterial();
        x = -(g_keysOffset + g_keysOffset);
        glTranslatef(x, 0.0, 0.0);
        for (note = 0; note < 88; ++note) {
            v17 = 10000;
            for (slot = 0; slot < 16; ++slot) {
                if (g_xylophone[i].field_1E46[note][slot] > 0 && g_xylophone[i].field_1E46[note][slot] < v17)
                    v17 = g_xylophone[i].field_1E46[note][slot];
            }
            if (v17 <= 0 || (double)v17 >= 300.0) {
                if (g_xylophone[i].field_6[note] > 0.0) {
                    glPushMatrix();
                    if (KEY_SHAPE_INDEX[note] < 9 || KEY_SHAPE_INDEX[note] > 13) {
                        yc = (10.0 - g_xylophone[i].field_6[note]) / 10.0 * 3.5 + 1.728;
                        glTranslatef(0.0, yc, 15.0);
                    }
                    else {
                        yb = (10.0 - g_xylophone[i].field_6[note]) / 10.0 * 3.5 + 3.7279999;
                        glTranslatef(0.0, yb, 8.0);
                    }
                    anglea = (10.0 - g_xylophone[i].field_6[note]) / 10.0 * 65.0;
                    glRotatef(anglea, 1.0, 0.0, 0.0);
                    g_xylophoneMalletWhite_ms3d->Render();
                    glPopMatrix();
                }
            }
            else {
                glPushMatrix();
                if (KEY_SHAPE_INDEX[note] < 9 || KEY_SHAPE_INDEX[note] > 13) {
                    ya = (double)v17 / 300.0 * 3.5 + 1.728;
                    glTranslatef(0.0, ya, 15.0);
                }
                else {
                    y = (double)v17 / 300.0 * 3.5 + 3.7279999;
                    glTranslatef(0.0, y, 8.0);
                }
                angle = (double)v17 / 300.0 * 65.0;
                glRotatef(angle, 1.0, 0.0, 0.0);
                g_xylophoneMalletWhite_ms3d->Render();
                glPopMatrix();
            }
            glPushMatrix();
            yd = -(g_xylophone[i].field_6[note] * 0.125);
            glTranslatef(0.0, yd, 0.0);
            z = (88.0 - (double)note) / 88.0 * 1.25 + 0.25;
            xa = (88.0 - (double)note) / 88.0 * 0.2 + 0.80000001;
            glScalef(xa, 1.0, z);
            if (KEY_SHAPE_INDEX[note] >= 1 && KEY_SHAPE_INDEX[note] <= 7) {
                if (g_xylophone[i].field_6[note] <= 0.0)
                    goto LABEL_31;
            LABEL_30:
                g_xylophoneModels[g_xylophone_types[i]]->xylophoneWhiteBarDown->Render();
                goto LABEL_40;
            }
            if (KEY_SHAPE_INDEX[note] == 8) {
                if (g_xylophone[i].field_6[note] <= 0.0) {
                LABEL_31:
                    g_xylophoneModels[g_xylophone_types[i]]->xylophoneWhiteBar->Render();
                    goto LABEL_40;
                }
                goto LABEL_30;
            }
            if (KEY_SHAPE_INDEX[note] >= 9 && KEY_SHAPE_INDEX[note] <= 13) {
                glTranslatef(g_pianoKeyOffsetX[KEY_SHAPE_INDEX[note]], 0.0, 2.0);
                if (g_xylophone[i].field_6[note] <= 0.0)
                    g_xylophoneModels[g_xylophone_types[i]]->xylophoneBlackBar->Render();
                else
                    g_xylophoneModels[g_xylophone_types[i]]->xylophoneBlackBarDown->Render();
            }
        LABEL_40:
            glPopMatrix();
            if (v17 <= 0 || (double)v17 >= 300.0) {
                if (g_xylophone[i].field_6[note] > 0.0) {
                    glPushMatrix();
                    if (KEY_SHAPE_INDEX[note] < 9 || KEY_SHAPE_INDEX[note] > 13) {
                        yf = 0.76999998 - g_xylophone[i].field_6[note] * 0.125;
                        glTranslatef(0.0, yf, 6.25);
                    }
                    else {
                        ye = 2.77 - g_xylophone[i].field_6[note] * 0.125;
                        glTranslatef(0.0, ye, -0.75);
                    }
                    zb = g_xylophone[i].field_6[note] / 10.0;
                    glScalef(zb, 1.0, zb);
                    g_malletHitShadow_ms3d->Render();
                    glPopMatrix();
                }
            }
            else {
                glPushMatrix();
                if (KEY_SHAPE_INDEX[note] < 9 || KEY_SHAPE_INDEX[note] > 13)
                    glTranslatef(0.0, 0.76999998, 6.25);
                else
                    glTranslatef(0.0, 2.77, -0.75);
                za = (300.0 - (double)v17) / 300.0;
                glScalef(za, 1.0, za);
                g_malletHitShadow_ms3d->Render();
                glPopMatrix();
            }
            v0 = KEY_SHAPE_INDEX[note];
            xb = KEY_WIDTHS[v0] + KEY_WIDTHS[v0];
            glTranslatef(xb, 0.0, 0.0);
        }
        glPopMatrix();
        ++v20;
        SetTextureState(v18);
    }
    glPopMatrix();
    g_xylophoneVisible = 0;
    if (v20 > 0) {
        g_xylophoneVisible = v20;
        g_xylophoneLegs_ms3d->RenderModel();
    }
}

// FUNCTION: MIDIJAM 0x424C60
bool UpdateXylophone(MUSIC_TIME pmtNow) {
    __int16 v2; // [esp+8h] [ebp-14h]
    __int16 i; // [esp+Ch] [ebp-10h]
    __int16 slot; // [esp+10h] [ebp-Ch]
    __int16 note; // [esp+14h] [ebp-8h]
    __int16 v6; // [esp+18h] [ebp-4h]

    v6 = 0;
    for (i = 0; i < g_xylophoneCount; ++i) {
        v2 = 0;
        for (note = 0; note < 88; ++note) {
            for (slot = 0; slot < 16; ++slot) {
                if (g_xylophone[i].field_1E46[note][slot] > 0) {
                    v6 = 1;
                    g_xylophone[i].field_1E46[note][slot] -= pmtNow - g_currentGlobalTime;
                    if (g_xylophone[i].field_1E46[note][slot] <= 0) {
                        g_xylophone[i].field_1E46[note][slot] = 0;
                        g_xylophone[i].field_6[note] = (double)g_xylophone[i].field_18C6[note][slot] * 8.0 / 128.0
                            + 1.75;
                        g_xylophone[i].field_166[note] = g_xylophone[i].field_2C6[note][slot];
                        g_xylophone[i].field_2C6[note][slot] = 0;
                    }
                    g_xylophone[i].field_0 = 1;
                    g_xylophone[i].field_2 = 1000;
                    v2 = 1;
                }
            }
            if (g_xylophone[i].field_166[note] <= 0) {
                if (g_xylophone[i].field_6[note] > 0.0) {
                    v6 = 1;
                    g_xylophone[i].field_6[note] = g_xylophone[i].field_6[note] - 0.30000001;
                    if (g_xylophone[i].field_6[note] > 0.0) {
                        g_xylophone[i].field_0 = 1;
                        g_xylophone[i].field_2 = 1000;
                        v2 = 1;
                    }
                    else {
                        g_xylophone[i].field_6[note] = 0.0;
                    }
                }
            }
            else {
                g_xylophone[i].field_166[note] -= pmtNow - g_currentGlobalTime;
                if (g_xylophone[i].field_166[note] <= 0)
                    g_xylophone[i].field_166[note] = 0;
                g_xylophone[i].field_0 = 1;
                g_xylophone[i].field_2 = 1000;
                v2 = 1;
                g_xylophone[i].field_6[note] = g_xylophone[i].field_6[note] - 0.15000001;
                if (g_xylophone[i].field_6[note] > 0.0) {
                    g_xylophone[i].field_0 = 1;
                    g_xylophone[i].field_2 = 1000;
                    v2 = 1;
                }
                else {
                    g_xylophone[i].field_6[note] = 0.0;
                }
            }
        }
        if (!v2 && g_xylophone[i].field_0 == 1 && --g_xylophone[i].field_2 <= 0) {
            g_xylophone[i].field_2 = 0;
            g_xylophone[i].field_0 = 0;
        }
    }
    return v6 == 1;
}
