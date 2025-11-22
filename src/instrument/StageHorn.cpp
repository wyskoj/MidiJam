//
// Created by Jacob on 11/22/2025.
//

#include "StageHorn.h"

#include <iostream>

#include "../globals.h"
#include "../Ms3dBundle.h"

float flt_46E394 = 12.0 * (-202.5 / STAGE_BASE_Z);
float flt_46E39C = -1.125 * (-202.5 / STAGE_BASE_Z);

DEF_INST(stageHorn, I_DS_StageHorn)

void I_StageHorn()
{
    GLfloat angle; // [esp+0h] [ebp-2Ch]
    GLfloat anglea; // [esp+0h] [ebp-2Ch]
    GLfloat y; // [esp+8h] [ebp-24h]
    GLfloat z; // [esp+Ch] [ebp-20h]
    GLfloat za; // [esp+Ch] [ebp-20h]
    __int16 i; // [esp+20h] [ebp-Ch]
    __int16 horn; // [esp+24h] [ebp-8h]
    __int16 v7; // [esp+28h] [ebp-4h]

    v7 = 0;
    glPushMatrix();
    glTranslatef(0.0, 8.0, -14.0);
    for ( i = 0; i < g_ialloc_stageHorn; ++i )
    {
        if ( g_ds_stageHorn[i].isActive == 1 )
        {
            for ( horn = 0; horn < 12; ++horn )
            {
                if ( g_ds_stageHorn[i].field_4[horn] <= 0 )
                {
                    glPushMatrix();
                    anglea = flt_46E394 - (double)horn * flt_46E39C;
                    glRotatef(anglea, 0.0, 1.0, 0.0);
                    za = STAGE_BASE_Z - (double)v7 * 5.0;
                    glTranslatef(0.0, -10.0, za);
                }
                else
                {
                    glPushMatrix();
                    angle = flt_46E394 - (double)horn * flt_46E39C;
                    glRotatef(angle, 0.0, 1.0, 0.0);
                    z = STAGE_BASE_Z - (double)v7 * 5.0;
                    y = (1.0 - (double)g_ds_stageHorn[i].field_4[horn] / (double)g_ds_stageHorn[i].field_34[horn]) * -10.0;
                    glTranslatef(0.0, y, z);
                }
                g_stageHorn_ms3d->RenderModelShiny();
                glPopMatrix();
            }
            ++v7;
        }
    }
    glPopMatrix();
}

bool __cdecl I_StageHorn_MM(int a1)
{
    __int16 v2; // [esp+4h] [ebp-14h]
    __int16 i; // [esp+8h] [ebp-10h]
    __int16 slot; // [esp+Ch] [ebp-Ch]
    __int16 horn; // [esp+10h] [ebp-8h]
    __int16 v6; // [esp+14h] [ebp-4h]

    v6 = 0;
    for ( i = 0; i < g_ialloc_stageHorn; ++i )
    {
        v2 = 0;
        for ( horn = 0; horn < 12; ++horn )
        {
            for ( slot = 0; slot < 16; ++slot )
            {
                if ( g_ds_stageHorn[i].field_364[horn][slot] > 0 )
                {
                    v6 = 1;
                    g_ds_stageHorn[i].field_364[horn][slot] -= a1 - g_time_global_current;
                    if ( g_ds_stageHorn[i].field_364[horn][slot] <= 0 )
                    {
                        g_ds_stageHorn[i].field_364[horn][slot] = 0;
                        g_ds_stageHorn[i].field_4[horn] = g_ds_stageHorn[i].field_64[horn][slot];
                        g_ds_stageHorn[i].field_34[horn] = g_ds_stageHorn[i].field_64[horn][slot];
                        g_ds_stageHorn[i].field_64[horn][slot] = 0;
                    }
                    g_ds_stageHorn[i].isActive = 1;
                    g_ds_stageHorn[i].timeToLive = 1000;
                    v2 = 1;
                }
            }
            if ( g_ds_stageHorn[i].field_4[horn] > 0 )
            {
                g_ds_stageHorn[i].field_4[horn] -= a1 - g_time_global_current;
                if ( g_ds_stageHorn[i].field_4[horn] > 0 )
                {
                    g_ds_stageHorn[i].isActive = 1;
                    g_ds_stageHorn[i].timeToLive = 1000;
                    v2 = 1;
                    v6 = 1;
                }
                else
                {
                    g_ds_stageHorn[i].field_4[horn] = 0;
                }
            }
        }
        if ( !v2 && g_ds_stageHorn[i].isActive == 1 && --g_ds_stageHorn[i].timeToLive <= 0 )
        {
            g_ds_stageHorn[i].timeToLive = 0;
            g_ds_stageHorn[i].isActive = 0;
        }
    }
    return v6 == 1;
}