//
// Created by Jacob on 3/18/2026.
//

#include "update.h"

#include "audio/DirectMusicSegmentPlayer.h"
#include "audio/playback.h"
#include "camera.h"
#include "instruments/Accordion.h"
#include "instruments/Agogos.h"
#include "instruments/AltoSax.h"
#include "instruments/BaritoneSax.h"
#include "instruments/Bass.h"
#include "instruments/Cello.h"
#include "instruments/DoubleBass.h"
#include "instruments/Flute.h"
#include "instruments/FrenchHorn.h"
#include "instruments/Guitar.h"
#include "instruments/Harmonica.h"
#include "instruments/Harp.h"
#include "instruments/MelodicTom.h"
#include "instruments/MusicBox.h"
#include "instruments/Ocarina.h"
#include "instruments/PanPipe.h"
#include "instruments/Piano.h"
#include "instruments/Piccolo.h"
#include "instruments/PizzicatoStrings.h"
#include "instruments/PopBottles.h"
#include "instruments/Recorder.h"
#include "instruments/SapranoSax.h"
#include "instruments/StageChoir.h"
#include "instruments/StageHorn.h"
#include "instruments/StageString.h"
#include "instruments/SteelDrum.h"
#include "instruments/SynthDrum.h"
#include "instruments/Taiko.h"
#include "instruments/Telephone.h"
#include "instruments/TenorSax.h"
#include "instruments/Timpani.h"
#include "instruments/Trombone.h"
#include "instruments/Trumpet.h"
#include "instruments/Tuba.h"
#include "instruments/TubularBells.h"
#include "instruments/Viola.h"
#include "instruments/Violin.h"
#include "instruments/Whistles.h"
#include "instruments/Woodblocks.h"
#include "instruments/Xylophone.h"
#include "instruments/instrument_ids.h"
#include "instruments/percussion/Cymbals.h"
#include "instruments/percussion/HiHat.h"
#include "instruments/percussion/Metronome.h"
#include "instruments/percussion/SnareDrum.h"
#include "instruments/percussion/Toms.h"
#include "instruments/percussion/particles.h"
#include "instruments/percussion/percussion.h"
#include "macros.h"
#include "model/Ms3dBundle.h"
#include "render/text.h"
#include "scene/fadeout.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include <cmath>

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
extern short g_inst_visible_drumset;

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

// PERCUSSION MODELS //
extern Ms3dBundle* g_drumSet_Stick_ms3d;
extern Ms3dBundle* g_cowbell_ms3d;
extern Ms3dBundle* g_handRight_ms3d;
extern Ms3dBundle* g_handLeft_ms3d;
extern Ms3dBundle* g_handTambourine_ms3d;
extern Ms3dBundle* g_clave_ms3d;
extern Ms3dBundle* g_jingleBells_ms3d;
extern Ms3dBundle* g_castanets_ms3d;
extern Ms3dBundle* g_shaker_ms3d;
extern Ms3dBundle* g_zapper_ms3d;
extern Ms3dBundle* g_zapperLaser_ms3d;
extern Ms3dBundle* g_squareShaker_ms3d;
extern Ms3dBundle* g_maraca_ms3d;
extern Ms3dBundle* g_cabasa_ms3d;
extern Ms3dBundle* g_agogo_ms3d;
extern Ms3dBundle* g_woodBlockHigh_ms3d;
extern Ms3dBundle* g_woodBlockLow_ms3d;
extern Ms3dBundle* g_triangle_ms3d;
extern Ms3dBundle* g_mutedTriangle_ms3d;
extern Ms3dBundle* g_triangleStick_ms3d;
extern Ms3dBundle* g_drumSet_timbale_ms3d;
extern Ms3dBundle* g_drumSet_bongo_ms3d;
extern Ms3dBundle* g_drumSet_conga_ms3d;
extern Ms3dBundle* g_drumSet_bassDrum_ms3d;
extern Ms3dBundle* g_drumSet_snareDrum_ms3d;
extern Ms3dBundle* g_drumSet_tom_ms3d;
extern Ms3dBundle* g_drumSet_cymbal_ms3d;
extern Ms3dBundle* g_drumSet_chinaCymbal_ms3d;
extern Ms3dBundle* g_drumSet_bassDrumBeaterArm_ms3d;
extern Ms3dBundle* g_drumSet_bassDrumBeaterHolder_ms3d;
extern Ms3dBundle* g_drumSet_bassDrumPedal_ms3d;
extern Ms3dBundle* g_metronome_ms3d;
extern Ms3dBundle* g_metronomePendjulum1_ms3d;
extern Ms3dBundle* g_metronomePendjulum2_ms3d;

extern DS_Particles g_particles[2];
extern Ms3dBundle* g_steamCloud_0_ms3d;
extern Ms3dBundle* g_steamCloud_1_ms3d;

void RenderPercussion();
void UpdateAllRecoils(float scale);

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

        // -- DRUM SHADOW --
        if (g_inst_visible_drumset > 0) {
            glPushMatrix();
            glTranslatef(0.0, -32.0, -95.0);
            g_drumShadow_ms3d->RenderModel();
            glPopMatrix();
        }

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
        if (g_recorder) RenderRecorder();
        if (g_piccolo) RenderPiccolo();
        if (g_flute) RenderFlute();
        if (g_tuba) RenderTuba();
        if (g_stageHorn) RenderStageHorn();
        if (g_whistles) RenderWhistles();
        if (g_panPipe) RenderPanPipe();
        if (g_harmonica) RenderHarmonica();
        if (g_violin) RenderViolin();
        if (g_viola) RenderViola();
        if (g_cello) RenderCello();
        if (g_doubleBass) RenderDoubleBass();
        if (g_popBottles) RenderPopBottles();
        if (g_agogos) RenderAgogos();
        if (g_woodblocks) RenderWoodblocks();
        if (g_stageChoir) RenderStageChoir();
        if (g_accordion) RenderAccordion();
        if (g_stageString) RenderStageString();
        if (g_pizzicatoStrings) RenderPizzicatoStrings();
        if (g_ocarina) RenderOcarina();

        if (g_xylophone) {
            glPushMatrix();
            glTranslatef(-22.0, -10.0, 10.0);
            glRotatef(32.0, 0.0, 1.0, 0.0);
            glScalef(0.64999998, 0.64999998, 0.64999998);
            RenderXylophone();
            glPopMatrix();
        }

        if (g_musicBox) RenderMusicBox();

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
        if (g_steelDrum) RenderSteelDrum();
        if (g_timpani) RenderTimpani();
        if (g_taiko) RenderTaiko();
        if (g_telephone) RenderTelephone();
        if (g_tubularBells) RenderTubularBells();

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
        if (g_violin && UpdateViolin(pmtNow) == 1)
            anyInstrumentActive = 1;
        if (g_viola && UpdateViola(pmtNow) == 1)
            anyInstrumentActive = 1;
        if (g_cello && UpdateCello(pmtNow) == 1)
            anyInstrumentActive = 1;
        if (g_doubleBass && UpdateDoubleBass(pmtNow) == 1)
            anyInstrumentActive = 1;
        if (g_bass && UpdateBass(pmtNow))
            anyInstrumentActive = 1;
        if (g_guitar && UpdateGuitar(pmtNow))
            anyInstrumentActive = 1;
        if (g_stageHorn && UpdateStageHorn(pmtNow))
            anyInstrumentActive = 1;
        if (g_whistles && UpdateWhistles(pmtNow))
            anyInstrumentActive = 1;
        //     UpdateSteamPuffers();
        if (g_panPipe && UpdatePanPipe(pmtNow))
            anyInstrumentActive = 1;
        if (g_harmonica && UpdateHarmonica(pmtNow))
            anyInstrumentActive = 1;
        if (g_popBottles && UpdatePopBottles(pmtNow))
            anyInstrumentActive = 1;
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
        if (g_ocarina && UpdateOcarina(pmtNow))
            anyInstrumentActive = 1;
        if (g_baritoneSax && UpdateBaritoneSax(pmtNow))
            anyInstrumentActive = 1;
        if (g_tenorSax && UpdateTenorSax(pmtNow))
            anyInstrumentActive = 1;
        if (g_sapranoSax && UpdateSapranoSax(pmtNow))
            anyInstrumentActive = 1;
        if (g_altoSax && UpdateAltoSax(pmtNow))
            anyInstrumentActive = 1;
        if (g_flute && UpdateFlute(pmtNow))
            anyInstrumentActive = 1;
        if (g_piccolo && UpdatePiccolo(pmtNow))
            anyInstrumentActive = 1;
        if (g_recorder && UpdateRecorder(pmtNow))
            anyInstrumentActive = 1;
        if (g_trumpet && UpdateTrumpet(pmtNow))
            anyInstrumentActive = 1;
        if (g_stageChoir && UpdateStageChoir(pmtNow))
            anyInstrumentActive = 1;
        if (g_musicBox && UpdateMusicBox(pmtNow))
            anyInstrumentActive = 1;
        if (g_melodicTom && UpdateMelodicTom(pmtNow))
            anyInstrumentActive = 1;
        if (g_synthDrum && UpdateSynthDrum(pmtNow))
            anyInstrumentActive = 1;
        if (g_steelDrum && UpdateSteelDrum(pmtNow))
            anyInstrumentActive = 1;
        if (g_timpani && UpdateTimpani(pmtNow))
            anyInstrumentActive = 1;
        if (g_taiko && UpdateTaiko(pmtNow))
            anyInstrumentActive = 1;
        if (g_telephone && UpdateTelephone(pmtNow))
            anyInstrumentActive = 1;
        if (g_tubularBells && UpdateTubularBells(pmtNow))
            anyInstrumentActive = 1;
        if (g_stageString && UpdateStageString(pmtNow))
            anyInstrumentActive = 1;
        if (g_pizzicatoStrings && UpdatePizzicatoStrings(pmtNow))
            anyInstrumentActive = 1;
        UpdateAllRecoils(recoil_scale_factor);
        anyPercussionActive = 0;
        if (g_show_percussion == 1) {
            for (patch = 0; patch < 88; ++patch) {
                for (i = 0; i < 32; ++i) {
                    if (g_percussion_time_queue[patch][i] > 0) {
                        anyInstrumentActive = 1;
                        anyPercussionActive = 1;
                        g_percussion_framesWithEmptyQueue = 0;
                        g_percussion_time_queue[patch][i] -= pmtNow - g_currentGlobalTime;
                        if (g_percussion_time_queue[patch][i] <= 0) {
                            g_percussion_time_queue[patch][i] = 0;
                            velocity = g_percussion_velocity_queue[patch][i];
                            velocityFactor = (velocity * 0.75 + 32.0 + velocity * 0.75 + 32.0) / 128.0;
                            switch (patch) {
                                case ACOUSTIC_SNARE:
                                case ELECTRIC_SNARE:
                                    g_recoil_snaredrum = g_recoil_snaredrum + velocityFactor;
                                    if (g_recoil_snaredrum > MAX_RECOIL)
                                        g_recoil_snaredrum = MAX_RECOIL;
                                    g_recoil_snare = g_recoil_snare + velocityFactor;
                                    if (g_recoil_snare > MAX_RECOIL)
                                        g_recoil_snare = MAX_RECOIL;
                                    break;
                                case SIDE_STICK:
                                    g_recoil_snaredrum = velocityFactor * 0.25 + g_recoil_snaredrum;
                                    if (g_recoil_snaredrum > MAX_RECOIL)
                                        g_recoil_snaredrum = MAX_RECOIL;
                                    g_recoil_sideStick = g_recoil_sideStick + velocityFactor;
                                    if (g_recoil_sideStick > MAX_RECOIL)
                                        g_recoil_sideStick = MAX_RECOIL;
                                    break;
                                case HIGH_TOM:
                                    g_recoil_tom[0] = g_recoil_tom[0] + velocityFactor;
                                    if (g_recoil_tom[0] > MAX_RECOIL)
                                        g_recoil_tom[0] = MAX_RECOIL;
                                    break;
                                case HIGH_MID_TOM:
                                    g_recoil_tom[1] = g_recoil_tom[1] + velocityFactor;
                                    if (g_recoil_tom[1] > MAX_RECOIL)
                                        g_recoil_tom[1] = MAX_RECOIL;
                                    break;
                                case LOW_MID_TOM:
                                    g_recoil_tom[2] = g_recoil_tom[2] + velocityFactor;
                                    if (g_recoil_tom[2] > MAX_RECOIL)
                                        g_recoil_tom[2] = MAX_RECOIL;
                                    break;
                                case LOW_TOM:
                                    g_recoil_tom[3] = g_recoil_tom[3] + velocityFactor;
                                    if (g_recoil_tom[3] > MAX_RECOIL)
                                        g_recoil_tom[3] = MAX_RECOIL;
                                    break;
                                case HIGH_FLOOR_TOM:
                                    g_recoil_tom[4] = g_recoil_tom[4] + velocityFactor;
                                    if (g_recoil_tom[4] > MAX_RECOIL)
                                        g_recoil_tom[4] = MAX_RECOIL;
                                    break;
                                case LOW_FLOOR_TOM:
                                    g_recoil_tom[5] = g_recoil_tom[5] + velocityFactor;
                                    if (g_recoil_tom[5] > MAX_RECOIL)
                                        g_recoil_tom[5] = MAX_RECOIL;
                                    break;
                                case ACOUSTIC_BASS_DRUM:
                                case ELECTRIC_BASS_DRUM:
                                    g_recoil_bassDrumArm = 0.0;
                                    g_recoil_bassdrum = g_recoil_bassdrum + velocityFactor;
                                    if (g_recoil_bassdrum > MAX_RECOIL)
                                        g_recoil_bassdrum = MAX_RECOIL;
                                    break;
                                case COWBELL:
                                    g_recoil_cowbell = g_recoil_cowbell + velocityFactor;
                                    if (g_recoil_cowbell > MAX_RECOIL)
                                        g_recoil_cowbell = MAX_RECOIL;
                                    break;
                                case HAND_CLAP:
                                    g_recoil_clap_hand_r = g_recoil_clap_hand_r + velocityFactor;
                                    if (g_recoil_clap_hand_r > MAX_RECOIL)
                                        g_recoil_clap_hand_r = MAX_RECOIL;
                                    g_recoil_clap_hand_l = g_recoil_clap_hand_l + velocityFactor;
                                    if (g_recoil_clap_hand_l > MAX_RECOIL)
                                        g_recoil_clap_hand_l = MAX_RECOIL;
                                    break;
                                case TAMBOURINE:
                                    g_recoil_tambourine_hand = g_recoil_tambourine_hand + velocityFactor;
                                    if (g_recoil_tambourine_hand > MAX_RECOIL)
                                        g_recoil_tambourine_hand = MAX_RECOIL;
                                    g_recoil_tambourine = g_recoil_tambourine + velocityFactor;
                                    if (g_recoil_tambourine > MAX_RECOIL)
                                        g_recoil_tambourine = MAX_RECOIL;
                                    break;
                                case STICKS:
                                    g_recoil_sticks_1 = g_recoil_sticks_1 + velocityFactor;
                                    if (g_recoil_sticks_1 > MAX_RECOIL)
                                        g_recoil_sticks_1 = MAX_RECOIL;
                                    g_recoil_sticks_1 = g_recoil_sticks_1 + velocityFactor;
                                    if (g_recoil_sticks_1 > MAX_RECOIL)
                                        g_recoil_sticks_1 = MAX_RECOIL;
                                    break;
                                case CLAVES:
                                    g_recoil_clave_l = g_recoil_clave_l + velocityFactor;
                                    if (g_recoil_clave_l > MAX_RECOIL)
                                        g_recoil_clave_l = MAX_RECOIL;
                                    g_recoil_clave_l = g_recoil_clave_l + velocityFactor;
                                    if (g_recoil_clave_l > MAX_RECOIL)
                                        g_recoil_clave_l = MAX_RECOIL;
                                    break;
                                case JINGLE_BELL:
                                    g_recoil_jingleBell = g_recoil_jingleBell + velocityFactor;
                                    if (g_recoil_jingleBell > MAX_RECOIL)
                                        g_recoil_jingleBell = MAX_RECOIL;
                                    break;
                                case CASTANETS:
                                    g_recoil_castanets = g_recoil_castanets + velocityFactor;
                                    if (g_recoil_castanets > MAX_RECOIL)
                                        g_recoil_castanets = MAX_RECOIL;
                                    break;
                                case SHAKER:
                                    g_recoil_shaker = g_recoil_shaker + velocityFactor;
                                    if (g_recoil_shaker > MAX_RECOIL)
                                        g_recoil_shaker = MAX_RECOIL;
                                    break;
                                case HIGH_Q:
                                    g_recoil_highQ = g_recoil_highQ + velocityFactor;
                                    if (g_recoil_highQ > MAX_RECOIL)
                                        g_recoil_highQ = MAX_RECOIL;
                                    break;
                                case SQUARE_CLICK:
                                    g_recoil_squareClick = g_recoil_squareClick + velocityFactor;
                                    if (g_recoil_squareClick > MAX_RECOIL)
                                        g_recoil_squareClick = MAX_RECOIL;
                                    break;
                                case METRONOME_CLICK:
                                    g_recoil_metronomeClick = g_recoil_metronomeClick + velocityFactor;
                                    if (g_recoil_metronomeClick > MAX_RECOIL)
                                        g_recoil_metronomeClick = MAX_RECOIL;
                                    break;
                                case METRONOME_BELL:
                                    g_recoil_metronomeBell = g_recoil_metronomeBell + velocityFactor;
                                    if (g_recoil_metronomeBell > MAX_RECOIL)
                                        g_recoil_metronomeBell = MAX_RECOIL;
                                    break;
                                case MARACAS:
                                    g_recoil_Maracas = g_recoil_Maracas + velocityFactor;
                                    if (g_recoil_Maracas > MAX_RECOIL)
                                        g_recoil_Maracas = MAX_RECOIL;
                                    break;
                                case CABASA:
                                    g_recoil_cabasa = g_recoil_cabasa + velocityFactor;
                                    if (g_recoil_cabasa > MAX_RECOIL)
                                        g_recoil_cabasa = MAX_RECOIL;
                                    break;
                                case HIGH_AGOGO:
                                    g_recoil_agogo_hi = g_recoil_agogo_hi + velocityFactor;
                                    if (g_recoil_agogo_hi > MAX_RECOIL)
                                        g_recoil_agogo_hi = MAX_RECOIL;
                                    break;
                                case LOW_AGOGO:
                                    g_recoil_agogo_lo = g_recoil_agogo_lo + velocityFactor;
                                    if (g_recoil_agogo_lo > MAX_RECOIL)
                                        g_recoil_agogo_lo = MAX_RECOIL;
                                    break;
                                case HIGH_WOODBLOCK:
                                    g_recoil_woodblock_hi = g_recoil_woodblock_hi + velocityFactor;
                                    if (g_recoil_woodblock_hi > MAX_RECOIL)
                                        g_recoil_woodblock_hi = MAX_RECOIL;
                                    break;
                                case OPEN_TRIANGLE:
                                    g_recoil_triangle_open = g_recoil_triangle_open + velocityFactor;
                                    if (g_recoil_triangle_open > MAX_RECOIL)
                                        g_recoil_triangle_open = MAX_RECOIL;
                                    break;
                                case MUTE_TRIANGLE:
                                    g_recoil_triangle_muted = g_recoil_triangle_muted + velocityFactor;
                                    if (g_recoil_triangle_muted > MAX_RECOIL)
                                        g_recoil_triangle_muted = MAX_RECOIL;
                                    break;
                                case LONG_WHISTLE:
                                    g_recoil_whistle_long = MAX_RECOIL;
                                    break;
                                case SHORT_WHISTLE:
                                    g_recoil_whistle_short = MAX_RECOIL;
                                    break;
                                case LOW_WOODBLOCK:
                                    g_recoil_woodblock_lo = g_recoil_woodblock_lo + velocityFactor;
                                    if (g_recoil_woodblock_lo > MAX_RECOIL)
                                        g_recoil_woodblock_lo = MAX_RECOIL;
                                    break;
                                case HIGH_TIMBALE:
                                    g_recoil_timbale_hi = g_recoil_timbale_hi + velocityFactor;
                                    if (g_recoil_timbale_hi > MAX_RECOIL)
                                        g_recoil_timbale_hi = MAX_RECOIL;
                                    break;
                                case LOW_TIMBALE:
                                    g_recoil_timbale_lo = g_recoil_timbale_lo + velocityFactor;
                                    if (g_recoil_timbale_lo > MAX_RECOIL)
                                        g_recoil_timbale_lo = MAX_RECOIL;
                                    break;
                                case HIGH_BONGO:
                                    g_recoil_bongo_hi = g_recoil_bongo_hi + velocityFactor;
                                    if (g_recoil_bongo_hi > MAX_RECOIL)
                                        g_recoil_bongo_hi = MAX_RECOIL;
                                    break;
                                case LOW_BONGO:
                                    g_recoil_bongo_lo = g_recoil_bongo_lo + velocityFactor;
                                    if (g_recoil_bongo_lo > MAX_RECOIL)
                                        g_recoil_bongo_lo = MAX_RECOIL;
                                    break;
                                case MUTE_HIGH_CONGA:
                                    g_recoil_conga_high_mute = g_recoil_conga_high_mute + velocityFactor;
                                    if (g_recoil_conga_high_mute > MAX_RECOIL)
                                        g_recoil_conga_high_mute = MAX_RECOIL;
                                    break;
                                case OPEN_HIGH_CONGA:
                                    g_recoil_conga_high_open = g_recoil_conga_high_open + velocityFactor;
                                    if (g_recoil_conga_high_open > MAX_RECOIL)
                                        g_recoil_conga_high_open = MAX_RECOIL;
                                    break;
                                case LOW_CONGA:
                                    g_recoil_conga_low = g_recoil_conga_low + velocityFactor;
                                    if (g_recoil_conga_low > MAX_RECOIL)
                                        g_recoil_conga_low = MAX_RECOIL;
                                    break;
                                case PEDAL_HI_HAT:
                                    g_hihat_isOpen = 0;
                                    g_recoil_hihat = g_recoil_hihat + velocityFactor;
                                    if (g_recoil_hihat > MAX_RECOIL)
                                        g_recoil_hihat = MAX_RECOIL;
                                    break;
                                case CLOSED_HI_HAT:
                                    g_hihat_isOpen = 0;
                                    g_recoil_hihat = g_recoil_hihat + velocityFactor;
                                    if (g_recoil_hihat > MAX_RECOIL)
                                        g_recoil_hihat = MAX_RECOIL;
                                    break;
                                default:
                                    for (cymbalIndex = 0; cymbalIndex < 7; ++cymbalIndex) {
                                        if (patch == g_cymbal_patches[cymbalIndex]
                                            || patch == RIDE_BELL
                                            && (g_cymbal_patches[cymbalIndex] == RIDE_CYMBAL_1 && g_lastRideCymbal != 2
                                                || g_cymbal_patches[cymbalIndex] == RIDE_CYMBAL_2 && g_lastRideCymbal ==
                                                2)) {
                                            switch (patch) {
                                                case RIDE_CYMBAL_1:
                                                    g_lastRideCymbal = 1;
                                                    break;
                                                case RIDE_CYMBAL_2:
                                                    g_lastRideCymbal = 2;
                                                    break;
                                                case OPEN_HI_HAT:
                                                    g_hihat_isOpen = 1;
                                                    g_recoil_hihat = g_recoil_hihat + velocityFactor;
                                                    if (g_recoil_hihat > MAX_RECOIL)
                                                        g_recoil_hihat = MAX_RECOIL;
                                                    break;
                                            }
                                            if (patch == RIDE_BELL) {
                                                g_recoil_cymbals[cymbalIndex] = g_recoil_cymbals[cymbalIndex] +
                                                    CYMBAL_MASS_FACTOR[cymbalIndex];
                                                g_cymbalMassFactor[cymbalIndex] = CYMBAL_SCALE[cymbalIndex] + 13.75;
                                                if (g_cymbalCurrentWobble[cymbalIndex] < 0.0)
                                                    g_cymbalCurrentWobble[cymbalIndex] = 0.0;
                                                g_cymbalCurrentWobble[cymbalIndex] = g_percussion_velocity_queue[patch][
                                                        i]
                                                    * CYMBAL_MASS_FACTOR[cymbalIndex]
                                                    / 128.0
                                                    * 0.75
                                                    + g_cymbalCurrentWobble[cymbalIndex];
                                                g_cymbalAngularVelocity[cymbalIndex] = g_percussion_velocity_queue[
                                                        patch][i]
                                                    * CYMBAL_ANGULAR_VELOCITY_FACTOR[cymbalIndex]
                                                    / 128.0
                                                    * 0.75;
                                            }
                                            else {
                                                g_recoil_cymbals[cymbalIndex] = g_recoil_cymbals[cymbalIndex] +
                                                    CYMBAL_MASS_FACTOR[cymbalIndex];
                                                v5 = CYMBAL_SCALE[cymbalIndex];
                                                g_cymbalMassFactor[cymbalIndex] = v5 + v5 + 16.75;
                                                if (g_cymbalCurrentWobble[cymbalIndex] < 0.0)
                                                    g_cymbalCurrentWobble[cymbalIndex] = 0.0;
                                                // cymbal was hit, applying full wobble
                                                g_cymbalCurrentWobble[cymbalIndex] = g_percussion_velocity_queue[patch][
                                                        i]
                                                    * CYMBAL_MASS_FACTOR[cymbalIndex]
                                                    / 128.0
                                                    + g_cymbalCurrentWobble[cymbalIndex];
                                                g_cymbalAngularVelocity[cymbalIndex] = g_percussion_velocity_queue[
                                                        patch][i]
                                                    * CYMBAL_ANGULAR_VELOCITY_FACTOR[cymbalIndex]
                                                    / 128.0;
                                            }
                                            if (g_cymbalAngularVelocity[cymbalIndex] < 0.0)
                                                g_cymbalAngularVelocity[cymbalIndex] = CYMBAL_ANGULAR_VELOCITY_FACTOR[
                                                    cymbalIndex] * 0.25;
                                            if (g_cymbalAngularVelocity[cymbalIndex] > CYMBAL_ANGULAR_VELOCITY_FACTOR[
                                                cymbalIndex])
                                                g_cymbalAngularVelocity[cymbalIndex] = CYMBAL_ANGULAR_VELOCITY_FACTOR[
                                                    cymbalIndex];
                                            if (g_cymbalCurrentWobble[cymbalIndex] > CYMBAL_MAX_WOBBLE[cymbalIndex])
                                                g_cymbalCurrentWobble[cymbalIndex] = CYMBAL_MAX_WOBBLE[cymbalIndex];
                                            if (g_recoil_cymbals[cymbalIndex] > CYMBAL_MAX_WOBBLE[cymbalIndex])
                                                g_recoil_cymbals[cymbalIndex] = CYMBAL_MAX_WOBBLE[cymbalIndex];
                                            g_cymbalWobbleAmplitude[cymbalIndex] = CYMBAL_WOBBLE_AMPLITUDE[cymbalIndex];
                                            goto LABEL_135; // Break for-loop
                                        }
                                    }
                                    break;
                            }
                        }
                    }
                LABEL_135:
                    ;
                }
            }
            if (!anyPercussionActive && ++g_percussion_framesWithEmptyQueue > 1000) {
                g_show_percussion = 0;
                g_inst_visible_drumset = 0;
            }
        }
        if (g_songFillbarScale >= 1.0 && anyInstrumentActive && !g_isShuttingDown) {
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

void RenderPercussion() {
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

    // BASS DRUM
    glPushMatrix();
    glPushMatrix();
    glTranslatef(0.0, 5.5650001, 3.618);
    glRotatef(g_recoil_bassDrumArm, 1.0, 0.0, 0.0);
    g_drumSet_bassDrumBeaterArm_ms3d->RenderModel();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 0.0, 2.3);
    g_drumSet_bassDrumBeaterHolder_ms3d->RenderModel();
    glPopMatrix();
    glTranslatef(0.0, 0.477, 9.8540001);
    angle_bassDrumArm = g_recoil_bassDrumArm / 45.0 * 16.0;
    glRotatef(angle_bassDrumArm, 1.0, 0.0, 0.0);
    g_drumSet_bassDrumPedal_ms3d->RenderModel();
    glPopMatrix();
    glPushMatrix();
    recoil_bassDrum = -g_recoil_bassdrum;
    glTranslatef(0.0, 0.0, recoil_bassDrum);
    g_drumSet_bassDrum_ms3d->RenderModel();
    glPopMatrix();
    // TOMS
    for (tom_index = 0; tom_index < 6; ++tom_index) {
        glPushMatrix();
        glTranslatef(DRUMSET_TOM_LOC_X[tom_index], DRUMSET_TOM_LOC_Y[tom_index], DRUMSET_TOM_LOC_Z[tom_index]);
        glRotatef(DRUMSET_TOM_ROT_Y[tom_index], 0.0, 1.0, 0.0);
        glRotatef(DRUMSET_TOM_ROT_X[tom_index], 1.0, 0.0, 0.0);
        tomRecoil = -g_recoil_tom[tom_index];
        glTranslatef(0.0, tomRecoil, 0.0);
        glPushMatrix();
        glScalef(DRUMSET_TOM_SCALE_XZ[tom_index], DRUMSET_TOM_SCALE_Y[tom_index], DRUMSET_TOM_SCALE_XZ[tom_index]);
        g_drumSet_tom_ms3d->RenderModel();
        glPopMatrix();
        angle_tom = -0.5 * DRUMSET_TOM_ROT_Y[tom_index];
        glRotatef(angle_tom, 0.0, 1.0, 0.0);
        UpdateToms(tom_index);
        glPopMatrix();
    }
    // SNARE
    glPushMatrix();
    y_snare = 18.5 - g_recoil_snaredrum;
    glTranslatef(-10.64, y_snare, 7.6799998);
    glRotatef(45.0, 0.0, 1.0, 0.0);
    glRotatef(15.0, 1.0, 0.0, 0.0);
    g_drumSet_snareDrum_ms3d->RenderModel();
    UpdateSnareDrum();
    UpdateSideStick();
    glPopMatrix();
    // TIMBALES
    if (IsShowPercussionInstrument(HIGH_TIMBALE, g_recoil_timbale_hi)
        || IsShowPercussionInstrument(LOW_TIMBALE, g_recoil_timbale_lo)) {
        glPushMatrix();
        glTranslatef(-42.5, 50.0, 17.0);
        glRotatef(55.0, 0.0, 1.0, 0.0);
        glTranslatef(6.0999999, 0.0, 0.0);
        glRotatef(30.0, 1.0, 0.0, 0.0);
        yd = -g_recoil_timbale_hi;
        glTranslatef(0.0, yd, 0.0);
        glPushMatrix();
        glScalef(0.75, 0.75, 0.75); // make hi timable smaller
        g_drumSet_timbale_ms3d->RenderModel();
        glPopMatrix();
        glRotatef(-18.75, 0.0, 1.0, 0.0);
        UpdatePercussionStriker(HIGH_TIMBALE, g_recoil_timbale_hi, 3.0, g_drumSet_Stick_ms3d, 0, 0.0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-42.5, 50.0, 17.0);
        glRotatef(55.0, 0.0, 1.0, 0.0);
        glTranslatef(-6.0999999, 0.0, 0.0);
        glRotatef(30.0, 1.0, 0.0, 0.0);
        ye = -g_recoil_timbale_lo;
        glTranslatef(0.0, ye, 0.0);
        glPushMatrix();
        g_drumSet_timbale_ms3d->RenderModel();
        glPopMatrix();
        glRotatef(-18.75, 0.0, 1.0, 0.0);
        UpdatePercussionStriker(LOW_TIMBALE, g_recoil_timbale_lo, 3.0, g_drumSet_Stick_ms3d, 0, 0.0);
        glPopMatrix();
    }
    // WHISTLE, SHORT
    if (IsShowPercussionInstrument(SHORT_WHISTLE, g_recoil_whistle_short)) {
        glPushMatrix();
        yf = g_recoil_whistle_short + 40.5;
        glTranslatef(-8.5, yf, -8.0);
        glRotatef(15.5, 0.0, 1.0, 0.0);
        glRotatef(12.5, 1.0, 0.0, 0.0);
        g_whistle_ms3d->RenderModelShiny();
        glPopMatrix();
    }
    for (i = 0; i < 100; ++i) {
        if (g_particles[1].field_0[i] > 0.0) {
            glPushMatrix();
            glTranslatef(-8.5, 44.25, -8.0);
            glRotatef(15.5, 0.0, 1.0, 0.0);
            glRotatef(12.5, 1.0, 0.0, 0.0);
            yg = g_particles[1].field_0[i] / 20.0 * (g_particles[1].field_0[i] / 20.0)
                + g_particles[1].field_190[i]
                + g_particles[1].field_960[i];
            x = g_particles[1].field_7D0[i] + 0.0;
            glTranslatef(x, yg, g_particles[1].field_0[i]);
            v0 = 1.1 - g_particles[1].field_320[i] / 0.25;
            v43 = v0 + v0;
            glScalef(v43, v43, v43);
            glRotatef(g_particles[1].field_4B0[i], 1.0, 0.0, 0.0);
            glRotatef(g_particles[1].field_640[i], 0.0, 0.0, 1.0);
            g_steamCloud_0_ms3d->RenderModel();
            glPopMatrix();
        }
    }
    // WHISTLE, LONG
    if (IsShowPercussionInstrument(LONG_WHISTLE, g_recoil_whistle_long)) {
        glPushMatrix();
        yh = g_recoil_whistle_long + 37.5;
        glTranslatef(-3.5, yh, -8.0);
        glRotatef(15.5, 0.0, 1.0, 0.0);
        glRotatef(12.5, 1.0, 0.0, 0.0);
        g_whistle_ms3d->RenderModelShiny();
        glPopMatrix();
    }
    for (particleIndex = 0; particleIndex < 100; ++particleIndex) {
        if (g_particles[0].field_0[particleIndex] > 0.0) {
            glPushMatrix();
            glTranslatef(-3.5, 41.25, -8.0);
            glRotatef(15.5, 0.0, 1.0, 0.0);
            glRotatef(12.5, 1.0, 0.0, 0.0);
            yi = g_particles[0].field_0[particleIndex] / 20.0 * (g_particles[0].field_0[particleIndex] / 20.0)
                + g_particles[0].field_190[particleIndex]
                + g_particles[0].field_960[particleIndex];
            xa = g_particles[0].field_7D0[particleIndex] + 0.0;
            glTranslatef(xa, yi, g_particles[0].field_0[particleIndex]);
            v1 = 1.1 - g_particles[0].field_320[particleIndex] / 0.25;
            v41 = v1 + v1;
            glScalef(v41, v41, v41);
            glRotatef(g_particles[0].field_4B0[particleIndex], 1.0, 0.0, 0.0);
            glRotatef(g_particles[0].field_640[particleIndex], 0.0, 0.0, 1.0);
            g_steamCloud_1_ms3d->RenderModel();
            glPopMatrix();
        }
    }
    // BONGOS
    if (IsShowPercussionInstrument(HIGH_BONGO, g_recoil_bongo_hi)
        || IsShowPercussionInstrument(LOW_BONGO, g_recoil_bongo_lo)) {
        glPushMatrix();
        glTranslatef(-37.5, 40.0, 22.0);
        glRotatef(75.0, 0.0, 1.0, 0.0);
        glTranslatef(-3.8, 0.0, 0.0);
        glRotatef(30.0, 1.0, 0.0, 0.0);
        yj = -g_recoil_bongo_hi;
        glTranslatef(0.0, yj, 0.0);
        glPushMatrix();
        glScalef(0.89999998, 0.89999998, 0.89999998);
        g_drumSet_bongo_ms3d->RenderModel();
        glPopMatrix();
        glRotatef(-37.5, 0.0, 1.0, 0.0);
        glTranslatef(0.0, 0.0, -6.0);
        UpdatePercussionStriker(HIGH_BONGO, g_recoil_bongo_hi, 0.0, g_handLeft_ms3d, 0, 0.0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-37.5, 40.0, 22.0);
        glRotatef(75.0, 0.0, 1.0, 0.0);
        glTranslatef(3.8, 0.0, 0.0);
        glRotatef(30.0, 1.0, 0.0, 0.0);
        yk = -g_recoil_bongo_lo;
        glTranslatef(0.0, yk, 0.0);
        glPushMatrix();
        g_drumSet_bongo_ms3d->RenderModel();
        glPopMatrix();
        glRotatef(-18.75, 0.0, 1.0, 0.0);
        glTranslatef(0.0, 0.0, -6.0);
        UpdatePercussionStriker(LOW_BONGO, g_recoil_bongo_lo, 0.0, g_handRight_ms3d, 0, 0.0);
        glPopMatrix();
    }
    // CONGAS
    if (IsShowPercussionInstrument(OPEN_HIGH_CONGA, g_recoil_conga_high_open)
        || IsShowPercussionInstrument(MUTE_HIGH_CONGA, g_recoil_conga_high_mute)
        || IsShowPercussionInstrument(LOW_CONGA, g_recoil_conga_low)) {
        glPushMatrix();
        glTranslatef(-27.5, 35.0, 32.0);
        glRotatef(75.0, 0.0, 1.0, 0.0);
        glTranslatef(-7.8000002, 0.0, 0.0);
        glRotatef(10.0, 1.0, 0.0, 0.0);
        if (g_recoil_conga_high_open <= (double)g_recoil_conga_high_mute)
            y = -g_recoil_conga_high_mute;
        else
            y = -g_recoil_conga_high_open;
        glTranslatef(0.0, y, 0.0);
        glPushMatrix();
        glScalef(0.89999998, 0.89999998, 0.89999998);
        g_drumSet_conga_ms3d->RenderModel();
        glPopMatrix();
        glPushMatrix();
        glRotatef(-18.75, 0.0, 1.0, 0.0);
        glTranslatef(0.0, 0.0, -6.0);
        UpdatePercussionStriker(OPEN_HIGH_CONGA, g_recoil_conga_high_open, 0.0, g_handLeft_ms3d, 0, 0.0);
        glPopMatrix();
        glPushMatrix();
        glRotatef(-9.375, 0.0, 1.0, 0.0);
        glTranslatef(-3.0, 0.0, -12.0);
        UpdatePercussionStriker(MUTE_HIGH_CONGA, g_recoil_conga_high_mute, 0.0, g_handLeft_ms3d, 0, 0.0);
        glPopMatrix();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-27.5, 35.0, 32.0);
        glRotatef(75.0, 0.0, 1.0, 0.0);
        glTranslatef(7.8000002, 0.0, 0.0);
        glRotatef(10.0, 1.0, 0.0, 0.0);
        yl = -g_recoil_conga_low;
        glTranslatef(0.0, yl, 0.0);
        glPushMatrix();
        g_drumSet_conga_ms3d->RenderModel();
        glPopMatrix();
        glRotatef(-18.75, 0.0, 1.0, 0.0);
        glTranslatef(0.0, 0.0, -6.0);
        UpdatePercussionStriker(LOW_CONGA, g_recoil_conga_low, 0.0, g_handRight_ms3d, 0, 0.0);
        glPopMatrix();
    }
    // CLAP
    if (IsShowPercussionInstrument(HAND_CLAP, g_recoil_clap_hand_r)) {
        glPushMatrix();
        glTranslatef(0.0, 50.5, 25.0);
        glRotatef(-30.0, 1.0, 0.0, 0.0);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        glRotatef(90.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(HAND_CLAP, g_recoil_clap_hand_l, 0.0, g_handLeft_ms3d, 0, 0.0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.0, 45.5, 20.0);
        glRotatef(15.0, 1.0, 0.0, 0.0);
        glRotatef(-90.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(HAND_CLAP, g_recoil_clap_hand_r, 0.0, g_handRight_ms3d, 0, 0.0);
        glPopMatrix();
    }
    // TAMBOURINE
    if (IsShowPercussionInstrument(TAMBOURINE, g_recoil_tambourine_hand)) {
        glPushMatrix();
        glTranslatef(10.0, 50.5, 25.0);
        glRotatef(-30.0, 1.0, 0.0, 0.0);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        glRotatef(90.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(TAMBOURINE, g_recoil_tambourine, 0.0, g_handTambourine_ms3d, 0, 0.0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(10.0, 45.5, 20.0);
        glRotatef(15.0, 1.0, 0.0, 0.0);
        glRotatef(-90.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(TAMBOURINE, g_recoil_tambourine_hand, 0.0, g_handRight_ms3d, 0, 0.0);
        glPopMatrix();
    }
    // CLAVES
    if (IsShowPercussionInstrument(CLAVES, g_recoil_clave_l)) {
        glPushMatrix();
        glTranslatef(-10.0, 50.5, 25.0);
        glRotatef(-30.0, 1.0, 0.0, 0.0);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        glRotatef(90.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(CLAVES, g_recoil_clave_l, 0.0, g_clave_ms3d, 1, 0.0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-10.0, 45.5, 20.0);
        glRotatef(15.0, 1.0, 0.0, 0.0);
        glRotatef(-90.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(CLAVES, g_recoil_clave_l, 0.0, g_clave_ms3d, 1, 0.0);
        glPopMatrix();
    }
    //  -- STICKS --
    if (IsShowPercussionInstrument(STICKS, g_recoil_sticks_1)) {
        glPushMatrix();
        glTranslatef(-5.0, 40.5, 5.0);
        glRotatef(-30.0, 1.0, 0.0, 0.0);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        glRotatef(90.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(STICKS, g_recoil_sticks_1, 0.0, g_drumSet_Stick_ms3d, 1, 0.0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-5.0, 35.5, 0.0);
        glRotatef(15.0, 1.0, 0.0, 0.0);
        glRotatef(-90.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(STICKS, g_recoil_sticks_1, 0.0, g_drumSet_Stick_ms3d, 1, 0.0);
        glPopMatrix();
    }
    //  -- SHAKER --
    if (IsShowPercussionInstrument(SHAKER, g_recoil_shaker)) {
        glPushMatrix();
        glTranslatef(12.0, 42.5, 25.0);
        glRotatef(-25.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(SHAKER, g_recoil_shaker, 1.0, g_shaker_ms3d, 0, 0.0);
        glPopMatrix();
    }
    //  -- JINGLE BELL --
    if (IsShowPercussionInstrument(JINGLE_BELL, g_recoil_jingleBell)) {
        glPushMatrix();
        glTranslatef(17.5, 52.5, 15.0);
        glRotatef(45.0, 1.0, 0.0, 0.0);
        glRotatef(-45.0, 0.0, 1.0, 0.0);
        UpdatePercussionStriker(JINGLE_BELL, g_recoil_jingleBell, 1.0, g_jingleBells_ms3d, 0, 0.0);
        glPopMatrix();
    }
    //  -- CASTANETS --
    if (IsShowPercussionInstrument(CASTANETS, g_recoil_castanets)) {
        glPushMatrix();
        glTranslatef(27.5, 42.5, 15.0);
        glRotatef(-55.0, 0.0, 1.0, 0.0);
        UpdatePercussionStriker(CASTANETS, g_recoil_castanets, 1.0, g_castanets_ms3d, 0, 0.0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(27.5, 44.0, 15.0);
        glRotatef(-55.0, 0.0, 1.0, 0.0);
        glRotatef(180.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(CASTANETS, g_recoil_castanets, 1.0, g_castanets_ms3d, 0, 0.0);
        glPopMatrix();
    }
    //  -- HIGH Q --
    if (IsShowPercussionInstrument(HIGH_Q, g_recoil_highQ)) {
        glPushMatrix();
        glTranslatef(-15.0, 40.5, 5.0);
        glRotatef(140.0, 0.0, 1.0, 0.0);
        if (UpdatePercussionStriker(HIGH_Q, g_recoil_highQ, 0.0, g_zapper_ms3d, 0, 0.0))
            g_zapperLaser_ms3d->RenderModel();
        glPopMatrix();
    }
    //  -- METRONOME --
    if (IsShowPercussionInstrument(METRONOME_CLICK, g_recoil_metronomeClick)
        || IsShowPercussionInstrument(METRONOME_BELL, g_recoil_metronomeBell)) {
        glPushMatrix();
        glTranslatef(-20.0, 0.0, 35.0);
        glRotatef(23.0, 0.0, 1.0, 0.0);
        g_metronome_ms3d->RenderModel();
        // First pendulum (METRONOME CLICK)
        glPushMatrix();
        glTranslatef(0.0, 1.0, 1.0);
        clickOscillation = CalculatePendulumOscillation(METRONOME_CLICK, g_recoil_metronomeClick);
        clickSwingAmount = 1.0 - abs(clickOscillation);
        clickSwingDelta = clickSwingAmount - (1.0 - abs(g_prevClickOscillation));
        if (g_prevClickSwingDelta <= 0.0 && clickSwingDelta > 0.0)
            g_clickPendulumDirection = g_clickPendulumDirection == 0;
        g_prevClickSwingDelta = clickSwingDelta;
        g_prevClickOscillation = clickOscillation;
        if (g_clickPendulumDirection)
            anglec = -23.0 * clickSwingAmount;
        else
            anglec = 23.0 * clickSwingAmount;
        glRotatef(anglec, 0.0, 0.0, 1.0);
        g_metronomePendjulum1_ms3d->RenderModelShiny();
        glPopMatrix();
        // Second pendulum (METRONOME BELL)
        glPushMatrix();
        glTranslatef(0.0, 1.0, 0.375);
        bellOscillation = CalculatePendulumOscillation(METRONOME_BELL, g_recoil_metronomeBell);
        bellSwingAmount = 1.0 - abs(bellOscillation);
        bellSwingDelta = bellSwingAmount - (1.0 - abs(g_prevBellOscillation));
        if (g_prevBellSwingDelta <= 0.0 && bellSwingDelta > 0.0)
            g_bellPendulumDirection = g_bellPendulumDirection == 0;
        g_prevBellSwingDelta = bellSwingDelta;
        g_prevBellOscillation = bellOscillation;
        if (g_bellPendulumDirection)
            angle = -23.0 * bellSwingAmount;
        else
            angle = 23.0 * bellSwingAmount;
        glRotatef(angle, 0.0, 0.0, 1.0);
        g_metronomePendjulum2_ms3d->RenderModelShiny();
        glPopMatrix();
        glPopMatrix();
    }
    //  -- SQUARE CLICK --
    if (IsShowPercussionInstrument(SQUARE_CLICK, g_recoil_squareClick)) {
        glPushMatrix();
        glTranslatef(-35.0, 60.5, 5.0);
        glRotatef(-30.0, 1.0, 0.0, 0.0);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        glRotatef(90.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(SQUARE_CLICK, g_recoil_squareClick, 0.0, g_squareShaker_ms3d, 1, 0.0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-35.0, 55.5, 0.0);
        glRotatef(15.0, 1.0, 0.0, 0.0);
        glRotatef(-90.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(SQUARE_CLICK, g_recoil_squareClick, 0.0, g_drumSet_Stick_ms3d, 1, 0.0);
        glPopMatrix();
    }
    //  -- MARACAS --
    if (IsShowPercussionInstrument(MARACAS, g_recoil_Maracas)) {
        glPushMatrix();
        glTranslatef(-12.0, 62.5, 25.0);
        glRotatef(15.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(MARACAS, g_recoil_Maracas, 1.0, g_maraca_ms3d, 0, 0.0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-8.0, 60.5, 20.0);
        glRotatef(-15.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(MARACAS, g_recoil_Maracas, 1.0, g_maraca_ms3d, 0, 0.0);
        glPopMatrix();
    }
    //  -- CABASA --
    if (IsShowPercussionInstrument(CABASA, g_recoil_cabasa)) {
        glPushMatrix();
        glTranslatef(-10.0, 45.5, 10.0);
        glRotatef(40.0, 0.0, 0.0, 1.0);
        za = g_recoil_cabasa * 50.0;
        UpdatePercussionStriker(CABASA, g_recoil_cabasa, 1.0, g_cabasa_ms3d, 0, za);
        glPopMatrix();
    }
    //  -- COWBELL --
    if (IsShowPercussionInstrument(COWBELL, g_recoil_cowbell)) {
        glPushMatrix();
        y_cowbell = 40.5 - g_recoil_cowbell;
        glTranslatef(-9.5, y_cowbell, -7.0);
        glRotatef(25.0, 0.0, 1.0, 0.0);
        glRotatef(20.0, 1.0, 0.0, 0.0);
        g_cowbell_ms3d->RenderModelShiny();
        UpdatePercussionStriker(COWBELL, g_recoil_cowbell, 3.0, g_drumSet_Stick_ms3d, 0, 0.0);
        glPopMatrix();
    }
    //  -- AGOGOS --
    if (IsShowPercussionInstrument(HIGH_AGOGO, g_recoil_agogo_hi)
        || IsShowPercussionInstrument(LOW_AGOGO, g_recoil_agogo_lo)) {
        glPushMatrix();
        if (g_recoil_agogo_hi <= (double)g_recoil_agogo_lo)
            ya = 50.5 - g_recoil_agogo_lo;
        else
            ya = 50.5 - g_recoil_agogo_hi;
        glTranslatef(-5.5, ya, -8.0);
        g_agogo_ms3d->RenderModel();
        glTranslatef(3.4000001, -2.5, -2.25);
        UpdatePercussionStriker(HIGH_AGOGO, g_recoil_agogo_hi, 3.0, g_drumSet_Stick_ms3d, 0, 0.0);
        glTranslatef(4.5, 0.5, -1.25);
        UpdatePercussionStriker(LOW_AGOGO, g_recoil_agogo_lo, 3.0, g_drumSet_Stick_ms3d, 0, 0.0);
        glPopMatrix();
    }
    //  -- WOODBLOCK, HI --
    if (IsShowPercussionInstrument(HIGH_WOODBLOCK, g_recoil_woodblock_hi)) {
        glPushMatrix();
        yn = 40.5 - g_recoil_woodblock_hi;
        glTranslatef(-5.5, yn, -8.0);
        glRotatef(15.5, 0.0, 1.0, 0.0);
        glRotatef(12.5, 1.0, 0.0, 0.0);
        g_woodBlockHigh_ms3d->RenderModel();
        UpdatePercussionStriker(HIGH_WOODBLOCK, g_recoil_woodblock_hi, 3.0, g_drumSet_Stick_ms3d, 0, 0.0);
        glPopMatrix();
    }
    //  -- WOODBLOCK, LO --
    if (IsShowPercussionInstrument(LOW_WOODBLOCK, g_recoil_woodblock_lo)) {
        glPushMatrix();
        yo = 40.5 - g_recoil_woodblock_lo;
        glTranslatef(-0.5, yo, -9.0);
        glRotatef(3.0, 0.0, 1.0, 0.0);
        glRotatef(12.5, 1.0, 0.0, 0.0);
        g_woodBlockLow_ms3d->RenderModel();
        UpdatePercussionStriker(LOW_WOODBLOCK, g_recoil_woodblock_lo, 3.0, g_drumSet_Stick_ms3d, 0, 0.0);
        glPopMatrix();
    }
    // -- TRIANGLE, OPEN --
    if (IsShowPercussionInstrument(OPEN_TRIANGLE, g_recoil_triangle_open)) {
        glPushMatrix();
        yp = 50.5 - g_recoil_triangle_open;
        xb = -5.5 - g_recoil_triangle_open;
        glTranslatef(xb, yp, 10.0);
        glRotatef(15.5, 0.0, 1.0, 0.0);
        glRotatef(12.5, 1.0, 0.0, 0.0);
        g_triangle_ms3d->RenderModel();
        glTranslatef(1.05, 2.5899999, -7.0);
        glRotatef(-45.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(OPEN_TRIANGLE, g_recoil_triangle_open, 0.0, g_triangleStick_ms3d, 0, 0.0);
        glPopMatrix();
    }
    //  -- TRIANGLE, MUTED --
    if (IsShowPercussionInstrument(MUTE_TRIANGLE, g_recoil_triangle_muted)) {
        glPushMatrix();
        yq = 50.5 - g_recoil_triangle_muted;
        xc = 5.5 - g_recoil_triangle_muted;
        glTranslatef(xc, yq, 10.0);
        glRotatef(15.5, 0.0, 1.0, 0.0);
        glRotatef(12.5, 1.0, 0.0, 0.0);
        g_mutedTriangle_ms3d->RenderModel();
        glTranslatef(1.05, 2.5899999, -7.0);
        glRotatef(-45.0, 0.0, 0.0, 1.0);
        UpdatePercussionStriker(MUTE_TRIANGLE, g_recoil_triangle_muted, 0.0, g_triangleStick_ms3d, 0, 0.0);
        glPopMatrix();
    }
    if (g_hihat_isOpen == 1) {
        glPushMatrix();
        glTranslatef(-20.5, 23.5, 7.3540001);
        glRotatef(80.0, 0.0, 1.0, 0.0);
        glPushMatrix();
        glRotatef(180.0, 1.0, 0.0, 0.0);
        yr = g_recoil_hihat * 0.25;
        glTranslatef(0.0, yr, 0.0);
        glScalef(1.3, 1.3, 1.3);
        g_drumSet_cymbal_ms3d->RenderModelShiny();
        glPopMatrix();
        ys = MAX_RECOIL * 0.25 + 1.4949999 - g_recoil_hihat * 0.25;
        glTranslatef(0.0, ys, 0.0);
        UpdateHiHat();
        angled = g_cymbalCurrentWobble[6] * 9.0 / MAX_RECOIL;
        glRotatef(angled, 1.0, 0.0, 0.0);
    }
    else {
        glPushMatrix();
        yt = 23.5 - g_recoil_hihat * 0.5;
        glTranslatef(-20.5, yt, 7.3540001);
        glRotatef(80.0, 0.0, 1.0, 0.0);
        glPushMatrix();
        glRotatef(180.0, 1.0, 0.0, 0.0);
        glScalef(1.3, 1.3, 1.3);
        g_drumSet_cymbal_ms3d->RenderModelShiny();
        glPopMatrix();
        glTranslatef(0.0, 1.4949999, 0.0);
        UpdateHiHat();
    }
    glScalef(1.3, 1.3, 1.3);
    g_drumSet_cymbal_ms3d->RenderModelShiny(); // bottom hi-hat cymbal
    glPopMatrix();
    // CYMBALS
    for (j = 0; j < 6; ++j) {
        glPushMatrix();
        glTranslatef(CYMBAL_LOC_X[j], CYMBAL_LOC_Y[j], CYMBAL_LOC_Z[j]);
        glRotatef(g_cymbal_rot_y[j], 0.0, 1.0, 0.0);
        UpdateCymbals(j);
        wobble = g_cymbalRestingAngle[j] + g_cymbalCurrentWobble[j];
        glRotatef(wobble, 1.0, 0.0, 0.0);
        glScalef(CYMBAL_SCALE[j], CYMBAL_SCALE[j], CYMBAL_SCALE[j]);
        if (j == 5) // 5 = china
            g_drumSet_chinaCymbal_ms3d->RenderModelShiny();
        else
            g_drumSet_cymbal_ms3d->RenderModelShiny();
        glPopMatrix();
    }
}

void UpdateAllRecoils(const float scale) {
    __int16 i2; // [esp+0h] [ebp-48h]
    __int16 i1; // [esp+4h] [ebp-44h]
    __int16 nn; // [esp+8h] [ebp-40h]
    __int16 mm; // [esp+Ch] [ebp-3Ch]
    __int16 kk; // [esp+10h] [ebp-38h]
    __int16 jj; // [esp+14h] [ebp-34h]
    __int16 ii; // [esp+18h] [ebp-30h]
    __int16 n; // [esp+1Ch] [ebp-2Ch]
    __int16 m; // [esp+20h] [ebp-28h]
    __int16 k; // [esp+24h] [ebp-24h]
    __int16 j; // [esp+28h] [ebp-20h]
    float v12; // [esp+2Ch] [ebp-1Ch]
    __int16 i3; // [esp+30h] [ebp-18h]
    float v14; // [esp+34h] [ebp-14h]
    float v15; // [esp+34h] [ebp-14h]
    float recoil_scale_factor; // [esp+38h] [ebp-10h]
    __int16 i; // [esp+3Ch] [ebp-Ch]

    recoil_scale_factor = 0.050000001 * scale;
    if (g_recoil_snaredrum > 0.0) {
        g_recoil_snaredrum = g_recoil_snaredrum - recoil_scale_factor;
        if (g_recoil_snaredrum < 0.0)
            g_recoil_snaredrum = 0.0;
    }
    if (g_recoil_snare > 0.0) {
        g_recoil_snare = g_recoil_snare - recoil_scale_factor;
        if (g_recoil_snare < 0.0)
            g_recoil_snare = 0.0;
    }
    if (g_recoil_sideStick > 0.0) {
        g_recoil_sideStick = g_recoil_sideStick - recoil_scale_factor;
        if (g_recoil_sideStick < 0.0)
            g_recoil_sideStick = 0.0;
    }
    for (i = 0; i < 6; ++i) {
        if (g_recoil_tom[i] > 0.0) {
            g_recoil_tom[i] = g_recoil_tom[i] - recoil_scale_factor;
            if (g_recoil_tom[i] < 0.0)
                g_recoil_tom[i] = 0.0;
        }
    }
    if (g_recoil_bassdrum > 0.0) {
        g_recoil_bassdrum = g_recoil_bassdrum - recoil_scale_factor;
        if (g_recoil_bassdrum < 0.0)
            g_recoil_bassdrum = 0.0;
    }
    if (g_recoil_bassDrumArm < 45.0) {
        g_recoil_bassDrumArm = recoil_scale_factor * 30.0 + g_recoil_bassDrumArm;
        if (g_recoil_bassDrumArm >= 45.0)
            g_recoil_bassDrumArm = 45.0;
    }
    if (g_recoil_cowbell > 0.0) {
        g_recoil_cowbell = g_recoil_cowbell - recoil_scale_factor;
        if (g_recoil_cowbell < 0.0)
            g_recoil_cowbell = 0.0;
    }
    if (g_recoil_clap_hand_l > 0.0) {
        g_recoil_clap_hand_l = g_recoil_clap_hand_l - recoil_scale_factor;
        if (g_recoil_clap_hand_l < 0.0)
            g_recoil_clap_hand_l = 0.0;
    }
    if (g_recoil_clap_hand_r > 0.0) {
        g_recoil_clap_hand_r = g_recoil_clap_hand_r - recoil_scale_factor;
        if (g_recoil_clap_hand_r < 0.0)
            g_recoil_clap_hand_r = 0.0;
    }
    if (g_recoil_tambourine > 0.0) {
        g_recoil_tambourine = g_recoil_tambourine - recoil_scale_factor;
        if (g_recoil_tambourine < 0.0)
            g_recoil_tambourine = 0.0;
    }
    if (*(float*)&g_recoil_tambourine_hand > 0.0) {
        *(float*)&g_recoil_tambourine_hand = *(float*)&g_recoil_tambourine_hand - recoil_scale_factor;
        if (*(float*)&g_recoil_tambourine_hand < 0.0)
            g_recoil_tambourine_hand = 0;
    }
    if (g_recoil_sticks_1 > 0.0) {
        g_recoil_sticks_1 = g_recoil_sticks_1 - recoil_scale_factor;
        if (g_recoil_sticks_1 < 0.0)
            g_recoil_sticks_1 = 0.0;
    }
    if (g_recoil_shaker > 0.0) {
        g_recoil_shaker = g_recoil_shaker - recoil_scale_factor;
        if (g_recoil_shaker < 0.0)
            g_recoil_shaker = 0;
    }
    if (g_recoil_clave_l > 0.0) {
        g_recoil_clave_l = g_recoil_clave_l - recoil_scale_factor;
        if (g_recoil_clave_l < 0.0)
            g_recoil_clave_l = 0.0;
    }
    if (g_recoil_castanets > 0.0) {
        g_recoil_castanets = g_recoil_castanets - recoil_scale_factor;
        if (g_recoil_castanets < 0.0)
            g_recoil_castanets = 0.0;
    }
    if (g_recoil_jingleBell > 0.0) {
        g_recoil_jingleBell = g_recoil_jingleBell - recoil_scale_factor;
        if (g_recoil_jingleBell < 0.0)
            g_recoil_jingleBell = 0.0;
    }
    if (g_recoil_shaker > 0.0) {
        g_recoil_shaker = g_recoil_shaker - recoil_scale_factor;
        if (g_recoil_shaker < 0.0)
            g_recoil_shaker = 0.0;
    }
    if (g_recoil_highQ > 0.0) {
        g_recoil_highQ = g_recoil_highQ - recoil_scale_factor;
        if (g_recoil_highQ < 0.0)
            g_recoil_highQ = 0.0;
    }
    if (g_recoil_squareClick > 0.0) {
        g_recoil_squareClick = g_recoil_squareClick - recoil_scale_factor;
        if (g_recoil_squareClick < 0.0)
            g_recoil_squareClick = 0.0;
    }
    if (g_recoil_metronomeClick > 0.0) {
        g_recoil_metronomeClick = g_recoil_metronomeClick - recoil_scale_factor;
        if (g_recoil_metronomeClick < 0.0)
            g_recoil_metronomeClick = 0.0;
    }
    if (g_recoil_metronomeBell > 0.0) {
        g_recoil_metronomeBell = g_recoil_metronomeBell - recoil_scale_factor;
        if (g_recoil_metronomeBell < 0.0)
            g_recoil_metronomeBell = 0.0;
    }
    if (g_recoil_Maracas > 0.0) {
        g_recoil_Maracas = g_recoil_Maracas - recoil_scale_factor;
        if (g_recoil_Maracas < 0.0)
            g_recoil_Maracas = 0.0;
    }
    if (g_recoil_cabasa > 0.0) {
        g_recoil_cabasa = g_recoil_cabasa - recoil_scale_factor;
        if (g_recoil_cabasa < 0.0)
            g_recoil_cabasa = 0.0;
    }
    if (g_recoil_agogo_hi > 0.0) {
        g_recoil_agogo_hi = g_recoil_agogo_hi - recoil_scale_factor;
        if (g_recoil_agogo_hi < 0.0)
            g_recoil_agogo_hi = 0.0;
    }
    if (g_recoil_agogo_lo > 0.0) {
        g_recoil_agogo_lo = g_recoil_agogo_lo - recoil_scale_factor;
        if (g_recoil_agogo_lo < 0.0)
            g_recoil_agogo_lo = 0.0;
    }
    if (g_recoil_whistle_long > 0.0) {
        g_recoil_whistle_long = g_recoil_whistle_long - recoil_scale_factor;
        if (g_recoil_whistle_long < 0.0)
            g_recoil_whistle_long = 0.0;
    }
    if (g_recoil_whistle_short > 0.0) {
        g_recoil_whistle_short = g_recoil_whistle_short - (recoil_scale_factor + recoil_scale_factor);
        if (g_recoil_whistle_short < 0.0)
            g_recoil_whistle_short = 0.0;
    }
    if (g_recoil_woodblock_hi > 0.0) {
        g_recoil_woodblock_hi = g_recoil_woodblock_hi - recoil_scale_factor;
        if (g_recoil_woodblock_hi < 0.0)
            g_recoil_woodblock_hi = 0.0;
    }
    if (g_recoil_triangle_open > 0.0) {
        g_recoil_triangle_open = g_recoil_triangle_open - recoil_scale_factor;
        if (g_recoil_triangle_open < 0.0)
            g_recoil_triangle_open = 0.0;
    }
    if (g_recoil_triangle_muted > 0.0) {
        g_recoil_triangle_muted = g_recoil_triangle_muted - recoil_scale_factor;
        if (g_recoil_triangle_muted < 0.0)
            g_recoil_triangle_muted = 0.0;
    }
    if (g_recoil_woodblock_lo > 0.0) {
        g_recoil_woodblock_lo = g_recoil_woodblock_lo - recoil_scale_factor;
        if (g_recoil_woodblock_lo < 0.0)
            g_recoil_woodblock_lo = 0.0;
    }
    if (g_recoil_timbale_hi > 0.0) {
        g_recoil_timbale_hi = g_recoil_timbale_hi - recoil_scale_factor;
        if (g_recoil_timbale_hi < 0.0)
            g_recoil_timbale_hi = 0.0;
    }
    if (g_recoil_timbale_lo > 0.0) {
        g_recoil_timbale_lo = g_recoil_timbale_lo - recoil_scale_factor;
        if (g_recoil_timbale_lo < 0.0)
            g_recoil_timbale_lo = 0.0;
    }
    if (g_recoil_bongo_hi > 0.0) {
        g_recoil_bongo_hi = g_recoil_bongo_hi - recoil_scale_factor;
        if (g_recoil_bongo_hi < 0.0)
            g_recoil_bongo_hi = 0.0;
    }
    if (g_recoil_bongo_lo > 0.0) {
        g_recoil_bongo_lo = g_recoil_bongo_lo - recoil_scale_factor;
        if (g_recoil_bongo_lo < 0.0)
            g_recoil_bongo_lo = 0.0;
    }
    if (g_recoil_conga_high_mute > 0.0) {
        g_recoil_conga_high_mute = g_recoil_conga_high_mute - recoil_scale_factor;
        if (g_recoil_conga_high_mute < 0.0)
            g_recoil_conga_high_mute = 0.0;
    }
    if (g_recoil_conga_high_open > 0.0) {
        g_recoil_conga_high_open = g_recoil_conga_high_open - recoil_scale_factor;
        if (g_recoil_conga_high_open < 0.0)
            g_recoil_conga_high_open = 0.0;
    }
    if (g_recoil_conga_low > 0.0) {
        g_recoil_conga_low = g_recoil_conga_low - recoil_scale_factor;
        if (g_recoil_conga_low < 0.0)
            g_recoil_conga_low = 0.0;
    }
    for (j = 0; j < g_melodicTomCount; ++j) {
        g_melodicTom[j].recoilOffset = g_melodicTom[j].recoilOffset - recoil_scale_factor;
        if (g_melodicTom[j].recoilOffset < 0.0)
            g_melodicTom[j].recoilOffset = 0.0;
    }
    for (k = 0; k < g_agogosCount; ++k) {
        for (m = 0; m < 12; ++m) {
            g_agogos[k].recoilOffset[m] = g_agogos[k].recoilOffset[m] - recoil_scale_factor;
            if (g_agogos[k].recoilOffset[m] < 0.0)
                g_agogos[k].recoilOffset[m] = 0.0;
        }
    }
    for (n = 0; n < g_woodblocksCount; ++n) {
        for (ii = 0; ii < 12; ++ii) {
            g_woodblocks[n].recoilOffset[ii] = g_woodblocks[n].recoilOffset[ii] - recoil_scale_factor;
            if (g_woodblocks[n].recoilOffset[ii] < 0.0)
                g_woodblocks[n].recoilOffset[ii] = 0.0;
        }
    }
    for (jj = 0; jj < g_steelDrumCount; ++jj) {
        g_steelDrum[jj].recoilOffset = g_steelDrum[jj].recoilOffset - recoil_scale_factor;
        if (g_steelDrum[jj].recoilOffset < 0.0)
            g_steelDrum[jj].recoilOffset = 0.0;
    }
    for (kk = 0; kk < g_synthDrumCount; ++kk) {
        g_synthDrum[kk].recoilOffset = g_synthDrum[kk].recoilOffset - recoil_scale_factor;
        if (g_synthDrum[kk].recoilOffset < 0.0)
            g_synthDrum[kk].recoilOffset = 0.0;
    }
    for (mm = 0; mm < g_timpaniCount; ++mm) {
        g_timpani[mm].recoilOffset = g_timpani[mm].recoilOffset - recoil_scale_factor;
        if (g_timpani[mm].recoilOffset < 0.0)
            g_timpani[mm].recoilOffset = 0.0;
    }
    for (nn = 0; nn < g_taikoCount; ++nn) {
        g_taiko[nn].recoilOffset = g_taiko[nn].recoilOffset - recoil_scale_factor;
        if (g_taiko[nn].recoilOffset < 0.0)
            g_taiko[nn].recoilOffset = 0.0;
    }
    for (i1 = 0; i1 < g_telephoneCount; ++i1) {
        g_telephone[i1].field_4 = g_telephone[i1].field_4 - recoil_scale_factor;
        if (g_telephone[i1].field_4 < 0.0)
            g_telephone[i1].field_4 = 0.0;
    }
    for (i2 = 0; i2 < g_tubularBellsCount; ++i2) {
        g_tubularBells[i2].field_4 = g_tubularBells[i2].field_4 - recoil_scale_factor;
        if (g_tubularBells[i2].field_4 < 0.0)
            g_tubularBells[i2].field_4 = 0.0;
    }
    for (i3 = 0; i3 < 7; ++i3) {
        if (g_recoil_cymbals[i3] > 0.0) {
            v12 = 0.75 * scale;
            g_recoil_cymbals[i3] = g_recoil_cymbals[i3] - v12;
            if (g_recoil_cymbals[i3] < 0.0)
                g_recoil_cymbals[i3] = 0.0;
        }
        if (g_cymbalAngularVelocity[i3] != 0.0) {
            g_cymbalCurrentWobble[i3] = scale * g_cymbalAngularVelocity[i3] + g_cymbalCurrentWobble[i3];
            if (g_cymbalCurrentWobble[i3] <= 0.0) {
                if (g_cymbalCurrentWobble[i3] < 0.0) {
                    v15 = g_cymbalAngularVelocity[i3];
                    g_cymbalAngularVelocity[i3] = scale * g_cymbalWobbleAmplitude[i3] + v15;
                    if (v15 < 0.0 && g_cymbalAngularVelocity[i3] > 0.0)
                        g_cymbalWobbleAmplitude[i3] = g_cymbalWobbleAmplitude[i3] * 0.60000002;
                }
            }
            else {
                v14 = g_cymbalAngularVelocity[i3];
                g_cymbalAngularVelocity[i3] = v14 - scale * g_cymbalWobbleAmplitude[i3];
                if (v14 > 0.0 && g_cymbalAngularVelocity[i3] < 0.0)
                    g_cymbalWobbleAmplitude[i3] = g_cymbalWobbleAmplitude[i3] * 0.60000002;
            }
            g_cymbalAngularVelocity[i3] = g_cymbalAngularVelocity[i3] * 0.98000002;
            if (CYMBAL_WOBBLE_AMPLITUDE[i3] * -0.0099999998 <= g_cymbalAngularVelocity[i3]
                && CYMBAL_WOBBLE_AMPLITUDE[i3] * 0.0099999998 >= g_cymbalAngularVelocity[i3]
                && CYMBAL_WOBBLE_AMPLITUDE[i3] * -0.02 <= g_cymbalCurrentWobble[i3]
                && CYMBAL_WOBBLE_AMPLITUDE[i3] * 0.02 >= g_cymbalCurrentWobble[i3]) {
                g_cymbalAngularVelocity[i3] = 0.0;
                g_cymbalCurrentWobble[i3] = 0.0;
            }
            if (g_cymbalCurrentWobble[i3] > (double)CYMBAL_MAX_WOBBLE[i3])
                g_cymbalCurrentWobble[i3] = CYMBAL_MAX_WOBBLE[i3];
            if (-CYMBAL_MAX_WOBBLE[i3] > g_cymbalCurrentWobble[i3])
                g_cymbalCurrentWobble[i3] = -CYMBAL_MAX_WOBBLE[i3];
        }
    }
    if (g_recoil_hihat > 0.0) {
        g_recoil_hihat = g_recoil_hihat - recoil_scale_factor;
        if (g_recoil_hihat < 0.0)
            g_recoil_hihat = 0.0;
    }
}
