//
// Created by Jacob on 11/22/2025.
//

#include "windows.h"
#include "GL/GL.h"
#include "StageString.h"
#include "../globals.h"
#include "../Ms3dBundle.h"

DEF_INST(stageString, I_DS_StageString)

float flt_46E3AC = 10.53;
float flt_46E3C0 = 44.55;
float flt_46E398 = 0.81;

void I_StageString() {
    GLfloat angle; // [esp+0h] [ebp-2Ch]
    GLfloat y; // [esp+8h] [ebp-24h]
    GLfloat ya; // [esp+8h] [ebp-24h]
    GLfloat yb; // [esp+8h] [ebp-24h]
    GLfloat z; // [esp+Ch] [ebp-20h]
    __int16 i; // [esp+20h] [ebp-Ch]
    __int16 j; // [esp+24h] [ebp-8h]
    __int16 v7; // [esp+28h] [ebp-4h]

    v7 = 0;
    glPushMatrix();
    glTranslatef(0.0, 8.0, -14.0);
    for ( i = 0; i < g_ialloc_stageString; ++i )
    {
        if ( g_ds_stageString[i].isActive == 1 )
        {
            for ( j = 0; j < 12; ++j )
            {
                glPushMatrix();
                angle = (double)v7 * flt_46E3AC + flt_46E3C0 - (double)j * flt_46E398;
                glRotatef(angle, 0.0, 1.0, 0.0);
                if ( g_ds_stageString[i].field_4[j] <= 0 )
                {
                    z = STAGE_BASE_Z - 2.0;
                    yb = (double)(2 * (12 - j)) + -42.0;
                    glTranslatef(0.0, yb, z);
                    g_stageStringHolder_ms3d->RenderModel();
                    g_stageString_ms3d->RenderModel();
                }
                else
                {
                    y = (double)(2 * (12 - j)) + -42.0;
                    glTranslatef(0.0, y, STAGE_BASE_Z);
                    g_stageStringHolder_ms3d->RenderModel();
                    g_stageStringBottomX_ms3d[g_vibratingString_frame]->RenderModel();
                    glTranslatef(0.0, 45.0, 0.0);
                    glRotatef(-65.0, 0.0, 0.0, 1.0);
                    ya = (double)g_ds_stageString[i].field_4[j] / (double)g_ds_stageString[i].field_34[j] * -8.0 + 4.0;
                    glTranslatef(0.0, ya, 0.0);
                    g_stageStringBow_ms3d->RenderModel();
                }
                glPopMatrix();
            }
            ++v7;
        }
    }
    glPopMatrix();
}

bool __cdecl I_StageString_MM(int a1)
{
    __int16 v2; // [esp+4h] [ebp-14h]
    __int16 i; // [esp+8h] [ebp-10h]
    __int16 slot; // [esp+Ch] [ebp-Ch]
    __int16 note; // [esp+10h] [ebp-8h]
    __int16 v6; // [esp+14h] [ebp-4h]

    v6 = 0;
    for ( i = 0; i < g_ialloc_stageString; ++i )
    {
        v2 = 0;
        for ( note = 0; note < 12; ++note )
        {
            for ( slot = 0; slot < 16; ++slot )
            {
                if ( g_ds_stageString[i].field_364[note][slot] > 0 )
                {
                    v6 = 1;
                    g_ds_stageString[i].field_364[note][slot] -= a1 - g_time_global_current;
                    if ( g_ds_stageString[i].field_364[note][slot] <= 0 )
                    {
                        g_ds_stageString[i].field_364[note][slot] = 0;
                        g_ds_stageString[i].field_4[note] = g_ds_stageString[i].field_64[note][slot];
                        g_ds_stageString[i].field_34[note] = g_ds_stageString[i].field_64[note][slot];
                        g_ds_stageString[i].field_64[note][slot] = 0;
                    }
                    g_ds_stageString[i].isActive = 1;
                    g_ds_stageString[i].timeToLive = 1000;
                    v2 = 1;
                }
            }
            if ( g_ds_stageString[i].field_4[note] > 0 )
            {
                g_ds_stageString[i].field_4[note] -= a1 - g_time_global_current;
                if ( g_ds_stageString[i].field_4[note] > 0 )
                {
                    g_ds_stageString[i].isActive = 1;
                    g_ds_stageString[i].timeToLive = 1000;
                    v2 = 1;
                    v6 = 1;
                }
                else
                {
                    g_ds_stageString[i].field_4[note] = 0;
                }
            }
        }
        if ( !v2 && g_ds_stageString[i].isActive == 1 && --g_ds_stageString[i].timeToLive <= 0 )
        {
            g_ds_stageString[i].timeToLive = 0;
            g_ds_stageString[i].isActive = 0;
        }
    }
    return v6 == 1;
}