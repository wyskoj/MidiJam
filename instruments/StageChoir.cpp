//
// Created by Jacob on Friday, April 10, 2026.
//


#include "StageChoir.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

#include "scene/scene.h"

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(stageChoir, StageChoirState)

Ms3dBundle* g_stageChoir_ms3d = nullptr;
float g_stageChoir_rotation_base = 10.8f;
float g_stageChoir_rotation_delta = 5.67f;

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;

// FUNCTION: MIDIJAM 0x40B8C0
void RenderStageChoir() {
    GLfloat spin_1; // [esp+0h] [ebp-34h]
    GLfloat spin; // [esp+0h] [ebp-34h]
    float y; // [esp+8h] [ebp-2Ch]
    float z; // [esp+Ch] [ebp-28h]
    __int16 i; // [esp+28h] [ebp-Ch]
    __int16 multiple_instances_offset; // [esp+2Ch] [ebp-8h]
    __int16 j; // [esp+30h] [ebp-4h]

    multiple_instances_offset = 0;
    glPushMatrix();
    glTranslatef(0.0, 8.0, -14.0);
    for ( i = 0; i < g_stageChoirCount; ++i )
    {
        if ( g_stageChoir[i].isVisible == 1 )
        {
            for ( j = 0; j < 12; ++j )
            {
                if ( g_stageChoir[i].field_4[j] <= 0 )
                {
                    // At rest
                    glPushMatrix();
                    spin = g_stageChoir_rotation_base - (double)j * g_stageChoir_rotation_delta;
                    glRotatef(spin, 0.0, 1.0, 0.0);
                    z = STAGE_BASE_Z - (double)multiple_instances_offset * 13.0;
                    y = (double)multiple_instances_offset * 5.0 + -10.0;
                }
                else
                {
                    // Falling
                    glPushMatrix();
                    spin_1 = g_stageChoir_rotation_base - (double)j * g_stageChoir_rotation_delta;
                    glRotatef(spin_1, 0.0, 1.0, 0.0);
                    z = STAGE_BASE_Z - (double)multiple_instances_offset * 13.0;
                    y = (1.0 - (double)g_stageChoir[i].field_4[j] / (double)g_stageChoir[i].field_34[j]) * -10.0
                      + (double)multiple_instances_offset * 5.0;
                }
                glTranslatef(0.0, y, z);
                g_stageChoir_ms3d->RenderModel();
                glPopMatrix();
            }
            ++multiple_instances_offset;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x433190
bool UpdateStageChoir(MUSIC_TIME pmtNow) {
    __int16 v2; // [esp+4h] [ebp-14h]
    __int16 i; // [esp+8h] [ebp-10h]
    __int16 k; // [esp+Ch] [ebp-Ch]
    __int16 j; // [esp+10h] [ebp-8h]
    __int16 v6; // [esp+14h] [ebp-4h]

    v6 = 0;
    for ( i = 0; i < g_stageChoirCount; ++i )
    {
        v2 = 0;
        for ( j = 0; j < 12; ++j )
        {
            for ( k = 0; k < 16; ++k )
            {
                if ( g_stageChoir[i].field_364[j][k] > 0 )
                {
                    v6 = 1;
                    g_stageChoir[i].field_364[j][k] -= pmtNow - g_currentGlobalTime;
                    if ( g_stageChoir[i].field_364[j][k] <= 0 )
                    {
                        g_stageChoir[i].field_364[j][k] = 0;
                        g_stageChoir[i].field_4[j] = g_stageChoir[i].field_64[j][k];
                        g_stageChoir[i].field_34[j] = g_stageChoir[i].field_64[j][k];
                        g_stageChoir[i].field_64[j][k] = 0;
                    }
                    g_stageChoir[i].isVisible = 1;
                    g_stageChoir[i].timeToLive = 1000;
                    v2 = 1;
                }
            }
            if ( g_stageChoir[i].field_4[j] > 0 )
            {
                g_stageChoir[i].field_4[j] -= pmtNow - g_currentGlobalTime;
                if ( g_stageChoir[i].field_4[j] > 0 )
                {
                    g_stageChoir[i].isVisible = 1;
                    g_stageChoir[i].timeToLive = 1000;
                    v2 = 1;
                    v6 = 1;
                }
                else
                {
                    g_stageChoir[i].field_4[j] = 0;    // min = 0
                }
            }
        }
        if ( !v2 && g_stageChoir[i].isVisible == 1 && --g_stageChoir[i].timeToLive <= 0 )
        {
            g_stageChoir[i].timeToLive = 0;
            g_stageChoir[i].isVisible = 0;
        }
    }
    return v6 == 1;
}
