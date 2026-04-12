//
// Created by Jacob on Saturday, April 11, 2026.
//


#include "Telephone.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(telephone, TelephoneState)

Ms3dBundle* g_telephoneHandle_ms3d = nullptr;
Ms3dBundle* g_telephoneKeyX_ms3d[24] = {};
Ms3dBundle* g_telephoneBase_ms3d = nullptr;

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------
extern MUSIC_TIME g_currentGlobalTime;
extern float MAX_RECOIL;

// FUNCTION: MIDIJAM 0x40C360
void RenderTelephone() {
    GLfloat angle; // [esp+0h] [ebp-40h]
    GLfloat x; // [esp+4h] [ebp-3Ch]
    GLfloat xa; // [esp+4h] [ebp-3Ch]
    GLfloat xb; // [esp+4h] [ebp-3Ch]
    GLfloat y; // [esp+8h] [ebp-38h]
    GLfloat z; // [esp+Ch] [ebp-34h]
    GLfloat za; // [esp+Ch] [ebp-34h]
    char ringing; // [esp+33h] [ebp-Dh]
    __int16 i; // [esp+34h] [ebp-Ch]
    __int16 j; // [esp+38h] [ebp-8h]
    __int16 v10; // [esp+3Ch] [ebp-4h]

    v10 = 0;
    glPushMatrix();
    glTranslatef(0.0, 8.0, -14.0);
    for ( i = 0; i < g_telephoneCount; ++i )
    {
        if ( g_telephone[i].isActive == 1 )
        {
            glPushMatrix();
            x = (double)v10 * 20.0;
            glTranslatef(x, -37.5, -50.0);
            g_telephoneBase_ms3d->RenderModel();
            ringing = 0;
            for ( j = 0; j < 12; ++j )
            {
                glPushMatrix();
                glTranslatef(0.0, 3.8, 0.0);
                glRotatef(19.0, 1.0, 0.0, 0.0);
                z = (double)(__int16)(unsigned __int64)((double)j / 3.0) * 1.25 + -1.5;
                xa = (double)(j % 3) * 1.25 + -1.25;
                glTranslatef(xa, 0.0, z);
                if ( g_telephone[i].field_4E8[j] <= 0.0 && g_telephone[i].field_8[j] <= 0 )
                {
                    g_telephoneKeyX_ms3d[j + 12]->RenderModel();
                }
                else
                {
                    glTranslatef(0.0, -0.25, 0.0);
                    g_telephoneKeyX_ms3d[j]->RenderModel();
                    ringing = 1;
                }
                glPopMatrix();
            }
            glPushMatrix();
            if ( ringing == 1 )
            {
                glTranslatef(0.0, 2.0, 0.0);
                za = (double)(2 * rand() / 0x8000) - 1.0;
                y = (double)(2 * rand() / 0x8000) - 1.0;
                xb = (double)(2 * rand() / 0x8000) - 1.0;
                angle = (double)(20 * rand() / 0x8000) - 10.0;
                glRotatef(angle, xb, y, za);
            }
            g_telephoneHandle_ms3d->RenderModel();
            glPopMatrix();
            glPopMatrix();
            ++v10;
        }
    }
    glPopMatrix();
}

// FUNCTION: MIDIJAM 0x435E00
bool UpdateTelephone(MUSIC_TIME pmtNow) {
    int v2; // [esp+8h] [ebp-1Ch]
  float v3; // [esp+Ch] [ebp-18h]
  __int16 v4; // [esp+10h] [ebp-14h]
  __int16 i; // [esp+14h] [ebp-10h]
  __int16 k; // [esp+18h] [ebp-Ch]
  __int16 j; // [esp+1Ch] [ebp-8h]
  __int16 v8; // [esp+20h] [ebp-4h]

  v8 = 0;
  for ( i = 0; i < g_telephoneCount; ++i )
  {
    v4 = 0;
    for ( j = 0; j < 12; ++j )
    {
      for ( k = 0; k < 16; ++k )
      {
        if ( g_telephone[i].field_368[j][k] > 0 )
        {
          v8 = 1;
          g_telephone[i].field_368[j][k] -= pmtNow - g_currentGlobalTime;
          if ( g_telephone[i].field_368[j][k] <= 0 )
          {
            v2 = g_telephone[i].field_518[j][k];
            v3 = ((double)v2 * 0.75 + 32.0 + (double)v2 * 0.75 + 32.0) / 128.0;
            g_telephone[i].field_4 = v3 + g_telephone[i].field_4;
            if ( g_telephone[i].field_4 > (double)MAX_RECOIL )
              g_telephone[i].field_4 = MAX_RECOIL;
            g_telephone[i].field_368[j][k] = 0;
            g_telephone[i].field_4E8[j] = (double)g_telephone[i].field_518[j][k] * 8.0 / 128.0 + 1.75;
            g_telephone[i].field_8[j] = g_telephone[i].field_68[j][k];
            g_telephone[i].field_38[j] = g_telephone[i].field_68[j][k];
            g_telephone[i].field_68[j][k] = 0;
          }
          g_telephone[i].isActive = 1;
          g_telephone[i].timeToLive = 1000;
          v4 = 1;
        }
      }
      if ( g_telephone[i].field_8[j] <= 0 )
      {
        if ( g_telephone[i].field_4E8[j] > 0.0 )
        {
          v8 = 1;
          g_telephone[i].field_4E8[j] = g_telephone[i].field_4E8[j] - 0.30000001;
          if ( g_telephone[i].field_4E8[j] > 0.0 )
          {
            g_telephone[i].isActive = 1;
            g_telephone[i].timeToLive = 1000;
            v4 = 1;
          }
          else
          {
            g_telephone[i].field_4E8[j] = 0.0;
          }
        }
      }
      else
      {
        g_telephone[i].field_8[j] -= pmtNow - g_currentGlobalTime;
        if ( g_telephone[i].field_8[j] > 0 )
        {
          g_telephone[i].isActive = 1;
          g_telephone[i].timeToLive = 1000;
          v4 = 1;
          v8 = 1;
        }
        else
        {
          g_telephone[i].field_8[j] = 0;
        }
        g_telephone[i].field_4E8[j] = g_telephone[i].field_4E8[j] - 0.15000001;
        if ( g_telephone[i].field_4E8[j] > 0.0 )
        {
          g_telephone[i].isActive = 1;
          g_telephone[i].timeToLive = 1000;
          v4 = 1;
        }
        else
        {
          g_telephone[i].field_4E8[j] = 0.0;
        }
      }
    }
    if ( !v4 && g_telephone[i].isActive == 1 && --g_telephone[i].timeToLive <= 0 )
    {
      g_telephone[i].timeToLive = 0;
      g_telephone[i].isActive = 0;
    }
  }
  return v8 == 1;
}
