//
// Created by Jacob on 3/17/2026.
//

#include "audio/DirectMusicSystem.h"
#include "audio/DirectMusicSegmentPlayer.h"
#include "model/animation/AnimationController.h"
#include "instruments/instrument_ids.h"
#include "model/Ms3dBundle.h"
#include "scene/camera.h"

#include <dmusici.h>

#include "audio/MidiJamTool.h"

DirectMusicSegmentPlayer* g_directMusicSegmentPlayer = nullptr;
char g_midiFileNameDisp[260] = {};
MUSIC_TIME g_midiFileDuration = 0;
REFERENCE_TIME g_prtStart = 0;
MUSIC_TIME g_mtStart = 0;
AnimationControllerNode* g_pAnimationControllerList = nullptr;
int SHOW_ERRORS = 0;
int g_fontCellWidth = 0;
int g_charWidthProportional[128] = {};
float g_scaledCharacterWidths[128] = {};
GLuint g_fontTextureArray[2] = {};
HDC g_hdc = nullptr;
char g_leftButtonDown = 0;
char g_middleButtonDown = 0;
char g_rightButtonDown = 0;
char g_leftButtonClicked = 0;
char g_middleButtonClicked = 0;
char g_rightButtonClicked = 0;
char g_mouseWheelMoved = 0;
char g_mouseMoved = 0;
short g_mouseX = 0;
short g_mouseY = 0;
short g_mouseWheelDelta = 0;
GLuint g_fontDisplayListBase[2] = {};
HINSTANCE g_hInstance = nullptr;
int g_pixelFormatInitialized = 0;
PIXELFORMATDESCRIPTOR g_pixelFormatDescriptor = {};
HGLRC g_openglRenderingContext = nullptr;
char g_keyStateArray[256] = {};
char g_lastKeyPressed = 0;
char g_isWindowActive = 0;

GLfloat g_fadeFactor;
CHAR g_workingDirectory[1000];
bool g_fullscreen;
int g_windowCenter_X;
int g_windowCenter_Y;
__int16 g_windowWidth_0;
__int16 g_windowHeight_0;
HANDLE g_hMidiEvent;
DirectMusicSystem* g_directMusicSystem;
HWND g_hWnd;
IDirectMusicPerformance8* g_DirectMusicPerformance;
IDirectMusicGraph8* g_DirectMusicGraph;
MidiJamTool* g_midiJamTool;
MidiJamInstrumentId g_midiJamInstrumentIds[300] = {};
int g_currentTempo_scaleFactor0_5 = 0;
int g_currentTempo_scaleFactor0_9 = 0;
int g_currentTempo_scaleFactor1_15 = 0;
double g_currentTempo = 0.0;

// ---------------------------------------------------------------------------
// State — app lifecycle
// ---------------------------------------------------------------------------
int g_worldReady = 0;
int g_killApplication_0 = 0;
int g_killApplication = 0;
int g_isShuttingDown = 0;
int g_isFadingIn = 0;
DWORD g_applicationStartTime = 0;
#if _MSC_VER < 1400
long        g_lastUnixEpochTime = 0;
#else
time_t      g_lastUnixEpochTime = 0;
#endif
float g_framesPerSecond = 0.0f;
int g_framesAlive = 0;
int g_framesSinceStart = 0;
MMRESULT g_timerEventId = 0;

// ---------------------------------------------------------------------------
// Window / render
// ---------------------------------------------------------------------------
int g_windowWidth = 0;
float g_normalizedWindowScale = 0.0f;
int g_windowCenterX = 0;
int g_windowCenterY = 0;

// ---------------------------------------------------------------------------
// Key / input state
// ---------------------------------------------------------------------------
char g_shiftPressed = 0;
char g_isSpacePressed = 0;
char g_isAKeyPressed = 0;
char g_framesKeyRightPressed = 0;
char g_framesKeyLeftPressed = 0;
char g_framesKeyUpPressed = 0;
char g_framesKeyDownPressed = 0;
float g_playbackSpeed = 1.0f;

// ---------------------------------------------------------------------------
// Camera
// ---------------------------------------------------------------------------
float g_cameraLocation[6] = {};
int g_rotatingCameraIsActive = 0;
float g_rotatingCameraAngle = 0.0f;
RotatingCameraDirection g_rotatingCameraDirection = CLOCKWISE;
int g_rotatingCameraIdleTime = 0;
CameraAngle g_targetCameraAngle = CAMERA_1A;
float g_autoCamDeltaTransform[6] = {};
int g_autoCamIsIdle = 0;
int g_autoCamIdleTime = 0;
int g_autoCameraIsActive = 0;

// ---------------------------------------------------------------------------
// Animation
// ---------------------------------------------------------------------------
int g_isEvenFrame = 0;
int g_vibratingString_frameIndex = 0;
int g_vibratingString_frame = 0;

// ---------------------------------------------------------------------------
// Percussion
// ---------------------------------------------------------------------------
int g_show_percussion = 0;
short g_inst_visible_drumset = 0;
short g_inst_visible_xylophone = 0;
short g_inst_visible_guitar = 0;
int g_percussion_time_queue[88][32] = {};
int g_percussion_velocity_queue[88][32] = {};
int g_percussion_framesWithEmptyQueue = 0;
int g_hihat_isOpen = 0;
int g_lastRideCymbal = 0;
short g_cymbal_patches[7] = {};

// ---------------------------------------------------------------------------
// Recoil / physics constants (values populated from binary data; zeroed for now)
// ---------------------------------------------------------------------------
float RECOIL_SCALE_FACTOR = 5.0f;
float MAX_RECOIL = 0.0f;
float CYMBAL_LOC_X[7] = {};
float CYMBAL_LOC_Z[7] = {};
float g_cymbal_rot_y[7] = {};
float g_cymbalRestingAngle[7] = {};
float g_cymbalMassFactor[7] = {};
float g_cymbalCurrentWobble[7] = {};
float g_cymbalAngularVelocity[7] = {};
float g_cymbalWobbleAmplitude[7] = {};
float g_recoil_cymbals[7] = {};
float CYMBAL_MASS_FACTOR[7] = {};
float CYMBAL_SCALE[7] = {};
float CYMBAL_MAX_WOBBLE[7] = {};
float CYMBAL_WOBBLE_AMPLITUDE[7] = {};
float CYMBAL_ANGULAR_VELOCITY_FACTOR[7] = {};

float g_recoil_snaredrum = 0.0f;
float g_recoil_snare = 0.0f;
float g_recoil_sideStick = 0.0f;
float g_recoil_tom[6] = {};
float g_recoil_bassdrum = 0.0f;
float g_recoil_bassDrumArm = 0.0f;
float g_recoil_cowbell = 0.0f;
float g_recoil_clap_hand_r = 0.0f;
float g_recoil_clap_hand_l = 0.0f;
float g_recoil_tambourine_hand = 0.0f;
float g_recoil_tambourine = 0.0f;
float g_recoil_sticks_1 = 0.0f;
float g_recoil_clave_l = 0.0f;
float g_recoil_jingleBell = 0.0f;
float g_recoil_castanets = 0.0f;
float g_recoil_shaker = 0.0f;
float g_recoil_highQ = 0.0f;
float g_recoil_squareClick = 0.0f;
float g_recoil_metronomeClick = 0.0f;
float g_recoil_metronomeBell = 0.0f;
float g_recoil_Maracas = 0.0f;
float g_recoil_cabasa = 0.0f;
float g_recoil_agogo_hi = 0.0f;
float g_recoil_agogo_lo = 0.0f;
float g_recoil_woodblock_hi = 0.0f;
float g_recoil_woodblock_lo = 0.0f;
float g_recoil_triangle_open = 0.0f;
float g_recoil_triangle_muted = 0.0f;
float g_recoil_whistle_long = 0.0f;
float g_recoil_whistle_short = 0.0f;
float g_recoil_timbale_hi = 0.0f;
float g_recoil_timbale_lo = 0.0f;
float g_recoil_bongo_hi = 0.0f;
float g_recoil_bongo_lo = 0.0f;
float g_recoil_conga_high_mute = 0.0f;
float g_recoil_conga_high_open = 0.0f;
float g_recoil_conga_low = 0.0f;
float g_recoil_hihat = 0.0f;

// ---------------------------------------------------------------------------
// Instrument data tables (sized from binary analysis; zeroed for now)
// TODO: populate from binary when each instrument is transcribed
// ---------------------------------------------------------------------------
int g_latinSquare[36] = {};
int VIBRATING_STRING_ANIM_SEQUENCE[8] = {};
short word_46CEE0[23 * 6] = {};
short word_46B2D0[18 * 4] = {};
short word_4688C0[18 * 4] = {};
short word_46BBB0[28 * 4] = {};
short word_468258[49 * 4] = {};
float flt_4654A0[23] = {};
float flt_45EAD0[23] = {};
float flt_4679E0[49] = {};

// Clave/recoil adjacency table (interleaved with g_pianokey_translation_x)
// TODO: name when clave is transcribed
int unk_464F68 = 0;
int unk_464F6C = 0;
int unk_464F70 = 0;
int unk_464F74 = 0;
int unk_464F78 = 0;
int unk_464F7C = 0;
int unk_464F80 = 0;
int unk_464F84 = 0;
int unk_464F88 = 0;
int unk_464F8C = 0;
int unk_464F90 = 0;
int unk_464F94 = 0;
int unk_464F98 = 0;

// MIDI file name buffer
// IDA referenced the first byte as g_midiFileName and later bytes as byte_461DD6
// They are the same array; byte_461DD6 is a pointer alias into it.
char g_midiFileName[260] = {};
char* byte_461DD6 = g_midiFileName;

// HWF archive
FILE* g_hwfStream = nullptr;
void* g_pHwfAppendix = nullptr;
unsigned int g_nHwfAppendixItems = 0;

// Particle system — type unknown until transcribed
void* g_ds_particles = nullptr;

// Song fill bar
GLfloat g_songFillbarScale = 0.0f;

// ---------------------------------------------------------------------------
// Instrument data pointers — non-piano; nullptr until transcribed
// ---------------------------------------------------------------------------
void* g_ds_trombone = nullptr;
void* g_ds_trumpet = nullptr;
void* g_ds_frenchHorn = nullptr;
void* g_ds_baritoneSax = nullptr;
void* g_ds_tenorSax = nullptr;
void* g_ds_sapranoSax = nullptr;
void* g_ds_altoSax = nullptr;
void* g_ds_recorder = nullptr;
void* g_ds_piccolo = nullptr;
void* g_ds_flute = nullptr;
void* g_ds_tuba = nullptr;
void* g_ds_whistles = nullptr;
void* g_ds_panPipe = nullptr;
void* g_ds_harmonica = nullptr;
void* g_ds_violin = nullptr;
void* g_ds_viola = nullptr;
void* g_ds_cello = nullptr;
void* g_ds_doubleBass = nullptr;
void* g_ds_popBottle = nullptr;
void* g_ds_agogos = nullptr;
void* g_ds_woodblocks = nullptr;
void* g_ds_stageChoir = nullptr;
void* g_ds_stageString = nullptr;
void* g_ds_pizzicatoStrings = nullptr;
void* g_ds_ocarina = nullptr;
void* g_ds_xylophone = nullptr;
void* g_ds_musicBox = nullptr;
void* g_ds_melodicTom = nullptr;
void* g_ds_synthDrum = nullptr;
void* g_ds_steelDrum = nullptr;
void* g_ds_timpani = nullptr;
void* g_ds_taiko = nullptr;
void* g_ds_telephone = nullptr;
void* g_ds_tubularBells = nullptr;
void* g_ds_guitar = nullptr;

// ---------------------------------------------------------------------------
// Scene models — shadow and stage
// ---------------------------------------------------------------------------
Ms3dBundle* g_screenGradient_ms3d = nullptr;
Ms3dBundle* g_songFillbarBox_ms3d = nullptr;
Ms3dBundle* g_songFillbar_ms3d = nullptr;
Ms3dBundle* g_stage_ms3d = nullptr;
Ms3dBundle* g_pianoShadow_ms3d = nullptr;
Ms3dBundle* g_xylophoneShadow_ms3d = nullptr;
Ms3dBundle* g_guitarShadow_ms3d = nullptr;
Ms3dBundle* g_drumShadow_ms3d = nullptr;

Ms3dBundle* g_agogoSingle_ms3d = nullptr;
Ms3dBundle* g_woodBlockSingle_ms3d = nullptr;

// Drum set
Ms3dBundle* g_drumSet_Stick_ms3d = nullptr;
Ms3dBundle* g_cowbell_ms3d = nullptr;
Ms3dBundle* handRight_ms3d = nullptr;
Ms3dBundle* handLeft_ms3d = nullptr;
Ms3dBundle* handTambourine_ms3d = nullptr;
Ms3dBundle* clave_ms3d = nullptr;
Ms3dBundle* jingleBells_ms3d = nullptr;
Ms3dBundle* castanets_ms3d = nullptr;
Ms3dBundle* shaker_ms3d = nullptr;
Ms3dBundle* g_zapper_ms3d = nullptr;
Ms3dBundle* g_zapperLaser_ms3d = nullptr;
Ms3dBundle* g_squareShaker_ms3d = nullptr;
Ms3dBundle* g_maraca_ms3d = nullptr;
Ms3dBundle* g_cabasa_ms3d = nullptr;
Ms3dBundle* g_agogo_ms3d = nullptr;
Ms3dBundle* g_woodBlockHigh_ms3d = nullptr;
Ms3dBundle* g_woodBlockLow_ms3d = nullptr;
Ms3dBundle* g_triangle_ms3d = nullptr;
Ms3dBundle* g_mutedTriangle_ms3d = nullptr;
Ms3dBundle* g_triangleStick_ms3d = nullptr;
Ms3dBundle* g_drumSet_Timbale_ms3d = nullptr;
Ms3dBundle* g_drumSet_bongo_ms3d = nullptr;
Ms3dBundle* g_drumSet_conga_ms3d = nullptr;
Ms3dBundle* g_drumSet_bassDrum_ms3d = nullptr;
Ms3dBundle* g_drumSet_snareDrum_ms3d = nullptr;
Ms3dBundle* g_drumSet_tom_ms3d = nullptr;
Ms3dBundle* g_drumSet_cymbal_ms3d = nullptr;
Ms3dBundle* g_drumSet_chinaCymbal_ms3d = nullptr;
Ms3dBundle* g_drumSet_BassDrumBeaterArm_ms3d = nullptr;
Ms3dBundle* g_drumSet_BassDrumBeaterHolder_ms3d = nullptr;
Ms3dBundle* g_drumSet_BassDrumPedal_ms3d = nullptr;

// Percussion / melodic drums
Ms3dBundle* g_timpaniBody_ms3d = nullptr;
Ms3dBundle* g_timpaniHead_ms3d = nullptr;
Ms3dBundle* g_synthDrum_ms3d = nullptr;
Ms3dBundle* g_steelDrum_recoil = nullptr;
Ms3dBundle* g_steelDrumMallet_ms3d = nullptr;
Ms3dBundle* g_melodicTom_ms3d = nullptr;
Ms3dBundle* g_taiko_ms3d = nullptr;
Ms3dBundle* g_taikoStick_ms3d = nullptr;
Ms3dBundle* metronome_ms3d = nullptr;
Ms3dBundle* metronomePendjulum1_ms3d = nullptr;
Ms3dBundle* metronomePendjulum2_ms3d = nullptr;

// Music box
Ms3dBundle* g_musicBoxSpindle_ms3d = nullptr;
Ms3dBundle* g_musicBoxPoint_ms3d = nullptr;
Ms3dBundle* g_musicBoxKey_ms3d = nullptr;
Ms3dBundle* g_musicBoxTopBlade_ms3d = nullptr;
Ms3dBundle* g_musicBoxCase_ms3d = nullptr;

// Strings / winds
Ms3dBundle* g_stageStringHolder_ms3d = nullptr;
Ms3dBundle* g_pizzicatoStringHolder_ms3d = nullptr;
Ms3dBundle* g_stageStringBow_ms3d = nullptr;
Ms3dBundle* g_stageString_ms3d = nullptr;
Ms3dBundle* g_stageStringBottomX_ms3d[5] = {};
Ms3dBundle* whistle_ms3d = nullptr;
Ms3dBundle* g_panPipe_ms3d = nullptr;
Ms3dBundle* g_calliope_ms3d = nullptr;
Ms3dBundle* g_harmonica_ms3d = nullptr;
Ms3dBundle* g_popBottle_ms3d = nullptr;
Ms3dBundle* g_popBottleMiddle_ms3d = nullptr;
Ms3dBundle* g_popBottlePop_ms3d = nullptr;
Ms3dBundle* popBottleLabel_ms3d = nullptr;
Ms3dBundle* steamCloud_0_ms3d = nullptr;
Ms3dBundle* steamCloud_1_ms3d = nullptr;
Ms3dBundle* steamCloud_2_ms3d = nullptr;
Ms3dBundle* g_steamPuff_harmonica_ms3d = nullptr;
Ms3dBundle* g_violin_ms3d = nullptr;
Ms3dBundle* g_viola_ms3d = nullptr;
Ms3dBundle* g_cello_ms3d = nullptr;
Ms3dBundle* g_doubleBass_ms3d = nullptr;
Ms3dBundle* g_violinString_ms3d = nullptr;
Ms3dBundle* g_violinStringPlayedX_ms3d[5] = {};
// g_violinFinger_ms3d — IDA uses this as a 21-element array base
// [0..19] = telephone keys (lit/dark), [20] = actual violin finger
// Declared as a single pointer; array access uses pointer arithmetic
Ms3dBundle* g_violinFinger_ms3d = nullptr;
Ms3dBundle* g_violinBow_ms3d = nullptr;

// Tubular bells
Ms3dBundle* g_tubularBell_ms3d = nullptr;
Ms3dBundle* g_tubularBellDark_ms3d = nullptr;
Ms3dBundle* g_tubularBellMallet_ms3d = nullptr;

// Choir / accordion
Ms3dBundle* g_stageChoir_ms3d = nullptr;

// Xylophone
Ms3dBundle* g_xylophoneMalletWhite_ms3d = nullptr;
Ms3dBundle* g_malletHitShadow_ms3d = nullptr;
Ms3dBundle* g_xylophoneLegs_ms3d = nullptr;
Ms3dBundle* g_xylophoneCase_ms3d = nullptr;
Ms3dBundle* g_xylophoneWhiteBar_ms3d[4] = {};
Ms3dBundle* g_xylophoneWhiteBarDown_ms3d[4] = {};
Ms3dBundle* g_xylophoneBlackBar_ms3d[4] = {};
Ms3dBundle* g_xylophoneBlackBarDown_ms3d[4] = {};

// Sax
Ms3dBundle* g_baritoneSaxBody_ms3d = nullptr;
Ms3dBundle* g_baritoneSaxHorn_ms3d = nullptr;
Ms3dBundle* g_tenorSaxBody_ms3d = nullptr;
Ms3dBundle* g_tenorSaxHorn_ms3d = nullptr;
Ms3dBundle* g_sapranoSaxBody_ms3d = nullptr;
Ms3dBundle* g_sapranoSaxHorn_ms3d = nullptr;
Ms3dBundle* g_altoSaxBody_ms3d = nullptr;
Ms3dBundle* g_altoSaxHorn_ms3d = nullptr;
Ms3dBundle* g_altoSaxKeyX_ms3d[40] = {};

// Flute / piccolo / recorder
Ms3dBundle* g_flute_ms3d = nullptr;
Ms3dBundle* g_piccolo_ms3d = nullptr;
Ms3dBundle* g_flute_leftHandX_ms3d[13] = {};
Ms3dBundle* g_flute_rightHandX_ms3d[12] = {};
Ms3dBundle* g_recorder_ms3d = nullptr;
Ms3dBundle* g_recorderLeftHandX_ms3d[13] = {};
Ms3dBundle* g_recorderRightHandX_ms3d = nullptr;
Ms3dBundle* dword_465230 = nullptr;

// Telephone
Ms3dBundle* g_telephoneBase_ms3d = nullptr;
Ms3dBundle* g_telephoneHandle_ms3d = nullptr;
Ms3dBundle* g_telephoneKeyX_ms3d = nullptr;
Ms3dBundle* dword_46D25C = nullptr;
Ms3dBundle* dword_46D260 = nullptr;
Ms3dBundle* dword_46D264 = nullptr;

// Ocarina
Ms3dBundle* g_ocarina_ms3d = nullptr;
Ms3dBundle* g_ocarinaHandX_ms3d = nullptr;

// Brass
Ms3dBundle* g_frenchHornBody_ms3d = nullptr;
Ms3dBundle* g_frenchHornHorn_ms3d = nullptr;
Ms3dBundle* g_frenchHornKey1_ms3d = nullptr;
Ms3dBundle* g_frenchHornKey2_ms3d = nullptr;
Ms3dBundle* g_frenchHornKey3_ms3d = nullptr;
Ms3dBundle* g_frenchHornTrigger_ms3d = nullptr;
Ms3dBundle* g_tubaBody_ms3d = nullptr;
Ms3dBundle* g_tubaHorn_ms3d = nullptr;
Ms3dBundle* g_tubaKey_1_ms3d = nullptr;
Ms3dBundle* g_tubaKey_2_ms3d = nullptr;
Ms3dBundle* g_tubaKey_3_ms3d = nullptr;
Ms3dBundle* g_tubaKey_4_ms3d = nullptr;
Ms3dBundle* g_trumpetBody_ms3d = nullptr;
Ms3dBundle* g_trumpetHorn_ms3d = nullptr;
Ms3dBundle* g_trumpetMute_ms3d = nullptr;
Ms3dBundle* g_trumpetKey1_ms3d = nullptr;
Ms3dBundle* g_trumpetKey2_ms3d = nullptr;
Ms3dBundle* g_trumpetKey3_ms3d = nullptr;
Ms3dBundle* g_trombone_ms3d = nullptr;
Ms3dBundle* g_tromboneSlide_ms3d = nullptr;

// Bass / guitar
Ms3dBundle* guitar_ms3d = nullptr;
Ms3dBundle* dword_462FA8 = nullptr;
Ms3dBundle* guitarStringLow_ms3d = nullptr;
Ms3dBundle* guitarLowStringBottomX_ms3d = nullptr;
Ms3dBundle* guitarHighStringBottomX_ms3d = nullptr;
Ms3dBundle* guitarNoteFinger_ms3d = nullptr;

// Camera positions — populated by MidiJamInitialize
// Size: 11 entries (CAMERA_1A through CAMERA_6)
struct CameraPosition
{
    float cameraX, cameraY, cameraZ;
    float lookAtX, lookAtY, lookAtZ;
};

CameraPosition CAMERA_POSITIONS[11] = {
    {-2.0f, 60.0f, 120.0f, -2.0f, 20.0f, 0.0f}, // CAMERA_1A
    {60.0f, 60.0f, 110.0f, 10.0f, 20.0f, 0.0f}, // CAMERA_1B
    {-60.0f, 60.0f, 80.0f, -20.0f, 20.0f, 0.0f}, // CAMERA_1C
    {0.0f, 40.0f, 30.0f, -50.0f, 20.0f, -20.0f}, // CAMERA_2A
    {0.0f, 50.0f, 20.0f, 50.0f, 24.0f, -25.0f}, // CAMERA_4A
    {0.0f, 30.0f, 25.0f, 0.0f, 5.0f, -65.0f}, // CAMERA_3A
    {5.0f, 400.0f, 10.0f, 5.0f, 0.0f, -40.0f}, // CAMERA_5
    {-35.0f, 45.0f, 20.0f, -50.0f, -25.0f, -25.0f}, // CAMERA_2B
    {35.0f, -5.0f, -30.0f, 55.0f, 25.0f, -40.0f}, // CAMERA_4B
    {-20.0f, 45.0f, -10.0f, 5.0f, 0.0f, -95.0f}, // CAMERA_3B
    {17.0f, -1.0f, 30.0f, 42.0f, 4.0f, -5.0f}, // CAMERA_6
};

// DirectMusic GUID
GUID GUID_PERF_MASTER_TEMPO = {
    0x54344c80, 0xddc3, 0x11d1,
    {0x87, 0x54, 0x00, 0x60, 0x08, 0x33, 0xdb, 0xd8}
};


char g_useSound = 0;
GLsizei g_windowHeight = 0;
int g_bitDepth = 0;
GLfloat g_refreshRate = 0.0f;

// ---------------------------------------------------------------------------
// Percussion
// ---------------------------------------------------------------------------

short g_stick_visible[37] = {};

// ---------------------------------------------------------------------------
// Per-instrument allocation counters
// ---------------------------------------------------------------------------

short g_ialloc_guitar = 0;
short g_ialloc_xylophone = 0;
short g_ialloc_stageChoir = 0;
short g_ialloc_whistles = 0;
short g_ialloc_panPipe = 0;
short g_ialloc_harmonica = 0;
short g_ialloc_popBottle = 0;
short g_ialloc_agogo = 0;
short g_ialloc_woodblocks = 0;
short g_ialloc_stageString = 0;
short g_ialloc_pizzicatoStrings = 0;
short g_ialloc_musicBox = 0;
short g_ialloc_melodicTom = 0;
short g_ialloc_synthDrum = 0;
short g_ialloc_steelDrum = 0;
short g_ialloc_timpani = 0;
short g_ialloc_taiko = 0;
short g_ialloc_tubularBells = 0;
short g_ialloc_telephone = 0;
short g_ialloc_trombone = 0;
short g_ialloc_tuba = 0;
short g_ialloc_ocarina = 0;
short g_ialloc_frenchHorn = 0;
short g_ialloc_baritoneSax = 0;
short g_ialloc_tenorSax = 0;
short g_ialloc_sapranoSax = 0;
short g_ialloc_altoSax = 0;
short g_ialloc_recorder = 0;
short g_ialloc_piccolo = 0;
short g_ialloc_flute = 0;
short g_ialloc_trumpet = 0;

// ---------------------------------------------------------------------------
// Per-channel instrument assignment arrays (300 channels)
// ---------------------------------------------------------------------------

short g_violin_assignment[300] = {};
short g_viola_assignment[300] = {};
short g_cello_assignment[300] = {};
short g_doubleBass_assignment[300] = {};
short g_doubleBass_playingStyle[300] = {};
short g_xylophone_types[300] = {};
short g_guitar_assignment[300] = {};
short g_xylophone_assignment[300] = {};
short g_stateChoir_assignment[300] = {};
short g_whistles_assignment[300] = {};
short g_panPipe_assignment[300] = {};
short g_isPanPipeCalliope[300] = {};
short g_harmonica_assignment[300] = {};
short g_popBottle_assignment[300] = {};
short g_agogos_assignment[300] = {};
short g_woodblocks_assignment[300] = {};
short g_stageStrings_assignment[300] = {};
short g_pizzicatoStrings_assignment[300] = {};
short g_musicBox_assignment[300] = {};
short g_melodicTom_assignment[300] = {};
short g_synthDrum_assignment[300] = {};
short g_steelDrum_assignment[300] = {};
short g_timpani_assignment[300] = {};
short g_taiko_assignment[300] = {};
short g_tubularBells_assignment[300] = {};
short g_telephone_assignment[300] = {};
short g_trombone_assignment[300] = {};
short g_tuba_assignment[300] = {};
short g_ocarina_assignment[300] = {};
short g_frenchHorn_assignment[300] = {};
short g_baritoneSax_assignment[300] = {};
short g_tenorSax_assignment[300] = {};
short g_sapranoSax_assignment[300] = {};
short g_altoSax_assignment[300] = {};
short g_recorder_assignment[300] = {};
short g_piccolo_assignment[300] = {};
short g_flute_assignment[300] = {};
short g_trumpet_assignment[300] = {};

// ---------------------------------------------------------------------------
// Unnamed globals — TODO: name when context is clearer
// ---------------------------------------------------------------------------

int dword_464700 = 0;
int dword_468EC0 = 0;
short word_468974 = 0;
short word_4614AC = 0;
short word_461C90 = 0;
short word_46AF68 = 0;
short word_463228 = 0;
int dword_46ACF4 = 0;
short word_464710 = 0;
