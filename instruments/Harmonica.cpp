//
// Created by Jacob on Saturday, April 11, 2026.
//

#include "Harmonica.h"

#include "scene/scene.h"

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(harmonica, HarmonicaState)

Ms3dBundle* g_harmonica_ms3d = nullptr;
Ms3dBundle* g_steamPuff_harmonica_ms3d = nullptr;

extern MUSIC_TIME g_currentGlobalTime;

// FUNCTION: MIDIJAM 0x407250
void RenderHarmonica() {
    GLfloat angle; // [esp+0h] [ebp-38h]
    GLfloat anglea; // [esp+0h] [ebp-38h]
    GLfloat x; // [esp+4h] [ebp-34h]
    GLfloat y; // [esp+8h] [ebp-30h]
    GLfloat ya; // [esp+8h] [ebp-30h]
    GLfloat z; // [esp+Ch] [ebp-2Ch]
    GLfloat v6; // [esp+24h] [ebp-14h]
    __int16 k; // [esp+28h] [ebp-10h]
    __int16 i; // [esp+2Ch] [ebp-Ch]
    __int16 v9; // [esp+30h] [ebp-8h]
    __int16 j; // [esp+34h] [ebp-4h]

    v9 = 0;
    glPushMatrix();
    glTranslatef(EAST_SPIRAL_X, EAST_SPIRAL_Y, EAST_SPIRAL_Z);
    for ( i = 0; i < g_harmonicaCount; ++i )
    {
        if ( g_harmonica[i].isActive == 1 )
        {
            glPushMatrix();
            angle = (double)v9 * 90.0 + WHISTLES_ROTATION_FACTOR;
            glRotatef(angle, 0.0, 1.0, 0.0);
            y = (double)v9 * 3.0 + 6.5;
            glTranslatef(0.0, y, 0.0);
            g_harmonica_ms3d->RenderModel();
            glPopMatrix();
            for ( j = 0; j < 12; ++j )
            {
                for ( k = 0; k < 100; ++k )
                {
                    if ( g_harmonica[i].field_4E4[j][k] > 0.0 )
                    {
                        glPushMatrix();
                        anglea = (double)j * 5.0 + -27.5 + (double)v9 * 90.0 + WHISTLES_ROTATION_FACTOR;
                        glRotatef(anglea, 0.0, 1.0, 0.0);
                        z = g_harmonica[i].field_4E4[j][k] * 0.75 + 7.0;
                        ya = (double)v9 * 3.0
                           + g_harmonica[i].field_17A4[j][k]
                           + g_harmonica[i].field_4E4[j][k] / 20.0 * (g_harmonica[i].field_4E4[j][k] / 20.0) * 10.0
                           + g_harmonica[i].field_7564[j][k];
                        x = g_harmonica[i].field_62A4[j][k] + 0.0;
                        glTranslatef(x, ya, z);
                        v6 = (1.2 - g_harmonica[i].field_2A64[j][k] / 0.25) * 1.125;
                        glScalef(v6, v6, v6);
                        glRotatef(g_harmonica[i].field_3D24[j][k], 1.0, 0.0, 0.0);
                        glRotatef(g_harmonica[i].field_4FE4[j][k], 0.0, 0.0, 1.0);
                        g_steamPuff_harmonica_ms3d->RenderModel();
                        glPopMatrix();
                    }
                }
            }
            ++v9;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x42BA50
bool UpdateHarmonica(MUSIC_TIME pmtNow) {
    __int16 v2; // [esp+14h] [ebp-18h]
  __int16 i; // [esp+18h] [ebp-14h]
  __int16 k; // [esp+1Ch] [ebp-10h]
  __int16 m; // [esp+20h] [ebp-Ch]
  __int16 j; // [esp+24h] [ebp-8h]
  __int16 v7; // [esp+28h] [ebp-4h]

  v7 = 0;
  for ( i = 0; i < g_harmonicaCount; ++i )
  {
    v2 = 0;
    for ( j = 0; j < 12; ++j )
    {
      for ( k = 0; k < 16; ++k )
      {
        if ( g_harmonica[i].field_364[j][k] > 0 )
        {
          v7 = 1;
          g_harmonica[i].field_364[j][k] -= pmtNow - g_currentGlobalTime;
          if ( g_harmonica[i].field_364[j][k] <= 0 )
          {
            g_harmonica[i].field_364[j][k] = 0;
            g_harmonica[i].field_4[j] = g_harmonica[i].field_64[j][k];
            g_harmonica[i].field_34[j] = g_harmonica[i].field_64[j][k];
            g_harmonica[i].field_64[j][k] = 0;
          }
          g_harmonica[i].isActive = 1;
          g_harmonica[i].timeToLive = 1000;
          v2 = 1;
          g_harmonica[i].field_D33C[j] = 0;
        }
      }
      if ( g_harmonica[i].field_4[j] > 0 )
      {
        g_harmonica[i].field_4[j] -= pmtNow - g_currentGlobalTime;
        if ( g_harmonica[i].field_4[j] > 0 )
        {
          g_harmonica[i].isActive = 1;
          g_harmonica[i].timeToLive = 1000;
          v2 = 1;
          v7 = 1;
          if ( g_harmonica[i].field_D33C[j] <= 0 )
          {
            g_harmonica[i].field_4E4[j][g_harmonica[i].field_D324[j]] = 0.1;
            g_harmonica[i].field_2A64[j][g_harmonica[i].field_D324[j]] = 0.25;
            g_harmonica[i].field_3D24[j][g_harmonica[i].field_D324[j]] = 0.0;
            g_harmonica[i].field_4FE4[j][g_harmonica[i].field_D324[j]] = 0.0;
            g_harmonica[i].field_62A4[j][g_harmonica[i].field_D324[j]] = 0.0;
            g_harmonica[i].field_7564[j][g_harmonica[i].field_D324[j]] = 0.0;
            g_harmonica[i].field_8824[j][g_harmonica[i].field_D324[j]] = ((double)(50 * rand() / 0x8000) - 25.0)
                                                                             * 0.1;
            g_harmonica[i].field_9AE4[j][g_harmonica[i].field_D324[j]] = ((double)(50 * rand() / 0x8000) - 25.0)
                                                                             * 0.1;
            g_harmonica[i].field_ADA4[j][g_harmonica[i].field_D324[j]] = ((double)(25 * rand() / 0x8000) - 12.5)
                                                                             * 0.001;
            g_harmonica[i].field_C064[j][g_harmonica[i].field_D324[j]] = ((double)(25 * rand() / 0x8000) - 12.5)
                                                                             * 0.001;
            g_harmonica[i].field_17A4[j][g_harmonica[i].field_D324[j]++] = 6.5;
            if ( g_harmonica[i].field_D324[j] >= 100 )
              g_harmonica[i].field_D324[j] = 0;
            g_harmonica[i].field_D33C[j] = 2;
          }
          else
          {
            --g_harmonica[i].field_D33C[j];
          }
        }
        else
        {
          g_harmonica[i].field_4[j] = 0;
        }
      }
      for ( m = 0; m < 100; ++m )
      {
        if ( g_harmonica[i].field_4E4[j][m] <= 0.0 )
        {
          g_harmonica[i].field_4E4[j][m] = 0.0;
        }
        else
        {
          g_harmonica[i].field_4E4[j][m] = g_harmonica[i].field_4E4[j][m] + g_harmonica[i].field_2A64[j][m];
          g_harmonica[i].field_2A64[j][m] = g_harmonica[i].field_2A64[j][m] * 0.95999998;
          g_harmonica[i].field_3D24[j][m] = g_harmonica[i].field_3D24[j][m] + g_harmonica[i].field_8824[j][m];
          g_harmonica[i].field_4FE4[j][m] = g_harmonica[i].field_4FE4[j][m] + g_harmonica[i].field_9AE4[j][m];
          g_harmonica[i].field_62A4[j][m] = g_harmonica[i].field_62A4[j][m] + g_harmonica[i].field_ADA4[j][m];
          g_harmonica[i].field_7564[j][m] = g_harmonica[i].field_7564[j][m] + g_harmonica[i].field_C064[j][m];
          if ( g_harmonica[i].field_2A64[j][m] < 0.0099999998 )
          {
            g_harmonica[i].field_4E4[j][m] = 0.0;
            g_harmonica[i].field_2A64[j][m] = 0.0;
          }
        }
      }
    }
    if ( !v2 && g_harmonica[i].isActive == 1 && --g_harmonica[i].timeToLive <= 0 )
    {
      g_harmonica[i].timeToLive = 0;
      g_harmonica[i].isActive = 0;
    }
  }
  return v7 == 1;
}

