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
#include "instruments/Bass.h"
#include "instruments/Guitar.h"
#include "instruments/Harp.h"
#include "instruments/StageHorn.h"
#include "instruments/StageString.h"
#include "instruments/Xylophone.h"
#include "instruments/Trombone.h"

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------

extern int g_worldReady;
extern float g_cameraLocation[6];
extern int g_rotatingCameraIsActive;
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

static inline void UpdateAllRecoils(float) {
}

static inline bool IsCameraAngleViable_2() { return true; }
static inline bool IsCameraAngleViable_3() { return true; }
static inline bool IsCameraAngleViable_4() { return true; }
static inline bool IsCameraAngleViable_6() { return true; }

static inline void TriggerAutoCam() {
}

static inline void MoveCameraToAngle(CameraAngle, int) {
}

BOOL UpdateMidiJam() {
    GLfloat shadowX;
    GLfloat shadowZ;
    short m, k, j, i;
    GLfloat z;
    char headsUpDisplayText[104];

    if (g_worldReady != 1)
        return TRUE;

    glClear(0x4500u);
    glLoadIdentity();

    // --- Phase 1: Screen gradient (no depth test) ---
    glDisable(GL_DEPTH_TEST);
    g_screenGradient_ms3d->RenderModel();
    glEnable(GL_DEPTH_TEST);

    // --- Phase 2: Camera transform ---
    gluLookAt(
        g_cameraLocation[0], g_cameraLocation[1], g_cameraLocation[2],
        g_cameraLocation[3], g_cameraLocation[4], g_cameraLocation[5],
        0.0, 1.0, 0.0);

    if (g_rotatingCameraIsActive == 1) {
        glRotatef(g_rotatingCameraAngle, 0.0f, 1.0f, 0.0f);
        if (g_rotatingCameraDirection) {
            g_rotatingCameraAngle -= 0.025f;
            if (g_rotatingCameraAngle < -20.0f) {
                g_rotatingCameraAngle = -20.0f;
                if (++g_rotatingCameraIdleTime > 500) {
                    g_rotatingCameraIdleTime = 0;
                    g_rotatingCameraDirection = CLOCKWISE;
                }
            }
        }
        else {
            g_rotatingCameraAngle += 0.025f;
            if (g_rotatingCameraAngle > 20.0f) {
                g_rotatingCameraAngle = 20.0f;
                if (++g_rotatingCameraIdleTime > 500) {
                    g_rotatingCameraIdleTime = 0;
                    g_rotatingCameraDirection = ANTI_CLOCKWISE;
                }
            }
        }
    }

    // --- Phase 3: Stage ---
    glPushMatrix();
    glTranslatef(0.0f, -32.0f, -14.0f);
    g_stage_ms3d->RenderModel();
    glPopMatrix();

    // --- Phase 4: Shadows ---
    if (g_pianoVisible > 0) {
        glPushMatrix();
        glTranslatef(-50.0f, -32.0f, -20.0f);
        glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 5.0f);
        if (g_pianoVisible > 1) {
            z = (g_pianoVisible - 1) * 0.5f + 1.0f;
            glScalef(1.0f, 1.0f, z);
        }
        g_pianoShadow_ms3d->RenderModel();
        glPopMatrix();
    }

    if (g_xylophoneVisible > 0) {
        glPushMatrix();
        glTranslatef(-22.0f, -32.0f, 10.0f);
        glRotatef(32.0f, 0.0f, 1.0f, 0.0f);
        glScalef(0.64999998f, 0.64999998f, 0.64999998f);
        for (i = 0; i < g_xylophoneVisible; ++i) {
            if (i > 0) {
                glTranslatef(-2.0f, 0.0f, 0.0f);
                glRotatef(-18.0f, 0.0f, 1.0f, 0.0f);
                glTranslatef(0.0f, 3.0f, -23.0f);
            }
            g_xylophoneShadow_ms3d->RenderModel();
        }
        glPopMatrix();
    }

    if (g_bassVisible > 0) {
        for (j = 0; j < g_bassVisible; ++j) {
            glPushMatrix();
            shadowZ = j * -5.0f + -25.0f;
            shadowX = j * 5.0f + 50.0f;
            glTranslatef(shadowX, -32.0f, shadowZ);
            glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
            g_bassShadow_ms3d->RenderModel();
            glPopMatrix();
        }
    }

    if (g_guitarVisible > 0) {
        for (k = 0; k < g_guitarVisible; ++k) {
            glPushMatrix();
            shadowZ = k * -5.0f + -5.0f;
            shadowX = k * 5.0f + 43.0f;
            glTranslatef(shadowX, -32.0f, shadowZ);
            glRotatef(-50.0f, 0.0f, 1.0f, 0.0f);
            g_guitarShadow_ms3d->RenderModel();
            glPopMatrix();
        }
    }

    // if (g_drumsetVisible > 0) {
    //     glPushMatrix();
    //     glTranslatef(0.0f, -32.0f, -95.0f);
    //     g_drumShadow_ms3d->RenderModel();
    //     glPopMatrix();
    // }

    if (g_harpVisible > 0) {
        for (m = 0; m < g_harpVisible; ++m) {
            glPushMatrix();
            shadowX = m * 16.0f + 7.0f;
            glTranslatef(shadowX, -32.0f, 5.0f);
            glRotatef(-33.0f, 0.0f, 1.0f, 0.0f);
            g_harpShadow_ms3d->RenderModel();
            glPopMatrix();
        }
    }

    // --- Phase 5: Instruments (piano only; others stubbed) ---
    if (g_harp) {
        glPushMatrix();
        glTranslatef(7.0f, -28.4f, 5.0f);
        glRotatef(-33.0f, 0.0f, 1.0f, 0.0f);
        RenderHarp();
        glPopMatrix();
    }
    if (g_trombone) RenderTrombone();
    // if (g_trumpet) RenderTrumpet();
    // if (g_frenchHorn) RenderFrenchHorn();
    // if (g_baritoneSax) RenderBaritoneSax();
    // if (g_tenorSax) RenderTenorSax();
    // if (g_sapranoSax) RenderSapranoSax();
    // if (g_altoSax) RenderAltoSax();
    // if (g_recorder) RenderRecorder();
    // if (g_piccolo) RenderPiccolo();
    // if (g_flute) RenderFlute();
    // if (g_tuba) RenderTuba();
    if (g_stageHorn) RenderStageHorn();
    // if (g_whistles) RenderWhistles();
    // if (g_panPipe) RenderPanPipe();
    // if (g_harmonica) RenderHarmonica();
    // if (g_violin) RenderViolin();
    // if (g_viola) RenderViola();
    // if (g_cello) RenderCello();
    // if (g_doubleBass) RenderDoubleBass();
    // if (g_popBottle) RenderPopBottle();
    // if (g_agogos) RenderAgogo();
    // if (g_woodblocks) RenderWoodblock();
    // if (g_stageChoir) RenderStageChoir();
    if (g_accordion) RenderAccordion();
    if (g_stageString) RenderStageString();
    // if (g_pizzicatoStrings) RenderPizzicatoStrings();
    // if (g_ocarina) RenderOcarina();

    if (g_xylophone) {
        glPushMatrix();
        glTranslatef(-22.0f, -10.0f, 10.0f);
        glRotatef(32.0f, 0.0f, 1.0f, 0.0f);
        glScalef(0.64999998f, 0.64999998f, 0.64999998f);
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

    // if (g_melodicTom) RenderMelodicTom();
    // if (g_synthDrum) RenderSynthDrum();
    // if (g_steelDrum) RenderSteelDrum();
    // if (g_timpani) RenderTimpani();
    // if (g_taiko) RenderTaiko();
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

    // --- Phase 6: HUD ---
    sprintf(headsUpDisplayText, "%s fps:%0.2f", g_midiFileNameDisp, g_framesPerSecond);

    glPushMatrix();
    glLoadIdentity();
    g_songFillbarBox_ms3d->RenderModel();
    glTranslatef(-122.087f, 0.0f, 0.0f);
    glScalef(g_songFillbarScale, 1.0f, 1.0f);
    g_songFillbar_ms3d->RenderModel();
    glPopMatrix();

    RenderTextWithShadow(75, 460, 3, 3, headsUpDisplayText, 1, 0.5f, 0.5f);

    // --- Phase 7: Fadeout ---
    if (g_fadeFactor != 0.0f) {
        glPushMatrix();
        glLoadIdentity();
        gluLookAt(0.0, 0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        RenderFadeout();
        glPopMatrix();
    }

    glFlush();
    return TRUE;
}

// ---------------------------------------------------------------------------
// FUNCTION: MIDIJAM 0x4221E0
// ---------------------------------------------------------------------------
void __stdcall UpdateMidiJamMM(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) {
    if (g_worldReady != 1)
        return;

    float recoil_scale_factor = RECOIL_SCALE_FACTOR * 0.25f;
    REFERENCE_TIME prtNow = 0;
    MUSIC_TIME pmtNow = 0;

    if (g_directMusicSegmentPlayer->IsPlaying()) {
        g_DirectMusicPerformance->GetTime(&prtNow, &pmtNow);
        if (g_songFillbarScale < 1.0f) {
            g_songFillbarScale = static_cast<float>(pmtNow - g_mtStart) / static_cast<float>(g_midiFileDuration);
            if (g_songFillbarScale > 1.0f)
                g_songFillbarScale = 1.0f;
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

    // --- Phase 1: Instrument updates ---
    short anyInstrumentActive = 0;

    if (g_accordion && UpdateAccordion(pmtNow)) anyInstrumentActive = 1;
    if (g_harp && UpdateHarp(pmtNow)) anyInstrumentActive = 1;
    if (g_piano && UpdatePiano(pmtNow)) anyInstrumentActive = 1;
    if (g_xylophone && UpdateXylophone(pmtNow)) anyInstrumentActive = 1;
    // if (g_violin && UpdateViolin(pmtNow) == 1) anyInstrumentActive = 1;
    // if (g_viola && UpdateViola(pmtNow) == 1) anyInstrumentActive = 1;
    // if (g_cello && UpdateCello(pmtNow) == 1) anyInstrumentActive = 1;
    // if (g_doubleBass && UpdateDoubleBass(pmtNow) == 1) anyInstrumentActive = 1;
    if (g_bass && UpdateBass(pmtNow)) anyInstrumentActive = 1;
    if (g_guitar && UpdateGuitar(pmtNow)) anyInstrumentActive = 1;
    if (g_stageHorn && UpdateStageHorn(pmtNow)) anyInstrumentActive = 1;
    // if (g_whistles && UpdateWhistles(pmtNow)) anyInstrumentActive = 1;

    UpdateSteamPuffers();

    // if (g_panPipe && UpdatePanPipe(pmtNow)) anyInstrumentActive = 1;
    // if (g_harmonica && UpdateHarmonica(pmtNow)) anyInstrumentActive = 1;
    // if (g_popBottle && UpdatePopBottle(pmtNow)) anyInstrumentActive = 1;
    // if (g_agogos && UpdateAgogo(pmtNow)) anyInstrumentActive = 1;
    // if (g_woodblocks && UpdateWoodblocks(pmtNow)) anyInstrumentActive = 1;
    if (g_trombone && UpdateTrombone(pmtNow)) anyInstrumentActive = 1;
    // if (g_tuba && UpdateTuba(pmtNow)) anyInstrumentActive = 1;
    // if (g_frenchHorn && UpdateFrenchHorn(pmtNow)) anyInstrumentActive = 1;
    // if (g_ocarina && UpdateOcarina(pmtNow)) anyInstrumentActive = 1;
    // if (g_baritoneSax && UpdateBaritoneSax(pmtNow)) anyInstrumentActive = 1;
    // if (g_tenorSax && UpdateTenorSax(pmtNow)) anyInstrumentActive = 1;
    // if (g_sapranoSax && UpdateSapranoSax(pmtNow)) anyInstrumentActive = 1;
    // if (g_altoSax && UpdateAltoSax(pmtNow)) anyInstrumentActive = 1;
    // if (g_flute && UpdateFlute(pmtNow)) anyInstrumentActive = 1;
    // if (g_piccolo && UpdatePiccolo(pmtNow)) anyInstrumentActive = 1;
    // if (g_recorder && UpdateRecorder(pmtNow)) anyInstrumentActive = 1;
    // if (g_trumpet && UpdateTrumpet(pmtNow)) anyInstrumentActive = 1;
    // if (g_stageChoir && UpdateStageChoir(pmtNow)) anyInstrumentActive = 1;
    // if (g_musicBox && UpdateMusicBox(pmtNow)) anyInstrumentActive = 1;
    // if (g_melodicTom && UpdateMelodicTom(pmtNow)) anyInstrumentActive = 1;
    // if (g_synthDrum && UpdateSynthDrum(pmtNow)) anyInstrumentActive = 1;
    // if (g_steelDrum && UpdateSteelDrum(pmtNow)) anyInstrumentActive = 1;
    // if (g_timpani && UpdateTimpani(pmtNow)) anyInstrumentActive = 1;
    // if (g_taiko && UpdateTaiko(pmtNow)) anyInstrumentActive = 1;
    // if (g_telephone && UpdateTelephone(pmtNow)) anyInstrumentActive = 1;
    // if (g_tubularBells && UpdateTubularBells(pmtNow)) anyInstrumentActive = 1;
    if (g_stageString && UpdateStageString(pmtNow)) anyInstrumentActive = 1;
    // if (g_pizzicatoStrings && UpdatePizzicatoStrings(pmtNow)) anyInstrumentActive = 1;

    UpdateAllRecoils(recoil_scale_factor);

    // --- Phase 2: Percussion ---
    short anyPercussionActive = 0;
    if (g_show_percussion == 1) {
        for (short percussionIndex = 0; percussionIndex < 88; ++percussionIndex) {
            for (short i = 0; i < 32; ++i) {
                if (g_percussion_time_queue[percussionIndex][i] > 0) {
                    anyInstrumentActive = 1;
                    anyPercussionActive = 1;
                    g_percussion_framesWithEmptyQueue = 0;
                    g_percussion_time_queue[percussionIndex][i] -= pmtNow - g_currentGlobalTime;

                    if (g_percussion_time_queue[percussionIndex][i] <= 0) {
                        g_percussion_time_queue[percussionIndex][i] = 0;
                        int velocity = g_percussion_velocity_queue[percussionIndex][i];
                        float velocityFactor = (velocity * 0.75f + 32.0f + velocity * 0.75f + 32.0f) / 128.0f;

                        switch (static_cast<GM_PERCUSSION>(percussionIndex)) {
                            case ACOUSTIC_SNARE:
                            case ELECTRIC_SNARE:
                                ADD_RECOIL(g_recoil_snaredrum, velocityFactor, MAX_RECOIL);
                                ADD_RECOIL(g_recoil_snare, velocityFactor, MAX_RECOIL);
                                break;
                            case SIDE_STICK:
                                ADD_RECOIL(g_recoil_snaredrum, velocityFactor * 0.25f, MAX_RECOIL);
                                ADD_RECOIL(g_recoil_sideStick, velocityFactor, MAX_RECOIL);
                                break;
                            case HIGH_TOM:
                                ADD_RECOIL(g_recoil_tom[0], velocityFactor, MAX_RECOIL);
                                break;
                            case HIGH_MID_TOM:
                                ADD_RECOIL(g_recoil_tom[1], velocityFactor, MAX_RECOIL);
                                break;
                            case LOW_MID_TOM:
                                ADD_RECOIL(g_recoil_tom[2], velocityFactor, MAX_RECOIL);
                                break;
                            case LOW_TOM:
                                ADD_RECOIL(g_recoil_tom[3], velocityFactor, MAX_RECOIL);
                                break;
                            case HIGH_FLOOR_TOM:
                                ADD_RECOIL(g_recoil_tom[4], velocityFactor, MAX_RECOIL);
                                break;
                            case LOW_FLOOR_TOM:
                                ADD_RECOIL(g_recoil_tom[5], velocityFactor, MAX_RECOIL);
                                break;
                            case ACOUSTIC_BASS_DRUM:
                            case ELECTRIC_BASS_DRUM:
                                g_recoil_bassDrumArm = 0.0f;
                                ADD_RECOIL(g_recoil_bassdrum, velocityFactor, MAX_RECOIL);
                                break;
                            case COWBELL:
                                ADD_RECOIL(g_recoil_cowbell, velocityFactor, MAX_RECOIL);
                                break;
                            case HAND_CLAP:
                                ADD_RECOIL(g_recoil_clap_hand_r, velocityFactor, MAX_RECOIL);
                                ADD_RECOIL(g_recoil_clap_hand_l, velocityFactor, MAX_RECOIL);
                                break;
                            case TAMBOURINE:
                                ADD_RECOIL(g_recoil_tambourine_hand, velocityFactor, MAX_RECOIL);
                                ADD_RECOIL(g_recoil_tambourine, velocityFactor, MAX_RECOIL);
                                break;
                            case STICKS:
                                ADD_RECOIL(g_recoil_sticks_1, velocityFactor, MAX_RECOIL);
                                break;
                            case CLAVES:
                                ADD_RECOIL(g_pianoKeyOffsetX[0], velocityFactor, MAX_RECOIL);
                                ADD_RECOIL(g_recoil_clave_l, velocityFactor, MAX_RECOIL);
                                break;
                            case JINGLE_BELL:
                                ADD_RECOIL(g_recoil_jingleBell, velocityFactor, MAX_RECOIL);
                                break;
                            case CASTANETS:
                                ADD_RECOIL(g_recoil_castanets, velocityFactor, MAX_RECOIL);
                                break;
                            case SHAKER:
                                ADD_RECOIL(g_recoil_shaker, velocityFactor, MAX_RECOIL);
                                break;
                            case HIGH_Q:
                                ADD_RECOIL(g_recoil_highQ, velocityFactor, MAX_RECOIL);
                                break;
                            case SQUARE_CLICK:
                                ADD_RECOIL(g_recoil_squareClick, velocityFactor, MAX_RECOIL);
                                break;
                            case METRONOME_CLICK:
                                ADD_RECOIL(g_recoil_metronomeClick, velocityFactor, MAX_RECOIL);
                                break;
                            case METRONOME_BELL:
                                ADD_RECOIL(g_recoil_metronomeBell, velocityFactor, MAX_RECOIL);
                                break;
                            case MARACAS:
                                ADD_RECOIL(g_recoil_Maracas, velocityFactor, MAX_RECOIL);
                                break;
                            case CABASA:
                                ADD_RECOIL(g_recoil_cabasa, velocityFactor, MAX_RECOIL);
                                break;
                            case HIGH_AGOGO:
                                ADD_RECOIL(g_recoil_agogo_hi, velocityFactor, MAX_RECOIL);
                                break;
                            case LOW_AGOGO:
                                ADD_RECOIL(g_recoil_agogo_lo, velocityFactor, MAX_RECOIL);
                                break;
                            case HIGH_WOODBLOCK:
                                ADD_RECOIL(g_recoil_woodblock_hi, velocityFactor, MAX_RECOIL);
                                break;
                            case OPEN_TRIANGLE:
                                ADD_RECOIL(g_recoil_triangle_open, velocityFactor, MAX_RECOIL);
                                break;
                            case MUTE_TRIANGLE:
                                ADD_RECOIL(g_recoil_triangle_muted, velocityFactor, MAX_RECOIL);
                                break;
                            case LONG_WHISTLE:
                                g_recoil_whistle_long = MAX_RECOIL;
                                break;
                            case SHORT_WHISTLE:
                                g_recoil_whistle_short = MAX_RECOIL;
                                break;
                            case LOW_WOODBLOCK:
                                ADD_RECOIL(g_recoil_woodblock_lo, velocityFactor, MAX_RECOIL);
                                break;
                            case HIGH_TIMBALE:
                                ADD_RECOIL(g_recoil_timbale_hi, velocityFactor, MAX_RECOIL);
                                break;
                            case LOW_TIMBALE:
                                ADD_RECOIL(g_recoil_timbale_lo, velocityFactor, MAX_RECOIL);
                                break;
                            case HIGH_BONGO:
                                ADD_RECOIL(g_recoil_bongo_hi, velocityFactor, MAX_RECOIL);
                                break;
                            case LOW_BONGO:
                                ADD_RECOIL(g_recoil_bongo_lo, velocityFactor, MAX_RECOIL);
                                break;
                            case MUTE_HIGH_CONGA:
                                ADD_RECOIL(g_recoil_conga_high_mute, velocityFactor, MAX_RECOIL);
                                break;
                            case OPEN_HIGH_CONGA:
                                ADD_RECOIL(g_recoil_conga_high_open, velocityFactor, MAX_RECOIL);
                                break;
                            case LOW_CONGA:
                                ADD_RECOIL(g_recoil_conga_low, velocityFactor, MAX_RECOIL);
                                break;
                            case PEDAL_HI_HAT:
                            case CLOSED_HI_HAT:
                                g_hihat_isOpen = 0;
                                ADD_RECOIL(g_recoil_hihat, velocityFactor, MAX_RECOIL);
                                break;
                            default: {
                                // Cymbal handling — search the cymbal patch table
                                GM_PERCUSSION gmp = static_cast<GM_PERCUSSION>(percussionIndex);
                                for (short cymbalIndex = 0; cymbalIndex < 7; ++cymbalIndex) {
                                    bool isCymbalMatch = (percussionIndex == g_cymbal_patches[cymbalIndex]);
                                    bool isRideBell = (gmp == RIDE_BELL)
                                        && ((g_cymbal_patches[cymbalIndex] == RIDE_CYMBAL_1 && g_lastRideCymbal != 2)
                                            || (g_cymbal_patches[cymbalIndex] == RIDE_CYMBAL_2 && g_lastRideCymbal ==
                                                2));

                                    if (isCymbalMatch || isRideBell) {
                                        if (gmp == RIDE_CYMBAL_1) g_lastRideCymbal = 1;
                                        if (gmp == RIDE_CYMBAL_2) g_lastRideCymbal = 2;
                                        if (gmp == OPEN_HI_HAT) {
                                            g_hihat_isOpen = 1;
                                            ADD_RECOIL(g_recoil_hihat, velocityFactor, MAX_RECOIL);
                                        }

                                        if (gmp == RIDE_BELL) {
                                            ADD_RECOIL(g_recoil_cymbals[cymbalIndex], CYMBAL_MASS_FACTOR[cymbalIndex],
                                                       CYMBAL_MAX_WOBBLE[cymbalIndex]);
                                            g_cymbalMassFactor[cymbalIndex] = CYMBAL_SCALE[cymbalIndex] + 13.75f;
                                            if (g_cymbalCurrentWobble[cymbalIndex] < 0.0f)
                                                g_cymbalCurrentWobble[cymbalIndex] = 0.0f;
                                            g_cymbalCurrentWobble[cymbalIndex] +=
                                                g_percussion_velocity_queue[percussionIndex][i]
                                                * CYMBAL_MASS_FACTOR[cymbalIndex] / 128.0f * 0.75f;
                                            if (g_cymbalCurrentWobble[cymbalIndex] > CYMBAL_MAX_WOBBLE[cymbalIndex])
                                                g_cymbalCurrentWobble[cymbalIndex] = CYMBAL_MAX_WOBBLE[cymbalIndex];
                                            g_cymbalAngularVelocity[cymbalIndex] =
                                                g_percussion_velocity_queue[percussionIndex][i]
                                                * CYMBAL_ANGULAR_VELOCITY_FACTOR[cymbalIndex] / 128.0f * 0.75f;
                                        }
                                        else {
                                            ADD_RECOIL(g_recoil_cymbals[cymbalIndex], CYMBAL_MASS_FACTOR[cymbalIndex],
                                                       CYMBAL_MAX_WOBBLE[cymbalIndex]);
                                            g_cymbalMassFactor[cymbalIndex] = CYMBAL_SCALE[cymbalIndex] * 2.0f + 16.75f;
                                            if (g_cymbalCurrentWobble[cymbalIndex] < 0.0f)
                                                g_cymbalCurrentWobble[cymbalIndex] = 0.0f;
                                            // Cymbal was hit — apply full wobble
                                            g_cymbalCurrentWobble[cymbalIndex] +=
                                                g_percussion_velocity_queue[percussionIndex][i]
                                                * CYMBAL_MASS_FACTOR[cymbalIndex] / 128.0f;
                                            if (g_cymbalCurrentWobble[cymbalIndex] > CYMBAL_MAX_WOBBLE[cymbalIndex])
                                                g_cymbalCurrentWobble[cymbalIndex] = CYMBAL_MAX_WOBBLE[cymbalIndex];
                                            g_cymbalAngularVelocity[cymbalIndex] =
                                                g_percussion_velocity_queue[percussionIndex][i]
                                                * CYMBAL_ANGULAR_VELOCITY_FACTOR[cymbalIndex] / 128.0f;
                                        }

                                        if (g_cymbalAngularVelocity[cymbalIndex] < 0.0f)
                                            g_cymbalAngularVelocity[cymbalIndex] = CYMBAL_ANGULAR_VELOCITY_FACTOR[
                                                cymbalIndex] * 0.25f;
                                        if (g_cymbalAngularVelocity[cymbalIndex] > CYMBAL_ANGULAR_VELOCITY_FACTOR[
                                            cymbalIndex])
                                            g_cymbalAngularVelocity[cymbalIndex] = CYMBAL_ANGULAR_VELOCITY_FACTOR[
                                                cymbalIndex];
                                        if (g_recoil_cymbals[cymbalIndex] > CYMBAL_MAX_WOBBLE[cymbalIndex])
                                            g_recoil_cymbals[cymbalIndex] = CYMBAL_MAX_WOBBLE[cymbalIndex];
                                        g_cymbalWobbleAmplitude[cymbalIndex] = CYMBAL_WOBBLE_AMPLITUDE[cymbalIndex];
                                        break; // found the cymbal — exit the cymbal search loop
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }

        // if (!anyPercussionActive && ++g_percussion_framesWithEmptyQueue > 1000) {
        //     g_show_percussion = 0;
        //     g_drumsetVisible = 0;
        // }
    }

    // --- Phase 3: Shutdown / fade ---
    if (g_songFillbarScale >= 1.0f && !anyInstrumentActive && !g_isShuttingDown) {
        g_fadeFactor = 0.0f;
        g_isShuttingDown = 1;
    }

    g_currentGlobalTime = pmtNow;
    ++g_framesAlive;
    HandleKeyPresses();

    DWORD globalTimeMs = timeGetTime();
    float currentTimeMs = static_cast<float>(globalTimeMs - g_applicationStartTime);
    recoil_scale_factor = currentTimeMs / RECOIL_SCALE_FACTOR;
    g_applicationStartTime = globalTimeMs;

    if (g_isFadingIn == 1) {
        g_fadeFactor -= RECOIL_SCALE_FACTOR * 0.0024999999f;
        if (g_fadeFactor <= 0.0f) {
            g_fadeFactor = 0.0f;
            g_isFadingIn = 0;
            g_isShuttingDown = 0;
            PlaySegment();
        }
    }
    else if (g_isShuttingDown == 1) {
        g_fadeFactor += RECOIL_SCALE_FACTOR * 0.0024999999f;
        if (g_fadeFactor >= 1.0f) {
            g_fadeFactor = 1.0f;
            g_isFadingIn = 0;
            g_isShuttingDown = 0;
            g_killApplication = 1;
        }
    }

    if (g_killApplication == 1)
        g_killApplication_0 = 1;

    // --- Phase 4: Auto camera interpolation ---
    short cameraTransformComponentsAtTarget = 0;
    for (short patch = 0; patch < 6; ++patch) {
        float* loc = &g_cameraLocation[patch];
        float target = *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + patch);

        if (*loc >= target) {
            if (*loc <= target) {
                if (*loc == target)
                    ++cameraTransformComponentsAtTarget;
            }
            else {
                *loc += g_autoCamDeltaTransform[patch];
                if (*loc <= target) {
                    *loc = target;
                    ++cameraTransformComponentsAtTarget;
                }
            }
        }
        else {
            *loc += g_autoCamDeltaTransform[patch];
            if (*loc >= target) {
                *loc = target;
                ++cameraTransformComponentsAtTarget;
            }
        }
    }

    if (cameraTransformComponentsAtTarget == 6)
        g_autoCamIsIdle = 1;

    ++g_framesSinceStart;
    if (!(++g_autoCamIdleTime % 1000)
        || (!(g_autoCamIdleTime % 200)
            && (((g_targetCameraAngle == CAMERA_2A || g_targetCameraAngle == CAMERA_2B) && !IsCameraAngleViable_2())
                || ((g_targetCameraAngle == CAMERA_3A || g_targetCameraAngle == CAMERA_3B) && !IsCameraAngleViable_3())
                || ((g_targetCameraAngle == CAMERA_4A || g_targetCameraAngle == CAMERA_4B) && !IsCameraAngleViable_4())
                || (g_targetCameraAngle == CAMERA_6 && !IsCameraAngleViable_6())))) {
        TriggerAutoCam();
    }
}

// ---------------------------------------------------------------------------
// FUNCTION: MIDIJAM 0x421A30
// ---------------------------------------------------------------------------
void HandleKeyPresses() {
    g_shiftPressed = g_keyStateArray[VK_SHIFT] != 0;

    // Space — play/stop toggle
    if (g_keyStateArray[VK_SPACE]) {
        if (!g_isSpacePressed) {
            g_isSpacePressed = 1;
            if (g_directMusicSegmentPlayer->IsPlaying())
                g_directMusicSegmentPlayer->Stop(4096);
            else
                PlaySegment();
        }
    }
    else {
        g_isSpacePressed = 0;
    }

    // A key (unused)
    if (g_keyStateArray['A']) {
        if (!g_isAKeyPressed)
            g_isAKeyPressed = 1;
    }
    else {
        g_isAKeyPressed = 0;
    }

    // Right arrow — speed increase
    if (g_keyStateArray[VK_RIGHT]) {
        if (!g_framesKeyRightPressed) {
            g_playbackSpeed += 1.0f;
            if (g_playbackSpeed > 10.0f)
                g_playbackSpeed = 10.0f;
            g_DirectMusicPerformance->SetGlobalParam(GUID_PERF_MASTER_TEMPO, &g_playbackSpeed, 4);
            g_framesKeyRightPressed = 1;
        }
    }
    else {
        g_framesKeyRightPressed = 0;
    }

    // Left arrow — speed decrease
    if (g_keyStateArray[VK_LEFT]) {
        if (!g_framesKeyLeftPressed) {
            g_playbackSpeed -= 1.0f;
            if (g_playbackSpeed < 1.0f)
                g_playbackSpeed = 1.0f;
            g_DirectMusicPerformance->SetGlobalParam(GUID_PERF_MASTER_TEMPO, &g_playbackSpeed, 4);
            g_framesKeyLeftPressed = 1;
        }
    }
    else {
        g_framesKeyLeftPressed = 0;
    }

    // Up arrow
    if (g_keyStateArray[VK_UP]) {
        if (!g_framesKeyUpPressed)
            g_framesKeyUpPressed = 1;
    }
    else {
        g_framesKeyUpPressed = 0;
    }

    // Down arrow
    if (g_keyStateArray[VK_DOWN]) {
        if (!g_framesKeyDownPressed)
            g_framesKeyDownPressed = 1;
    }
    else {
        g_framesKeyDownPressed = 0;
    }

    DEBOUNCE_KEY(g_framesKeyRightPressed, 50);
    DEBOUNCE_KEY(g_framesKeyLeftPressed, 50);
    DEBOUNCE_KEY(g_framesKeyUpPressed, 50);
    DEBOUNCE_KEY(g_framesKeyDownPressed, 50);

    // Escape — stop and begin shutdown
    if (g_keyStateArray[VK_ESCAPE]) {
        g_directMusicSegmentPlayer->Stop(4096);
        g_isShuttingDown = 1;
    }

    // Camera keys
    if (g_keyStateArray['1']) {
        g_targetCameraAngle = static_cast<CameraAngle>(static_cast<short>(g_targetCameraAngle) + 1);
        g_rotatingCameraIsActive = 0;
        if (g_targetCameraAngle > CAMERA_1C)
            g_targetCameraAngle = CAMERA_1A;
        MoveCameraToAngle(g_targetCameraAngle, 0);
        g_keyStateArray['1'] = 0;
    }
    if (g_keyStateArray['2']) {
        g_targetCameraAngle = (g_targetCameraAngle == CAMERA_2A) ? CAMERA_2B : CAMERA_2A;
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(g_targetCameraAngle, 0);
        g_keyStateArray['2'] = 0;
    }
    if (g_keyStateArray['3']) {
        g_targetCameraAngle = (g_targetCameraAngle == CAMERA_3A) ? CAMERA_3B : CAMERA_3A;
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(g_targetCameraAngle, 0);
        g_keyStateArray['3'] = 0;
    }
    if (g_keyStateArray['4']) {
        g_targetCameraAngle = (g_targetCameraAngle == CAMERA_4A) ? CAMERA_4B : CAMERA_4A;
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(g_targetCameraAngle, 0);
        g_keyStateArray['4'] = 0;
    }
    if (g_keyStateArray['5']) {
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(CAMERA_5, 0);
        g_keyStateArray['5'] = 0;
    }
    if (g_keyStateArray['6']) {
        g_targetCameraAngle = CAMERA_6;
        g_rotatingCameraIsActive = 0;
        MoveCameraToAngle(CAMERA_6, 0);
        g_keyStateArray['6'] = 0;
    }
    if (g_keyStateArray['9']) {
        g_targetCameraAngle = CAMERA_1A;
        g_rotatingCameraIsActive = 1;
        g_rotatingCameraAngle = 0.0f;
        g_rotatingCameraDirection = CLOCKWISE;
        g_rotatingCameraIdleTime = 0;
        MoveCameraToAngle(CAMERA_1A, 0);
        g_keyStateArray['9'] = 0;
    }
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
