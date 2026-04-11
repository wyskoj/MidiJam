//
// Created by Jacob on 3/18/2026.
//

#include <windows.h>
#include <mmsystem.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>

#include "audio/initialize.h"
#include "audio/playback.h"
#include "instruments/Bass.h"
#include "instruments/Piano.h"
#include "instruments/keys.h"
#include "macros.h"
#include "instruments/Accordion.h"
#include "instruments/Agogos.h"
#include "instruments/AltoSax.h"
#include "instruments/BaritoneSax.h"
#include "instruments/FrenchHorn.h"
#include "instruments/Guitar.h"
#include "instruments/Harp.h"
#include "instruments/MelodicTom.h"
#include "instruments/PizzicatoStrings.h"
#include "instruments/SapranoSax.h"
#include "instruments/StageHorn.h"
#include "instruments/StageString.h"
#include "instruments/SynthDrum.h"
#include "instruments/Taiko.h"
#include "instruments/TenorSax.h"
#include "instruments/Timpani.h"
#include "instruments/Trumpet.h"
#include "instruments/Tuba.h"
#include "instruments/TubularBells.h"
#include "instruments/Xylophone.h"
#include "model/Ms3dBundle.h"
#include "render/texture.h"
#include "render/window.h"
#include "scene/camera.h"
#include "scene/update.h"

// ---------------------------------------------------------------------------
// Forward declarations for functions not yet transcribed
// ---------------------------------------------------------------------------
void MidiJamInitialize();
void MidiJamWindowCleanup();
int MidiJamMain(const char* title, HINSTANCE hInstance, FILE* hwfStream, void* pHwfAppendix, int nHwfAppendixItems);
int LoadAndPlayMidiFile(const char* filename);
int SwapBuffers_0();
float arctan(float x);

// ---------------------------------------------------------------------------
// Extern globals defined in globals.cpp
// ---------------------------------------------------------------------------
extern HINSTANCE g_hInstance;
extern int g_worldReady;
extern int g_killApplication_0;
extern DWORD g_applicationStartTime;
#if _MSC_VER < 1400
extern long g_lastUnixEpochTime;
#else
extern time_t g_lastUnixEpochTime;
#endif
extern float g_framesPerSecond;
extern GLfloat g_fadeFactor;
extern int g_isFadingIn;
extern int g_isShuttingDown;
extern int g_killApplication;
extern char g_isWindowActive;
extern FILE* g_hwfStream;
extern void* g_pHwfAppendix;
extern unsigned int g_nHwfAppendixItems;
extern char g_workingDirectory[1000];
extern int g_windowWidth;
extern float g_normalizedWindowScale;
extern int g_windowCenter_X;
extern int g_windowCenter_Y;
extern int g_windowCenterX;
extern int g_windowCenterY;
extern MMRESULT g_timerEventId;
extern CameraAngle g_targetCameraAngle;
extern float g_autoCamDeltaTransform[6];
extern int g_autoCamIsIdle;
extern int g_autoCamIdleTime;
extern int g_autoCameraIsActive;
extern int g_framesSinceStart;
extern int g_framesAlive;
extern float g_playbackSpeed;
extern float RECOIL_SCALE_FACTOR;

// Cymbal geometry
extern float CYMBAL_LOC_X[7];
extern float CYMBAL_LOC_Z[7];
extern float g_cymbal_rot_y[7];
extern float g_cymbalRestingAngle[7];
extern short g_latinSquare[6][6];

// Piano key geometry
extern GLfloat g_pianoKeyOffsetX[14];
extern GLfloat g_pianoKeyBackScale[14];

// Vibrating string
extern int VIBRATING_STRING_ANIM_SEQUENCE[8];
extern int g_vibratingString_frame;
extern int g_vibratingString_frameIndex;
extern int g_isEvenFrame;

// Instrument-specific data tables (named when transcribed)
extern short word_46B2D0[];
extern short word_4688C0[];
extern short word_46BBB0[];
extern short word_468258[];
extern float flt_468BF4[];
extern float flt_4654A0[];
extern float flt_45EAD0[];
extern float flt_4679E0[];
extern int unk_464F68;
extern int unk_464F6C;
extern int unk_464F70;
extern int unk_464F74;
extern int unk_464F78;
extern int unk_464F7C;
extern int unk_464F80;
extern int unk_464F84;
extern int unk_464F88;
extern int unk_464F8C;
extern int unk_464F90;
extern int unk_464F94;
extern int unk_464F98;
extern char g_midiFileName[]; // char array, g_midiFileName[0] is first byte
extern char byte_461DD6[]; // alias into the same buffer, used for quote stripping

// Scene / render model globals
extern Ms3dBundle* g_screenGradient_ms3d;
extern Ms3dBundle* g_songFillbarBox_ms3d;
extern Ms3dBundle* g_songFillbar_ms3d;
extern Ms3dBundle* g_stage_ms3d;
extern Ms3dBundle* g_pianoShadow_ms3d;

extern Ms3dBundle* g_guitarShadow_ms3d;
extern Ms3dBundle* g_drumShadow_ms3d;
extern Ms3dBundle* g_pianoStand_ms3d;
extern Ms3dBundle* g_woodBlockSingle_ms3d;

// Drum set models
extern Ms3dBundle* g_drumSet_Stick_ms3d;
extern Ms3dBundle* g_cowbell_ms3d;
extern Ms3dBundle* handRight_ms3d;
extern Ms3dBundle* handLeft_ms3d;
extern Ms3dBundle* handTambourine_ms3d;
extern Ms3dBundle* clave_ms3d;
extern Ms3dBundle* jingleBells_ms3d;
extern Ms3dBundle* castanets_ms3d;
extern Ms3dBundle* shaker_ms3d;
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
extern Ms3dBundle* g_drumSet_Timbale_ms3d;
extern Ms3dBundle* g_drumSet_bongo_ms3d;
extern Ms3dBundle* g_drumSet_conga_ms3d;
extern Ms3dBundle* g_drumSet_bassDrum_ms3d;
extern Ms3dBundle* g_drumSet_snareDrum_ms3d;
extern Ms3dBundle* g_drumSet_tom_ms3d;
extern Ms3dBundle* g_drumSet_cymbal_ms3d;
extern Ms3dBundle* g_drumSet_chinaCymbal_ms3d;
extern Ms3dBundle* g_drumSet_BassDrumBeaterArm_ms3d;
extern Ms3dBundle* g_drumSet_BassDrumBeaterHolder_ms3d;
extern Ms3dBundle* g_drumSet_BassDrumPedal_ms3d;

// Percussion / melodic drums
extern Ms3dBundle* g_steelDrum_recoil; // IDA named this confusingly; it's the steel drum body model
extern Ms3dBundle* g_steelDrumMallet_ms3d;
extern Ms3dBundle* metronome_ms3d;
extern Ms3dBundle* metronomePendjulum1_ms3d;
extern Ms3dBundle* metronomePendjulum2_ms3d;

// Music box
extern Ms3dBundle* g_musicBoxSpindle_ms3d;
extern Ms3dBundle* g_musicBoxPoint_ms3d;
extern Ms3dBundle* g_musicBoxKey_ms3d;
extern Ms3dBundle* g_musicBoxTopBlade_ms3d;
extern Ms3dBundle* g_musicBoxCase_ms3d;

// String instruments
extern Ms3dBundle* whistle_ms3d;
extern Ms3dBundle* g_panPipe_ms3d;
extern Ms3dBundle* g_calliope_ms3d;
extern Ms3dBundle* g_harmonica_ms3d;
extern Ms3dBundle* g_popBottle_ms3d;
extern Ms3dBundle* g_popBottleMiddle_ms3d;
extern Ms3dBundle* g_popBottlePop_ms3d;
extern Ms3dBundle* popBottleLabel_ms3d;
extern Ms3dBundle* steamCloud_0_ms3d;
extern Ms3dBundle* steamCloud_1_ms3d;
extern Ms3dBundle* steamCloud_2_ms3d;
extern Ms3dBundle* g_steamPuff_harmonica_ms3d;
extern Ms3dBundle* g_violin_ms3d;
extern Ms3dBundle* g_viola_ms3d;
extern Ms3dBundle* g_cello_ms3d;
extern Ms3dBundle* g_doubleBass_ms3d;
extern Ms3dBundle* g_violinString_ms3d;
extern Ms3dBundle* g_violinStringPlayedX_ms3d[5]; // TODO: verify array type — IDA accessed via vtable offset
extern Ms3dBundle* g_violinFinger_ms3d; // [0] — telephone keys start at [0] per IDA
extern Ms3dBundle* g_violinBow_ms3d;

// Choir
extern Ms3dBundle* g_stageChoir_ms3d;

// Flute / piccolo / recorder
extern Ms3dBundle* g_flute_ms3d;
extern Ms3dBundle* g_piccolo_ms3d;
extern Ms3dBundle* g_flute_leftHandX_ms3d[13];
extern Ms3dBundle* g_flute_rightHandX_ms3d[12];
extern Ms3dBundle* g_recorder_ms3d;
extern Ms3dBundle* g_recorderLeftHandX_ms3d[13];
extern Ms3dBundle* g_recorderRightHandX_ms3d; // [0..10] via pointer arithmetic
extern Ms3dBundle* dword_465230; // RecorderHandRight10 — TODO: name

// Telephone keys — IDA stored these via pointer arithmetic off g_violinFinger_ms3d
extern Ms3dBundle* g_telephoneBase_ms3d;
extern Ms3dBundle* g_telephoneHandle_ms3d;
extern Ms3dBundle* g_telephoneKeyX_ms3d; // [0..23] via pointer arithmetic

// TODO: the following two are accessed via dword_ in IDA, name when telephone is transcribed
extern Ms3dBundle* dword_46D25C;
extern Ms3dBundle* dword_46D260;
extern Ms3dBundle* dword_46D264;

// Ocarina
extern Ms3dBundle* g_ocarina_ms3d;
extern Ms3dBundle* g_ocarinaHandX_ms3d; // [0..11] via pointer arithmetic

// Trombone
extern Ms3dBundle* g_trombone_ms3d;
extern Ms3dBundle* g_tromboneSlide_ms3d;

// Piano
extern PianoModels g_pianoModels[4];

// Particle system
extern void* g_ds_particles; // TODO: type when transcribed

//

extern __int16 word_46CEE0[23][6];

// FUNCTION: MIDIJAM 0x413920
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    g_killApplication_0 = 0;
    g_worldReady = 0;
    g_applicationStartTime = timeGetTime();

    // --- Latin square init ---
    for (short i = 0; i < 6; ++i)
        for (short j = 0; j < 6; ++j)
            g_latinSquare[i][j] = (i + j) % 6;

    g_keysOffset = 0.0f;

    // --- Cymbal azimuth precompute ---
    for (short i = 0; i < 7; ++i) {
        float cymbalAzimuth = CYMBAL_LOC_X[i] / (CYMBAL_LOC_Z[i] - 24.0f);
        g_cymbal_rot_y[i] = arctan(cymbalAzimuth) * 57.29578f;
        g_cymbalRestingAngle[i] = 20.0f;
    }

    // --- Guitar/Bass note table init ---
    for (short i = 0; i < 23; ++i) {
        g_bassNotes[i][0] = i + 7;
        g_bassNotes[i][1] = i + 12;
        g_bassNotes[i][2] = i + 17;
        g_bassNotes[i][3] = i + 22;
        flt_468BF4[i + 1] = BASS_FRET_HEIGHTS[i + 1] / -46.081001;
        word_46CEE0[i][0] = i + 19;
        word_46CEE0[i][1] = i + 24;
        word_46CEE0[i][2] = i + 29;
        word_46CEE0[i][3] = i + 34;
        word_46CEE0[i][4] = i + 38;
        word_46CEE0[i][5] = i + 43;
        GUITAR_FRET_HEIGHTS_AS_PERCENT[i] = GUITAR_FRET_HEIGHTS[i + 1] / -34.700001;
    }

    for (short i = 0; i < 18; ++i) {
        // word_46B2D0[4 * i] = i + 34;
        // word_46B2D2[4 * i] = i + 41;
        // word_46B2D4[4 * i] = i + 48;
        // word_46B2D6[4 * i] = i + 55;
        // word_4688C0[4 * i] = i + 27;
        // word_4688C2[4 * i] = i + 34;
        // word_4688C4[4 * i] = i + 41;
        // word_4688C6[4 * i] = i + 48;
    }

    // --- Wind instrument note table init ---
    for (short i = 0; i < 28; ++i) {
        word_46BBB0[4 * i + 0] = i + 15;
        word_46BBB0[4 * i + 1] = i + 22;
        word_46BBB0[4 * i + 2] = i + 29;
        word_46BBB0[4 * i + 3] = i + 36;
    }

    // --- Xylophone/vibraphone note table and fret height init ---
    float v603 = 0.0f;
    float v612 = 0.052499998f;
    for (short i = 0; i < 49; ++i) {
        word_468258[4 * i + 0] = i + 7;
        word_468258[4 * i + 1] = i + 12;
        word_468258[4 * i + 2] = i + 17;
        word_468258[4 * i + 3] = i + 22;
        flt_4679E0[i] = v603 * 0.80000001f;
        v603 += v612;
        v612 *= 0.94999999f;
    }

    // --- Vibrating string animation ---
    g_vibratingString_frame = VIBRATING_STRING_ANIM_SEQUENCE[0];
    g_vibratingString_frameIndex = 0;
    g_isEvenFrame = 0;

    // --- Piano key offset ---
    for (short i = 0; i < 88; ++i)
        g_keysOffset += KEY_WIDTHS[KEY_SHAPE_INDEX[i]];
    g_keysOffset *= 0.5f;

    // --- Clave / recoil table init ---
    g_pianoKeyOffsetX[1] = -0.20833333f;
    g_pianoKeyBackScale[1] = 0.58333331f;
    g_pianoKeyOffsetX[2] = 0.0f;
    g_pianoKeyBackScale[2] = 0.5f;
    g_pianoKeyOffsetX[3] = 0.20833333f;
    g_pianoKeyBackScale[3] = 0.58333331f;
    g_pianoKeyOffsetX[4] = -0.25f;
    g_pianoKeyBackScale[4] = 0.5f;
    g_pianoKeyOffsetX[5] = -0.083333336f;
    g_pianoKeyBackScale[5] = 0.5f;
    g_pianoKeyOffsetX[6] = 0.083333336f;
    g_pianoKeyBackScale[6] = 0.5f;
    g_pianoKeyOffsetX[7] = 0.25f;
    g_pianoKeyBackScale[7] = 0.5f;
    g_pianoKeyOffsetX[8] = 0.0f;
    g_pianoKeyBackScale[8] = 1.0f;
    g_pianoKeyOffsetX[9] = -0.125f;
    g_pianoKeyBackScale[9] = 1.0f;
    g_pianoKeyOffsetX[10] = 0.125f;
    g_pianoKeyBackScale[10] = 1.0f;
    g_pianoKeyOffsetX[11] = -0.25f;
    g_pianoKeyBackScale[11] = 1.0f;
    g_pianoKeyOffsetX[12] = 0.0f;
    g_pianoKeyBackScale[12] = 1.0f;
    g_pianoKeyOffsetX[13] = 0.25f;
    g_pianoKeyBackScale[13] = 1.0f;

    // --- Misc state init ---
    g_windowCenterX = 0;
    g_windowCenterY = 0;
    g_framesSinceStart = 0;
    g_autoCamIdleTime = 0;
    g_fadeFactor = 1.0f;
    g_isFadingIn = 1;
    g_isShuttingDown = 0;
    g_killApplication = 0;

    // --- Working directory ---
    GetModuleFileNameA(nullptr, g_workingDirectory, 1000u);
    while (strlen(g_workingDirectory) && g_workingDirectory[strlen(g_workingDirectory) - 1] != '\\')
        g_workingDirectory[strlen(g_workingDirectory) - 1] = 0;
    if (g_workingDirectory[strlen(g_workingDirectory) - 1] == '\\')
        g_workingDirectory[strlen(g_workingDirectory) - 1] = 0;

    g_lastUnixEpochTime = time(nullptr);

    // --- HWF file ---
    char hwfPath[1000];
    sprintf(hwfPath, "%s\\MidiJam.HWF", g_workingDirectory);
    g_hwfStream = fopen(hwfPath, "rb");
    g_pHwfAppendix = ReadHwfAppendix(static_cast<char*>(g_pHwfAppendix), g_hwfStream, &g_nHwfAppendixItems);

    // TODO: sizeof when type is known
    // memset(g_ds_particles, 0, sizeof(g_ds_particles));

    if (MidiJamMain("MidiJam", hInstance, g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems) == FAILURE) {
        MessageBoxA(nullptr, "Video mode not supported, try running CONFIG.EXE first", "SHUTDOWN ERROR",
                    MB_ICONASTERISK);
        return 0;
    }

    gluLookAt(0.0, 50.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // --- Lighting ---
    GLfloat light0_ambient[] = {0.25f, 0.25f, 0.25f, 1.0f};
    GLfloat light0_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0_position[] = {15.0f, 75.0f, 5.0f, 1.0f};
    GLfloat light1_ambient[] = {0.025f, 0.025f, 0.025f, 1.0f};
    GLfloat light1_diffuse[] = {0.5f, 0.5f, 1.0f, 1.0f};
    GLfloat light1_position[] = {-25.0f, 5.0f, 25.0f, 1.0f};
    GLfloat light2_ambient[] = {0.25f, 0.25f, 0.25f, 1.0f};
    GLfloat light2_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light2_position[] = {-15.0f, 45.0f, 35.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glEnable(GL_LIGHT2);

    g_normalizedWindowScale = g_windowWidth / 1024.0f;
    g_windowCenterX = g_windowCenter_X;
    g_windowCenterY = g_windowCenter_Y;

    // --- Model loading ---
    LOAD_MODEL(g_drumSet_Stick_ms3d, "DrumSet_Stick.ms3d");
    LOAD_MODEL(g_cowbell_ms3d, "CowBell.ms3d");
    LOAD_MODEL(handRight_ms3d, "hand_right.ms3d");
    LOAD_MODEL(handLeft_ms3d, "hand_left.ms3d");
    LOAD_MODEL(handTambourine_ms3d, "hand_tamborine.ms3d");
    LOAD_MODEL(clave_ms3d, "clave.ms3d");
    LOAD_MODEL(jingleBells_ms3d, "jinglebells.ms3d");
    LOAD_MODEL(castanets_ms3d, "Castanets.ms3d");
    LOAD_MODEL(shaker_ms3d, "shaker.ms3d");
    LOAD_MODEL(g_zapper_ms3d, "Zapper.ms3d");
    LOAD_MODEL(g_zapperLaser_ms3d, "ZapperLaser.ms3d");
    LOAD_MODEL(g_squareShaker_ms3d, "squareshaker.ms3d");
    LOAD_MODEL(g_maraca_ms3d, "maraca.ms3d");
    LOAD_MODEL(g_cabasa_ms3d, "cabasa.ms3d");
    LOAD_MODEL(g_agogo_ms3d, "Agogo.ms3d");
    LOAD_MODEL(g_woodBlockHigh_ms3d, "WoodBlockHigh.ms3d");
    LOAD_MODEL(g_woodBlockLow_ms3d, "WoodBlockLow.ms3d");
    LOAD_MODEL(g_triangle_ms3d, "Triangle.ms3d");
    LOAD_MODEL(g_mutedTriangle_ms3d, "MutedTriangle.ms3d");
    LOAD_MODEL(g_triangleStick_ms3d, "Triangle_Stick.ms3d");
    LOAD_MODEL(g_drumSet_Timbale_ms3d, "DrumSet_Timbale.ms3d");
    LOAD_MODEL(g_drumSet_bongo_ms3d, "DrumSet_Bongo.ms3d");
    LOAD_MODEL(g_drumSet_conga_ms3d, "DrumSet_Conga.ms3d");
    LOAD_MODEL(g_drumSet_bassDrum_ms3d, "DrumSet_BassDrum.ms3d");
    LOAD_MODEL(g_drumSet_snareDrum_ms3d, "DrumSet_SnareDrum.ms3d");
    LOAD_MODEL(g_drumSet_tom_ms3d, "DrumSet_Tom.ms3d");
    LOAD_MODEL(g_drumSet_cymbal_ms3d, "DrumSet_Cymbal.ms3d");
    LOAD_MODEL(g_drumSet_chinaCymbal_ms3d, "DrumSet_ChinaCymbal.ms3d");
    LOAD_MODEL(g_drumSet_BassDrumBeaterArm_ms3d, "DrumSet_BassDrumBeaterArm.ms3d");
    LOAD_MODEL(g_drumSet_BassDrumBeaterHolder_ms3d, "DrumSet_BassDrumBeaterHolder.ms3d");
    LOAD_MODEL(g_drumSet_BassDrumPedal_ms3d, "DrumSet_BassDrumPedal.ms3d");

    // Harp and strings (chained success checks in original)
    LOAD_MODEL(g_harp_ms3d, "Harp.ms3d");
    LOAD_MODEL(g_harpString_ms3d[0], "HarpString.ms3d");
    LOAD_MODEL(g_harpString_ms3d[1], "HarpString.ms3d");
    REPLACE_TEX(g_harpString_ms3d[1], "HarpStringWhite.bmp", "HarpStringRed.bmp");
    LOAD_MODEL(g_harpString_ms3d[2], "HarpString.ms3d");
    REPLACE_TEX(g_harpString_ms3d[2], "HarpStringWhite.bmp", "HarpStringBlue.bmp");

    static const char* harpPlayingFiles[5] = {
        "HarpStringPlaying0.ms3d", "HarpStringPlaying1.ms3d", "HarpStringPlaying2.ms3d",
        "HarpStringPlaying3.ms3d", "HarpStringPlaying4.ms3d"
    };
    static const char* harpPlayingTextures[3][2] = {
        {"HarpStringWhite.bmp", "HarpStringWhitePlaying.bmp"},
        {"HarpStringWhite.bmp", "HarpStringRedPlaying.bmp"},
        {"HarpStringWhite.bmp", "HarpStringBluePlaying.bmp"},
    };
    for (short c = 0; c < 3; ++c)
        for (short f = 0; f < 5; ++f) {
            LOAD_MODEL(g_harpStringPlaying_ms3d[c][f], harpPlayingFiles[f]);
            REPLACE_TEX(g_harpStringPlaying_ms3d[c][f], harpPlayingTextures[c][0], harpPlayingTextures[c][1]);
        }

    LOAD_MODEL(g_timpaniBody_ms3d, "TimpaniBody.ms3d");
    LOAD_MODEL(g_timpaniHead_ms3d, "TimpaniHead.ms3d");
    LOAD_MODEL(g_synthDrum_ms3d, "SynthDrum.ms3d");
    LOAD_MODEL(g_steelDrum_recoil, "SteelDrum.ms3d");
    LOAD_MODEL(g_steelDrumMallet_ms3d, "SteelDrumMallet.ms3d");
    LOAD_MODEL(g_melodicTom_ms3d, "MelodicTom.ms3d");
    LOAD_MODEL(g_taiko_ms3d, "Taiko.ms3d");
    LOAD_MODEL(g_taikoStick_ms3d, "TaikoStick.ms3d");
    LOAD_MODEL(metronome_ms3d, "MetronomeBox.ms3d");
    LOAD_MODEL(metronomePendjulum1_ms3d, "MetronomePendjulum1.ms3d");
    LOAD_MODEL(metronomePendjulum2_ms3d, "MetronomePendjulum2.ms3d");
    LOAD_MODEL(g_musicBoxSpindle_ms3d, "MusicBoxSpindle.ms3d");
    LOAD_MODEL(g_musicBoxPoint_ms3d, "MusicBoxPoint.ms3d");
    LOAD_MODEL(g_musicBoxKey_ms3d, "MusicBoxKey.ms3d");
    LOAD_MODEL(g_musicBoxTopBlade_ms3d, "MusicBoxTopBlade.ms3d");
    LOAD_MODEL(g_musicBoxCase_ms3d, "MusicBoxCase.ms3d");
    LOAD_MODEL(g_stageStringHolder_ms3d, "StageStringHolder.ms3d");
    LOAD_MODEL(g_pizzicatoStringHolder_ms3d, "PizzicatoStringHolder.ms3d");
    LOAD_MODEL(g_stageStringBow_ms3d, "StageStringBow.ms3d");
    LOAD_MODEL(g_stageString_ms3d, "StageString.ms3d");

    static const char* stageStringBottomFiles[5] = {
        "StageStringBottom0.ms3d", "StageStringBottom1.ms3d", "StageStringBottom2.ms3d",
        "StageStringBottom3.ms3d", "StageStringBottom4.ms3d"
    };
    for (short i = 0; i < 5; ++i) {
        LOAD_MODEL(g_stageStringBottomX_ms3d[i], stageStringBottomFiles[i]);
    }


    LOAD_MODEL(g_stageHorn_ms3d, "StageHorn.ms3d");
    LOAD_MODEL(whistle_ms3d, "Whistle.ms3d");
    LOAD_MODEL(g_panPipe_ms3d, "PanPipe.ms3d");
    LOAD_MODEL(g_calliope_ms3d, "PanPipe.ms3d");
    REPLACE_TEX(g_calliope_ms3d, "Wood.bmp", "HornSkin.bmp");
    LOAD_MODEL(g_harmonica_ms3d, "Harmonica.ms3d");
    LOAD_MODEL(g_popBottle_ms3d, "PopBottle.ms3d");
    LOAD_MODEL(g_popBottleMiddle_ms3d, "PopBottleMiddle.ms3d");
    LOAD_MODEL(g_popBottlePop_ms3d, "PopBottlePop.ms3d");
    LOAD_MODEL(popBottleLabel_ms3d, "PopBottleLabel.ms3d");
    LOAD_MODEL(steamCloud_0_ms3d, "SteamCloud.ms3d");
    LOAD_MODEL(steamCloud_1_ms3d, "SteamCloud.ms3d");
    REPLACE_TEX(steamCloud_1_ms3d, "SteamPuff.bmp", "SteamPuff_Whistle.bmp");
    LOAD_MODEL(steamCloud_2_ms3d, "SteamCloud.ms3d");
    REPLACE_TEX(steamCloud_2_ms3d, "SteamPuff.bmp", "SteamPuff_Pop.bmp");
    LOAD_MODEL(g_steamPuff_harmonica_ms3d, "SteamCloud.ms3d");
    REPLACE_TEX(g_steamPuff_harmonica_ms3d, "SteamPuff.bmp", "SteamPuff_Harmonica.bmp");

    LOAD_MODEL(g_violin_ms3d, "Violin.ms3d");
    LOAD_MODEL(g_viola_ms3d, "Violin.ms3d");
    REPLACE_TEX(g_viola_ms3d, "ViolinSkin.bmp", "ViolaSkin.bmp");
    LOAD_MODEL(g_cello_ms3d, "Cello.ms3d");
    LOAD_MODEL(g_doubleBass_ms3d, "DoubleBass.ms3d");
    LOAD_MODEL(g_violinString_ms3d, "ViolinString.ms3d");

    static const char* violinStringPlayedFiles[5] = {
        "ViolinStringPlayed0.ms3d", "ViolinStringPlayed1.ms3d", "ViolinStringPlayed2.ms3d",
        "ViolinStringPlayed3.ms3d", "ViolinStringPlayed4.ms3d"
    };
    for (short i = 0; i < 5; ++i) {
        LOAD_MODEL(g_violinStringPlayedX_ms3d[i], violinStringPlayedFiles[i]);
    }


    // ViolinFinger.ms3d — IDA stored this at offset +20 from g_violinFinger_ms3d
    // (i.e. a 21-element array, index 20 = the actual violin finger)
    // TODO: clarify array layout when violin is transcribed
    LOAD_MODEL((&g_violinFinger_ms3d)[20], "ViolinFinger.ms3d");
    LOAD_MODEL(g_violinBow_ms3d, "ViolinBow.ms3d");

    LOAD_MODEL(g_tubularBell_ms3d, "TubularBell.ms3d");
    LOAD_MODEL(g_tubularBellDark_ms3d, "TubularBellDark.ms3d");
    LOAD_MODEL(g_tubularBellMallet_ms3d, "TubularBellMallet.ms3d");
    LOAD_MODEL(g_stageChoir_ms3d, "StageChoir.ms3d");

    LOAD_MODEL(g_accordionKeyWhiteBack_ms3d, "AccordianKeyWhiteBack.ms3d");
    LOAD_MODEL(g_accordionKeyWhiteFront_ms3d, "AccordianKeyWhiteFront.ms3d");
    LOAD_MODEL(g_accordionKeyWhiteBack_Down_ms3d, "AccordianKeyWhiteBack.ms3d");
    REPLACE_TEX(g_accordionKeyWhiteBack_Down_ms3d, "AccordianKey.bmp", "AccordianKeyDown.bmp");
    LOAD_MODEL(g_accordionKeyWhiteFront_Down_ms3d, "AccordianKeyWhiteFront.ms3d");
    REPLACE_TEX(g_accordionKeyWhiteFront_Down_ms3d, "AccordianKey.bmp", "AccordianKeyDown.bmp");
    LOAD_MODEL(g_accordionKeyBlack_ms3d, "AccordianKeyBlack.ms3d");
    LOAD_MODEL(g_accordionKeyBlack_Down_ms3d, "AccordianKeyBlack.ms3d");
    REPLACE_TEX(g_accordionKeyBlack_Down_ms3d, "AccordianKeyBlack.bmp", "AccordianKeyBlackDown.bmp");
    LOAD_MODEL(g_accordionFold_ms3d, "AccordianFold.ms3d");
    LOAD_MODEL(g_accordionLeftHand_ms3d, "AccordianLeftHand.ms3d");
    LOAD_MODEL(g_accordionRightHand_ms3d, "AccordianRightHand.ms3d");

    LOAD_MODEL(g_screenGradient_ms3d, "ScreenGradient.ms3d");
    LOAD_MODEL(g_songFillbarBox_ms3d, "SongFillbarBox.ms3d");
    LOAD_MODEL(g_songFillbar_ms3d, "SongFillbar.ms3d");

    LOAD_MODEL(g_agogoSingle_ms3d, "AgogoSingle.ms3d");
    LOAD_MODEL(g_woodBlockSingle_ms3d, "WoodBlockSingle.ms3d");

    LOAD_MODEL(g_xylophoneMalletWhite_ms3d, "XylophoneMalletWhite.ms3d");
    LOAD_MODEL(g_malletHitShadow_ms3d, "MalletHitShadow.ms3d");
    LOAD_MODEL(g_xylophoneLegs_ms3d, "XylophoneLegs.ms3d");
    LOAD_MODEL(g_xylophoneCase_ms3d, "XylophoneCase.ms3d");

    static const char* xyloBarTextures[4] = {
        "XylophoneBar.bmp", "GlockenspielBar.bmp", "VibesBar.bmp", "MarimbaBar.bmp"
    };
    for (short i = 0; i < 4; ++i) {
        g_xylophoneModels[i] = new XylophoneModels();
    }
    for (short i = 0; i < 4; ++i) {
        LOAD_MODEL(g_xylophoneModels[i]->xylophoneWhiteBar, "XylophoneWhiteBar.ms3d");
        if (i > 0)
            REPLACE_TEX(g_xylophoneModels[i]->xylophoneWhiteBar, xyloBarTextures[0], xyloBarTextures[i]);
        LOAD_MODEL(g_xylophoneModels[i]->xylophoneWhiteBarDown, "XylophoneWhiteBarDown.ms3d");
        if (i > 0)
            REPLACE_TEX(g_xylophoneModels[i]->xylophoneWhiteBarDown, xyloBarTextures[0], xyloBarTextures[i]);
        LOAD_MODEL(g_xylophoneModels[i]->xylophoneBlackBar, "XylophoneBlackBar.ms3d");
        if (i > 0)
            REPLACE_TEX(g_xylophoneModels[i]->xylophoneBlackBar, xyloBarTextures[0], xyloBarTextures[i]);
        LOAD_MODEL(g_xylophoneModels[i]->xylophoneBlackBarDown, "XylophoneBlackBarDown.ms3d");
        if (i > 0)
            REPLACE_TEX(g_xylophoneModels[i]->xylophoneBlackBarDown, xyloBarTextures[0], xyloBarTextures[i]);
    }

    LOAD_MODEL(g_baritoneSaxBody_ms3d, "BaritoneSaxBody.ms3d");
    LOAD_MODEL(g_baritoneSaxHorn_ms3d, "BaritoneSaxHorn.ms3d");
    LOAD_MODEL(g_tenorSaxBody_ms3d, "TenorSaxBody.ms3d");
    LOAD_MODEL(g_tenorSaxHorn_ms3d, "TenorSaxHorn.ms3d");
    LOAD_MODEL(g_sapranoSaxBody_ms3d, "SapranoSaxBody.ms3d");
    LOAD_MODEL(g_sapranoSaxHorn_ms3d, "SapranoSaxHorn.ms3d");
    LOAD_MODEL(g_altoSaxBody_ms3d, "AltoSaxBody.ms3d");
    LOAD_MODEL(g_altoSaxHorn_ms3d, "AltoSaxHorn.ms3d");

    LOAD_MODEL(g_flute_ms3d, "Flute.ms3d");
    LOAD_MODEL(g_piccolo_ms3d, "Piccolo.ms3d");

    for (short i = 0; i < 13; ++i) {
        char filename[64];
        sprintf(filename, "Flute_LeftHand%02d.ms3d", i);
        LOAD_MODEL(g_flute_leftHandX_ms3d[i], filename);
    }
    for (short i = 0; i < 12; ++i) {
        char filename[64];
        sprintf(filename, "Flute_RightHand%02d.ms3d", i);
        LOAD_MODEL(g_flute_rightHandX_ms3d[i], filename);
    }

    LOAD_MODEL(g_telephoneBase_ms3d, "TelePhoneBase.ms3d");
    LOAD_MODEL(g_telephoneHandle_ms3d, "TelePhoneHandle.ms3d");

    // Telephone keys — IDA stores these as a flat array via pointer arithmetic
    // off g_violinFinger_ms3d and g_telephoneKeyX_ms3d. Transcribed faithfully
    // using the pointer arithmetic pattern; will be cleaned up when telephone
    // is transcribed.
    static const char* telephoneKeyFiles[12] = {
        "TelePhoneKey1.bmp", "TelePhoneKey2.bmp", "TelePhoneKey3.bmp",
        "TelePhoneKey4.bmp", "TelePhoneKey5.bmp", "TelePhoneKey6.bmp",
        "TelePhoneKey7.bmp", "TelePhoneKey8.bmp", "TelePhoneKey9.bmp",
        "TelePhoneKeyStar.bmp", "TelePhoneKey0.bmp", "TelePhoneKeyPound.bmp"
    };
    static const char* telephoneKeyFilesDark[12] = {
        "TelePhoneKey1Dark.bmp", "TelePhoneKey2Dark.bmp", "TelePhoneKey3Dark.bmp",
        "TelePhoneKey4Dark.bmp", "TelePhoneKey5Dark.bmp", "TelePhoneKey6Dark.bmp",
        "TelePhoneKey7Dark.bmp", "TelePhoneKey8Dark.bmp", "TelePhoneKey9Dark.bmp",
        "TelePhoneKeyStarDark.bmp", "TelePhoneKey0Dark.bmp", "TelePhoneKeyPoundDark.bmp"
    };
    // First 12 keys (lit)
    LOAD_MODEL(g_telephoneKeyX_ms3d, "TelePhoneKey.ms3d");
    REPLACE_TEX(g_telephoneKeyX_ms3d, "TelePhoneKey.bmp", telephoneKeyFiles[0]);
    LOAD_MODEL(dword_46D25C, "TelePhoneKey.ms3d");
    REPLACE_TEX(dword_46D25C, "TelePhoneKey.bmp", telephoneKeyFiles[1]);
    LOAD_MODEL(dword_46D260, "TelePhoneKey.ms3d");
    REPLACE_TEX(dword_46D260, "TelePhoneKey.bmp", telephoneKeyFiles[2]);
    LOAD_MODEL(dword_46D264, "TelePhoneKey.ms3d");
    REPLACE_TEX(dword_46D264, "TelePhoneKey.bmp", telephoneKeyFiles[3]);
    // IDA accessed keys 4-19 via pointer arithmetic off g_violinFinger_ms3d [0..19]
    for (short i = 0; i < 8; ++i) {
        LOAD_MODEL((&g_violinFinger_ms3d)[i], "TelePhoneKey.ms3d");
        REPLACE_TEX((&g_violinFinger_ms3d)[i], "TelePhoneKey.bmp", telephoneKeyFiles[4 + i]);
    }
    for (short i = 0; i < 12; ++i) {
        LOAD_MODEL((&g_violinFinger_ms3d)[8 + i], "TelePhoneKey.ms3d");
        REPLACE_TEX((&g_violinFinger_ms3d)[8 + i], "TelePhoneKey.bmp", telephoneKeyFilesDark[i]);
    }

    LOAD_MODEL(g_recorder_ms3d, "Recorder.ms3d");
    for (short i = 0; i < 13; ++i) {
        char filename[64];
        sprintf(filename, "RecorderHandLeft%d.ms3d", i);
        LOAD_MODEL(g_recorderLeftHandX_ms3d[i], filename);
    }
    for (short i = 0; i < 10; ++i) {
        char filename[64];
        sprintf(filename, "RecorderHandRight%d.ms3d", i);
        LOAD_MODEL((&g_recorderRightHandX_ms3d)[i], filename);
    }
    LOAD_MODEL(dword_465230, "RecorderHandRight10.ms3d");

    for (short i = 0; i < 20; ++i) {
        char filename[64];
        sprintf(filename, "AltoSaxKeyUp%d.ms3d", i);
        LOAD_MODEL(g_altoSaxKeyX_ms3d[i][0], filename);
        sprintf(filename, "AltoSaxKeyDown%d.ms3d", i);
        LOAD_MODEL(g_altoSaxKeyX_ms3d[i][1], filename);
    }

    LOAD_MODEL(g_ocarina_ms3d, "Ocarina.ms3d");
    for (short i = 0; i < 12; ++i) {
        char filename[64];
        sprintf(filename, "OcarinaHand%d.ms3d", i);
        LOAD_MODEL((&g_ocarinaHandX_ms3d)[i], filename);
    }

    LOAD_MODEL(g_frenchHornBody_ms3d, "FrenchHornBody.ms3d");
    LOAD_MODEL(g_frenchHornHorn_ms3d, "FrenchHornHorn.ms3d");
    LOAD_MODEL(g_frenchHornKey1_ms3d, "FrenchHornKey1.ms3d");
    LOAD_MODEL(g_frenchHornKey2_ms3d, "FrenchHornKey2.ms3d");
    LOAD_MODEL(g_frenchHornKey3_ms3d, "FrenchHornKey3.ms3d");
    LOAD_MODEL(g_frenchHornTrigger_ms3d, "FrenchHornTrigger.ms3d");
    LOAD_MODEL(g_tubaBody_ms3d, "TubaBody.ms3d");
    LOAD_MODEL(g_tubaHorn_ms3d, "TubaHorn.ms3d");
    LOAD_MODEL(g_tubaKey_1_ms3d, "TubaKey1.ms3d");
    LOAD_MODEL(g_tubaKey_2_ms3d, "TubaKey2.ms3d");
    LOAD_MODEL(g_tubaKey_3_ms3d, "TubaKey3.ms3d");
    LOAD_MODEL(g_tubaKey_4_ms3d, "TubaKey4.ms3d");
    LOAD_MODEL(g_trumpetBody_ms3d, "TrumpetBody.ms3d");
    LOAD_MODEL(g_trumpetHorn_ms3d, "TrumpetHorn.ms3d");
    LOAD_MODEL(g_trumpetMute_ms3d, "TrumpetMute.ms3d");
    LOAD_MODEL(g_trumpetKey1_ms3d, "TrumpetKey1.ms3d");
    LOAD_MODEL(g_trumpetKey2_ms3d, "TrumpetKey2.ms3d");
    LOAD_MODEL(g_trumpetKey3_ms3d, "TrumpetKey3.ms3d");
    LOAD_MODEL(g_trombone_ms3d, "Trombone.ms3d");
    LOAD_MODEL(g_tromboneSlide_ms3d, "TromboneSlide.ms3d");

    LOAD_MODEL(g_pianoShadow_ms3d, "PianoShadow.ms3d");
    LOAD_MODEL(g_xylophoneShadow_ms3d, "XylophoneShadow.ms3d");
    LOAD_MODEL(g_bassShadow_ms3d, "BassShadow.ms3d");
    LOAD_MODEL(g_guitarShadow_ms3d, "GuitarShadow.ms3d");
    LOAD_MODEL(g_drumShadow_ms3d, "DrumShadow.ms3d");
    LOAD_MODEL(g_harpShadow_ms3d, "HarpShadow.ms3d");
    LOAD_MODEL(g_pianoStand_ms3d, "PianoStand.ms3d");
    LOAD_MODEL(g_stage_ms3d, "Stage.ms3d");

    LOAD_MODEL(g_bass_ms3d, "Bass.ms3d");
    LOAD_MODEL(g_bassString_ms3d, "BassString.ms3d");
    static const char* bassStringBottomFiles[5] = {
        "BassStringBottom0.ms3d", "BassStringBottom1.ms3d", "BassStringBottom2.ms3d",
        "BassStringBottom3.ms3d", "BassStringBottom4.ms3d"
    };
    for (short i = 0; i < 5; ++i) {
        LOAD_MODEL(g_bassStringBottomX_ms3d[i], bassStringBottomFiles[i]);
    }

    LOAD_MODEL(g_bassNoteFinger_ms3d, "BassNoteFinger.ms3d");

    LOAD_MODEL(g_guitar_ms3d, "Guitar.ms3d");
    LOAD_MODEL(g_guitarStringHigh_ms3d, "GuitarStringHigh.ms3d");
    LOAD_MODEL(g_guitarStringLow_ms3d, "GuitarStringLow.ms3d");
    static const char* guitarLowBottomFiles[5] = {
        "GuitarLowStringBottom0.ms3d", "GuitarLowStringBottom1.ms3d", "GuitarLowStringBottom2.ms3d",
        "GuitarLowStringBottom3.ms3d", "GuitarLowStringBottom4.ms3d"
    };
    static const char* guitarHighBottomFiles[5] = {
        "GuitarHighStringBottom0.ms3d", "GuitarHighStringBottom1.ms3d", "GuitarHighStringBottom2.ms3d",
        "GuitarHighStringBottom3.ms3d", "GuitarHighStringBottom4.ms3d"
    };
    for (short i = 0; i < 5; ++i) {
        LOAD_MODEL(g_guitarLowStringBottomX_ms3d[i], guitarLowBottomFiles[i]);
        LOAD_MODEL(g_guitarHighStringBottomX_ms3d[i], guitarHighBottomFiles[i]);
    }
    LOAD_MODEL(g_guitarNoteFinger_ms3d, "GuitarNoteFinger.ms3d");

    // Piano models (4 variants: standard, honky-tonk/wood, synth, harpsichord)
    for (short i = 0; i < 4; ++i) {
        LOAD_MODEL(g_pianoModels[i].pianoCase, "PianoCase.ms3d");
        LOAD_MODEL(g_pianoModels[i].pianoKeyBlack, "PianoKeyBlack.ms3d");
        LOAD_MODEL(g_pianoModels[i].pianoKeyBlackDown, "PianoKeyBlackDown.ms3d");
        LOAD_MODEL(g_pianoModels[i].pianoKeyWhiteFront, "PianoKeyWhiteFront.ms3d");
        LOAD_MODEL(g_pianoModels[i].pianoKeyWhiteBack, "PianoKeyWhiteBack.ms3d");
        LOAD_MODEL(g_pianoModels[i].pianoKeyWhiteFrontDown, "PianoKeyWhiteFrontDown.ms3d");
        LOAD_MODEL(g_pianoModels[i].pianoKeyWhiteBackDown, "PianoKeyWhiteBackDown.ms3d");
    }
    // Variant 1: Electric piano / wood skin
    for (short m = 0; m < 7; ++m)
        REPLACE_TEX(reinterpret_cast<Ms3dBundle**>(&g_pianoModels[1])[m], "PianoSkin.bmp", "PianoSkin_Wood.bmp");
    // Variant 2: Honky-tonk / synth skin
    for (short m = 0; m < 7; ++m)
        REPLACE_TEX(reinterpret_cast<Ms3dBundle**>(&g_pianoModels[2])[m], "PianoSkin.bmp", "SynthSkin.bmp");
    // Variant 3: Harpsichord skin
    for (short m = 0; m < 7; ++m)
        REPLACE_TEX(reinterpret_cast<Ms3dBundle**>(&g_pianoModels[3])[m], "PianoSkin.bmp", "HarpsichordSkin.bmp");

    // --- Seed RNG and initialize ---
    srand(static_cast<long>(time(nullptr)));
    MidiJamInitialize();

    // --- ApplyTextures pass ---
    APPLY_TEX(g_drumSet_Stick_ms3d);
    APPLY_TEX(g_cowbell_ms3d);
    APPLY_TEX(handRight_ms3d);
    APPLY_TEX(handLeft_ms3d);
    APPLY_TEX(handTambourine_ms3d);
    APPLY_TEX(clave_ms3d);
    APPLY_TEX(castanets_ms3d);
    APPLY_TEX(jingleBells_ms3d);
    APPLY_TEX(shaker_ms3d);
    APPLY_TEX(g_zapper_ms3d);
    APPLY_TEX(g_zapperLaser_ms3d);
    APPLY_TEX(g_squareShaker_ms3d);
    APPLY_TEX(g_maraca_ms3d);
    APPLY_TEX(g_cabasa_ms3d);
    APPLY_TEX(g_agogo_ms3d);
    APPLY_TEX(g_woodBlockHigh_ms3d);
    APPLY_TEX(g_woodBlockLow_ms3d);
    APPLY_TEX(g_triangle_ms3d);
    APPLY_TEX(g_mutedTriangle_ms3d);
    APPLY_TEX(g_triangleStick_ms3d);
    APPLY_TEX(g_drumSet_Timbale_ms3d);
    APPLY_TEX(g_drumSet_bongo_ms3d);
    APPLY_TEX(g_drumSet_conga_ms3d);
    APPLY_TEX(g_drumSet_bassDrum_ms3d);
    APPLY_TEX(g_drumSet_snareDrum_ms3d);
    APPLY_TEX(g_drumSet_tom_ms3d);
    APPLY_TEX(g_drumSet_cymbal_ms3d);
    APPLY_TEX(g_drumSet_chinaCymbal_ms3d);
    APPLY_TEX(g_drumSet_BassDrumBeaterArm_ms3d);
    APPLY_TEX(g_drumSet_BassDrumBeaterHolder_ms3d);
    APPLY_TEX(g_drumSet_BassDrumPedal_ms3d);
    APPLY_TEX(g_harp_ms3d);
    for (short i = 0; i < 3; ++i) {
        APPLY_TEX(g_harpString_ms3d[i]);
        for (short k = 0; k < 5; ++k)
            APPLY_TEX(g_harpStringPlaying_ms3d[i][k]);
    }
    APPLY_TEX(g_timpaniBody_ms3d);
    APPLY_TEX(g_timpaniHead_ms3d);
    APPLY_TEX(g_synthDrum_ms3d);
    APPLY_TEX(g_steelDrum_recoil);
    APPLY_TEX(g_steelDrumMallet_ms3d);
    APPLY_TEX(g_melodicTom_ms3d);
    APPLY_TEX(g_taiko_ms3d);
    APPLY_TEX(g_taikoStick_ms3d);
    APPLY_TEX(metronome_ms3d);
    APPLY_TEX(metronomePendjulum1_ms3d);
    APPLY_TEX(metronomePendjulum2_ms3d);
    APPLY_TEX(g_musicBoxSpindle_ms3d);
    APPLY_TEX(g_musicBoxPoint_ms3d);
    APPLY_TEX(g_musicBoxKey_ms3d);
    APPLY_TEX(g_musicBoxTopBlade_ms3d);
    APPLY_TEX(g_musicBoxCase_ms3d);
    APPLY_TEX(g_stageStringHolder_ms3d);
    APPLY_TEX(g_pizzicatoStringHolder_ms3d);
    APPLY_TEX(g_stageStringBow_ms3d);
    APPLY_TEX(g_stageString_ms3d);
    for (short i = 0; i < 5; ++i)
        APPLY_TEX(g_stageStringBottomX_ms3d[i]);
    APPLY_TEX(g_stageHorn_ms3d);
    APPLY_TEX(whistle_ms3d);
    APPLY_TEX(g_panPipe_ms3d);
    APPLY_TEX(g_calliope_ms3d);
    APPLY_TEX(g_harmonica_ms3d);
    APPLY_TEX(g_popBottle_ms3d);
    APPLY_TEX(g_popBottleMiddle_ms3d);
    APPLY_TEX(g_popBottlePop_ms3d);
    APPLY_TEX(popBottleLabel_ms3d);
    APPLY_TEX(steamCloud_0_ms3d);
    APPLY_TEX(steamCloud_1_ms3d);
    APPLY_TEX(steamCloud_2_ms3d);
    APPLY_TEX(g_steamPuff_harmonica_ms3d);
    APPLY_TEX(g_violin_ms3d);
    APPLY_TEX(g_viola_ms3d);
    APPLY_TEX(g_cello_ms3d);
    APPLY_TEX(g_doubleBass_ms3d);
    APPLY_TEX(g_violinString_ms3d);
    for (short i = 0; i < 5; ++i)
        APPLY_TEX(g_violinStringPlayedX_ms3d[i]);
    APPLY_TEX((&g_violinFinger_ms3d)[20]);
    APPLY_TEX(g_violinBow_ms3d);
    APPLY_TEX(g_tubularBell_ms3d);
    APPLY_TEX(g_tubularBellDark_ms3d);
    APPLY_TEX(g_tubularBellMallet_ms3d);
    APPLY_TEX(g_stageChoir_ms3d);
    APPLY_TEX(g_accordionKeyWhiteBack_ms3d);
    APPLY_TEX(g_accordionKeyWhiteFront_ms3d);
    APPLY_TEX(g_accordionKeyWhiteBack_Down_ms3d);
    APPLY_TEX(g_accordionKeyWhiteFront_Down_ms3d);
    APPLY_TEX(g_accordionKeyBlack_ms3d);
    APPLY_TEX(g_accordionKeyBlack_Down_ms3d);
    APPLY_TEX(g_accordionFold_ms3d);
    APPLY_TEX(g_accordionLeftHand_ms3d);
    APPLY_TEX(g_accordionRightHand_ms3d);
    APPLY_TEX(g_screenGradient_ms3d);
    APPLY_TEX(g_songFillbarBox_ms3d);
    APPLY_TEX(g_songFillbar_ms3d);
    APPLY_TEX(g_agogoSingle_ms3d);
    APPLY_TEX(g_woodBlockSingle_ms3d);
    APPLY_TEX(g_xylophoneMalletWhite_ms3d);
    APPLY_TEX(g_malletHitShadow_ms3d);
    APPLY_TEX(g_xylophoneLegs_ms3d);
    APPLY_TEX(g_xylophoneCase_ms3d);
    for (short i = 0; i < 4; ++i) {
        APPLY_TEX(g_xylophoneModels[i]->xylophoneWhiteBar)
        APPLY_TEX(g_xylophoneModels[i]->xylophoneWhiteBarDown)
        APPLY_TEX(g_xylophoneModels[i]->xylophoneBlackBar)
        APPLY_TEX(g_xylophoneModels[i]->xylophoneBlackBarDown)
    }
    APPLY_TEX(g_baritoneSaxBody_ms3d);
    APPLY_TEX(g_baritoneSaxHorn_ms3d);
    APPLY_TEX(g_tenorSaxBody_ms3d);
    APPLY_TEX(g_tenorSaxHorn_ms3d);
    APPLY_TEX(g_sapranoSaxBody_ms3d);
    APPLY_TEX(g_sapranoSaxHorn_ms3d);
    APPLY_TEX(g_altoSaxBody_ms3d);
    APPLY_TEX(g_altoSaxHorn_ms3d);
    APPLY_TEX(g_flute_ms3d);
    APPLY_TEX(g_piccolo_ms3d);
    for (short i = 0; i < 13; ++i)
        APPLY_TEX(g_flute_leftHandX_ms3d[i]);
    for (short i = 0; i < 12; ++i)
        APPLY_TEX(g_flute_rightHandX_ms3d[i]);
    APPLY_TEX(g_telephoneBase_ms3d);
    APPLY_TEX(g_telephoneHandle_ms3d);
    // for (short i = 0; i < 24; ++i)
    //     APPLY_TEX((&g_telephoneKeyX_ms3d)[i]);
    APPLY_TEX(g_recorder_ms3d);
    for (short i = 0; i < 13; ++i)
        APPLY_TEX(g_recorderLeftHandX_ms3d[i]);
    for (short i = 0; i < 11; ++i)
        APPLY_TEX((&g_recorderRightHandX_ms3d)[i]);
    for (short i = 0; i < 20; ++i) {
        APPLY_TEX(g_altoSaxKeyX_ms3d[i][0]);
        APPLY_TEX(g_altoSaxKeyX_ms3d[i][1]);
    }
    APPLY_TEX(g_ocarina_ms3d);
    // for (short i = 0; i < 12; ++i)
    //     APPLY_TEX((&g_ocarinaHandX_ms3d)[i]);
    APPLY_TEX(g_frenchHornBody_ms3d);
    APPLY_TEX(g_frenchHornHorn_ms3d);
    APPLY_TEX(g_frenchHornKey1_ms3d);
    APPLY_TEX(g_frenchHornKey2_ms3d);
    APPLY_TEX(g_frenchHornKey3_ms3d);
    APPLY_TEX(g_frenchHornTrigger_ms3d);
    APPLY_TEX(g_tubaBody_ms3d);
    APPLY_TEX(g_tubaHorn_ms3d);
    APPLY_TEX(g_tubaKey_1_ms3d);
    APPLY_TEX(g_tubaKey_2_ms3d);
    APPLY_TEX(g_tubaKey_3_ms3d);
    APPLY_TEX(g_tubaKey_4_ms3d);
    APPLY_TEX(g_trumpetBody_ms3d);
    APPLY_TEX(g_trumpetHorn_ms3d);
    APPLY_TEX(g_trumpetMute_ms3d);
    APPLY_TEX(g_trumpetKey1_ms3d);
    APPLY_TEX(g_trumpetKey2_ms3d);
    APPLY_TEX(g_trumpetKey3_ms3d);
    APPLY_TEX(g_trombone_ms3d);
    APPLY_TEX(g_tromboneSlide_ms3d);
    APPLY_TEX(g_pianoShadow_ms3d);
    APPLY_TEX(g_xylophoneShadow_ms3d);
    APPLY_TEX(g_bassShadow_ms3d);
    APPLY_TEX(g_guitarShadow_ms3d);
    APPLY_TEX(g_drumShadow_ms3d);
    APPLY_TEX(g_harpShadow_ms3d);
    APPLY_TEX(g_pianoStand_ms3d);
    APPLY_TEX(g_stage_ms3d);
    APPLY_TEX(g_bass_ms3d);
    APPLY_TEX(g_bassString_ms3d);
    for (short i = 0; i < 5; ++i)
        APPLY_TEX(g_bassStringBottomX_ms3d[i]);
    APPLY_TEX(g_bassNoteFinger_ms3d);
    APPLY_TEX(g_guitar_ms3d);
    APPLY_TEX(g_guitarStringLow_ms3d);
    APPLY_TEX(g_guitarStringHigh_ms3d);
    for (short i = 0; i < 5; ++i) {
        APPLY_TEX(g_guitarLowStringBottomX_ms3d[i]);
        APPLY_TEX(g_guitarHighStringBottomX_ms3d[i]);
    }
    APPLY_TEX(g_guitarNoteFinger_ms3d);
    for (short i = 0; i < 4; ++i) {
        APPLY_TEX(g_pianoModels[i].pianoCase);
        APPLY_TEX(g_pianoModels[i].pianoKeyBlack);
        APPLY_TEX(g_pianoModels[i].pianoKeyBlackDown);
        APPLY_TEX(g_pianoModels[i].pianoKeyWhiteFront);
        APPLY_TEX(g_pianoModels[i].pianoKeyWhiteBack);
        APPLY_TEX(g_pianoModels[i].pianoKeyWhiteFrontDown);
        APPLY_TEX(g_pianoModels[i].pianoKeyWhiteBackDown);
    }

    // --- Timer and MIDI startup ---
    int v613 = 0;
    RECOIL_SCALE_FACTOR = 5.0f;
    g_timerEventId = timeSetEvent(5u, 0, UpdateMidiJamMM, 0, TIME_PERIODIC);

    // --- MIDI file from command line ---
    if (strlen(lpCmdLine) > 4) {
        // Copy in midi file name (without extension)
        short m;
        for (m = 0; m < static_cast<__int16>(strlen(lpCmdLine) - 4) && (lpCmdLine[m] != '.' || lpCmdLine[m + 1] != 'M'
                 || lpCmdLine[m + 2] != 'I' || lpCmdLine[m + 3] != 'D' || lpCmdLine[m] != '.' || lpCmdLine[m + 1] != 'R'
                 || lpCmdLine[m + 2] != 'M' || lpCmdLine[m + 3] != 'I'); ++m) {
            g_midiFileName[m] = lpCmdLine[m];
        }
        // Copy in file extension
        g_midiFileName[m] = lpCmdLine[m];
        g_midiFileName[m + 1] = lpCmdLine[m + 1];
        g_midiFileName[m + 2] = lpCmdLine[m + 2];
        g_midiFileName[m + 3] = lpCmdLine[m + 3];
        g_midiFileName[m + 4] = 0;
        // Remove quotes if it has any
        if (g_midiFileName[0] == '"') {
            short n;
            for (n = 0; g_midiFileName[n]; ++n) {
                g_midiFileName[n] = g_midiFileName[n + 1];
            }
            if (byte_461DD6[n] == '"') // replace beginning quote with null terminator
                byte_461DD6[n] = 0;
        }
    }
    if (strlen(lpCmdLine) <= 4) {
        // no midi file passed, load default song
        if (LoadAndPlayMidiFile("rocky_1.mid") == 1)
            g_killApplication_0 = 1;
    }
    // load custom midi file
    else if (LoadAndPlayMidiFile(g_midiFileName) == 1) {
        g_killApplication_0 = 1;
    }
    g_worldReady = 1;
    MSG Msg = {};

    while (!g_killApplication_0) {
        if (PeekMessageA(&Msg, 0, 0, 0, 1u)) {
            if (Msg.message == WM_QUIT) {
                g_killApplication_0 = 1;
            }
            else {
                TranslateMessage(&Msg);
                DispatchMessageA(&Msg);
            }
        }
        else if (!g_isWindowActive || UpdateMidiJam()) {
            SwapBuffers_0();
            ++v613;
            // run once per second
            if (time(0) > g_lastUnixEpochTime) {
                g_framesPerSecond = static_cast<double>(v613);
                v613 = 0;
                g_lastUnixEpochTime = time(0);
            }
        }
        else {
            g_killApplication_0 = 1;
        }
    }
    timeKillEvent(g_timerEventId);
    MidiJamWindowCleanup();
    fclose(g_hwfStream);
    return Msg.wParam; // End of WinMain in normal scenario
}
