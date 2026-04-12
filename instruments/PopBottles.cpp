 //
// Created by Jacob on Saturday, April 11, 2026.
//


#include "PopBottles.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

#include "scene/scene.h"

 // ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(popBottles, PopBottlesState)

Ms3dBundle* g_popBottle_ms3d = nullptr;
Ms3dBundle* g_popBottleMiddle_ms3d = nullptr;
Ms3dBundle* g_popBottlePop_ms3d = nullptr;
Ms3dBundle* g_popBottleLabel_ms3d = nullptr;

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern Ms3dBundle* g_steamCloud_2_ms3d;

// FUNCTION: MIDIJAM 0x407620
void RenderPopBottles() {
    GLfloat angle; // [esp+0h] [ebp-4Ch]
  GLfloat anglea; // [esp+0h] [ebp-4Ch]
  GLfloat x; // [esp+4h] [ebp-48h]
  GLfloat y; // [esp+8h] [ebp-44h]
  GLfloat ya; // [esp+8h] [ebp-44h]
  GLfloat yb; // [esp+8h] [ebp-44h]
  GLfloat yc; // [esp+8h] [ebp-44h]
  GLfloat z; // [esp+Ch] [ebp-40h]
  GLfloat v8; // [esp+38h] [ebp-14h]
  __int16 k; // [esp+3Ch] [ebp-10h]
  __int16 i; // [esp+40h] [ebp-Ch]
  __int16 j; // [esp+44h] [ebp-8h]
  __int16 v12; // [esp+48h] [ebp-4h]

  v12 = 0;
  glPushMatrix();
  glTranslatef(EAST_SPIRAL_X, EAST_SPIRAL_Y, EAST_SPIRAL_Z);
  for ( i = 0; i < g_popBottlesCount; ++i )
  {
    if ( g_popBottles[i].isActive == 1 )
    {
      for ( j = 0; j < 12; ++j )
      {
        glPushMatrix();
        angle = (double)j * 7.5 + (double)v12 * 90.0 + WHISTLES_ROTATION_FACTOR;
        glRotatef(angle, 0.0, 1.0, 0.0);
        y = (double)v12 * 3.0 + -6.0 + (double)j * 0.25;
        glTranslatef(0.0, y, 15.0);
        glRotatef(-125.0, 0.0, 1.0, 0.0);
        g_popBottle_ms3d->RenderModelShiny();
        glPushMatrix();
        ya = (double)(12 - j) / 12.0 * 0.5 + 0.25;
        glScalef(1.0, ya, 1.0);
        g_popBottleMiddle_ms3d->RenderModelShiny();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.0, -3.25, 0.0);
        yb = (double)j / 12.0 * 0.5 + 0.25;
        glScalef(1.0, yb, 1.0);
        g_popBottlePop_ms3d->RenderModelShiny();
        glPopMatrix();
        g_popBottleLabel_ms3d->RenderModel();
        glPopMatrix();
        for ( k = 0; k < 100; ++k )
        {
          if ( g_popBottles[i].field_4E4[j][k] > 0.0 )
          {
            glPushMatrix();
            anglea = (double)j * 7.5 + (double)v12 * 90.0 + WHISTLES_ROTATION_FACTOR;
            glRotatef(anglea, 0.0, 1.0, 0.0);
            z = g_popBottles[i].field_4E4[j][k] * 0.75 + 14.0;
            yc = (double)v12 * 3.0
               + (double)j * 0.25
               + g_popBottles[i].field_17A4[j][k]
               + g_popBottles[i].field_4E4[j][k] / 20.0 * (g_popBottles[i].field_4E4[j][k] / 20.0)
               + g_popBottles[i].field_7564[j][k];
            x = g_popBottles[i].field_62A4[j][k] + 0.0;
            glTranslatef(x, yc, z);
            v8 = (1.1 - g_popBottles[i].field_2A64[j][k] / 0.25) * 1.5;
            glScalef(v8, v8, v8);
            glRotatef(g_popBottles[i].field_3D24[j][k], 1.0, 0.0, 0.0);
            glRotatef(g_popBottles[i].field_4FE4[j][k], 0.0, 0.0, 1.0);
            g_steamCloud_2_ms3d->RenderModel();
            glPopMatrix();
          }
        }
      }
      ++v12;
    }
  }
  glPopMatrix();
}

// FUNCTION: MIDIJAM 0x42C660
bool UpdatePopBottles(MUSIC_TIME pmtNow) {
    __int16 v2; // [esp+14h] [ebp-18h]
  __int16 i; // [esp+18h] [ebp-14h]
  __int16 k; // [esp+1Ch] [ebp-10h]
  __int16 m; // [esp+20h] [ebp-Ch]
  __int16 j; // [esp+24h] [ebp-8h]
  __int16 v7; // [esp+28h] [ebp-4h]

  v7 = 0;
  for ( i = 0; i < g_popBottlesCount; ++i )
  {
    v2 = 0;
    for ( j = 0; j < 12; ++j )
    {
      for ( k = 0; k < 16; ++k )
      {
        if ( g_popBottles[i].field_364[j][k] > 0 )
        {
          v7 = 1;
          g_popBottles[i].field_364[j][k] -= pmtNow - g_currentGlobalTime;
          if ( g_popBottles[i].field_364[j][k] <= 0 )
          {
            g_popBottles[i].field_364[j][k] = 0;
            g_popBottles[i].field_4[j] = g_popBottles[i].field_64[j][k];
            g_popBottles[i].field_34[j] = g_popBottles[i].field_64[j][k];
            g_popBottles[i].field_64[j][k] = 0;
          }
          g_popBottles[i].isActive = 1;
          g_popBottles[i].timeToLive = 1000;
          v2 = 1;
          g_popBottles[i].field_D33C[j] = 0;
        }
      }
      if ( g_popBottles[i].field_4[j] > 0 )
      {
        g_popBottles[i].field_4[j] -= pmtNow - g_currentGlobalTime;
        if ( g_popBottles[i].field_4[j] > 0 )
        {
          g_popBottles[i].isActive = 1;
          g_popBottles[i].timeToLive = 1000;
          v2 = 1;
          v7 = 1;
          if ( g_popBottles[i].field_D33C[j] <= 0 )
          {
            g_popBottles[i].field_4E4[j][g_popBottles[i].field_D324[j]] = 0.1;
            g_popBottles[i].field_2A64[j][g_popBottles[i].field_D324[j]] = 0.25;
            g_popBottles[i].field_3D24[j][g_popBottles[i].field_D324[j]] = 0.0;
            g_popBottles[i].field_4FE4[j][g_popBottles[i].field_D324[j]] = 0.0;
            g_popBottles[i].field_62A4[j][g_popBottles[i].field_D324[j]] = 0.0;
            g_popBottles[i].field_7564[j][g_popBottles[i].field_D324[j]] = 0.0;
            g_popBottles[i].field_8824[j][g_popBottles[i].field_D324[j]] = ((double)(50 * rand() / 0x8000) - 25.0)
                                                                             * 0.1;
            g_popBottles[i].field_9AE4[j][g_popBottles[i].field_D324[j]] = ((double)(50 * rand() / 0x8000) - 25.0)
                                                                             * 0.1;
            g_popBottles[i].field_ADA4[j][g_popBottles[i].field_D324[j]] = ((double)(25 * rand() / 0x8000) - 12.5)
                                                                             * 0.001;
            g_popBottles[i].field_C064[j][g_popBottles[i].field_D324[j]] = ((double)(25 * rand() / 0x8000) - 12.5)
                                                                             * 0.001;
            g_popBottles[i].field_17A4[j][g_popBottles[i].field_D324[j]++] = -2.625;
            if ( g_popBottles[i].field_D324[j] >= 100 )
              g_popBottles[i].field_D324[j] = 0;
            g_popBottles[i].field_D33C[j] = 2;
          }
          else
          {
            --g_popBottles[i].field_D33C[j];
          }
        }
        else
        {
          g_popBottles[i].field_4[j] = 0;
        }
      }
      for ( m = 0; m < 100; ++m )
      {
        if ( g_popBottles[i].field_4E4[j][m] <= 0.0 )
        {
          g_popBottles[i].field_4E4[j][m] = 0.0;
        }
        else
        {
          g_popBottles[i].field_4E4[j][m] = g_popBottles[i].field_4E4[j][m] + g_popBottles[i].field_2A64[j][m];
          g_popBottles[i].field_2A64[j][m] = g_popBottles[i].field_2A64[j][m] * 0.95999998;
          g_popBottles[i].field_3D24[j][m] = g_popBottles[i].field_3D24[j][m] + g_popBottles[i].field_8824[j][m];
          g_popBottles[i].field_4FE4[j][m] = g_popBottles[i].field_4FE4[j][m] + g_popBottles[i].field_9AE4[j][m];
          g_popBottles[i].field_62A4[j][m] = g_popBottles[i].field_62A4[j][m] + g_popBottles[i].field_ADA4[j][m];
          g_popBottles[i].field_7564[j][m] = g_popBottles[i].field_7564[j][m] + g_popBottles[i].field_C064[j][m];
          if ( g_popBottles[i].field_2A64[j][m] < 0.0099999998 )
          {
            g_popBottles[i].field_4E4[j][m] = 0.0;
            g_popBottles[i].field_2A64[j][m] = 0.0;
          }
        }
      }
    }
    if ( !v2 && g_popBottles[i].isActive == 1 && --g_popBottles[i].timeToLive <= 0 )
    {
      g_popBottles[i].timeToLive = 0;
      g_popBottles[i].isActive = 0;
    }
  }
  return v7 == 1;
}
