//
// Created by Jacob on 11/16/2025.
//

#ifndef MIDIJAM_GLOBALS_H
#define MIDIJAM_GLOBALS_H
#include <cstdio>
#include <windows.h>
#include <GL/GL.h>

#include "dmusici.h"
#include "instrument/instrument.h"
#include "instrument/Piano.h"

class Ms3dBundle;

// State
extern int g_killApplication_0;
extern int g_framesSinceStart;
extern __int16 g_isFadingIn;
extern __int16 g_isShuttingDown;
extern __int16 g_killApplication;
extern int g_lastUnixEpochTime;
extern UINT g_timerEventId;
extern char g_midiFileName[1000];
extern int g_time_global_current;
extern __int16 g_framesAlive;
extern REFERENCE_TIME g_prtStart;
extern MUSIC_TIME g_mtStart;
extern MidiJamInstrumentId g_midiJamInstrumentIds[300];
extern double g_currentTempo;
extern double g_currentTempo_scaleFactor0_5 ;
extern double g_currentTempo_scaleFactor0_9 ;
extern double g_currentTempo_scaleFactor1_15;

// HWF
extern FILE *g_hwfStream;
extern char *g_pHwfAppendix;
extern unsigned int g_nHwfAppendixItems;

// World
extern float RECOIL_SCALE_FACTOR;
extern GLfloat g_pianokey_translation_x[14];

// Models
extern Ms3dBundle *g_accordionFold_ms3d;
extern Ms3dBundle *g_accordionKeyBlack_Down_ms3d;
extern Ms3dBundle *g_accordionKeyBlack_ms3d;
extern Ms3dBundle *g_accordionKeyWhiteBack_Down_ms3d;
extern Ms3dBundle *g_accordionKeyWhiteBack_ms3d;
extern Ms3dBundle *g_accordionKeyWhiteFront_Down_ms3d;
extern Ms3dBundle *g_accordionKeyWhiteFront_ms3d;
extern Ms3dBundle *g_accordionLeftHand_ms3d;
extern Ms3dBundle *g_accordionRightHand_ms3d;
extern Ms3dBundle *g_agogoSingle_ms3d;
extern Ms3dBundle *g_agogo_ms3d;
extern Ms3dBundle *g_altoSaxBody_ms3d;
extern Ms3dBundle *g_altoSaxHorn_ms3d;
extern Ms3dBundle *g_altoSaxKeyX_ms3d;
extern Ms3dBundle *g_baritoneSaxBody_ms3d;
extern Ms3dBundle *g_baritoneSaxHorn_ms3d;
extern Ms3dBundle *g_bassNoteFinger_ms3d;
extern Ms3dBundle *g_bassStringBottomX_ms3d;
extern Ms3dBundle *g_bassString_ms3d;
extern Ms3dBundle *g_cabasa_ms3d;
extern Ms3dBundle *g_calliope_ms3d;
extern Ms3dBundle *g_cello_ms3d;
extern Ms3dBundle *g_cowbell_ms3d;
extern Ms3dBundle *g_doubleBass_ms3d;
extern Ms3dBundle *g_drumSet_BassDrumBeaterArm_ms3d;
extern Ms3dBundle *g_drumSet_BassDrumBeaterHolder_ms3d;
extern Ms3dBundle *g_drumSet_BassDrumPedal_ms3d;
extern Ms3dBundle *g_drumSet_Stick_ms3d;
extern Ms3dBundle *g_drumSet_Timbale_ms3d;
extern Ms3dBundle *g_drumSet_bassDrum_ms3d;
extern Ms3dBundle *g_drumSet_bongo_ms3d;
extern Ms3dBundle *g_drumSet_chinaCymbal_ms3d;
extern Ms3dBundle *g_drumSet_conga_ms3d;
extern Ms3dBundle *g_drumSet_cymbal_ms3d;
extern Ms3dBundle *g_drumSet_snareDrum_ms3d;
extern Ms3dBundle *g_drumSet_tom_ms3d;
extern Ms3dBundle *g_flute_leftHandX_ms3d;
extern Ms3dBundle *g_flute_ms3d;
extern Ms3dBundle *g_flute_rightHandX_ms3d;
extern Ms3dBundle *g_frenchHornBody_ms3d;
extern Ms3dBundle *g_frenchHornHorn_ms3d;
extern Ms3dBundle *g_frenchHornKey1_ms3d;
extern Ms3dBundle *g_frenchHornKey2_ms3d;
extern Ms3dBundle *g_frenchHornKey3_ms3d;
extern Ms3dBundle *g_frenchHornTrigger_ms3d;
extern Ms3dBundle *g_harmonica_ms3d;
extern Ms3dBundle *g_harpString_ms3d;
extern Ms3dBundle *g_harpString_white_ms3d;
extern Ms3dBundle *g_harp_ms3d;
extern Ms3dBundle *g_malletHitShadow_ms3d;
extern Ms3dBundle *g_maraca_ms3d;
extern Ms3dBundle *g_melodicTom_ms3d;
extern Ms3dBundle *g_musicBoxCase_ms3d;
extern Ms3dBundle *g_musicBoxKey_ms3d;
extern Ms3dBundle *g_musicBoxPoint_ms3d;
extern Ms3dBundle *g_musicBoxSpindle_ms3d;
extern Ms3dBundle *g_musicBoxTopBlade_ms3d;
extern Ms3dBundle *g_mutedTriangle_ms3d;
extern Ms3dBundle *g_ocarinaHandX_ms3d;
extern Ms3dBundle *g_ocarina_ms3d;
extern Ms3dBundle *g_panPipe_ms3d;
extern Ms3dBundle **g_pianoCase_X_ms3d;  // Array of 28 pointers
extern Ms3dBundle *g_pianoKeyBlackDown_X_ms3d;
extern Ms3dBundle *g_pianoKeyBlack_X_ms3d;
extern Ms3dBundle **g_pianoKeyWhiteBackDown_X_ms3d;  // Array of 28 pointers
extern Ms3dBundle **g_pianoKeyWhiteBack_X_ms3d;  // Array of 28 pointers
extern Ms3dBundle **g_pianoKeyWhiteFrontDown_X_ms3d;  // Array of 28 pointers
extern Ms3dBundle *g_pianoKeyWhiteFront_X_ms3d;
extern Ms3dBundle *g_pianoShadow_ms3d;
extern Ms3dBundle *g_pianoStand_ms3d;
extern Ms3dBundle *g_piccolo_ms3d;
extern Ms3dBundle *g_pizzicatoStringHolder_ms3d;
extern Ms3dBundle *g_popBottleMiddle_ms3d;
extern Ms3dBundle *g_popBottlePop_ms3d;
extern Ms3dBundle *g_popBottle_ms3d;
extern Ms3dBundle *g_recorderLeftHandX_ms3d;
extern Ms3dBundle *g_recorder_ms3d;
extern Ms3dBundle *g_sapranoSaxBody_ms3d;
extern Ms3dBundle *g_screenGradient_ms3d;
extern Ms3dBundle *g_songFillbarBox_ms3d;
extern Ms3dBundle *g_songFillbar_ms3d;
extern Ms3dBundle *g_squareShaker_ms3d;
extern Ms3dBundle *g_stageChoir_ms3d;
extern Ms3dBundle *g_stageHorn_ms3d;
extern Ms3dBundle *g_stageStringBottomX_ms3d;
extern Ms3dBundle *g_stageStringBow_ms3d;
extern Ms3dBundle *g_stageStringHolder_ms3d;
extern Ms3dBundle *g_stageString_ms3d;
extern Ms3dBundle *g_stage_ms3d;
extern Ms3dBundle *g_steamPuff_harmonica_ms3d;
extern Ms3dBundle *g_steelDrumMallet_ms3d;
extern Ms3dBundle *g_synthDrum_ms3d;
extern Ms3dBundle *g_taikoStick_ms3d;
extern Ms3dBundle *g_taiko_ms3d;
extern Ms3dBundle *g_telephoneBase_ms3d;
extern Ms3dBundle *g_telephoneHandle_ms3d;
extern Ms3dBundle *g_telephoneKeyX_ms3d;
extern Ms3dBundle *g_tenorSaxBody_ms3d;
extern Ms3dBundle *g_tenorSaxHorn_ms3d;
extern Ms3dBundle *g_timpaniBody_ms3d;
extern Ms3dBundle *g_timpaniHead_ms3d;
extern Ms3dBundle *g_triangleStick_ms3d;
extern Ms3dBundle *g_triangle_ms3d;
extern Ms3dBundle *g_tromboneSlide_ms3d;
extern Ms3dBundle *g_trombone_ms3d;
extern Ms3dBundle *g_trumpetBody_ms3d;
extern Ms3dBundle *g_trumpetHorn_ms3d;
extern Ms3dBundle *g_trumpetKey1_ms3d;
extern Ms3dBundle *g_trumpetKey2_ms3d;
extern Ms3dBundle *g_trumpetKey3_ms3d;
extern Ms3dBundle *g_trumpetMute_ms3d;
extern Ms3dBundle *g_tubaBody_ms3d;
extern Ms3dBundle *g_tubaHorn_ms3d;
extern Ms3dBundle *g_tubaKey_1_ms3d;
extern Ms3dBundle *g_tubaKey_2_ms3d;
extern Ms3dBundle *g_tubaKey_3_ms3d;
extern Ms3dBundle *g_tubaKey_4_ms3d;
extern Ms3dBundle *g_tubularBellDark_ms3d;
extern Ms3dBundle *g_tubularBellMallet_ms3d;
extern Ms3dBundle *g_tubularBell_ms3d;
extern Ms3dBundle *g_viola_ms3d;
extern Ms3dBundle *g_violinBow_ms3d;
extern Ms3dBundle *g_violinFinger_ms3d;
extern Ms3dBundle *g_violinStringPlayedX_ms3d;
extern Ms3dBundle *g_violinString_ms3d;
extern Ms3dBundle *g_violin_ms3d;
extern Ms3dBundle *g_woodBlockHigh_ms3d;
extern Ms3dBundle *g_woodBlockLow_ms3d;
extern Ms3dBundle *g_xylophoneBlackBarDown_glockenspiel_ms3d;
extern Ms3dBundle *g_xylophoneBlackBarDown_marimba_ms3d;
extern Ms3dBundle *g_xylophoneBlackBarDown_ms3d;
extern Ms3dBundle *g_xylophoneBlackBarDown_vibes_ms3d;
extern Ms3dBundle *g_xylophoneBlackBar_glockenspiel_ms3d;
extern Ms3dBundle *g_xylophoneBlackBar_marimba_ms3d;
extern Ms3dBundle *g_xylophoneBlackBar_ms3d;
extern Ms3dBundle *g_xylophoneBlackBar_vibes_ms3d;
extern Ms3dBundle *g_xylophoneCase_ms3d;
extern Ms3dBundle *g_xylophoneLegs_ms3d;
extern Ms3dBundle *g_xylophoneMalletWhite_ms3d;
extern Ms3dBundle *g_xylophoneWhiteBarDown_glockenspiel_ms3d;
extern Ms3dBundle *g_xylophoneWhiteBarDown_marimba_ms3d;
extern Ms3dBundle *g_xylophoneWhiteBarDown_ms3d;
extern Ms3dBundle *g_xylophoneWhiteBarDown_vibes_ms3d;
extern Ms3dBundle *g_xylophoneWhiteBar_glockenspiel_ms3d;
extern Ms3dBundle *g_xylophoneWhiteBar_marimba_ms3d;
extern Ms3dBundle *g_xylophoneWhiteBar_ms3d;
extern Ms3dBundle *g_xylophoneWhiteBar_vibes_ms3d;
extern Ms3dBundle *g_zapperLaser_ms3d;
extern Ms3dBundle *g_zapper_ms3d;

extern PianoModels **g_pianoModels_ms3d_arr;

// Camera


// Window
extern int g_windowCenterX;
extern int g_windowCenterY;

// Domain
extern char g_midiFileNameDisp[1000];
extern double g_framesPerSecond;
extern CHAR g_workingDirectory[1000];
extern int g_applicationStartTime;

// Fillbar
extern GLfloat g_songFillbarScale;

// Cymbals
extern GLfloat CYMBAL_LOC_X[7];
extern GLfloat CYMBAL_LOC_Y[7];
extern GLfloat CYMBAL_LOC_Z[7];

// Stringed instruments
extern __int16 VIBRATING_STRING_ANIM_SEQUENCE[8];
extern __int16 g_vibratingString_frame;
extern __int16 g_vibratingString_frameIndex;

// Misc.
extern __int16 g_latinSquare[36];
extern float g_pianoXylophoneOffset;
extern GLfloat g_cymbal_rot_y[7];
extern float g_cymbalRestingAngle[7];

// Unknown
extern GLfloat dword_45EBDC[14];
extern __int16 word_45EB30[88];
extern DWORD dword_46E6A8;
extern __int16 word_46B5D8[92];
extern __int16 word_46B5DA[92];
extern __int16 word_46B5DC[92];
extern __int16 word_46B5DE[92];
extern float flt_468BF8[23];
extern float flt_45EA70[23];
extern __int16 word_46CEE0[138];
extern __int16 word_46CEE2[138];
extern __int16 word_46CEE4[138];
extern __int16 word_46CEE6[138];
extern __int16 word_46CEE8[138];
extern __int16 word_46CEEA[138];
extern float flt_4654A0[23];
extern float flt_45EAD0[24];
extern __int16 word_46B2D0[72];
extern __int16 word_46B2D2[72];
extern __int16 word_46B2D4[72];
extern __int16 word_46B2D6[72];
extern __int16 word_4688C0[72];
extern __int16 word_4688C2[72];
extern __int16 word_4688C4[72];
extern __int16 word_4688C6[72];
extern __int16 word_46BBB0[112];
extern __int16 word_46BBB2[112];
extern __int16 word_46BBB4[112];
extern __int16 word_46BBB6[112];
extern __int16 word_468258[196];
extern __int16 word_46825A[196];
extern __int16 word_46825C[196];
extern __int16 word_46825E[196];
extern float flt_4679E0[49];
extern __int16 word_4688B0;
extern int dword_464BFC;
extern int dword_464F68;
extern int dword_464C00;
extern int dword_464F6C;
extern int dword_464C04;
extern int dword_464F70;
extern int dword_464C08;
extern int dword_464F74;
extern int dword_464C0C;
extern int dword_464F78;
extern int dword_464C10;
extern int dword_464F7C;
extern int dword_464C14;
extern int dword_464F80;
extern int dword_464C18;
extern int dword_464F84;
extern int dword_464C1C;
extern int dword_464F88;
extern int dword_464C20;
extern int dword_464F8C;
extern int dword_464C24;
extern int dword_464F90;
extern int dword_464C28;
extern int dword_464F94;
extern int dword_464C2C;
extern int dword_464F98;
extern __int16 word_46E268;


#endif //MIDIJAM_GLOBALS_H