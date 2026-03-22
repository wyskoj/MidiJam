//
// Created by Jacob on 3/19/2026.
//

#include "initialize.h"

#include "DirectMusicSystem.h"
#include "MidiJamTool.h"
#include "../instruments/instrument_ids.h"
#include "../render/window.h"
#include "scene/camera.h"

#include <windows.h>
#include <shellapi.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <GL/gl.h>

#include "instruments/Bass.h"
#include "instruments/Harp.h"
#include "instruments/Piano.h"
#include "instruments/StageHorn.h"
#include "instruments/StageString.h"
#include "instruments/Xylophone.h"
#include "scene/update.h"


// ---------------------------------------------------------------------------
// GUIDs
// ---------------------------------------------------------------------------

static const GUID NULL_GUID = {};

static const IID IID_I_DIRECT_MUSIC_GRAPH8 = {
    0x2BEFC277, 0x5497, 0x11D2,
    {0xBC, 0xCB, 0x00, 0xA0, 0xC9, 0x22, 0xE6, 0xEB}
};

// ---------------------------------------------------------------------------
// Extern globals
// ---------------------------------------------------------------------------

extern char g_useSound;
extern GLsizei g_windowWidth;
extern GLsizei g_windowHeight;
extern GLfloat g_refreshRate;
extern int g_bitDepth;
extern bool g_fullscreen;
extern char g_workingDirectory[1000];
extern int g_windowCenter_X;
extern int g_windowCenter_Y;
extern __int16 g_windowWidth_0;
extern __int16 g_windowHeight_0;
extern HANDLE g_hMidiEvent;
extern HWND g_hWnd;
extern DirectMusicSystem* g_directMusicSystem;
extern IDirectMusicPerformance8* g_DirectMusicPerformance;
extern IDirectMusicGraph8* g_DirectMusicGraph;
extern MidiJamTool* g_midiJamTool;

// --- Cymbal physics ---
extern float            CYMBAL_SCALE[7];
extern float            g_cymbalMassFactor[7];
extern float            g_recoil_cymbals[7];
extern float            g_cymbalCurrentWobble[7];
extern float            g_cymbalAngularVelocity[7];
extern float            g_cymbalWobbleAmplitude[7];

// --- Percussion ---
extern int              g_lastRideCymbal;
extern int              g_show_percussion;
extern short            g_stick_visible[37];
extern int              g_percussion_time_queue[88][32];
extern int              g_percussion_velocity_queue[88][32];
extern int              g_percussion_framesWithEmptyQueue;
extern int              g_hihat_isOpen;
extern float            g_recoil_bassDrumArm;
extern GLfloat          g_songFillbarScale;

// --- Recoil ---
extern float            g_recoil_cowbell;
extern float            g_recoil_clap_hand_l;
extern float            g_recoil_clap_hand_r;
extern float            g_recoil_tambourine;
extern float            g_recoil_tambourine_hand;
extern float            g_recoil_sticks_1;
extern float            g_recoil_clave_l;
extern float            g_recoil_castanets;
extern float            g_recoil_jingleBell;
extern float            g_recoil_shaker;
extern float            g_recoil_highQ;
extern float            g_recoil_squareClick;
extern float            g_recoil_metronomeClick;
extern float            g_recoil_metronomeBell;
extern float            g_recoil_Maracas;
extern float            g_recoil_cabasa;
extern float            g_recoil_agogo_hi;
extern float            g_recoil_agogo_lo;
extern float            g_recoil_woodblock_hi;
extern float            g_recoil_woodblock_lo;
extern float            g_recoil_triangle_open;
extern float            g_recoil_triangle_muted;
extern float            g_recoil_whistle_long;
extern float            g_recoil_whistle_short;
extern float            g_recoil_timbale_hi;
extern float            g_recoil_timbale_lo;
extern float            g_recoil_bongo_hi;
extern float            g_recoil_bongo_lo;
extern float            g_recoil_conga_high_mute;
extern float            g_recoil_conga_high_open;
extern float            g_recoil_conga_low;
extern float            g_recoil_snaredrum;
extern float            g_recoil_snare;
extern float            g_recoil_sideStick;
extern float            g_recoil_tom[6];
extern float            g_recoil_bassdrum;
extern float            g_recoil_hihat;
extern GLfloat          g_pianoKeyOffsetX[14];

// --- Allocation counters ---
extern short            g_ialloc_guitar;
extern short            g_ialloc_xylophone;
extern short            g_ialloc_stageChoir;
extern short            g_ialloc_whistles;
extern short            g_ialloc_panPipe;
extern short            g_ialloc_harmonica;
extern short            g_ialloc_popBottle;
extern short            g_ialloc_agogo;
extern short            g_ialloc_woodblocks;
extern short            g_ialloc_pizzicatoStrings;
extern short            g_ialloc_musicBox;
extern short            g_ialloc_melodicTom;
extern short            g_ialloc_synthDrum;
extern short            g_ialloc_steelDrum;
extern short            g_ialloc_timpani;
extern short            g_ialloc_taiko;
extern short            g_ialloc_tubularBells;
extern short            g_ialloc_telephone;
extern short            g_ialloc_trombone;
extern short            g_ialloc_tuba;
extern short            g_ialloc_ocarina;
extern short            g_ialloc_frenchHorn;
extern short            g_ialloc_baritoneSax;
extern short            g_ialloc_tenorSax;
extern short            g_ialloc_sapranoSax;
extern short            g_ialloc_altoSax;
extern short            g_ialloc_recorder;
extern short            g_ialloc_piccolo;
extern short            g_ialloc_flute;
extern short            g_ialloc_trumpet;

// --- Assignment arrays ---
extern short            g_violin_assignment[300];
extern short            g_viola_assignment[300];
extern short            g_cello_assignment[300];
extern short            g_doubleBass_assignment[300];
extern short            g_doubleBass_playingStyle[300];
extern short            g_xylophone_types[300];
extern short            g_guitar_assignment[300];
extern short            g_stateChoir_assignment[300];
extern short            g_whistles_assignment[300];
extern short            g_panPipe_assignment[300];
extern short            g_isPanPipeCalliope[300];
extern short            g_harmonica_assignment[300];
extern short            g_popBottle_assignment[300];
extern short            g_agogos_assignment[300];
extern short            g_woodblocks_assignment[300];
extern short            g_pizzicatoStrings_assignment[300];
extern short            g_musicBox_assignment[300];
extern short            g_melodicTom_assignment[300];
extern short            g_synthDrum_assignment[300];
extern short            g_steelDrum_assignment[300];
extern short            g_timpani_assignment[300];
extern short            g_taiko_assignment[300];
extern short            g_tubularBells_assignment[300];
extern short            g_telephone_assignment[300];
extern short            g_trombone_assignment[300];
extern short            g_tuba_assignment[300];
extern short            g_ocarina_assignment[300];
extern short            g_frenchHorn_assignment[300];
extern short            g_baritoneSax_assignment[300];
extern short            g_tenorSax_assignment[300];
extern short            g_sapranoSax_assignment[300];
extern short            g_altoSax_assignment[300];
extern short            g_recorder_assignment[300];
extern short            g_piccolo_assignment[300];
extern short            g_flute_assignment[300];
extern short            g_trumpet_assignment[300];
extern MidiJamInstrumentId g_midiJamInstrumentIds[300];

// --- Unnamed ---
extern int              dword_464700;
extern int              dword_468EC0;
extern short            word_468974;
extern short            word_4614AC;
extern short            word_461C90;
extern short            word_46AF68;
extern short            word_463228;
extern int              dword_46ACF4;
extern short            word_464710;

// --- Key state ---
extern char             g_framesKeyLeftPressed;
extern char             g_framesKeyRightPressed;
extern char             g_framesKeyUpPressed;
extern char             g_framesKeyDownPressed;
extern char             g_isAKeyPressed;
extern char             g_isSpacePressed;
extern char             g_shiftPressed;

// --- Camera ---
extern float            g_cameraLocation[6];
extern CameraAngle     g_targetCameraAngle;
extern int              g_rotatingCameraIsActive;
extern CameraPosition   CAMERA_POSITIONS[11];

extern GLfloat g_fadeFactor;
extern int g_isFadingIn;

// ---------------------------------------------------------------------------
// FUNCTION: MIDIJAM 0x421610 (called from WinMain after HWF load)
// ---------------------------------------------------------------------------
WINDOW_CREATE_STATUS MidiJamMain(
    LPCSTR lpWindowName,
    HINSTANCE hInstance,
    FILE* hwfStream,
    void* pHwfAppendix,
    unsigned int nHwfAppendixItems)
{
    // --- Defaults ---
    g_useSound = 1;
    g_windowWidth = 1024;
    g_windowHeight = 768;
    g_bitDepth = 32;
    g_refreshRate = 75.0f;

    // --- Read HWF.cfg ---
    char configFileBuffer[1000];
    sprintf(configFileBuffer, "%s\\HWF.cfg", g_workingDirectory);
    FILE* configFileStream = fopen(configFileBuffer, "rb");

    if (!configFileStream)
    {
        // Config file missing — launch config.exe and wait
        char configExecutablePath[1000];
        SHELLEXECUTEINFOA configInfo = {};
        configInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
        sprintf(configExecutablePath, "%s\\config.exe", g_workingDirectory);
        configInfo.lpFile = configExecutablePath;
        configInfo.lpVerb = "open";
        configInfo.nShow = SW_SHOWNORMAL;
        configInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

        if (ShellExecuteExA(&configInfo))
        {
            if (WaitForSingleObject(configInfo.hProcess, INFINITE))
            {
                MessageBoxA(0, "Error waiting for program to start", "WAIT FOR CONFIG ERROR", MB_ICONASTERISK);
                return FAILURE;
            }
        }
        else
        {
            MessageBoxA(0, "Error executing config program", "WAIT FOR CONFIG ERROR", MB_ICONASTERISK);
        }

        configFileStream = fopen(configFileBuffer, "rb");
    }

    if (configFileStream)
    {
        char configUseSound = 0;
        char configFullscreen = 0;
        int configResolution = 0;
        int configBitDepth = 0;
        int configRefreshRate = 0;

        fread(&configUseSound, 1, 1, configFileStream);
        fread(&configFullscreen, 1, 1, configFileStream);
        fread(&configResolution, 4, 1, configFileStream);
        fread(&configBitDepth, 4, 1, configFileStream);
        fread(&configRefreshRate, 4, 1, configFileStream);
        fclose(configFileStream);

        g_useSound = (configUseSound == 1);
        g_fullscreen = (configFullscreen == 1);

        switch (configResolution)
        {
        case 0: g_windowWidth = 640;
            g_windowHeight = 480;
            break;
        case 1: g_windowWidth = 800;
            g_windowHeight = 600;
            break;
        case 2: g_windowWidth = 1024;
            g_windowHeight = 768;
            break;
        case 3: g_windowWidth = 1152;
            g_windowHeight = 864;
            break;
        case 4: g_windowWidth = 1280;
            g_windowHeight = 1024;
            break;
        case 5: g_windowWidth = 1600;
            g_windowHeight = 1200;
            break;
        }

        if (configBitDepth == 0)
            g_bitDepth = 16;
        else if (configBitDepth == 1)
            g_bitDepth = 32;

        switch (configRefreshRate)
        {
        case 0: g_refreshRate = 60.0f;
            break;
        case 1: g_refreshRate = 70.0f;
            break;
        case 2: g_refreshRate = 72.0f;
            break;
        case 3: g_refreshRate = 75.0f;
            break;
        case 4: g_refreshRate = 85.0f;
            break;
        case 5: g_refreshRate = 100.0f;
            break;
        case 6: g_refreshRate = 120.0f;
            break;
        case 7: g_refreshRate = 0.0f;
            break;
        }
    }

    g_windowCenter_X = g_windowWidth / 2;
    g_windowCenter_Y = g_windowHeight / 2;

    if (!CreateMidijamWindow(
        lpWindowName,
        hInstance,
        g_windowWidth,
        g_windowHeight,
        g_bitDepth,
        static_cast<DWORD>(g_refreshRate),
        g_fullscreen,
        hwfStream,
        static_cast<char*>(pHwfAppendix),
        nHwfAppendixItems))
        return FAILURE;

    glClear(0x4500u);
    glFlush();
    glClear(0x4500u);
    glFlush();

    g_windowWidth_0 = static_cast<__int16>(g_windowWidth);
    g_windowHeight_0 = static_cast<__int16>(g_windowHeight);

    g_hMidiEvent = CreateEventA(0, 0, 0, 0);

    // --- DirectMusic system ---
    DirectMusicSystem* directMusicSystem = new DirectMusicSystem();
    g_directMusicSystem = directMusicSystem;

    HRESULT hResult = directMusicSystem->Init(g_hWnd, 128, 8);
    g_DirectMusicPerformance = directMusicSystem->GetPerformance();
    IDirectMusicAudioPath* pDefaultAudioPath = directMusicSystem->GetDefaultAudioPath();

    hResult = pDefaultAudioPath->GetObjectInPath(
        0,
        DMUS_PATH_PERFORMANCE_GRAPH,
        0,
        NULL_GUID,
        0,
        IID_I_DIRECT_MUSIC_GRAPH8,
        reinterpret_cast<void**>(&g_DirectMusicGraph));
    if (hResult < 0)
        return FAILURE;

    // --- Notification setup (GUID_NOTIFICATION_SEGMENT) ---
    GUID guidNotificationSegment;
    guidNotificationSegment.Data1 = 0xD2AC2899;
    guidNotificationSegment.Data2 = 0x11D1;
    guidNotificationSegment.Data3 = 0xB39B;
    guidNotificationSegment.Data4[0] = 0x60;
    guidNotificationSegment.Data4[1] = 0x00;
    guidNotificationSegment.Data4[2] = 0x04;
    guidNotificationSegment.Data4[3] = 0x87;
    guidNotificationSegment.Data4[4] = 0xBD;
    guidNotificationSegment.Data4[5] = 0xB1;
    guidNotificationSegment.Data4[6] = 0x93;
    guidNotificationSegment.Data4[7] = 0x08;

    hResult = g_DirectMusicPerformance->AddNotificationType(guidNotificationSegment);
    if (hResult < 0)
        return FAILURE;

    hResult = g_DirectMusicPerformance->SetNotificationHandle(g_hMidiEvent, 0);
    if (hResult < 0)
        return FAILURE;

    // --- MidiJam tool ---
    MidiJamTool* midiJamTool = new MidiJamTool();
    g_midiJamTool = midiJamTool;
    hResult = (g_DirectMusicGraph->InsertTool)(midiJamTool, 0, 0, 0);

    if (hResult < 0)
        return FAILURE;

    return SUCCESS;
}


// FUNCTION: MIDIJAM 0x4202E0
void MidiJamInitialize()
{
    g_fadeFactor  = 1.0f;
    g_isFadingIn  = 1;
    g_targetCameraAngle      = CAMERA_1A;
    g_rotatingCameraIsActive = 0;
    for (short i = 0; i < 6; ++i)
        g_cameraLocation[i] = *(&CAMERA_POSITIONS[g_targetCameraAngle].cameraX + i);

    g_percussion_framesWithEmptyQueue = 0;

    // --- Harp string colors ---
    for (short j = 0; j < 47; ++j)
    {
        if (j % 7 == 0)
            g_harpStringColors[j] = Red;
        else if (j % 7 == 3)
            g_harpStringColors[j] = Blue;
        else
            g_harpStringColors[j] = White;
    }

    // --- Harp string geometry ---
    for (short i = 0; i < 47; ++i)
    {
        g_harpStringRestLength[i] = g_harpStringRestLength[i] - (i / 47.0f * 4.5f + 1.0f);
        g_harpStringY[i]   = i / 47.0f * 42.0f + 4.7379999f;
        g_harpStringZ[i]   = -i * 0.75f - 4.0f;
        g_harpStringScale[i] = ((1.0f - i / 47.0f) * 42.0f + g_harpStringRestLength[i] - 42.0f) / 72.0f;
    }

    // --- Cymbal physics init ---
    for (short m = 0; m < 7; ++m)
    {
        double cymbalScale        = CYMBAL_SCALE[m];
        g_cymbalMassFactor[m]     = static_cast<float>(cymbalScale + cymbalScale + 16.75);
        g_recoil_cymbals[m]       = 0.0f;
        g_cymbalCurrentWobble[m]  = 0.0f;
        g_cymbalAngularVelocity[m]= 0.0f;
        g_cymbalWobbleAmplitude[m]= 0.0f;
    }

    g_lastRideCymbal  = 1;
    g_show_percussion = 0;

    // --- Allocation counters ---
    g_pianoCount          = 0;
    g_bassCount           = 0;
    g_ialloc_guitar         = 0;
    dword_464700            = 0;
    dword_468EC0            = 0;
    g_harpCount           = 0;
    g_ialloc_xylophone      = 0;
    g_ialloc_stageChoir     = 0;
    g_stageHornCount      = 0;
    g_ialloc_whistles       = 0;
    g_ialloc_panPipe        = 0;
    g_ialloc_harmonica      = 0;
    g_ialloc_popBottle      = 0;
    g_ialloc_agogo          = 0;
    g_ialloc_woodblocks     = 0;
    g_stageStringCount    = 0;
    g_ialloc_pizzicatoStrings = 0;
    g_ialloc_musicBox       = 0;
    g_ialloc_melodicTom     = 0;
    g_ialloc_synthDrum      = 0;
    g_ialloc_steelDrum      = 0;
    g_ialloc_timpani        = 0;
    g_ialloc_taiko          = 0;
    g_ialloc_tubularBells   = 0;
    g_ialloc_telephone      = 0;
    g_ialloc_trombone       = 0;
    g_ialloc_tuba           = 0;
    g_ialloc_ocarina        = 0;
    g_ialloc_frenchHorn     = 0;
    g_ialloc_baritoneSax    = 0;
    g_ialloc_tenorSax       = 0;
    g_ialloc_sapranoSax     = 0;
    g_ialloc_altoSax        = 0;
    g_ialloc_recorder       = 0;
    g_ialloc_piccolo        = 0;
    g_ialloc_flute          = 0;
    g_ialloc_trumpet        = 0;

    // --- Recoil reset ---
    g_recoil_cowbell         = 0.0f;
    g_recoil_clap_hand_l     = 0.0f;
    g_recoil_clap_hand_r     = 0.0f;
    g_recoil_tambourine      = 0.0f;
    g_recoil_tambourine_hand = 0.0f;
    g_recoil_sticks_1        = 0.0f;
    g_recoil_clave_l         = 0.0f;
    g_pianoKeyOffsetX[0] = 0.0f;
    g_recoil_castanets       = 0.0f;
    g_recoil_jingleBell      = 0.0f;
    g_recoil_shaker          = 0.0f;
    g_recoil_highQ           = 0.0f;
    g_recoil_squareClick     = 0.0f;
    g_recoil_metronomeClick  = 0.0f;
    g_recoil_metronomeBell   = 0.0f;
    g_recoil_Maracas         = 0.0f;
    g_recoil_cabasa          = 0.0f;
    g_recoil_agogo_hi        = 0.0f;
    g_recoil_agogo_lo        = 0.0f;
    g_recoil_woodblock_hi    = 0.0f;
    g_recoil_woodblock_lo    = 0.0f;
    g_recoil_triangle_open   = 0.0f;
    g_recoil_triangle_muted  = 0.0f;
    g_recoil_whistle_long    = 0.0f;
    g_recoil_whistle_short   = 0.0f;
    g_recoil_timbale_hi      = 0.0f;
    g_recoil_timbale_lo      = 0.0f;
    g_recoil_bongo_hi        = 0.0f;
    g_recoil_bongo_lo        = 0.0f;
    g_recoil_conga_high_mute = 0.0f;
    g_recoil_conga_high_open = 0.0f;
    g_recoil_conga_low       = 0.0f;
    g_recoil_snaredrum       = 0.0f;
    g_recoil_snare           = 0.0f;
    g_recoil_sideStick       = 0.0f;
    for (short n = 0; n < 6; ++n)
        g_recoil_tom[n]      = 0.0f;
    g_recoil_bassdrum        = 0.0f;
    g_recoil_hihat           = 0.0f;
    g_hihat_isOpen           = 0;
    g_recoil_bassDrumArm     = 45.0f;
    g_songFillbarScale       = 0.0f;

    // --- Percussion queues ---
    for (short patch = 0; patch < 88; ++patch)
    {
        g_stick_visible[patch] = 0;
        for (short queue = 0; queue < 32; ++queue)
        {
            g_percussion_time_queue[patch][queue]     = 0;
            g_percussion_velocity_queue[patch][queue] = 0;
        }
    }

    // --- Per-channel assignment init ---
    for (short slot = 0; slot < 300; ++slot)
    {
        // g_violinChannel[slot]        = 0;
        // g_violaChannel[slot]         = 0;
        // g_celloChannel[slot]         = 0;
        // g_doubleBassChannel[slot]    = 0;
        g_doubleBass_playingStyle[slot]  = -1;
        g_midiJamInstrumentIds[slot]     = static_cast<MidiJamInstrumentId>(0);
        g_xylophone_types[slot]          = 0;
        g_pianoChannel[slot]         = -1;
        g_bassChannel[slot]          = -1;
        // g_guitarChannel[slot]        = -1;
        g_xylophoneChannel[slot]     = -1;
        // g_stateChoirChannel[slot]    = -1;
        g_stageHornChannel[slot]     = -1;
        // g_whistlesChannel[slot]      = -1;
        // g_panPipeChannel[slot]       = -1;
        g_isPanPipeCalliope[slot]        = -1;
        // g_harmonicaChannel[slot]     = -1;
        // g_popBottleChannel[slot]     = -1;
        // g_agogosChannel[slot]        = -1;
        // g_woodblocksChannel[slot]    = -1;
        g_stageStringChannel[slot]  = -1;
        // g_pizzicatoStringsChannel[slot] = -1;
        // g_musicBoxChannel[slot]      = -1;
        // g_melodicTomChannel[slot]    = -1;
        // g_synthDrumChannel[slot]     = -1;
        // g_steelDrumChannel[slot]     = -1;
        // g_timpaniChannel[slot]       = -1;
        // g_taikoChannel[slot]         = -1;
        // g_tubularBellsChannel[slot]  = -1;
        // g_telephoneChannel[slot]     = -1;
        // g_tromboneChannel[slot]      = -1;
        // g_tubaChannel[slot]          = -1;
        // g_ocarinaChannel[slot]       = -1;
        // g_frenchHornChannel[slot]    = -1;
        // g_baritoneSaxChannel[slot]   = -1;
        // g_tenorSaxChannel[slot]      = -1;
        // g_sapranoSaxChannel[slot]    = -1;
        // g_altoSaxChannel[slot]       = -1;
        // g_recorderChannel[slot]      = -1;
        // g_piccoloChannel[slot]       = -1;
        // g_fluteChannel[slot]         = -1;
        // g_trumpetChannel[slot]       = -1;
        g_harpChannel[slot]          = -1;
    }

    // --- Key state reset ---
    g_framesKeyLeftPressed  = 0;
    g_framesKeyRightPressed = 0;
    g_framesKeyUpPressed    = 0;
    g_framesKeyDownPressed  = 0;
    word_468974             = 0;
    word_4614AC             = 0;
    word_461C90             = 0;
    g_isAKeyPressed         = 0;
    g_isSpacePressed        = 0;
    word_46AF68             = 0;
    word_463228             = 0;
    g_shiftPressed          = 0;
    dword_46ACF4            = 0;
    word_464710             = 0;
}