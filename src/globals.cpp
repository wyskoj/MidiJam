//
// Global variable definitions
// This file contains the actual definitions of all global variables
// that are declared as extern in the header files.
//

#include "globals.h"
#include "camera.h"
#include "MidiJam.h"
#include "Ms3dBundle.h"
#include "instrument/Piano.h"

// State
int g_killApplication_0;
int g_framesSinceStart;
__int16 g_isFadingIn;
__int16 g_isShuttingDown;
__int16 g_killApplication;
int g_lastUnixEpochTime;
UINT g_timerEventId;
char g_midiFileName[1000];
int g_time_global_current;
__int16 g_framesAlive;
REFERENCE_TIME g_prtStart;
MUSIC_TIME g_mtStart;
MidiJamInstrumentId g_midiJamInstrumentIds[300];
double g_currentTempo;
double g_currentTempo_scaleFactor0_5;
double g_currentTempo_scaleFactor0_9;
double g_currentTempo_scaleFactor1_15;
float g_playbackSpeed;
__int16 g_isEvenFrame;

// HWF
FILE *g_hwfStream;
char *g_pHwfAppendix;
unsigned int g_nHwfAppendixItems;

// World
float RECOIL_SCALE_FACTOR;
GLfloat g_pianokey_translation_x[14];
GLfloat STAGE_BASE_Z = -150.0;

// Models
Ms3dBundle *g_accordionFold_ms3d;
Ms3dBundle *g_accordionKeyBlack_Down_ms3d;
Ms3dBundle *g_accordionKeyBlack_ms3d;
Ms3dBundle *g_accordionKeyWhiteBack_Down_ms3d;
Ms3dBundle *g_accordionKeyWhiteBack_ms3d;
Ms3dBundle *g_accordionKeyWhiteFront_Down_ms3d;
Ms3dBundle *g_accordionKeyWhiteFront_ms3d;
Ms3dBundle *g_accordionLeftHand_ms3d;
Ms3dBundle *g_accordionRightHand_ms3d;
Ms3dBundle *g_agogoSingle_ms3d;
Ms3dBundle *g_agogo_ms3d;
Ms3dBundle *g_altoSaxBody_ms3d;
Ms3dBundle *g_altoSaxHorn_ms3d;
Ms3dBundle *g_altoSaxKeyX_ms3d;
Ms3dBundle *g_baritoneSaxBody_ms3d;
Ms3dBundle *g_baritoneSaxHorn_ms3d;
Ms3dBundle *g_bass_ms3d;
Ms3dBundle *g_bassNoteFinger_ms3d;
Ms3dBundle **g_bassStringBottomX_ms3d;
Ms3dBundle *g_bassString_ms3d;
Ms3dBundle *g_cabasa_ms3d;
Ms3dBundle *g_calliope_ms3d;
Ms3dBundle *g_cello_ms3d;
Ms3dBundle *g_cowbell_ms3d;
Ms3dBundle *g_doubleBass_ms3d;
Ms3dBundle *g_drumSet_BassDrumBeaterArm_ms3d;
Ms3dBundle *g_drumSet_BassDrumBeaterHolder_ms3d;
Ms3dBundle *g_drumSet_BassDrumPedal_ms3d;
Ms3dBundle *g_drumSet_Stick_ms3d;
Ms3dBundle *g_drumSet_Timbale_ms3d;
Ms3dBundle *g_drumSet_bassDrum_ms3d;
Ms3dBundle *g_drumSet_bongo_ms3d;
Ms3dBundle *g_drumSet_chinaCymbal_ms3d;
Ms3dBundle *g_drumSet_conga_ms3d;
Ms3dBundle *g_drumSet_cymbal_ms3d;
Ms3dBundle *g_drumSet_snareDrum_ms3d;
Ms3dBundle *g_drumSet_tom_ms3d;
Ms3dBundle *g_flute_leftHandX_ms3d;
Ms3dBundle *g_flute_ms3d;
Ms3dBundle *g_flute_rightHandX_ms3d;
Ms3dBundle *g_frenchHornBody_ms3d;
Ms3dBundle *g_frenchHornHorn_ms3d;
Ms3dBundle *g_frenchHornKey1_ms3d;
Ms3dBundle *g_frenchHornKey2_ms3d;
Ms3dBundle *g_frenchHornKey3_ms3d;
Ms3dBundle *g_frenchHornTrigger_ms3d;
Ms3dBundle *g_harmonica_ms3d;
Ms3dBundle *g_harpShadow_ms3d;
Ms3dBundle *g_harpString_ms3d[3];
Ms3dBundle *g_harpStringPlayingX_ms3d[3][5];
Ms3dBundle *g_harp_ms3d;
Ms3dBundle *g_malletHitShadow_ms3d;
Ms3dBundle *g_maraca_ms3d;
Ms3dBundle *g_melodicTom_ms3d;
Ms3dBundle *g_musicBoxCase_ms3d;
Ms3dBundle *g_musicBoxKey_ms3d;
Ms3dBundle *g_musicBoxPoint_ms3d;
Ms3dBundle *g_musicBoxSpindle_ms3d;
Ms3dBundle *g_musicBoxTopBlade_ms3d;
Ms3dBundle *g_mutedTriangle_ms3d;
Ms3dBundle *g_ocarinaHandX_ms3d;
Ms3dBundle *g_ocarina_ms3d;
Ms3dBundle *g_panPipe_ms3d;
Ms3dBundle *g_pianoShadow_ms3d;
Ms3dBundle *g_pianoStand_ms3d;
Ms3dBundle *g_piccolo_ms3d;
Ms3dBundle *g_pizzicatoStringHolder_ms3d;
Ms3dBundle *g_popBottleMiddle_ms3d;
Ms3dBundle *g_popBottlePop_ms3d;
Ms3dBundle *g_popBottle_ms3d;
Ms3dBundle *g_recorderLeftHandX_ms3d;
Ms3dBundle *g_recorder_ms3d;
Ms3dBundle *g_sapranoSaxBody_ms3d;
Ms3dBundle *g_screenGradient_ms3d;
Ms3dBundle *g_songFillbarBox_ms3d;
Ms3dBundle *g_songFillbar_ms3d;
Ms3dBundle *g_squareShaker_ms3d;
Ms3dBundle *g_stageChoir_ms3d;
Ms3dBundle *g_stageHorn_ms3d;
Ms3dBundle *g_stageStringBottomX_ms3d[5];
Ms3dBundle *g_stageStringBow_ms3d;
Ms3dBundle *g_stageStringHolder_ms3d;
Ms3dBundle *g_stageString_ms3d;
Ms3dBundle *g_stage_ms3d;
Ms3dBundle *g_steamPuff_harmonica_ms3d;
Ms3dBundle *g_steelDrumMallet_ms3d;
Ms3dBundle *g_synthDrum_ms3d;
Ms3dBundle *g_taikoStick_ms3d;
Ms3dBundle *g_taiko_ms3d;
Ms3dBundle *g_telephoneBase_ms3d;
Ms3dBundle *g_telephoneHandle_ms3d;
Ms3dBundle *g_telephoneKeyX_ms3d;
Ms3dBundle *g_tenorSaxBody_ms3d;
Ms3dBundle *g_tenorSaxHorn_ms3d;
Ms3dBundle *g_timpaniBody_ms3d;
Ms3dBundle *g_timpaniHead_ms3d;
Ms3dBundle *g_triangleStick_ms3d;
Ms3dBundle *g_triangle_ms3d;
Ms3dBundle *g_tromboneSlide_ms3d;
Ms3dBundle *g_trombone_ms3d;
Ms3dBundle *g_trumpetBody_ms3d;
Ms3dBundle *g_trumpetHorn_ms3d;
Ms3dBundle *g_trumpetKey1_ms3d;
Ms3dBundle *g_trumpetKey2_ms3d;
Ms3dBundle *g_trumpetKey3_ms3d;
Ms3dBundle *g_trumpetMute_ms3d;
Ms3dBundle *g_tubaBody_ms3d;
Ms3dBundle *g_tubaHorn_ms3d;
Ms3dBundle *g_tubaKey_1_ms3d;
Ms3dBundle *g_tubaKey_2_ms3d;
Ms3dBundle *g_tubaKey_3_ms3d;
Ms3dBundle *g_tubaKey_4_ms3d;
Ms3dBundle *g_tubularBellDark_ms3d;
Ms3dBundle *g_tubularBellMallet_ms3d;
Ms3dBundle *g_tubularBell_ms3d;
Ms3dBundle *g_viola_ms3d;
Ms3dBundle *g_violinBow_ms3d;
Ms3dBundle *g_violinFinger_ms3d;
Ms3dBundle *g_violinStringPlayedX_ms3d;
Ms3dBundle *g_violinString_ms3d;
Ms3dBundle *g_violin_ms3d;
Ms3dBundle *g_woodBlockHigh_ms3d;
Ms3dBundle *g_woodBlockLow_ms3d;
Ms3dBundle *g_xylophoneCase_ms3d;
Ms3dBundle *g_xylophoneLegs_ms3d;
Ms3dBundle *g_xylophoneMalletWhite_ms3d;
Ms3dBundle *g_xylophoneShadow_ms3d;
Ms3dBundle *g_zapperLaser_ms3d;
Ms3dBundle *g_zapper_ms3d;

PianoModels **g_pianoModels_ms3d_arr;
XylophoneModels **g_xylophoneModels_ms3d_arr;

// Camera
CameraPosition CAMERA_POSITIONS[11] = {
    {-2.0, 60.0, 120.0, -2.0, 20.0, 0.0},
    {60.0, 60.0, 110.0, 10.0, 20.0, 0.0},
    {-60.0, 60.0, 80.0, -20.0, 20.0, 0.0},
    {0.0, 40.0, 30.0, -50.0, 20.0, -20.0},
    {0.0, 50.0, 20.0, 50.0, 24.0, -25.0},
    {0.0, 30.0, 25.0, 0.0, 5.0, -65.0},
    {5.0, 400.0, 10.0, 5.0, 0.0, -40.0},
    {-35.0, 45.0, 20.0, -50.0, -25.0, -25.0},
    {35.0, -5.0, -30.0, 55.0, 25.0, -40.0},
    {-20.0, 45.0, -10.0, 5.0, 0.0, -95.0},
    {17.0, -1.0, 30.0, 42.0, 4.0, -5.0},
};
int g_autoCamIdleTime;
CameraAngle g_targetCameraAngle;
float g_cameraLocation[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
__int16 g_rotatingCameraIsActive;
GLfloat g_rotatingCameraAngle;
ROTATING_CAMERA_DIRECTION g_rotatingCameraDirection;
__int16 g_rotatingCameraIdleTime;
float g_autoCamDeltaTransform[6];
__int16 g_autoCamIsIdle;
__int16 g_autoCameraIsActive = 1;
__int16 g_lastCameraAngle;

// Window
int g_windowCenterX;
int g_windowCenterY;

// Domain
char g_midiFileNameDisp[1000];
double g_framesPerSecond;
CHAR g_workingDirectory[1000];
int g_applicationStartTime;

// Fillbar
GLfloat g_songFillbarScale;

// Cymbals
GLfloat CYMBAL_LOC_X[7] = {-2.100, -19.630, 13.000, 24.360, -26.350, 31.754, 31.754};
GLfloat CYMBAL_LOC_Y[7] = {47.500, 47.000, 46.500, 40.000, 39.000, 33.000, 33.000};
GLfloat CYMBAL_LOC_Z[7] = {-13.860, -12.672, -14.533, -3.600, 0.086, 9.120, 9.120};

// Stringed instruments
__int16 VIBRATING_STRING_ANIM_SEQUENCE[8] = {0, 1, 2, 1, 0, 3, 4, 3};
__int16 g_vibratingString_frame;
__int16 g_vibratingString_frameIndex;

// Misc.
__int16 g_latinSquare[36];
float g_pianoXylophoneOffset;
GLfloat g_cymbal_rot_y[7];
float g_cymbalRestingAngle[7];

// Unknown
GLfloat KEY_WIDTHS[14] = {
    0.5, 0.5, 0.5, 1.0, 0.5, 0.5, 0.5, 1.0, 1.0, 0.5, 0.5, 0.5, 0.5, 0.5
};
__int16 word_45EB30[88] = {
    1, 12, 3,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1,
};
DWORD dword_46E6A8;
float flt_468BF8[23];
float flt_45EA70[23];
__int16 word_46CEE0[138];
__int16 word_46CEE2[138];
__int16 word_46CEE4[138];
__int16 word_46CEE6[138];
__int16 word_46CEE8[138];
__int16 word_46CEEA[138];
float flt_4654A0[23];
float flt_45EAD0[24];
__int16 word_46B2D0[72];
__int16 word_46B2D2[72];
__int16 word_46B2D4[72];
__int16 word_46B2D6[72];
__int16 word_4688C0[72];
__int16 word_4688C2[72];
__int16 word_4688C4[72];
__int16 word_4688C6[72];
__int16 word_46BBB0[112];
__int16 word_46BBB2[112];
__int16 word_46BBB4[112];
__int16 word_46BBB6[112];
__int16 word_468258[196];
__int16 word_46825A[196];
__int16 word_46825C[196];
__int16 word_46825E[196];
float flt_4679E0[49];
__int16 word_4688B0;
int dword_464BFC;
int dword_464F68;
int dword_464C00;
int dword_464F6C;
int dword_464C04;
int dword_464F70;
int dword_464C08;
int dword_464F74;
int dword_464C0C;
int dword_464F78;
int dword_464C10;
int dword_464F7C;
int dword_464C14;
int dword_464F80;
int dword_464C18;
int dword_464F84;
int dword_464C1C;
int dword_464F88;
int dword_464C20;
int dword_464F8C;
int dword_464C24;
int dword_464F90;
int dword_464C28;
int dword_464F94;
int dword_464C2C;
int dword_464F98;
__int16 word_46E268;

// Config and window (from MidiJam.h)
bool g_useSound;
bool g_fullscreen;
GLsizei g_windowWidth;
GLsizei g_windowHeight;
__int16 g_worldReady;
int g_bitDepth;
GLfloat g_refreshRate;
int g_windowCenter_X;
int g_windowCenter_Y;
__int16 g_windowWidth_0;
__int16 g_windowHeight_0;
HANDLE g_hMidiEvent;
HWND g_hWnd;
HINSTANCE g_hInstance;
int g_pixelFormatInitialized;
PIXELFORMATDESCRIPTOR g_pixelFormatDescriptor;
HGLRC g_openglRenderingContext;
float g_normalizedWindowScale;


// Input (from MidiJam.h)
char g_leftButtonDown;
char g_leftButtonClicked;
char g_middleButtonDown;
char g_middleButtonClicked;
char g_rightButtonDown;
char g_rightButtonClicked;
char g_mouseWheelMoved;
int g_mouseWheelDelta;
char g_mouseMoved;
__int16 g_mouseY;
__int16 g_mouseX;
char g_keyStateArray[256];
char g_lastKeyPressed;
char g_isWindowActive;

// DirectMusic (from MidiJam.h)
DirectMusicSystem *g_DirectMusicSystem;
DirectMusicSegmentWrapper *g_DirectMusicSegmentWrapper;
IDirectMusicPerformance8 *g_DirectMusicPerformance;
IDirectMusicGraph8 *g_DirectMusicGraph;
IMidiJamTool *g_midiJamTool;
HDC g_hdc;

// Lighting (from MidiJam.h)
GLfloat LIGHT_PARAMS_0_AMBIENT[4] = {
    0.25, 0.25, 0.25, 1.0
};
GLfloat LIGHT_PARAMS_0_DIFFUSE[4] = {
    1.0, 1.0, 1.0, 1.0
};
GLfloat LIGHT_PARAMS_0_POSITION[4] = {
    250.0, 175.0, 150.0, 1.0
};
GLfloat LIGHT_PARAMS_0_SPECULAR[4] = {
    1.0, 1.0, 1.0, 1.0
};
GLfloat LIGHT_PARAMS_1_AMBIENT[4] = {
    0.025, 0.025, 0.025, 1.0
};
GLfloat LIGHT_PARAMS_1_DIFFUSE[4] = {
    1.0, 1.0, 1.0, 1.0
};
GLfloat LIGHT_PARAMS_1_POSITION[4] = {
    -250.0, 100.0, -125.0, 1.0
};
