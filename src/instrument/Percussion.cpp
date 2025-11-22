//
// Created by Jacob on 11/21/2025.
//

#include "Percussion.h"

#include <windows.h>
#include <GL/gl.h>

void RenderPercussion()
{
  double v0; // st7
  double v1; // st7
  GLfloat angle_bassDrumArm; // [esp+0h] [ebp-3Ch]
  GLfloat angle_tom; // [esp+0h] [ebp-3Ch]
  float anglec; // [esp+0h] [ebp-3Ch]
  float angle; // [esp+0h] [ebp-3Ch]
  GLfloat angled; // [esp+0h] [ebp-3Ch]
  GLfloat wobble; // [esp+0h] [ebp-3Ch]
  GLfloat x; // [esp+4h] [ebp-38h]
  GLfloat xa; // [esp+4h] [ebp-38h]
  GLfloat xb; // [esp+4h] [ebp-38h]
  GLfloat xc; // [esp+4h] [ebp-38h]
  GLfloat tomRecoil; // [esp+8h] [ebp-34h]
  GLfloat y_snare; // [esp+8h] [ebp-34h]
  GLfloat yd; // [esp+8h] [ebp-34h]
  GLfloat ye; // [esp+8h] [ebp-34h]
  GLfloat yf; // [esp+8h] [ebp-34h]
  GLfloat yg; // [esp+8h] [ebp-34h]
  GLfloat yh; // [esp+8h] [ebp-34h]
  GLfloat yi; // [esp+8h] [ebp-34h]
  GLfloat yj; // [esp+8h] [ebp-34h]
  GLfloat yk; // [esp+8h] [ebp-34h]
  float y; // [esp+8h] [ebp-34h]
  GLfloat yl; // [esp+8h] [ebp-34h]
  GLfloat y_cowbell; // [esp+8h] [ebp-34h]
  float ya; // [esp+8h] [ebp-34h]
  GLfloat yn; // [esp+8h] [ebp-34h]
  GLfloat yo; // [esp+8h] [ebp-34h]
  GLfloat yp; // [esp+8h] [ebp-34h]
  GLfloat yq; // [esp+8h] [ebp-34h]
  GLfloat yr; // [esp+8h] [ebp-34h]
  GLfloat ys; // [esp+8h] [ebp-34h]
  GLfloat yt; // [esp+8h] [ebp-34h]
  GLfloat recoil_bassDrum; // [esp+Ch] [ebp-30h]
  GLfloat za; // [esp+Ch] [ebp-30h]
  float bellSwingAmount; // [esp+10h] [ebp-2Ch]
  float bellSwingDelta; // [esp+14h] [ebp-28h]
  float bellOscillation; // [esp+18h] [ebp-24h]
  float clickSwingAmount; // [esp+1Ch] [ebp-20h]
  float clickSwingDelta; // [esp+20h] [ebp-1Ch]
  float clickOscillation; // [esp+24h] [ebp-18h]
  GLfloat v41; // [esp+28h] [ebp-14h]
  __int16 particleIndex; // [esp+2Ch] [ebp-10h]
  GLfloat v43; // [esp+30h] [ebp-Ch]
  __int16 i; // [esp+34h] [ebp-8h]
  __int16 tom_index; // [esp+38h] [ebp-4h]
  __int16 j; // [esp+38h] [ebp-4h]

  // // BASS DRUM
  // glPushMatrix();
  // glPushMatrix();
  // glTranslatef(0.0, 5.5650001, 3.618);
  // glRotatef(g_recoil_bassDrumArm, 1.0, 0.0, 0.0);
  // Ms3dBundle::RenderModel(g_drumSet_BassDrumBeaterArm_ms3d);
  // glPopMatrix();
  // glPushMatrix();
  // glTranslatef(0.0, 0.0, 2.3);
  // Ms3dBundle::RenderModel(g_drumSet_BassDrumBeaterHolder_ms3d);
  // glPopMatrix();
  // glTranslatef(0.0, 0.477, 9.8540001);
  // angle_bassDrumArm = g_recoil_bassDrumArm / 45.0 * 16.0;
  // glRotatef(angle_bassDrumArm, 1.0, 0.0, 0.0);
  // Ms3dBundle::RenderModel(g_drumSet_BassDrumPedal_ms3d);
  // glPopMatrix();
  // glPushMatrix();
  // recoil_bassDrum = -g_recoil_bassdrum;
  // glTranslatef(0.0, 0.0, recoil_bassDrum);
  // Ms3dBundle::RenderModel(g_drumSet_bassDrum_ms3d);
  // glPopMatrix();
  // // TOMS
  // for ( tom_index = 0; tom_index < 6; ++tom_index )
  // {
  //   glPushMatrix();
  //   glTranslatef(DRUMSET_TOM_LOC_X[tom_index], DRUMSET_TOM_LOC_Y[tom_index], DRUMSET_TOM_LOC_Z[tom_index]);
  //   glRotatef(DRUMSET_TOM_ROT_Y[tom_index], 0.0, 1.0, 0.0);
  //   glRotatef(DRUMSET_TOM_ROT_X[tom_index], 1.0, 0.0, 0.0);
  //   tomRecoil = -g_recoil_tom[tom_index];
  //   glTranslatef(0.0, tomRecoil, 0.0);
  //   glPushMatrix();
  //   glScalef(DRUMSET_TOM_SCALE_XZ[tom_index], DRUMSET_TOM_SCALE_Y[tom_index], DRUMSET_TOM_SCALE_XZ[tom_index]);
  //   Ms3dBundle::RenderModel(g_drumSet_tom_ms3d);
  //   glPopMatrix();
  //   angle_tom = -0.5 * DRUMSET_TOM_ROT_Y[tom_index];
  //   glRotatef(angle_tom, 0.0, 1.0, 0.0);
  //   I_Tom(tom_index);
  //   glPopMatrix();
  // }
  // // SNARE
  // glPushMatrix();
  // y_snare = 18.5 - g_recoil_snaredrum;
  // glTranslatef(-10.64, y_snare, 7.6799998);
  // glRotatef(45.0, 0.0, 1.0, 0.0);
  // glRotatef(15.0, 1.0, 0.0, 0.0);
  // Ms3dBundle::RenderModel(g_drumSet_snareDrum_ms3d);
  // I_SnareDrum();
  // I_SideStick();
  // glPopMatrix();
  // // TIMBALES
  // if ( IsShowPercussionInstrument(HIGH_TIMBALE, g_recoil_timbale_hi)
  //   || IsShowPercussionInstrument(LOW_TIMBALE, g_recoil_timbale_lo) )
  // {
  //   glPushMatrix();
  //   glTranslatef(-42.5, 50.0, 17.0);
  //   glRotatef(55.0, 0.0, 1.0, 0.0);
  //   glTranslatef(6.0999999, 0.0, 0.0);
  //   glRotatef(30.0, 1.0, 0.0, 0.0);
  //   yd = -g_recoil_timbale_hi;
  //   glTranslatef(0.0, yd, 0.0);
  //   glPushMatrix();
  //   glScalef(0.75, 0.75, 0.75);                 // make hi timable smaller
  //   Ms3dBundle::RenderModel(g_drumSet_Timbale_ms3d);
  //   glPopMatrix();
  //   glRotatef(-18.75, 0.0, 1.0, 0.0);
  //   UpdatePercussionStriker(HIGH_TIMBALE, g_recoil_timbale_hi, 3.0, g_drumSet_Stick_ms3d, 0, 0.0);
  //   glPopMatrix();
  //   glPushMatrix();
  //   glTranslatef(-42.5, 50.0, 17.0);
  //   glRotatef(55.0, 0.0, 1.0, 0.0);
  //   glTranslatef(-6.0999999, 0.0, 0.0);
  //   glRotatef(30.0, 1.0, 0.0, 0.0);
  //   ye = -g_recoil_timbale_lo;
  //   glTranslatef(0.0, ye, 0.0);
  //   glPushMatrix();
  //   Ms3dBundle::RenderModel(g_drumSet_Timbale_ms3d);
  //   glPopMatrix();
  //   glRotatef(-18.75, 0.0, 1.0, 0.0);
  //   UpdatePercussionStriker(LOW_TIMBALE, g_recoil_timbale_lo, 3.0, g_drumSet_Stick_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // // WHISTLE, SHORT
  // if ( IsShowPercussionInstrument(SHORT_WHISTLE, g_recoil_whistle_short) )
  // {
  //   glPushMatrix();
  //   yf = g_recoil_whistle_short + 40.5;
  //   glTranslatef(-8.5, yf, -8.0);
  //   glRotatef(15.5, 0.0, 1.0, 0.0);
  //   glRotatef(12.5, 1.0, 0.0, 0.0);
  //   Ms3dBundle::RenderModelShiny(whistle_ms3d);
  //   glPopMatrix();
  // }
  // for ( i = 0; i < 100; ++i )
  // {
  //   if ( g_ds_particles[i + 1101] > 0.0 )
  //   {
  //     glPushMatrix();
  //     glTranslatef(-8.5, 44.25, -8.0);
  //     glRotatef(15.5, 0.0, 1.0, 0.0);
  //     glRotatef(12.5, 1.0, 0.0, 0.0);
  //     yg = g_ds_particles[i + 1101] / 20.0 * (g_ds_particles[i + 1101] / 20.0)
  //        + g_ds_particles[i + 1201]
  //        + g_ds_particles[i + 1701];
  //     x = g_ds_particles[i + 1601] + 0.0;
  //     glTranslatef(x, yg, g_ds_particles[i + 1101]);
  //     v0 = 1.1 - g_ds_particles[i + 1301] / 0.25;
  //     v43 = v0 + v0;
  //     glScalef(v43, v43, v43);
  //     glRotatef(g_ds_particles[i + 1401], 1.0, 0.0, 0.0);
  //     glRotatef(g_ds_particles[i + 1501], 0.0, 0.0, 1.0);
  //     Ms3dBundle::RenderModel(steamCloud_0_ms3d);
  //     glPopMatrix();
  //   }
  // }
  // // WHISTLE, LONG
  // if ( IsShowPercussionInstrument(LONG_WHISTLE, g_recoil_whistle_long) )
  // {
  //   glPushMatrix();
  //   yh = g_recoil_whistle_long + 37.5;
  //   glTranslatef(-3.5, yh, -8.0);
  //   glRotatef(15.5, 0.0, 1.0, 0.0);
  //   glRotatef(12.5, 1.0, 0.0, 0.0);
  //   Ms3dBundle::RenderModelShiny(whistle_ms3d);
  //   glPopMatrix();
  // }
  // for ( particleIndex = 0; particleIndex < 100; ++particleIndex )
  // {
  //   if ( g_ds_particles[particleIndex] > 0.0 )
  //   {
  //     glPushMatrix();
  //     glTranslatef(-3.5, 41.25, -8.0);
  //     glRotatef(15.5, 0.0, 1.0, 0.0);
  //     glRotatef(12.5, 1.0, 0.0, 0.0);
  //     yi = g_ds_particles[particleIndex] / 20.0 * (g_ds_particles[particleIndex] / 20.0)
  //        + g_ds_particles[particleIndex + 100]
  //        + g_ds_particles[particleIndex + 600];
  //     xa = g_ds_particles[particleIndex + 500] + 0.0;
  //     glTranslatef(xa, yi, g_ds_particles[particleIndex]);
  //     v1 = 1.1 - g_ds_particles[particleIndex + 200] / 0.25;
  //     v41 = v1 + v1;
  //     glScalef(v41, v41, v41);
  //     glRotatef(g_ds_particles[particleIndex + 300], 1.0, 0.0, 0.0);
  //     glRotatef(g_ds_particles[particleIndex + 400], 0.0, 0.0, 1.0);
  //     Ms3dBundle::RenderModel(steamCloud_1_ms3d);
  //     glPopMatrix();
  //   }
  // }
  // // BONGOS
  // if ( IsShowPercussionInstrument(HIGH_BONGO, g_recoil_bongo_hi)
  //   || IsShowPercussionInstrument(LOW_BONGO, g_recoil_bongo_lo) )
  // {
  //   glPushMatrix();
  //   glTranslatef(-37.5, 40.0, 22.0);
  //   glRotatef(75.0, 0.0, 1.0, 0.0);
  //   glTranslatef(-3.8, 0.0, 0.0);
  //   glRotatef(30.0, 1.0, 0.0, 0.0);
  //   yj = -g_recoil_bongo_hi;
  //   glTranslatef(0.0, yj, 0.0);
  //   glPushMatrix();
  //   glScalef(0.89999998, 0.89999998, 0.89999998);
  //   Ms3dBundle::RenderModel(g_drumSet_bongo_ms3d);
  //   glPopMatrix();
  //   glRotatef(-37.5, 0.0, 1.0, 0.0);
  //   glTranslatef(0.0, 0.0, -6.0);
  //   UpdatePercussionStriker(HIGH_BONGO, g_recoil_bongo_hi, 0.0, handLeft_ms3d, 0, 0.0);
  //   glPopMatrix();
  //   glPushMatrix();
  //   glTranslatef(-37.5, 40.0, 22.0);
  //   glRotatef(75.0, 0.0, 1.0, 0.0);
  //   glTranslatef(3.8, 0.0, 0.0);
  //   glRotatef(30.0, 1.0, 0.0, 0.0);
  //   yk = -g_recoil_bongo_lo;
  //   glTranslatef(0.0, yk, 0.0);
  //   glPushMatrix();
  //   Ms3dBundle::RenderModel(g_drumSet_bongo_ms3d);
  //   glPopMatrix();
  //   glRotatef(-18.75, 0.0, 1.0, 0.0);
  //   glTranslatef(0.0, 0.0, -6.0);
  //   UpdatePercussionStriker(LOW_BONGO, g_recoil_bongo_lo, 0.0, handRight_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // // CONGAS
  // if ( IsShowPercussionInstrument(OPEN_HIGH_CONGA, g_recoil_conga_high_open)
  //   || IsShowPercussionInstrument(MUTE_HIGH_CONGA, g_recoil_conga_high_mute)
  //   || IsShowPercussionInstrument(LOW_CONGA, g_recoil_conga_low) )
  // {
  //   glPushMatrix();
  //   glTranslatef(-27.5, 35.0, 32.0);
  //   glRotatef(75.0, 0.0, 1.0, 0.0);
  //   glTranslatef(-7.8000002, 0.0, 0.0);
  //   glRotatef(10.0, 1.0, 0.0, 0.0);
  //   if ( g_recoil_conga_high_open <= g_recoil_conga_high_mute )
  //     y = -g_recoil_conga_high_mute;
  //   else
  //     y = -g_recoil_conga_high_open;
  //   glTranslatef(0.0, y, 0.0);
  //   glPushMatrix();
  //   glScalef(0.89999998, 0.89999998, 0.89999998);
  //   Ms3dBundle::RenderModel(g_drumSet_conga_ms3d);
  //   glPopMatrix();
  //   glPushMatrix();
  //   glRotatef(-18.75, 0.0, 1.0, 0.0);
  //   glTranslatef(0.0, 0.0, -6.0);
  //   UpdatePercussionStriker(OPEN_HIGH_CONGA, g_recoil_conga_high_open, 0.0, handLeft_ms3d, 0, 0.0);
  //   glPopMatrix();
  //   glPushMatrix();
  //   glRotatef(-9.375, 0.0, 1.0, 0.0);
  //   glTranslatef(-3.0, 0.0, -12.0);
  //   UpdatePercussionStriker(MUTE_HIGH_CONGA, g_recoil_conga_high_mute, 0.0, handLeft_ms3d, 0, 0.0);
  //   glPopMatrix();
  //   glPopMatrix();
  //   glPushMatrix();
  //   glTranslatef(-27.5, 35.0, 32.0);
  //   glRotatef(75.0, 0.0, 1.0, 0.0);
  //   glTranslatef(7.8000002, 0.0, 0.0);
  //   glRotatef(10.0, 1.0, 0.0, 0.0);
  //   yl = -g_recoil_conga_low;
  //   glTranslatef(0.0, yl, 0.0);
  //   glPushMatrix();
  //   Ms3dBundle::RenderModel(g_drumSet_conga_ms3d);
  //   glPopMatrix();
  //   glRotatef(-18.75, 0.0, 1.0, 0.0);
  //   glTranslatef(0.0, 0.0, -6.0);
  //   UpdatePercussionStriker(LOW_CONGA, g_recoil_conga_low, 0.0, handRight_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // // CLAP
  // if ( IsShowPercussionInstrument(HAND_CLAP, g_recoil_clap_hand_r) )
  // {
  //   glPushMatrix();
  //   glTranslatef(0.0, 50.5, 25.0);
  //   glRotatef(-30.0, 1.0, 0.0, 0.0);
  //   glRotatef(90.0, 1.0, 0.0, 0.0);
  //   glRotatef(90.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(HAND_CLAP, g_recoil_clap_hand_l, 0.0, handLeft_ms3d, 0, 0.0);
  //   glPopMatrix();
  //   glPushMatrix();
  //   glTranslatef(0.0, 45.5, 20.0);
  //   glRotatef(15.0, 1.0, 0.0, 0.0);
  //   glRotatef(-90.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(HAND_CLAP, g_recoil_clap_hand_r, 0.0, handRight_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // // TAMBOURINE
  // if ( IsShowPercussionInstrument(TAMBOURINE, g_recoil_tambourine_hand) )
  // {
  //   glPushMatrix();
  //   glTranslatef(10.0, 50.5, 25.0);
  //   glRotatef(-30.0, 1.0, 0.0, 0.0);
  //   glRotatef(90.0, 1.0, 0.0, 0.0);
  //   glRotatef(90.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(TAMBOURINE, g_recoil_tambourine, 0.0, handTambourine_ms3d, 0, 0.0);
  //   glPopMatrix();
  //   glPushMatrix();
  //   glTranslatef(10.0, 45.5, 20.0);
  //   glRotatef(15.0, 1.0, 0.0, 0.0);
  //   glRotatef(-90.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(TAMBOURINE, g_recoil_tambourine_hand, 0.0, handRight_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // // CLAVES
  // if ( IsShowPercussionInstrument(CLAVES, g_pianokey_translation_x) )
  // {
  //   glPushMatrix();
  //   glTranslatef(-10.0, 50.5, 25.0);
  //   glRotatef(-30.0, 1.0, 0.0, 0.0);
  //   glRotatef(90.0, 1.0, 0.0, 0.0);
  //   glRotatef(90.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(CLAVES, g_recoil_clave_l, 0.0, clave_ms3d, 1, 0.0);
  //   glPopMatrix();
  //   glPushMatrix();
  //   glTranslatef(-10.0, 45.5, 20.0);
  //   glRotatef(15.0, 1.0, 0.0, 0.0);
  //   glRotatef(-90.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(CLAVES, g_pianokey_translation_x, 0.0, clave_ms3d, 1, 0.0);
  //   glPopMatrix();
  // }
  // //  -- STICKS --
  // if ( IsShowPercussionInstrument(STICKS, g_recoil_sticks_2) )
  // {
  //   glPushMatrix();
  //   glTranslatef(-5.0, 40.5, 5.0);
  //   glRotatef(-30.0, 1.0, 0.0, 0.0);
  //   glRotatef(90.0, 1.0, 0.0, 0.0);
  //   glRotatef(90.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(STICKS, g_recoil_sticks_1, 0.0, g_drumSet_Stick_ms3d, 1, 0.0);
  //   glPopMatrix();
  //   glPushMatrix();
  //   glTranslatef(-5.0, 35.5, 0.0);
  //   glRotatef(15.0, 1.0, 0.0, 0.0);
  //   glRotatef(-90.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(STICKS, g_recoil_sticks_2, 0.0, g_drumSet_Stick_ms3d, 1, 0.0);
  //   glPopMatrix();
  // }
  // //  -- SHAKER --
  // if ( IsShowPercussionInstrument(SHAKER, g_recoil_shaker) )
  // {
  //   glPushMatrix();
  //   glTranslatef(12.0, 42.5, 25.0);
  //   glRotatef(-25.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(SHAKER, g_recoil_shaker, 1.0, shaker_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // //  -- JINGLE BELL --
  // if ( IsShowPercussionInstrument(JINGLE_BELL, g_recoil_jingleBell) )
  // {
  //   glPushMatrix();
  //   glTranslatef(17.5, 52.5, 15.0);
  //   glRotatef(45.0, 1.0, 0.0, 0.0);
  //   glRotatef(-45.0, 0.0, 1.0, 0.0);
  //   UpdatePercussionStriker(JINGLE_BELL, g_recoil_jingleBell, 1.0, jingleBells_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // //  -- CASTANETS --
  // if ( IsShowPercussionInstrument(CASTANETS, g_recoil_castanets) )
  // {
  //   glPushMatrix();
  //   glTranslatef(27.5, 42.5, 15.0);
  //   glRotatef(-55.0, 0.0, 1.0, 0.0);
  //   UpdatePercussionStriker(CASTANETS, g_recoil_castanets, 1.0, castanets_ms3d, 0, 0.0);
  //   glPopMatrix();
  //   glPushMatrix();
  //   glTranslatef(27.5, 44.0, 15.0);
  //   glRotatef(-55.0, 0.0, 1.0, 0.0);
  //   glRotatef(180.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(CASTANETS, g_recoil_castanets, 1.0, castanets_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // //  -- HIGH Q --
  // if ( IsShowPercussionInstrument(HIGH_Q, g_recoil_highQ) )
  // {
  //   glPushMatrix();
  //   glTranslatef(-15.0, 40.5, 5.0);
  //   glRotatef(140.0, 0.0, 1.0, 0.0);
  //   if ( UpdatePercussionStriker(HIGH_Q, g_recoil_highQ, 0.0, g_zapper_ms3d, 0, 0.0) )
  //     Ms3dBundle::RenderModel(g_zapperLaser_ms3d);
  //   glPopMatrix();
  // }
  // //  -- METRONOME --
  // if ( IsShowPercussionInstrument(METRONOME_CLICK, g_recoil_metronomeClick)
  //   || IsShowPercussionInstrument(METRONOME_BELL, g_recoil_metronomeBell) )
  // {
  //   glPushMatrix();
  //   glTranslatef(-20.0, 0.0, 35.0);
  //   glRotatef(23.0, 0.0, 1.0, 0.0);
  //   Ms3dBundle::RenderModel(metronome_ms3d);
  //   // First pendulum (METRONOME CLICK)
  //   glPushMatrix();
  //   glTranslatef(0.0, 1.0, 1.0);
  //   clickOscillation = CalculatePendulumOscillation(METRONOME_CLICK, g_recoil_metronomeClick);
  //   clickSwingAmount = 1.0 - fAbs(clickOscillation);
  //   clickSwingDelta = clickSwingAmount - (1.0 - fAbs(g_prevClickOscillation));
  //   if ( g_prevClickSwingDelta <= 0.0 && clickSwingDelta > 0.0 )
  //     g_clickPendulumDirection = g_clickPendulumDirection == 0;
  //   g_prevClickSwingDelta = clickSwingDelta;
  //   g_prevClickOscillation = clickOscillation;
  //   if ( g_clickPendulumDirection )
  //     anglec = -23.0 * clickSwingAmount;
  //   else
  //     anglec = 23.0 * clickSwingAmount;
  //   glRotatef(anglec, 0.0, 0.0, 1.0);
  //   Ms3dBundle::RenderModelShiny(metronomePendjulum1_ms3d);
  //   glPopMatrix();
  //   // Second pendulum (METRONOME BELL)
  //   glPushMatrix();
  //   glTranslatef(0.0, 1.0, 0.375);
  //   bellOscillation = CalculatePendulumOscillation(METRONOME_BELL, g_recoil_metronomeBell);
  //   bellSwingAmount = 1.0 - fAbs(bellOscillation);
  //   bellSwingDelta = bellSwingAmount - (1.0 - fAbs(g_prevBellOscillation));
  //   if ( g_prevBellSwingDelta <= 0.0 && bellSwingDelta > 0.0 )
  //     g_bellPendulumDirection = g_bellPendulumDirection == 0;
  //   g_prevBellSwingDelta = bellSwingDelta;
  //   g_prevBellOscillation = bellOscillation;
  //   if ( g_bellPendulumDirection )
  //     angle = -23.0 * bellSwingAmount;
  //   else
  //     angle = 23.0 * bellSwingAmount;
  //   glRotatef(angle, 0.0, 0.0, 1.0);
  //   Ms3dBundle::RenderModelShiny(metronomePendjulum2_ms3d);
  //   glPopMatrix();
  //   glPopMatrix();
  // }
  // //  -- SQUARE CLICK --
  // if ( IsShowPercussionInstrument(SQUARE_CLICK, g_recoil_squareClick) )
  // {
  //   glPushMatrix();
  //   glTranslatef(-35.0, 60.5, 5.0);
  //   glRotatef(-30.0, 1.0, 0.0, 0.0);
  //   glRotatef(90.0, 1.0, 0.0, 0.0);
  //   glRotatef(90.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(SQUARE_CLICK, g_recoil_squareClick, 0.0, g_squareShaker_ms3d, 1, 0.0);
  //   glPopMatrix();
  //   glPushMatrix();
  //   glTranslatef(-35.0, 55.5, 0.0);
  //   glRotatef(15.0, 1.0, 0.0, 0.0);
  //   glRotatef(-90.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(SQUARE_CLICK, g_recoil_squareClick, 0.0, g_drumSet_Stick_ms3d, 1, 0.0);
  //   glPopMatrix();
  // }
  // //  -- MARACAS --
  // if ( IsShowPercussionInstrument(MARACAS, g_recoil_Maracas) )
  // {
  //   glPushMatrix();
  //   glTranslatef(-12.0, 62.5, 25.0);
  //   glRotatef(15.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(MARACAS, g_recoil_Maracas, 1.0, g_maraca_ms3d, 0, 0.0);
  //   glPopMatrix();
  //   glPushMatrix();
  //   glTranslatef(-8.0, 60.5, 20.0);
  //   glRotatef(-15.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(MARACAS, g_recoil_Maracas, 1.0, g_maraca_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // //  -- CABASA --
  // if ( IsShowPercussionInstrument(CABASA, g_recoil_cabasa) )
  // {
  //   glPushMatrix();
  //   glTranslatef(-10.0, 45.5, 10.0);
  //   glRotatef(40.0, 0.0, 0.0, 1.0);
  //   za = g_recoil_cabasa * 50.0;
  //   UpdatePercussionStriker(CABASA, g_recoil_cabasa, 1.0, g_cabasa_ms3d, 0, za);
  //   glPopMatrix();
  // }
  // //  -- COWBELL --
  // if ( IsShowPercussionInstrument(COWBELL, g_recoil_cowbell) )
  // {
  //   glPushMatrix();
  //   y_cowbell = 40.5 - g_recoil_cowbell;
  //   glTranslatef(-9.5, y_cowbell, -7.0);
  //   glRotatef(25.0, 0.0, 1.0, 0.0);
  //   glRotatef(20.0, 1.0, 0.0, 0.0);
  //   Ms3dBundle::RenderModelShiny(g_cowbell_ms3d);
  //   UpdatePercussionStriker(COWBELL, g_recoil_cowbell, 3.0, g_drumSet_Stick_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // //  -- AGOGOS --
  // if ( IsShowPercussionInstrument(HIGH_AGOGO, g_recoil_agogo_hi)
  //   || IsShowPercussionInstrument(LOW_AGOGO, g_recoil_agogo_lo) )
  // {
  //   glPushMatrix();
  //   if ( g_recoil_agogo_hi <= g_recoil_agogo_lo )
  //     ya = 50.5 - g_recoil_agogo_lo;
  //   else
  //     ya = 50.5 - g_recoil_agogo_hi;
  //   glTranslatef(-5.5, ya, -8.0);
  //   Ms3dBundle::RenderModel(g_agogo_ms3d);
  //   glTranslatef(3.4000001, -2.5, -2.25);
  //   UpdatePercussionStriker(HIGH_AGOGO, g_recoil_agogo_hi, 3.0, g_drumSet_Stick_ms3d, 0, 0.0);
  //   glTranslatef(4.5, 0.5, -1.25);
  //   UpdatePercussionStriker(LOW_AGOGO, g_recoil_agogo_lo, 3.0, g_drumSet_Stick_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // //  -- WOODBLOCK, HI --
  // if ( IsShowPercussionInstrument(HIGH_WOODBLOCK, g_recoil_woodblock_hi) )
  // {
  //   glPushMatrix();
  //   yn = 40.5 - g_recoil_woodblock_hi;
  //   glTranslatef(-5.5, yn, -8.0);
  //   glRotatef(15.5, 0.0, 1.0, 0.0);
  //   glRotatef(12.5, 1.0, 0.0, 0.0);
  //   Ms3dBundle::RenderModel(g_woodBlockHigh_ms3d);
  //   UpdatePercussionStriker(HIGH_WOODBLOCK, g_recoil_woodblock_hi, 3.0, g_drumSet_Stick_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // //  -- WOODBLOCK, LO --
  // if ( IsShowPercussionInstrument(LOW_WOODBLOCK, g_recoil_woodblock_lo) )
  // {
  //   glPushMatrix();
  //   yo = 40.5 - g_recoil_woodblock_lo;
  //   glTranslatef(-0.5, yo, -9.0);
  //   glRotatef(3.0, 0.0, 1.0, 0.0);
  //   glRotatef(12.5, 1.0, 0.0, 0.0);
  //   Ms3dBundle::RenderModel(g_woodBlockLow_ms3d);
  //   UpdatePercussionStriker(LOW_WOODBLOCK, g_recoil_woodblock_lo, 3.0, g_drumSet_Stick_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // // -- TRIANGLE, OPEN --
  // if ( IsShowPercussionInstrument(OPEN_TRIANGLE, g_recoil_triangle_open) )
  // {
  //   glPushMatrix();
  //   yp = 50.5 - g_recoil_triangle_open;
  //   xb = -5.5 - g_recoil_triangle_open;
  //   glTranslatef(xb, yp, 10.0);
  //   glRotatef(15.5, 0.0, 1.0, 0.0);
  //   glRotatef(12.5, 1.0, 0.0, 0.0);
  //   Ms3dBundle::RenderModel(g_triangle_ms3d);
  //   glTranslatef(1.05, 2.5899999, -7.0);
  //   glRotatef(-45.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(OPEN_TRIANGLE, g_recoil_triangle_open, 0.0, g_triangleStick_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // //  -- TRIANGLE, MUTED --
  // if ( IsShowPercussionInstrument(MUTE_TRIANGLE, g_recoil_triangle_muted) )
  // {
  //   glPushMatrix();
  //   yq = 50.5 - g_recoil_triangle_muted;
  //   xc = 5.5 - g_recoil_triangle_muted;
  //   glTranslatef(xc, yq, 10.0);
  //   glRotatef(15.5, 0.0, 1.0, 0.0);
  //   glRotatef(12.5, 1.0, 0.0, 0.0);
  //   Ms3dBundle::RenderModel(g_mutedTriangle_ms3d);
  //   glTranslatef(1.05, 2.5899999, -7.0);
  //   glRotatef(-45.0, 0.0, 0.0, 1.0);
  //   UpdatePercussionStriker(MUTE_TRIANGLE, g_recoil_triangle_muted, 0.0, g_triangleStick_ms3d, 0, 0.0);
  //   glPopMatrix();
  // }
  // if ( g_hihat_isOpen == 1 )
  // {
  //   glPushMatrix();
  //   glTranslatef(-20.5, 23.5, 7.3540001);
  //   glRotatef(80.0, 0.0, 1.0, 0.0);
  //   glPushMatrix();
  //   glRotatef(180.0, 1.0, 0.0, 0.0);
  //   yr = g_recoil_hihat * 0.25;
  //   glTranslatef(0.0, yr, 0.0);
  //   glScalef(1.3, 1.3, 1.3);
  //   Ms3dBundle::RenderModelShiny(g_drumSet_cymbal_ms3d);
  //   glPopMatrix();
  //   ys = MAX_RECOIL * 0.25 + 1.4949999 - g_recoil_hihat * 0.25;
  //   glTranslatef(0.0, ys, 0.0);
  //   I_HiHat();
  //   angled = flt_461DB4 * 9.0 / MAX_RECOIL;
  //   glRotatef(angled, 1.0, 0.0, 0.0);
  // }
  // else
  // {
  //   glPushMatrix();
  //   yt = 23.5 - g_recoil_hihat * 0.5;
  //   glTranslatef(-20.5, yt, 7.3540001);
  //   glRotatef(80.0, 0.0, 1.0, 0.0);
  //   glPushMatrix();
  //   glRotatef(180.0, 1.0, 0.0, 0.0);
  //   glScalef(1.3, 1.3, 1.3);
  //   Ms3dBundle::RenderModelShiny(g_drumSet_cymbal_ms3d);
  //   glPopMatrix();
  //   glTranslatef(0.0, 1.4949999, 0.0);
  //   I_HiHat();
  // }
  // glScalef(1.3, 1.3, 1.3);
  // Ms3dBundle::RenderModelShiny(g_drumSet_cymbal_ms3d);// bottom hi-hat cymbal
  // glPopMatrix();
  // // CYMBALS
  // for ( j = 0; j < 6; ++j )
  // {
  //   glPushMatrix();
  //   glTranslatef(CYMBAL_LOC_X[j], CYMBAL_LOC_Y[j], CYMBAL_LOC_Z[j]);
  //   glRotatef(g_cymbal_rot_y[j], 0.0, 1.0, 0.0);
  //   I_Cymbals(j);
  //   wobble = g_cymbalRestingAngle[j] + g_cymbalCurrentWobble[j];
  //   glRotatef(wobble, 1.0, 0.0, 0.0);
  //   glScalef(CYMBAL_SCALE[j], CYMBAL_SCALE[j], CYMBAL_SCALE[j]);
  //   if ( j == 5 )                               // 5 = china
  //     Ms3dBundle::RenderModelShiny(g_drumSet_chinaCymbal_ms3d);
  //   else
  //     Ms3dBundle::RenderModelShiny(g_drumSet_cymbal_ms3d);
  //   glPopMatrix();
  // }
}
