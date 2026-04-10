//
// Created by Jacob on 03/22/2026.
//

#include "Guitar.h"
#include "../render/texture.h"
#include "../macros.h"

#include <GL/gl.h>

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
DEF_INST(guitar, GuitarState)

extern MUSIC_TIME g_currentGlobalTime;
extern int g_vibratingString_frame;
extern GLfloat g_refreshRate;
extern short g_latinSquare[6][6];

Ms3dBundle* g_guitar_ms3d = nullptr;
Ms3dBundle* g_guitarStringLow_ms3d = nullptr;
Ms3dBundle* g_guitarLowStringBottomX_ms3d[5];
Ms3dBundle* g_guitarHighStringBottomX_ms3d[5];
Ms3dBundle* g_guitarStringHigh_ms3d = nullptr;
Ms3dBundle* g_guitarNoteFinger_ms3d = nullptr;

GLfloat flt_45EACC = 0.0f;
__int16 word_46CEE0[23][6];
float flt_4654A0[23] = {};

// ---------------------------------------------------------------------------
// FUNCTION: MIDIJAM 0xXXXXXXXX
// ---------------------------------------------------------------------------
void RenderGuitar() {
    GLfloat x; // [esp+0h] [ebp-20h]
    GLfloat y; // [esp+4h] [ebp-1Ch]
    GLfloat ya; // [esp+4h] [ebp-1Ch]
    GLfloat yb; // [esp+4h] [ebp-1Ch]
    GLfloat yc; // [esp+4h] [ebp-1Ch]
    GLfloat yd; // [esp+4h] [ebp-1Ch]
    GLfloat ye; // [esp+4h] [ebp-1Ch]
    GLfloat z; // [esp+8h] [ebp-18h]
    __int16 i; // [esp+14h] [ebp-Ch]
    GLboolean v9; // [esp+1Bh] [ebp-5h]
    __int16 v10; // [esp+1Ch] [ebp-4h]

    v10 = 0;
    v9 = g_guitar_ms3d->ApplyMaterial();
    for (i = 0; i < g_guitarCount; ++i) {
        if (g_guitar[i].isActive == 1) {
            glPushMatrix();
            z = (double)v10 * -5.0;
            x = (double)v10 * 5.0;
            glTranslatef(x, 0.0, z);
            g_guitar_ms3d->Render();
            glPushMatrix();
            glTranslatef(-0.92699999, 16.594999, 0.0);
            glRotatef(-1.035, 0.0, 0.0, 1.0);
            if (g_guitar[i].field_2C4[0] <= 1 || g_guitar[i].field_2C4[0] > 23) {
                if (!g_guitar[i].field_2C4[0])
                    g_guitarStringLow_ms3d->Render();
            }
            else {
                glPushMatrix();
                glTranslatef(0.0, *(&flt_45EACC + g_guitar[i].field_2C4[0]), 0.0);
                g_guitarNoteFinger_ms3d->Render();
                glPopMatrix();
                glScalef(1.0, flt_4654A0[g_guitar[i].field_2C4[0]], 1.0);
                g_guitarStringLow_ms3d->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(-0.565, 16.594999, 0.0);
            glRotatef(-0.60000002, 0.0, 0.0, 1.0);
            if (g_guitar[i].field_2C4[1] <= 1 || g_guitar[i].field_2C4[1] > 23) {
                if (!g_guitar[i].field_2C4[1])
                    g_guitarStringLow_ms3d->Render();
            }
            else {
                glPushMatrix();
                glTranslatef(0.0, *(&flt_45EACC + g_guitar[i].field_2C4[1]), 0.0);
                g_guitarNoteFinger_ms3d->Render();
                glPopMatrix();
                glScalef(1.0, flt_4654A0[g_guitar[i].field_2C4[1]], 1.0);
                g_guitarStringLow_ms3d->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(-0.20999999, 16.594999, 0.0);
            glRotatef(-0.23, 0.0, 0.0, 1.0);
            if (g_guitar[i].field_2C4[2] <= 1 || g_guitar[i].field_2C4[2] > 23) {
                if (!g_guitar[i].field_2C4[2])
                    g_guitarStringLow_ms3d->Render();
            }
            else {
                glPushMatrix();
                glTranslatef(0.0, *(&flt_45EACC + g_guitar[i].field_2C4[2]), 0.0);
                g_guitarNoteFinger_ms3d->Render();
                glPopMatrix();
                glScalef(1.0, flt_4654A0[g_guitar[i].field_2C4[2]], 1.0);
                g_guitarStringLow_ms3d->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(0.19750001, 16.594999, 0.0);
            glRotatef(0.096500002, 0.0, 0.0, 1.0);
            if (g_guitar[i].field_2C4[3] <= 1 || g_guitar[i].field_2C4[3] > 23) {
                if (!g_guitar[i].field_2C4[3])
                    g_guitarStringHigh_ms3d->Render();
            }
            else {
                glPushMatrix();
                glTranslatef(0.0, *(&flt_45EACC + g_guitar[i].field_2C4[3]), 0.0);
                g_guitarNoteFinger_ms3d->Render();
                glPopMatrix();
                glScalef(1.0, flt_4654A0[g_guitar[i].field_2C4[3]], 1.0);
                g_guitarStringHigh_ms3d->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(0.56300002, 16.594999, 0.0);
            glRotatef(0.45500001, 0.0, 0.0, 1.0);
            if (g_guitar[i].field_2C4[4] <= 1 || g_guitar[i].field_2C4[4] > 23) {
                if (!g_guitar[i].field_2C4[4])
                    g_guitarStringHigh_ms3d->Render();
            }
            else {
                glPushMatrix();
                glTranslatef(0.0, *(&flt_45EACC + g_guitar[i].field_2C4[4]), 0.0);
                g_guitarNoteFinger_ms3d->Render();
                glPopMatrix();
                glScalef(1.0, flt_4654A0[g_guitar[i].field_2C4[4]], 1.0);
                g_guitarStringHigh_ms3d->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(0.88800001, 16.594999, 0.0);
            glRotatef(0.91500002, 0.0, 0.0, 1.0);
            if (g_guitar[i].field_2C4[5] <= 1 || g_guitar[i].field_2C4[5] > 23) {
                if (!g_guitar[i].field_2C4[5])
                    g_guitarStringHigh_ms3d->Render();
            }
            else {
                glPushMatrix();
                glTranslatef(0.0, *(&flt_45EACC + g_guitar[i].field_2C4[5]), 0.0);
                g_guitarNoteFinger_ms3d->Render();
                glPopMatrix();
                glScalef(1.0, flt_4654A0[g_guitar[i].field_2C4[5]], 1.0);
                g_guitarStringHigh_ms3d->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(-1.545, -18.099001, 0.0);
            glRotatef(-1.035, 0.0, 0.0, 1.0);
            if (g_guitar[i].field_2C4[0] <= 1 || g_guitar[i].field_2C4[0] > 23) {
                if (g_guitar[i].field_2C4[0] == 1)
                    g_guitarLowStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            else {
                y = 1.0 - flt_4654A0[g_guitar[i].field_2C4[0]];
                glScalef(1.0, y, 1.0);
                g_guitarLowStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(-0.92199999, -18.099001, 0.0);
            glRotatef(-0.60000002, 0.0, 0.0, 1.0);
            if (g_guitar[i].field_2C4[1] <= 1 || g_guitar[i].field_2C4[1] > 23) {
                if (g_guitar[i].field_2C4[1] == 1)
                    g_guitarLowStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            else {
                ya = 1.0 - flt_4654A0[g_guitar[i].field_2C4[1]];
                glScalef(1.0, ya, 1.0);
                g_guitarLowStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(-0.34400001, -18.099001, 0.0);
            glRotatef(-0.23, 0.0, 0.0, 1.0);
            if (g_guitar[i].field_2C4[2] <= 1 || g_guitar[i].field_2C4[2] > 23) {
                if (g_guitar[i].field_2C4[2] == 1)
                    g_guitarLowStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            else {
                yb = 1.0 - flt_4654A0[g_guitar[i].field_2C4[2]];
                glScalef(1.0, yb, 1.0);
                g_guitarLowStringBottomX_ms3d[g_vibratingString_frame]->Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(0.25999999, -18.099001, 0.0);
            glRotatef(0.096500002, 0.0, 0.0, 1.0);
            if (g_guitar[i].field_2C4[3] <= 1 || g_guitar[i].field_2C4[3] > 23) {
                if (g_guitar[i].field_2C4[3] == 1)
                    g_guitarStringHigh_ms3d[g_vibratingString_frame].Render();
            }
            else {
                yc = 1.0 - flt_4654A0[g_guitar[i].field_2C4[3]];
                glScalef(1.0, yc, 1.0);
                g_guitarStringHigh_ms3d[g_vibratingString_frame].Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(0.84200001, -18.099001, 0.0);
            glRotatef(0.45500001, 0.0, 0.0, 1.0);
            if (g_guitar[i].field_2C4[4] <= 1 || g_guitar[i].field_2C4[4] > 23) {
                if (g_guitar[i].field_2C4[4] == 1)
                    g_guitarStringHigh_ms3d[g_vibratingString_frame].Render();
            }
            else {
                yd = 1.0 - flt_4654A0[g_guitar[i].field_2C4[4]];
                glScalef(1.0, yd, 1.0);
                g_guitarStringHigh_ms3d[g_vibratingString_frame].Render();
            }
            glPopMatrix();
            glPushMatrix();
            glTranslatef(1.446, -18.099001, 0.0);
            glRotatef(0.91500002, 0.0, 0.0, 1.0);
            if (g_guitar[i].field_2C4[5] <= 1 || g_guitar[i].field_2C4[5] > 23) {
                if (g_guitar[i].field_2C4[5] == 1)
                    g_guitarStringHigh_ms3d[g_vibratingString_frame].Render();
            }
            else {
                ye = 1.0 - flt_4654A0[g_guitar[i].field_2C4[5]];
                glScalef(1.0, ye, 1.0);
                g_guitarStringHigh_ms3d[g_vibratingString_frame].Render();
            }
            glPopMatrix();
            glPopMatrix();
            ++v10;
        }
    }
    SetTextureState(v9);
    g_guitarVisible = 0;
    if (v10 > 0)
        g_guitarVisible = v10;
}

// ---------------------------------------------------------------------------
// FUNCTION: MIDIJAM 0xXXXXXXXX
// ---------------------------------------------------------------------------
bool UpdateGuitar(MUSIC_TIME pmtNow) {
    __int16 v2; // [esp+4h] [ebp-138h]
  __int16 ii; // [esp+8h] [ebp-134h]
  __int16 n; // [esp+Ch] [ebp-130h]
  __int16 m; // [esp+10h] [ebp-12Ch]
  __int16 v6[6][6]; // [esp+14h] [ebp-128h] BYREF
  __int16 v7; // [esp+5Ch] [ebp-E0h]
  __int16 v8; // [esp+60h] [ebp-DCh]
  __int16 v9; // [esp+64h] [ebp-D8h]
  __int16 v10[12]; // [esp+68h] [ebp-D4h]
  __int16 v11; // [esp+80h] [ebp-BCh]
  __int16 v12[6][6]; // [esp+84h] [ebp-B8h] BYREF
  __int16 v13[12]; // [esp+CCh] [ebp-70h]
  __int16 k; // [esp+E4h] [ebp-58h]
  __int16 v15; // [esp+E8h] [ebp-54h]
  __int16 v16; // [esp+ECh] [ebp-50h]
  __int16 v17; // [esp+F0h] [ebp-4Ch]
  __int16 v18; // [esp+F4h] [ebp-48h]
  __int16 v19[6]; // [esp+F8h] [ebp-44h]
  __int16 v20; // [esp+104h] [ebp-38h]
  __int16 j; // [esp+108h] [ebp-34h]
  __int16 v22; // [esp+10Ch] [ebp-30h]
  __int16 v23; // [esp+110h] [ebp-2Ch]
  __int16 v24; // [esp+114h] [ebp-28h]
  __int16 v25; // [esp+118h] [ebp-24h]
  __int16 v26; // [esp+11Ch] [ebp-20h]
  __int16 v27[6]; // [esp+120h] [ebp-1Ch]
  __int16 i; // [esp+12Ch] [ebp-10h]
  __int16 slot; // [esp+130h] [ebp-Ch]
  __int16 note; // [esp+134h] [ebp-8h]
  __int16 bActive; // [esp+138h] [ebp-4h]

  bActive = 0;
  for ( i = 0; i < g_guitarCount; ++i )
  {
    v25 = 0;
    for ( note = 0; note < 88; ++note )
    {
      if ( g_guitar[i].field_4[note] > 0 )
      {
        g_guitar[i].field_4[note] -= pmtNow - g_currentGlobalTime;
        if ( g_guitar[i].field_4[note] > 0 )
        {
          g_guitar[i].isActive = 1;
          g_guitar[i].timeToLive = 1000;
          v25 = 1;
        }
        else
        {
          g_guitar[i].field_4[note] = 0;
          *((__int16 *)&g_guitar[i].field_164[87] + g_guitar[i].field_164[note] + 1) = 0;
          g_guitar[i].field_164[note] = 0;
        }
      }
    }
    for ( note = 0; note < 6; ++note )
    {
      if ( g_guitar[i].field_2C4[note] > 0
        && g_guitar[i].field_4[word_46CEE0[g_guitar[i].field_2C4[note] - 1][note]] <= 0 )
      {
        g_guitar[i].field_4[word_46CEE0[g_guitar[i].field_2C4[note] - 1][note]] = 0;
        g_guitar[i].field_164[word_46CEE0[g_guitar[i].field_2C4[note] - 1][note]] = 0;
        g_guitar[i].field_2C4[note] = 0;
      }
    }
    v26 = 0;
    for ( note = 0; note < 88; ++note )
    {
      for ( slot = 0; slot < 16; ++slot )
      {
        if ( g_guitar[i].field_18D2[note][slot] > 0 )
        {
          bActive = 1;
          g_guitar[i].field_18D2[note][slot] -= pmtNow - g_currentGlobalTime;
          if ( g_guitar[i].field_18D2[note][slot] > 0 )
          {
            g_guitar[i].isActive = 1;
            g_guitar[i].timeToLive = 1000;
            v25 = 1;
          }
          else
          {
            g_guitar[i].field_18D2[note][slot] = 0;
            g_guitar[i].field_4[note] = g_guitar[i].field_2D2[note][slot];
            g_guitar[i].field_2D2[note][slot] = 0;
            if ( v26 < 6 )
              v27[v26++] = note;
          }
        }
      }
    }
    if ( v26 > 0 )
    {
      for ( note = 0; note < v26; ++note )
      {
        for ( j = 0; j < 6; ++j )
        {
          for ( k = 0; k < 6; ++k )
          {
            for ( m = 0; m < 23; ++m )
            {
              if ( word_46CEE0[m][k] == v27[note] && g_guitar[i].field_2C4[k] == m + 1 )
              {
                g_guitar[i].field_164[v27[note]] = 0;
                g_guitar[i].field_2C4[k] = 0;
              }
            }
          }
        }
      }
      for ( j = 0; j < 6; ++j )
      {
        for ( k = 0; k < 6; ++k )
        {
          v12[k][j] = 0;
          v6[k][j] = 0;
          if ( g_guitar[i].field_2C4[k] > 0 )
            v12[k][j] = g_guitar[i].field_2C4[k];
        }
      }
      for ( j = 0; j < 6; ++j )
      {
        for ( note = 0; note < v26; ++note )
        {
          for ( n = 0; n < 6; ++n )
          {
            v2 = 0;
            for ( ii = 0; ii < 23; ++ii )
            {
              if ( word_46CEE0[ii][g_latinSquare[j][n]] == v27[note] && !v12[g_latinSquare[j][n]][j] )
              {
                v12[g_latinSquare[j][n]][j] = ii + 1;
                v6[g_latinSquare[j][n]][j] = v27[note];
                v2 = 1;
                break;
              }
            }
            if ( v2 > 0 )
              break;
          }
        }
      }
      v17 = 0;
      for ( j = 0; j < 6; ++j )
      {
        v23 = 0;
        v13[j + 6] = 24;
        v20 = 24;
        v22 = 0;
        v24 = 5;
        v13[j] = 0;
        v10[j] = 25;
        v19[j] = 6;
        for ( k = 0; k < 6; ++k )
        {
          if ( v12[k][j] > 0 )
          {
            ++v13[j];
            if ( k > v22 )
              v22 = k;
            if ( k < v24 )
              v24 = k;
            if ( v12[k][j] > v23 )
              v23 = v12[k][j];
            if ( v12[k][j] < v13[j + 6] )
              v13[j + 6] = v12[k][j];
            if ( v12[k][j] > 1 && v12[k][j] < v20 )
              v20 = v12[k][j];
          }
        }
        if ( v13[j] > 0 )
        {
          v10[j] = v23 - v20;
          v19[j] = v22 - v24;
        }
        if ( v13[j] > v17 )
          v17 = v13[j];
      }
      v15 = 0;
      v8 = 24;
      v7 = 6;
      v9 = 25;
      for ( j = 5; j >= 0; --j )
        v10[j + 6] = 0;
      for ( j = 5; j >= 0; --j )
      {
        if ( v17 == v13[j] )
          v10[j + 6] = 1;
      }
      v11 = 25;
      for ( j = 5; j >= 0; --j )
      {
        if ( v10[j + 6] == 1 && v19[j] <= v11 )
          v11 = v19[j];
      }
      for ( j = 5; j >= 0; --j )
      {
        if ( v10[j + 6] == 1 && v19[j] > v11 )
          v10[j + 6] = 0;
      }
      v16 = 25;
      for ( j = 5; j >= 0; --j )
      {
        if ( v10[j + 6] == 1 && abs(v13[j + 6] - g_guitar[i].field_2D0) <= v16 )
          v16 = abs(v13[j + 6] - g_guitar[i].field_2D0);
      }
      for ( j = 5; j >= 0; --j )
      {
        if ( v10[j + 6] == 1 && abs(v13[j + 6] - g_guitar[i].field_2D0) > v16 )
          v10[j + 6] = 0;
      }
      v18 = 6;
      for ( j = 5; j >= 0; --j )
      {
        if ( v10[j + 6] == 1 && v10[j] <= v18 )
          v18 = v10[j];
      }
      for ( j = 5; j >= 0; --j )
      {
        if ( v10[j + 6] == 1 && v10[j] > v18 )
          v10[j + 6] = 0;
      }
      v15 = 0;
      for ( j = 5; j >= 0; --j )
      {
        if ( v10[j + 6] == 1 )
        {
          v15 = j;
          break;
        }
      }
      for ( k = 0; k < 6; ++k )
      {
        if ( v12[k][v15] > 0 && v12[k][v15] <= 23 && v6[k][v15] > 0 )
        {
          g_guitar[i].field_2C4[k] = v12[k][v15];
          g_guitar[i].field_164[v6[k][v15]] = k + 1;
          g_guitar[i].field_2D0 = v13[v15 + 6];
        }
      }
    }
    if ( !v25 && g_guitar[i].isActive == 1 && --g_guitar[i].timeToLive <= 0 )
    {
      g_guitar[i].timeToLive = 0;
      g_guitar[i].isActive = 0;
    }
  }
  return bActive == 1;
}
