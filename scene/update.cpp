//
// Created by Jacob on 3/18/2026.
//

#include "update.h"
#include "../macros.h"
#include "../model/Ms3dBundle.h"
#include "../audio/DirectMusicSegmentPlayer.h"
#include "../instruments/instrument_ids.h"
#include "../scene/fadeout.h"
#include "../render/text.h"
#include "../instruments/Piano.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include <cmath>

#include "camera.h"
#include "audio/playback.h"
#include "instruments/Accordion.h"
#include "instruments/Agogos.h"
#include "instruments/AltoSax.h"
#include "instruments/Bass.h"
#include "instruments/BaritoneSax.h"
#include "instruments/FrenchHorn.h"
#include "instruments/Guitar.h"
#include "instruments/Harp.h"
#include "instruments/MelodicTom.h"
#include "instruments/SapranoSax.h"
#include "instruments/StageChoir.h"
#include "instruments/StageHorn.h"
#include "instruments/StageString.h"
#include "instruments/SynthDrum.h"
#include "instruments/Taiko.h"
#include "instruments/TenorSax.h"
#include "instruments/Timpani.h"
#include "instruments/Xylophone.h"
#include "instruments/Trombone.h"
#include "instruments/Trumpet.h"
#include "instruments/Tuba.h"
#include "instruments/Woodblocks.h"

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------

extern int g_worldReady;
extern float g_cameraLocation[6];
extern short g_rotatingCameraIsActive;
extern float g_rotatingCameraAngle;
extern RotatingCameraDirection g_rotatingCameraDirection;
extern int g_rotatingCameraIdleTime;
extern Ms3dBundle* g_screenGradient_ms3d;
extern Ms3dBundle* g_stage_ms3d;
extern Ms3dBundle* g_pianoShadow_ms3d;
extern Ms3dBundle* g_bassShadow_ms3d;
extern Ms3dBundle* g_guitarShadow_ms3d;
extern Ms3dBundle* g_drumShadow_ms3d;
extern Ms3dBundle* g_songFillbarBox_ms3d;
extern Ms3dBundle* g_songFillbar_ms3d;
extern int g_show_percussion;
extern GLfloat g_fadeFactor;
extern GLfloat g_songFillbarScale;
extern char g_midiFileNameDisp[260];
extern float g_framesPerSecond;
extern IDirectMusicPerformance8* g_DirectMusicPerformance;
extern DirectMusicSegmentPlayer* g_directMusicSegmentPlayer;
extern MUSIC_TIME g_mtStart;
extern MUSIC_TIME g_midiFileDuration;
extern MUSIC_TIME g_currentGlobalTime;
extern int g_isShuttingDown;
extern int g_isFadingIn;
extern int g_killApplication;
extern int g_killApplication_0;
extern int g_isEvenFrame;
extern int g_vibratingString_frameIndex;
extern int g_vibratingString_frame;
extern int g_framesAlive;
extern int g_framesSinceStart;
extern DWORD g_applicationStartTime;
extern CameraAngle g_targetCameraAngle;
extern float g_autoCamDeltaTransform[6];
extern int g_autoCamIsIdle;
extern int g_autoCamIdleTime;
extern int g_autoCameraIsActive;

// Percussion globals
extern int g_percussion_time_queue[88][32];
extern int g_percussion_velocity_queue[88][32];
extern int g_percussion_framesWithEmptyQueue;
extern int g_hihat_isOpen;
extern int g_lastRideCymbal;
extern short g_cymbal_patches[7];
extern float g_cymbalRestingAngle[7];
extern float g_cymbal_rot_y[7];
extern float g_cymbalMassFactor[7];
extern float g_cymbalCurrentWobble[7];
extern float g_cymbalAngularVelocity[7];
extern float g_cymbalWobbleAmplitude[7];
extern float g_recoil_cymbals[7];
extern float CYMBAL_MASS_FACTOR[7];
extern float CYMBAL_SCALE[7];
extern float CYMBAL_MAX_WOBBLE[7];
extern float CYMBAL_WOBBLE_AMPLITUDE[7];
extern float CYMBAL_ANGULAR_VELOCITY_FACTOR[7];

// Recoil globals
extern float g_recoil_snaredrum;
extern float g_recoil_snare;
extern float g_recoil_sideStick;
extern float g_recoil_tom[6];
extern float g_recoil_bassdrum;
extern float g_recoil_bassDrumArm;
extern float g_recoil_cowbell;
extern float g_recoil_clap_hand_r;
extern float g_recoil_clap_hand_l;
extern float g_recoil_tambourine_hand;
extern float g_recoil_tambourine;
extern float g_recoil_sticks_1;
extern float g_pianoKeyOffsetX[14]; // doubles as recoil_clave_r
extern float g_recoil_clave_l;
extern float g_recoil_jingleBell;
extern float g_recoil_castanets;
extern float g_recoil_shaker;
extern float g_recoil_highQ;
extern float g_recoil_squareClick;
extern float g_recoil_metronomeClick;
extern float g_recoil_metronomeBell;
extern float g_recoil_Maracas;
extern float g_recoil_cabasa;
extern float g_recoil_agogo_hi;
extern float g_recoil_agogo_lo;
extern float g_recoil_woodblock_hi;
extern float g_recoil_woodblock_lo;
extern float g_recoil_triangle_open;
extern float g_recoil_triangle_muted;
extern float g_recoil_whistle_long;
extern float g_recoil_whistle_short;
extern float g_recoil_timbale_hi;
extern float g_recoil_timbale_lo;
extern float g_recoil_bongo_hi;
extern float g_recoil_bongo_lo;
extern float g_recoil_conga_high_mute;
extern float g_recoil_conga_high_open;
extern float g_recoil_conga_low;
extern float g_recoil_hihat;

extern float MAX_RECOIL;
extern float RECOIL_SCALE_FACTOR;

// Vibrating string animation
extern int VIBRATING_STRING_ANIM_SEQUENCE[8];

// Camera
extern CameraPosition CAMERA_POSITIONS[];
extern float g_normalizedWindowScale;

// Key state
extern char g_keyStateArray[256];
extern char g_shiftPressed;
extern char g_isSpacePressed;
extern char g_isAKeyPressed;
extern char g_framesKeyRightPressed;
extern char g_framesKeyLeftPressed;
extern char g_framesKeyUpPressed;
extern char g_framesKeyDownPressed;
extern float g_playbackSpeed;
extern GUID GUID_PERF_MASTER_TEMPO;
extern char g_mouseWheelMoved;
extern short g_mouseWheelDelta;

// Forward declarations for instrument render/update functions — stubbed until transcribed
void RenderPiano();
bool UpdatePiano(MUSIC_TIME pmtNow);

static inline void RenderPercussion() {
}

static inline void UpdateSteamPuffers() {
}

void UpdateAllRecoils(const float scale);

// FUNCTION: MIDIJAM 0x402A00
BOOL UpdateMidiJam() {
    GLfloat shadowX;
    GLfloat shadowZ;
    short m, k, j, i;
    GLfloat z;
    char headsUpDisplayText[104];

    GLfloat x_bassShadow; // [esp+3Ch] [ebp-A0h]
    GLfloat x_guitarShadow; // [esp+3Ch] [ebp-A0h]
    GLfloat x_harpShadow; // [esp+3Ch] [ebp-A0h]
    GLfloat z_bassShadow; // [esp+44h] [ebp-98h]
    GLfloat z_guitarShadow; // [esp+44h] [ebp-98h]

    if (g_worldReady == 1) {
        glClear(0x4500u);
        glLoadIdentity();
        // -- SCREEN GRADIENT --
        glDisable(GL_DEPTH_TEST);
        g_screenGradient_ms3d->RenderModel();
        glEnable(GL_DEPTH_TEST);
        // -- APPLY CAMERA TRANSFORM --
        gluLookAt(
            g_cameraLocation[0],
            g_cameraLocation[1],
            g_cameraLocation[2],
            g_cameraLocation[3],
            g_cameraLocation[4],
            g_cameraLocation[5],
            0.0,
            1.0,
            0.0
        );

        if (g_rotatingCameraIsActive == 1) {
            glRotatef(g_rotatingCameraAngle, 0.0, 1.0, 0.0);
            if (g_rotatingCameraDirection) {
                g_rotatingCameraAngle = g_rotatingCameraAngle - 0.025;
                if (g_rotatingCameraAngle < -20.0) {
                    g_rotatingCameraAngle = -20.0;
                    if (++g_rotatingCameraIdleTime > 500) {
                        g_rotatingCameraIdleTime = 0;
                        g_rotatingCameraDirection = CLOCKWISE;
                    }
                }
            }
            else {
                g_rotatingCameraAngle = g_rotatingCameraAngle + 0.025;
                if (g_rotatingCameraAngle > 20.0) {
                    g_rotatingCameraAngle = 20.0;
                    if (++g_rotatingCameraIdleTime > 500) {
                        g_rotatingCameraIdleTime = 0;
                        g_rotatingCameraDirection = ANTI_CLOCKWISE;
                    }
                }
            }
        }

        // -- STAGE --
        glPushMatrix();
        glTranslatef(0.0, -32.0, -14.0);
        g_stage_ms3d->RenderModel();
        glPopMatrix();

        // -- PIANO SHADOW --
        if (g_pianoVisible > 0) {
            glPushMatrix();
            glTranslatef(-50.0, -32.0, -20.0);
            glRotatef(45.0, 0.0, 1.0, 0.0);
            glTranslatef(0.0, 0.0, 5.0);
            if (g_pianoVisible > 1) {
                z = static_cast<double>(g_pianoVisible - 1) * 0.5 + 1.0;
                glScalef(1.0, 1.0, z);
            }
            g_pianoShadow_ms3d->RenderModel();
            glPopMatrix();
        }

        // -- XYLOPHONE SHADOW --
        if (g_xylophoneVisible > 0) {
            glPushMatrix();
            glTranslatef(-22.0, -32.0, 10.0);
            glRotatef(32.0, 0.0, 1.0, 0.0);
            glScalef(0.64999998, 0.64999998, 0.64999998);
            for (i = 0; i < g_xylophoneVisible; ++i) {
                if (i > 0) {
                    glTranslatef(-2.0, 0.0, 0.0);
                    glRotatef(-18.0, 0.0, 1.0, 0.0);
                    glTranslatef(0.0, 3.0, -23.0);
                }
                g_xylophoneShadow_ms3d->RenderModel();
            }
            glPopMatrix();
        }

        // -- BASS SHADOW --
        if (g_bassVisible > 0) {
            for (j = 0; j < g_bassVisible; ++j) {
                glPushMatrix();
                z_bassShadow = (double)j * -5.0 + -25.0;
                x_bassShadow = (double)j * 5.0 + 50.0;
                glTranslatef(x_bassShadow, -32.0, z_bassShadow);
                glRotatef(-45.0, 0.0, 1.0, 0.0);
                g_bassShadow_ms3d->RenderModel();
                glPopMatrix();
            }
        }

        // -- GUITAR SHADOW --
        if (g_guitarVisible > 0) {
            for (k = 0; k < g_guitarVisible; ++k) {
                glPushMatrix();
                z_guitarShadow = (double)k * -5.0 + -5.0;
                x_guitarShadow = (double)k * 5.0 + 43.0;
                glTranslatef(x_guitarShadow, -32.0, z_guitarShadow);
                glRotatef(-50.0, 0.0, 1.0, 0.0);
                g_guitarShadow_ms3d->RenderModel();
                glPopMatrix();
            }
        }

        // // -- DRUM SHADOW --
        // if ( g_inst_visible_drumset > 0 )
        // {
        //     glPushMatrix();
        //     glTranslatef(0.0, -32.0, -95.0);
        //     Ms3dBundle::RenderModel(g_drumShadow_ms3d);
        //     glPopMatrix();
        // }

        // -- HARP SHADOW --
        if (g_harpVisible > 0) {
            for (m = 0; m < g_harpVisible; ++m) {
                glPushMatrix();
                x_harpShadow = static_cast<double>(m) * 16.0 + 7.0;
                glTranslatef(x_harpShadow, -32.0, 5.0);
                glRotatef(-33.0, 0.0, 1.0, 0.0);
                g_harpShadow_ms3d->RenderModel();
                glPopMatrix();
            }
        }

        // --- Phase 5: Instruments ---
        if (g_harp) {
            glPushMatrix();
            glTranslatef(7.0, -28.4, 5.0);
            glRotatef(-33.0, 0.0, 1.0, 0.0);
            RenderHarp();
            glPopMatrix();
        }
        if (g_trombone) RenderTrombone();
        if (g_trumpet) RenderTrumpet();
        if (g_frenchHorn) RenderFrenchHorn();
        if (g_baritoneSax) RenderBaritoneSax();
        if (g_tenorSax) RenderTenorSax();
        if (g_sapranoSax) RenderSapranoSax();
        if (g_altoSax) RenderAltoSax();
        // if (g_recorder) RenderRecorder();
        // if (g_piccolo) RenderPiccolo();
        // if (g_flute) RenderFlute();
        if (g_tuba) RenderTuba();
        if (g_stageHorn) RenderStageHorn();
        // if (g_whistles) RenderWhistles();
        // if (g_panPipe) RenderPanPipe();
        // if (g_harmonica) RenderHarmonica();
        // if (g_violin) RenderViolin();
        // if (g_viola) RenderViola();
        // if (g_cello) RenderCello();
        // if (g_doubleBass) RenderDoubleBass();
        // if (g_popBottle) RenderPopBottle();
        if (g_agogos) RenderAgogos();
        if (g_woodblocks) RenderWoodblocks();
        if (g_stageChoir) RenderStageChoir();
        if (g_accordion) RenderAccordion();
        if (g_stageString) RenderStageString();
        // if (g_pizzicatoStrings) RenderPizzicatoStrings();
        // if (g_ocarina) RenderOcarina();

        if (g_xylophone) {
            glPushMatrix();
            glTranslatef(-22.0, -10.0, 10.0);
            glRotatef(32.0, 0.0, 1.0, 0.0);
            glScalef(0.64999998, 0.64999998, 0.64999998);
            RenderXylophone();
            glPopMatrix();
        }

        // if (g_musicBox) RenderMusicBox();

        if (g_piano) {
            glPushMatrix();
            glTranslatef(-50.0f, 0.0f, -20.0f);
            glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
            RenderPiano();
            glPopMatrix();
        }

        if (g_show_percussion == 1) {
            glPushMatrix();
            glTranslatef(0.0f, -32.0f, -95.0f);
            RenderPercussion();
            glPopMatrix();
        }

        if (g_melodicTom) RenderMelodicTom();
        if (g_synthDrum) RenderSynthDrum();
        // if (g_steelDrum) RenderSteelDrum();
        if (g_timpani) RenderTimpani();
        if (g_taiko) RenderTaiko();
        // if (g_telephone) RenderTelephone();
        // if (g_tubularBells) RenderTubularBells();

        if (g_guitar) {
            glPushMatrix();
            glTranslatef(43.0f, 4.0f, -5.0f);
            glRotatef(-50.0f, 0.0f, 1.0f, 0.0f);
            glRotatef(-60.0f, 0.0f, 0.0f, 1.0f);
            RenderGuitar();
            glPopMatrix();
        }

        if (g_bass) {
            glPushMatrix();
            glTranslatef(50.0f, 24.0f, -25.0f);
            glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
            glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
            RenderBass();
            glPopMatrix();
        }

        //  -- HEADS UP DISPLAY --
        sprintf(headsUpDisplayText, "%s fps:%0.2f", g_midiFileNameDisp, g_framesPerSecond);
        // -- SONG FILLBAR BOX --
        glPushMatrix();
        glLoadIdentity();
        g_songFillbarBox_ms3d->RenderModel();
        // -- SONG FILLBAR --
        glTranslatef(-122.087f, 0.0f, 0.0f);
        glScalef(g_songFillbarScale, 1.0f, 1.0f);
        g_songFillbar_ms3d->RenderModel();
        glPopMatrix();
        //  -- HUD TEXT --
        RenderTextWithShadow(75, 460, 3, 3, headsUpDisplayText, 1, 0.5, 0.5);
        if (g_fadeFactor != 0.0) {
            glPushMatrix();
            glLoadIdentity();
            gluLookAt(0.0, 0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            RenderFadeout();
            glPopMatrix();
        }
        glFlush();
    }
    return TRUE;
}

// ---------------------------------------------------------------------------
// FUNCTION: MIDIJAM 0x4221E0
// ---------------------------------------------------------------------------
void __stdcall UpdateMidiJamMM(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) {
    double v5; // st7
    int velocity; // [esp+24h] [ebp-44h]
    __int16 cymbalIndex; // [esp+30h] [ebp-38h]
    float velocityFactor; // [esp+34h] [ebp-34h]
    __int16 i; // [esp+38h] [ebp-30h]
    float currentTimeMs; // [esp+3Ch] [ebp-2Ch]
    REFERENCE_TIME prtNow; // [esp+40h] [ebp-28h] BYREF
    __int16 anyPercussionActive; // [esp+4Ch] [ebp-1Ch]
    MUSIC_TIME pmtNow; // [esp+50h] [ebp-18h] BYREF
    DWORD globalTimeMs; // [esp+54h] [ebp-14h]
    __int16 patch; // [esp+58h] [ebp-10h]
    // [esp+5Ch] [ebp-Ch]
    __int16 anyInstrumentActive; // [esp+60h] [ebp-8h]
    __int16 cameraTransformComponentsAtTarget; // [esp+64h] [ebp-4h]

    if (g_worldReady == 1) // this function no-ops if world not ready
    {
        float recoil_scale_factor = RECOIL_SCALE_FACTOR * 0.25;
        prtNow = 0;
        pmtNow = 0;
        if (g_directMusicSegmentPlayer->IsPlaying()) {
            g_DirectMusicPerformance->GetTime(&prtNow, &pmtNow);
            if (g_songFillbarScale < 1.0) {
                g_songFillbarScale = static_cast<double>(pmtNow - g_mtStart) / static_cast<double>(g_midiFileDuration);
                if (g_songFillbarScale > 1.0)
                    g_songFillbarScale = 1.0; // limit scale to 100%
            }
        }
        if (++g_isEvenFrame > 1) {
            g_isEvenFrame = 0;
            if (++g_vibratingString_frameIndex >= 8)
                g_vibratingString_frameIndex = 0;
            g_vibratingString_frame = VIBRATING_STRING_ANIM_SEQUENCE[g_vibratingString_frameIndex];
        }
        if (!g_currentGlobalTime)
            g_currentGlobalTime = pmtNow;
        if (g_isShuttingDown == 1)
            pmtNow = g_currentGlobalTime;
        anyInstrumentActive = 0;
        UpdateAccordion(pmtNow);
        if (g_harp && UpdateHarp(pmtNow))
            anyInstrumentActive = 1;
        if (g_piano && UpdatePiano(pmtNow))
            anyInstrumentActive = 1;
        if (g_xylophone && UpdateXylophone(pmtNow))
            anyInstrumentActive = 1;
        //     if ( g_violin && UpdateViolin(pmtNow) == 1 )
        //       anyInstrumentActive = 1;
        //     if ( g_viola && UpdateViola(pmtNow) == 1 )
        //       anyInstrumentActive = 1;
        //     if ( g_cello && UpdateCello(pmtNow) == 1 )
        //       anyInstrumentActive = 1;
        //     if ( g_doubleBass && UpdateDoubleBass(pmtNow) == 1 )
        //       anyInstrumentActive = 1;
        if (g_bass && UpdateBass(pmtNow))
            anyInstrumentActive = 1;
        if (g_guitar && UpdateGuitar(pmtNow))
            anyInstrumentActive = 1;
        if (g_stageHorn && UpdateStageHorn(pmtNow))
            anyInstrumentActive = 1;
        //     if ( g_whistles && UpdateWhistles(pmtNow) )
        //       anyInstrumentActive = 1;
        //     UpdateSteamPuffers();
        //     if ( g_panPipe && UpdatePanPipe(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_harmonica && UpdateHarmonica(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_popBottle && UpdatePopBottle(pmtNow) )
        //       anyInstrumentActive = 1;
        if (g_agogos && UpdateAgogos(pmtNow))
            anyInstrumentActive = 1;
        if (g_woodblocks && UpdateWoodblocks(pmtNow))
            anyInstrumentActive = 1;
        if (g_trombone && UpdateTrombone(pmtNow))
            anyInstrumentActive = 1;
        if (g_tuba && UpdateTuba(pmtNow))
            anyInstrumentActive = 1;
        if (g_frenchHorn && UpdateFrenchHorn(pmtNow))
            anyInstrumentActive = 1;
        //     if ( g_ocarina && UpdateOcarina(pmtNow) )
        //       anyInstrumentActive = 1;
        if (g_baritoneSax && UpdateBaritoneSax(pmtNow))
            anyInstrumentActive = 1;
        if (g_tenorSax && UpdateTenorSax(pmtNow))
            anyInstrumentActive = 1;
        if (g_sapranoSax && UpdateSapranoSax(pmtNow))
            anyInstrumentActive = 1;
        if (g_altoSax && UpdateAltoSax(pmtNow))
            anyInstrumentActive = 1;
        //     if ( g_flute && UpdateFlute(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_piccolo && UpdatePiccolo(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_recorder && UpdateRecorder(pmtNow) )
        //       anyInstrumentActive = 1;
        if (g_trumpet && UpdateTrumpet(pmtNow))
            anyInstrumentActive = 1;
        if (g_stageChoir && UpdateStageChoir(pmtNow))
            anyInstrumentActive = 1;
        //     if ( g_musicBox && UpdateMusicBox(pmtNow) )
        //       anyInstrumentActive = 1;
        if (g_melodicTom && UpdateMelodicTom(pmtNow))
            anyInstrumentActive = 1;
        if (g_synthDrum && UpdateSynthDrum(pmtNow))
            anyInstrumentActive = 1;
        //     if ( g_steelDrum && UpdateSteelDrum(pmtNow) )
        //       anyInstrumentActive = 1;
        if (g_timpani && UpdateTimpani(pmtNow))
            anyInstrumentActive = 1;
        if (g_taiko && UpdateTaiko(pmtNow))
            anyInstrumentActive = 1;
        //     if ( g_telephone && UpdateTelephone(pmtNow) )
        //       anyInstrumentActive = 1;
        //     if ( g_tubularBells && UpdateTubularBells(pmtNow) )
        //       anyInstrumentActive = 1;
        if (g_stageString && UpdateStageString(pmtNow))
            anyInstrumentActive = 1;
        //     if ( g_pizzicatoStrings && UpdatePizzicatoStrings(pmtNow) )
        //       anyInstrumentActive = 1;
        UpdateAllRecoils(recoil_scale_factor);
        //     anyPercussionActive = 0;
        //     if ( g_show_percussion == 1 )
        //     {
        //       for ( patch = 0; patch < 88; ++patch )
        //       {
        //         for ( i = 0; i < 32; ++i )
        //         {
        //           if ( g_percussion_time_queue[patch][i] > 0 )
        //           {
        //             anyInstrumentActive = 1;
        //             anyPercussionActive = 1;
        //             g_percussion_framesWithEmptyQueue = 0;
        //             g_percussion_time_queue[patch][i] -= pmtNow - g_time_global_current;
        //             if ( g_percussion_time_queue[patch][i] <= 0 )
        //             {
        //               g_percussion_time_queue[patch][i] = 0;
        //               velocity = g_percussion_velocity_queue[patch][i];
        //               velocityFactor = (velocity * 0.75 + 32.0 + velocity * 0.75 + 32.0) / 128.0;
        //               switch ( patch )
        //               {
        //                 case ACOUSTIC_SNARE:
        //                 case ELECTRIC_SNARE:
        //                   g_recoil_snaredrum = g_recoil_snaredrum + velocityFactor;
        //                   if ( g_recoil_snaredrum > MAX_RECOIL )
        //                     g_recoil_snaredrum = MAX_RECOIL;
        //                   g_recoil_snare = g_recoil_snare + velocityFactor;
        //                   if ( g_recoil_snare > MAX_RECOIL )
        //                     g_recoil_snare = MAX_RECOIL;
        //                   break;
        //                 case SIDE_STICK:
        //                   g_recoil_snaredrum = velocityFactor * 0.25 + g_recoil_snaredrum;
        //                   if ( g_recoil_snaredrum > MAX_RECOIL )
        //                     g_recoil_snaredrum = MAX_RECOIL;
        //                   g_recoil_sideStick = g_recoil_sideStick + velocityFactor;
        //                   if ( g_recoil_sideStick > MAX_RECOIL )
        //                     g_recoil_sideStick = MAX_RECOIL;
        //                   break;
        //                 case HIGH_TOM:
        //                   g_recoil_tom[0] = g_recoil_tom[0] + velocityFactor;
        //                   if ( g_recoil_tom[0] > MAX_RECOIL )
        //                     g_recoil_tom[0] = MAX_RECOIL;
        //                   break;
        //                 case HIGH_MID_TOM:
        //                   g_recoil_tom[1] = g_recoil_tom[1] + velocityFactor;
        //                   if ( g_recoil_tom[1] > MAX_RECOIL )
        //                     g_recoil_tom[1] = MAX_RECOIL;
        //                   break;
        //                 case LOW_MID_TOM:
        //                   g_recoil_tom[2] = g_recoil_tom[2] + velocityFactor;
        //                   if ( g_recoil_tom[2] > MAX_RECOIL )
        //                     g_recoil_tom[2] = MAX_RECOIL;
        //                   break;
        //                 case LOW_TOM:
        //                   g_recoil_tom[3] = g_recoil_tom[3] + velocityFactor;
        //                   if ( g_recoil_tom[3] > MAX_RECOIL )
        //                     g_recoil_tom[3] = MAX_RECOIL;
        //                   break;
        //                 case HIGH_FLOOR_TOM:
        //                   g_recoil_tom[4] = g_recoil_tom[4] + velocityFactor;
        //                   if ( g_recoil_tom[4] > MAX_RECOIL )
        //                     g_recoil_tom[4] = MAX_RECOIL;
        //                   break;
        //                 case LOW_FLOOR_TOM:
        //                   g_recoil_tom[5] = g_recoil_tom[5] + velocityFactor;
        //                   if ( g_recoil_tom[5] > MAX_RECOIL )
        //                     g_recoil_tom[5] = MAX_RECOIL;
        //                   break;
        //                 case ACOUSTIC_BASS_DRUM:
        //                 case ELECTRIC_BASS_DRUM:
        //                   g_recoil_bassDrumArm = 0.0;
        //                   g_recoil_bassdrum = g_recoil_bassdrum + velocityFactor;
        //                   if ( g_recoil_bassdrum > MAX_RECOIL )
        //                     g_recoil_bassdrum = MAX_RECOIL;
        //                   break;
        //                 case COWBELL:
        //                   g_recoil_cowbell = g_recoil_cowbell + velocityFactor;
        //                   if ( g_recoil_cowbell > MAX_RECOIL )
        //                     g_recoil_cowbell = MAX_RECOIL;
        //                   break;
        //                 case HAND_CLAP:
        //                   g_recoil_clap_hand_r = g_recoil_clap_hand_r + velocityFactor;
        //                   if ( g_recoil_clap_hand_r > MAX_RECOIL )
        //                     g_recoil_clap_hand_r = MAX_RECOIL;
        //                   g_recoil_clap_hand_l = g_recoil_clap_hand_l + velocityFactor;
        //                   if ( g_recoil_clap_hand_l > MAX_RECOIL )
        //                     g_recoil_clap_hand_l = MAX_RECOIL;
        //                   break;
        //                 case TAMBOURINE:
        //                   g_recoil_tambourine_hand = g_recoil_tambourine_hand + velocityFactor;
        //                   if ( g_recoil_tambourine_hand > MAX_RECOIL )
        //                     g_recoil_tambourine_hand = MAX_RECOIL;
        //                   g_recoil_tambourine = g_recoil_tambourine + velocityFactor;
        //                   if ( g_recoil_tambourine > MAX_RECOIL )
        //                     g_recoil_tambourine = MAX_RECOIL;
        //                   break;
        //                 case STICKS:
        //                   g_recoil_sticks_2 = g_recoil_sticks_2 + velocityFactor;
        //                   if ( g_recoil_sticks_2 > MAX_RECOIL )
        //                     g_recoil_sticks_2 = MAX_RECOIL;
        //                   g_recoil_sticks_1 = g_recoil_sticks_1 + velocityFactor;
        //                   if ( g_recoil_sticks_1 > MAX_RECOIL )
        //                     g_recoil_sticks_1 = MAX_RECOIL;
        //                   break;
        //                 case CLAVES:
        //                   g_recoil = g_recoil + velocityFactor;
        //                   if ( g_recoil > MAX_RECOIL )
        //                     g_recoil = MAX_RECOIL;
        //                   g_recoil_clave_l = g_recoil_clave_l + velocityFactor;
        //                   if ( g_recoil_clave_l > MAX_RECOIL )
        //                     g_recoil_clave_l = MAX_RECOIL;
        //                   break;
        //                 case JINGLE_BELL:
        //                   g_recoil_jingleBell = g_recoil_jingleBell + velocityFactor;
        //                   if ( g_recoil_jingleBell > MAX_RECOIL )
        //                     g_recoil_jingleBell = MAX_RECOIL;
        //                   break;
        //                 case CASTANETS:
        //                   g_recoil_castanets = g_recoil_castanets + velocityFactor;
        //                   if ( g_recoil_castanets > MAX_RECOIL )
        //                     g_recoil_castanets = MAX_RECOIL;
        //                   break;
        //                 case SHAKER:
        //                   g_recoil_shaker = g_recoil_shaker + velocityFactor;
        //                   if ( g_recoil_shaker > MAX_RECOIL )
        //                     g_recoil_shaker = MAX_RECOIL;
        //                   break;
        //                 case HIGH_Q:
        //                   g_recoil_highQ = g_recoil_highQ + velocityFactor;
        //                   if ( g_recoil_highQ > MAX_RECOIL )
        //                     g_recoil_highQ = MAX_RECOIL;
        //                   break;
        //                 case SQUARE_CLICK:
        //                   g_recoil_squareClick = g_recoil_squareClick + velocityFactor;
        //                   if ( g_recoil_squareClick > MAX_RECOIL )
        //                     g_recoil_squareClick = MAX_RECOIL;
        //                   break;
        //                 case METRONOME_CLICK:
        //                   g_recoil_metronomeClick = g_recoil_metronomeClick + velocityFactor;
        //                   if ( g_recoil_metronomeClick > MAX_RECOIL )
        //                     g_recoil_metronomeClick = MAX_RECOIL;
        //                   break;
        //                 case METRONOME_BELL:
        //                   g_recoil_metronomeBell = g_recoil_metronomeBell + velocityFactor;
        //                   if ( g_recoil_metronomeBell > MAX_RECOIL )
        //                     g_recoil_metronomeBell = MAX_RECOIL;
        //                   break;
        //                 case MARACAS:
        //                   g_recoil_Maracas = g_recoil_Maracas + velocityFactor;
        //                   if ( g_recoil_Maracas > MAX_RECOIL )
        //                     g_recoil_Maracas = MAX_RECOIL;
        //                   break;
        //                 case CABASA:
        //                   g_recoil_cabasa = g_recoil_cabasa + velocityFactor;
        //                   if ( g_recoil_cabasa > MAX_RECOIL )
        //                     g_recoil_cabasa = MAX_RECOIL;
        //                   break;
        //                 case HIGH_AGOGO:
        //                   g_recoil_agogo_hi = g_recoil_agogo_hi + velocityFactor;
        //                   if ( g_recoil_agogo_hi > MAX_RECOIL )
        //                     g_recoil_agogo_hi = MAX_RECOIL;
        //                   break;
        //                 case LOW_AGOGO:
        //                   g_recoil_agogo_lo = g_recoil_agogo_lo + velocityFactor;
        //                   if ( g_recoil_agogo_lo > MAX_RECOIL )
        //                     g_recoil_agogo_lo = MAX_RECOIL;
        //                   break;
        //                 case HIGH_WOODBLOCK:
        //                   g_recoil_woodblock_hi = g_recoil_woodblock_hi + velocityFactor;
        //                   if ( g_recoil_woodblock_hi > MAX_RECOIL )
        //                     g_recoil_woodblock_hi = MAX_RECOIL;
        //                   break;
        //                 case OPEN_TRIANGLE:
        //                   g_recoil_triangle_open = g_recoil_triangle_open + velocityFactor;
        //                   if ( g_recoil_triangle_open > MAX_RECOIL )
        //                     g_recoil_triangle_open = MAX_RECOIL;
        //                   break;
        //                 case MUTE_TRIANGLE:
        //                   g_recoil_triangle_muted = g_recoil_triangle_muted + velocityFactor;
        //                   if ( g_recoil_triangle_muted > MAX_RECOIL )
        //                     g_recoil_triangle_muted = MAX_RECOIL;
        //                   break;
        //                 case LONG_WHISTLE:
        //                   g_recoil_whistle_long = MAX_RECOIL;
        //                   break;
        //                 case SHORT_WHISTLE:
        //                   g_recoil_whistle_short = MAX_RECOIL;
        //                   break;
        //                 case LOW_WOODBLOCK:
        //                   g_recoil_woodblock_lo = g_recoil_woodblock_lo + velocityFactor;
        //                   if ( g_recoil_woodblock_lo > MAX_RECOIL )
        //                     g_recoil_woodblock_lo = MAX_RECOIL;
        //                   break;
        //                 case HIGH_TIMBALE:
        //                   g_recoil_timbale_hi = g_recoil_timbale_hi + velocityFactor;
        //                   if ( g_recoil_timbale_hi > MAX_RECOIL )
        //                     g_recoil_timbale_hi = MAX_RECOIL;
        //                   break;
        //                 case LOW_TIMBALE:
        //                   g_recoil_timbale_lo = g_recoil_timbale_lo + velocityFactor;
        //                   if ( g_recoil_timbale_lo > MAX_RECOIL )
        //                     g_recoil_timbale_lo = MAX_RECOIL;
        //                   break;
        //                 case HIGH_BONGO:
        //                   g_recoil_bongo_hi = g_recoil_bongo_hi + velocityFactor;
        //                   if ( g_recoil_bongo_hi > MAX_RECOIL )
        //                     g_recoil_bongo_hi = MAX_RECOIL;
        //                   break;
        //                 case LOW_BONGO:
        //                   g_recoil_bongo_lo = g_recoil_bongo_lo + velocityFactor;
        //                   if ( g_recoil_bongo_lo > MAX_RECOIL )
        //                     g_recoil_bongo_lo = MAX_RECOIL;
        //                   break;
        //                 case MUTE_HIGH_CONGA:
        //                   g_recoil_conga_high_mute = g_recoil_conga_high_mute + velocityFactor;
        //                   if ( g_recoil_conga_high_mute > MAX_RECOIL )
        //                     g_recoil_conga_high_mute = MAX_RECOIL;
        //                   break;
        //                 case OPEN_HIGH_CONGA:
        //                   g_recoil_conga_high_open = g_recoil_conga_high_open + velocityFactor;
        //                   if ( g_recoil_conga_high_open > MAX_RECOIL )
        //                     g_recoil_conga_high_open = MAX_RECOIL;
        //                   break;
        //                 case LOW_CONGA:
        //                   g_recoil_conga_low = g_recoil_conga_low + velocityFactor;
        //                   if ( g_recoil_conga_low > MAX_RECOIL )
        //                     g_recoil_conga_low = MAX_RECOIL;
        //                   break;
        //                 case PEDAL_HI_HAT:
        //                   g_hihat_isOpen = 0;
        //                   g_recoil_hihat = g_recoil_hihat + velocityFactor;
        //                   if ( g_recoil_hihat > MAX_RECOIL )
        //                     g_recoil_hihat = MAX_RECOIL;
        //                   break;
        //                 case CLOSED_HI_HAT:
        //                   g_hihat_isOpen = 0;
        //                   g_recoil_hihat = g_recoil_hihat + velocityFactor;
        //                   if ( g_recoil_hihat > MAX_RECOIL )
        //                     g_recoil_hihat = MAX_RECOIL;
        //                   break;
        //                 default:
        //                   for ( cymbalIndex = 0; cymbalIndex < 7; ++cymbalIndex )
        //                   {
        //                     if ( patch == g_cymbal_patches[cymbalIndex]
        //                       || patch == RIDE_BELL
        //                       && (g_cymbal_patches[cymbalIndex] == RIDE_CYMBAL_1 && g_lastRideCymbal != 2
        //                        || g_cymbal_patches[cymbalIndex] == RIDE_CYMBAL_2 && g_lastRideCymbal == 2) )
        //                     {
        //                       switch ( patch )
        //                       {
        //                         case RIDE_CYMBAL_1:
        //                           g_lastRideCymbal = 1;
        //                           break;
        //                         case RIDE_CYMBAL_2:
        //                           g_lastRideCymbal = 2;
        //                           break;
        //                         case OPEN_HI_HAT:
        //                           g_hihat_isOpen = 1;
        //                           g_recoil_hihat = g_recoil_hihat + velocityFactor;
        //                           if ( g_recoil_hihat > MAX_RECOIL )
        //                             g_recoil_hihat = MAX_RECOIL;
        //                           break;
        //                       }
        //                       if ( patch == RIDE_BELL )
        //                       {
        //                         g_recoil_cymbals[cymbalIndex] = g_recoil_cymbals[cymbalIndex] + CYMBAL_MASS_FACTOR[cymbalIndex];
        //                         g_cymbalMassFactor[cymbalIndex] = CYMBAL_SCALE[cymbalIndex] + 13.75;
        //                         if ( g_cymbalCurrentWobble[cymbalIndex] < 0.0 )
        //                           g_cymbalCurrentWobble[cymbalIndex] = 0.0;
        //                         g_cymbalCurrentWobble[cymbalIndex] = g_percussion_velocity_queue[patch][i]
        //                                                            * CYMBAL_MASS_FACTOR[cymbalIndex]
        //                                                            / 128.0
        //                                                            * 0.75
        //                                                            + g_cymbalCurrentWobble[cymbalIndex];
        //                         g_cymbalAngularVelocity[cymbalIndex] = g_percussion_velocity_queue[patch][i]
        //                                                              * CYMBAL_ANGULAR_VELOCITY_FACTOR[cymbalIndex]
        //                                                              / 128.0
        //                                                              * 0.75;
        //                       }
        //                       else
        //                       {
        //                         g_recoil_cymbals[cymbalIndex] = g_recoil_cymbals[cymbalIndex] + CYMBAL_MASS_FACTOR[cymbalIndex];
        //                         v5 = CYMBAL_SCALE[cymbalIndex];
        //                         g_cymbalMassFactor[cymbalIndex] = v5 + v5 + 16.75;
        //                         if ( g_cymbalCurrentWobble[cymbalIndex] < 0.0 )
        //                           g_cymbalCurrentWobble[cymbalIndex] = 0.0;
        //                         // cymbal was hit, applying full wobble
        //                         g_cymbalCurrentWobble[cymbalIndex] = g_percussion_velocity_queue[patch][i]
        //                                                            * CYMBAL_MASS_FACTOR[cymbalIndex]
        //                                                            / 128.0
        //                                                            + g_cymbalCurrentWobble[cymbalIndex];
        //                         g_cymbalAngularVelocity[cymbalIndex] = g_percussion_velocity_queue[patch][i]
        //                                                              * CYMBAL_ANGULAR_VELOCITY_FACTOR[cymbalIndex]
        //                                                              / 128.0;
        //                       }
        //                       if ( g_cymbalAngularVelocity[cymbalIndex] < 0.0 )
        //                         g_cymbalAngularVelocity[cymbalIndex] = CYMBAL_ANGULAR_VELOCITY_FACTOR[cymbalIndex] * 0.25;
        //                       if ( g_cymbalAngularVelocity[cymbalIndex] > CYMBAL_ANGULAR_VELOCITY_FACTOR[cymbalIndex] )
        //                         g_cymbalAngularVelocity[cymbalIndex] = CYMBAL_ANGULAR_VELOCITY_FACTOR[cymbalIndex];
        //                       if ( g_cymbalCurrentWobble[cymbalIndex] > CYMBAL_MAX_WOBBLE[cymbalIndex] )
        //                         g_cymbalCurrentWobble[cymbalIndex] = CYMBAL_MAX_WOBBLE[cymbalIndex];
        //                       if ( g_recoil_cymbals[cymbalIndex] > CYMBAL_MAX_WOBBLE[cymbalIndex] )
        //                         g_recoil_cymbals[cymbalIndex] = CYMBAL_MAX_WOBBLE[cymbalIndex];
        //                       g_cymbalWobbleAmplitude[cymbalIndex] = CYMBAL_WOBBLE_AMPLITUDE[cymbalIndex];
        //                       goto LABEL_135;           // Break for-loop
        //                     }
        //                   }
        //                   break;
        //               }
        //             }
        //           }
        // LABEL_135:
        //           ;
        //         }
        //       }
        //       if ( !anyPercussionActive && ++g_percussion_framesWithEmptyQueue > 1000 )
        //       {
        //         g_show_percussion = 0;
        //         g_inst_visible_drumset = 0;
        //       }
        //     }
        if (g_songFillbarScale >= 1.0 && /* TODO !anyInstrumentActive*/ true && !g_isShuttingDown) {
            g_fadeFactor = 0.0;
            g_isShuttingDown = 1;
        }
        g_currentGlobalTime = pmtNow;
        ++g_framesAlive;
        HandleKeyPresses();
        globalTimeMs = timeGetTime();
        currentTimeMs = (globalTimeMs - g_applicationStartTime);
        recoil_scale_factor = currentTimeMs / RECOIL_SCALE_FACTOR; // wtf?
        g_applicationStartTime = globalTimeMs;
        // FADE IN
        if (g_isFadingIn == 1) {
            g_fadeFactor = g_fadeFactor - RECOIL_SCALE_FACTOR * 0.0024999999;
            if (g_fadeFactor <= 0.0) {
                g_fadeFactor = 0.0;
                g_isFadingIn = 0;
                g_isShuttingDown = 0;
                PlaySegment();
            }
        }
        // FADE OUT
        else if (g_isShuttingDown == 1) {
            g_fadeFactor = RECOIL_SCALE_FACTOR * 0.0024999999 + g_fadeFactor;
            if (g_fadeFactor >= 1.0) {
                g_fadeFactor = 1.0;
                g_isFadingIn = 0;
                g_isShuttingDown = 0;
                g_killApplication = 1;
            }
        }
        if (g_killApplication == 1)
            g_killApplication_0 = 1;
        cameraTransformComponentsAtTarget = 0;
        // check if auto cam is at target transform
        for (patch = 0; patch < 6; ++patch) {
            if (g_cameraLocation[patch] >= *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch)) {
                if (g_cameraLocation[patch] <= *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch)) {
                    if (g_cameraLocation[patch] == *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch))
                        ++cameraTransformComponentsAtTarget;
                }
                else {
                    // adjust camera transform to reach target
                    g_cameraLocation[patch] = g_cameraLocation[patch] + g_autoCamDeltaTransform[patch];
                    // check for equality
                    if (g_cameraLocation[patch] <= *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch)) {
                        g_cameraLocation[patch] = *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch);
                        ++cameraTransformComponentsAtTarget;
                    }
                }
            }
            else {
                // adjust camera transform to reach target
                g_cameraLocation[patch] = g_cameraLocation[patch] + g_autoCamDeltaTransform[patch];
                // check for equality
                if (g_cameraLocation[patch] >= *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch)) {
                    g_cameraLocation[patch] = *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch);
                    ++cameraTransformComponentsAtTarget;
                }
            }
        }
        if (cameraTransformComponentsAtTarget == 6)
            g_autoCamIsIdle = 1;
        ++g_framesSinceStart;
        if (!(++g_autoCamIdleTime % 1000)
            || !(g_autoCamIdleTime % 200)
            && ((g_targetCameraAngle == CAMERA_2A || g_targetCameraAngle == CAMERA_2B) && !IsCameraAngleViable_2()
                || (g_targetCameraAngle == CAMERA_3A || g_targetCameraAngle == CAMERA_3B) && !IsCameraAngleViable_3()
                || (g_targetCameraAngle == CAMERA_4A || g_targetCameraAngle == CAMERA_4B) && !IsCameraAngleViable_4()
                || g_targetCameraAngle == CAMERA_6 && !IsCameraAngleViable_6())) {
            TriggerAutoCam();
        }
    }
}

// ---------------------------------------------------------------------------
// FUNCTION: MIDIJAM 0x421A30
// ---------------------------------------------------------------------------
void HandleKeyPresses() {
    g_shiftPressed = g_keyStateArray[VK_SHIFT] != 0;
    //  -- Space key --
    if (g_keyStateArray[VK_SPACE]) {
        if (!g_isSpacePressed) {
            g_isSpacePressed = 1;
            if (g_directMusicSegmentPlayer->IsPlaying())
                // ReSharper disable once CppExpressionWithoutSideEffects
                g_directMusicSegmentPlayer->Stop(4096);
            else
                PlaySegment();
        }
    }
    else {
        g_isSpacePressed = 0;
    }
    //  -- KEY A (unused) --
    if (g_keyStateArray['A']) {
        if (!g_isAKeyPressed)
            g_isAKeyPressed = 1;
    }
    else {
        g_isAKeyPressed = 0;
    }
    //  -- KEY RIGHT (SPEED INCREASE) --
    if (g_keyStateArray[VK_RIGHT]) {
        if (!g_framesKeyRightPressed) {
            g_playbackSpeed = g_playbackSpeed + 1.0;
            if (g_playbackSpeed > 10.0)
                g_playbackSpeed = 10.0;
            g_DirectMusicPerformance->SetGlobalParam(GUID_PERF_MASTER_TEMPO, &g_playbackSpeed, 4);
            g_framesKeyRightPressed = 1;
        }
    }
    else {
        g_framesKeyRightPressed = 0;
    }
    //  -- KEY LEFT (SPEED DECREASE) --
    if (g_keyStateArray[VK_LEFT]) {
        if (!g_framesKeyLeftPressed) {
            g_playbackSpeed = g_playbackSpeed - 1.0;
            if (g_playbackSpeed < 1.0)
                g_playbackSpeed = 1.0;
            g_DirectMusicPerformance->SetGlobalParam(GUID_PERF_MASTER_TEMPO, &g_playbackSpeed, 4);
            g_framesKeyLeftPressed = 1;
        }
    }
    else {
        g_framesKeyLeftPressed = 0;
    }
    //  -- KEY UP --
    if (g_keyStateArray[VK_UP]) {
        if (!g_framesKeyUpPressed)
            g_framesKeyUpPressed = 1;
    }
    else {
        g_framesKeyUpPressed = 0;
    }
    //  -- KEY DOWN --
    if (g_keyStateArray[VK_DOWN]) {
        if (!g_framesKeyDownPressed)
            g_framesKeyDownPressed = 1;
    }
    else {
        g_framesKeyDownPressed = 0;
    }
    if (g_framesKeyRightPressed > 0 && ++g_framesKeyRightPressed > 50)
        g_framesKeyRightPressed = 0;
    if (g_framesKeyLeftPressed > 0 && ++g_framesKeyLeftPressed > 50)
        g_framesKeyLeftPressed = 0;
    if (g_framesKeyUpPressed > 0 && ++g_framesKeyUpPressed > 50)
        g_framesKeyUpPressed = 0;
    if (g_framesKeyDownPressed > 0 && ++g_framesKeyDownPressed > 50)
        g_framesKeyDownPressed = 0;
    if (g_keyStateArray[VK_ESCAPE]) {
        // ReSharper disable once CppExpressionWithoutSideEffects
        g_directMusicSegmentPlayer->Stop(4096);
        g_isShuttingDown = 1;
    }
    //  -- CAMERA 1A, 1B, 1C --
    if (g_keyStateArray['1']) {
        g_targetCameraAngle = static_cast<CameraAngle>(static_cast<short>(g_targetCameraAngle) + 1);
        g_rotatingCameraIsActive = 0;
        // wrap to 0..2
        if (g_targetCameraAngle > CAMERA_1C)
            g_targetCameraAngle = CAMERA_1A;
        MoveCameraToAngle(g_targetCameraAngle, 0);
        g_keyStateArray['1'] = 0;
    }
    //  -- CAMERA 2A, 2B --
    if (g_keyStateArray['2']) {
        // toggle between 3 and 7
        if (g_targetCameraAngle == CAMERA_2A)
            g_targetCameraAngle = CAMERA_2B;
        else
            g_targetCameraAngle = CAMERA_2A;
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(g_targetCameraAngle, 0);
        g_keyStateArray['2'] = 0;
    }
    //  -- CAMERA 3A, 3B --
    if (g_keyStateArray['3']) {
        if (g_targetCameraAngle == CAMERA_3A)
            g_targetCameraAngle = CAMERA_3B;
        else
            g_targetCameraAngle = CAMERA_3A;
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(g_targetCameraAngle, 0);
        g_keyStateArray['3'] = 0;
    }
    //  -- CAMERA 4A, 4B --
    if (g_keyStateArray['4']) {
        if (g_targetCameraAngle == CAMERA_4A)
            g_targetCameraAngle = CAMERA_4B;
        else
            g_targetCameraAngle = CAMERA_4A;
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(g_targetCameraAngle, 0);
        g_keyStateArray['4'] = 0;
    }
    //  -- CAMERA 5 --
    if (g_keyStateArray['5']) {
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(CAMERA_5, 0);
        g_keyStateArray['5'] = 0;
    }
    //  -- CAMERA 6 --
    if (g_keyStateArray['6']) {
        g_targetCameraAngle = CAMERA_6;
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(CAMERA_6, 0);
        g_keyStateArray['6'] = 0;
    }
    //  -- CAMERA 9 (ROTATING) --
    if (g_keyStateArray['9']) {
        g_targetCameraAngle = CAMERA_1A;
        g_rotatingCameraIsActive = 1;
        g_rotatingCameraAngle = 0.0;
        g_rotatingCameraDirection = CLOCKWISE;
        g_rotatingCameraIdleTime = 0;
        MoveCameraToAngle(CAMERA_1A, 0);
        g_keyStateArray['9'] = 0;
    }
    //  -- CAMERA 0 (AUTO) --
    if (g_keyStateArray['0']) {
        g_autoCameraIsActive = 1;
        g_rotatingCameraIsActive = 0;
        TriggerAutoCam();
        g_keyStateArray['0'] = 0;
    }
    if (g_mouseWheelMoved) {
        g_mouseWheelDelta = 0;
        g_mouseWheelMoved = 0;
    }
}

void UpdateAllRecoils(const float scale) {
    const float recoil_scale_factor = 0.05 * scale;

    for (short i = 0; i < g_agogosCount; ++i) {
        for (short j = 0; j < 12; ++j) {
            g_agogos[i].recoilOffset[j] = g_agogos[i].recoilOffset[j] - recoil_scale_factor;
            if (g_agogos[i].recoilOffset[j] < 0.0)
                g_agogos[i].recoilOffset[j] = 0.0;
        }
    }

    for (short i = 0; i < g_woodblocksCount; ++i) {
        for (short j = 0; j < 12; ++j) {
            g_woodblocks[i].recoilOffset[j] = g_woodblocks[i].recoilOffset[j] - recoil_scale_factor;
            if (g_woodblocks[i].recoilOffset[j] < 0.0)
                g_woodblocks[i].recoilOffset[j] = 0.0;
        }
    }

    for (short i = 0; i < g_timpaniCount; ++i) {
        g_timpani[i].recoilOffset = g_timpani[i].recoilOffset - recoil_scale_factor;
        if (g_timpani[i].recoilOffset < 0.0)
            g_timpani[i].recoilOffset = 0.0;
    }

    for (short i = 0; i < g_melodicTomCount; ++i) {
        g_melodicTom[i].recoilOffset = g_melodicTom[i].recoilOffset - recoil_scale_factor;
        if (g_melodicTom[i].recoilOffset < 0.0)
            g_melodicTom[i].recoilOffset = 0.0;
    }

    for (short i = 0; i < g_synthDrumCount; ++i) {
        g_synthDrum[i].recoilOffset = g_synthDrum[i].recoilOffset - recoil_scale_factor;
        if (g_synthDrum[i].recoilOffset < 0.0)
            g_synthDrum[i].recoilOffset = 0.0;
    }

    for (short i = 0; i < g_taikoCount; ++i) {
        g_taiko[i].recoilOffset = g_taiko[i].recoilOffset - recoil_scale_factor;
        if (g_taiko[i].recoilOffset < 0.0)
            g_taiko[i].recoilOffset = 0.0;
    }
}
