#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>

#include "camera.h"
#include "fade.h"
#include "globals.h"
#include "hwf.h"
#include "midi.h"
#include "MidiJam.h"
#include "Ms3dBundle.h"
#include "instrument/Bass.h"
#include "instrument/Piano.h"

#define LOAD_MS3D_BUNDLE(varName, fileName) \
varName = new Ms3dBundle(); \
if (!varName->LoadFromHWF(fileName, g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems)) { \
MessageBoxA(0, "Couldn't load the model data\\model.ms3d", "Error", 0x10u); \
return 0; \
}

#define MS3D_APPLY_TEXTURES(varName) \
varName->ApplyTextures(g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems);

int main() {
    extern char ** __argv;
    int i;
    int j;

    g_killApplication_0 = 0;
    g_worldReady = 0;
    g_applicationStartTime = timeGetTime();

    for (i = 0; i < 6; ++i) {
        for (j = 0; j < 6; ++j) {
            g_latinSquare[6 * i + j] = (i + j) % 6;
        }
    }
    g_pianoXylophoneOffset = 0.0;

    for (i = 0; i < 7; ++i) {
        const float cymbalAzimuth = CYMBAL_LOC_X[i] / (CYMBAL_LOC_Z[i] - 24.0);
        g_cymbal_rot_y[i] = std::atan(cymbalAzimuth) * 57.29578;
        g_cymbalRestingAngle[i] = 20.0;
    }

    for (i = 0; i < 23; ++i) {
        BASS_NOTES[i][0] = i + 7;
        BASS_NOTES[i][1] = i + 12;
        BASS_NOTES[i][2] = i + 17;
        BASS_NOTES[i][3] = i + 22;
        flt_468BF4[i + 1] = BASS_FRET_HEIGHTS[i + 1] / -46.081001;
        word_46CEE0[6 * i] = i + 19;
        word_46CEE2[6 * i] = i + 24;
        word_46CEE4[6 * i] = i + 29;
        word_46CEE6[6 * i] = i + 34;
        word_46CEE8[6 * i] = i + 38;
        word_46CEEA[6 * i] = i + 43;
        flt_4654A0[i] = flt_45EAD0[i] / -34.700;
    }

    for (i = 0; i < 18; ++i) {
        word_46B2D0[4 * i] = i + 34;
        word_46B2D2[4 * i] = i + 41;
        word_46B2D4[4 * i] = i + 48;
        word_46B2D6[4 * i] = i + 55;
        word_4688C0[4 * i] = i + 27;
        word_4688C2[4 * i] = i + 34;
        word_4688C4[4 * i] = i + 41;
        word_4688C6[4 * i] = i + 48;
    }

    for (i = 0; i < 28; ++i) {
        word_46BBB0[4 * i] = i + 15;
        word_46BBB2[4 * i] = i + 22;
        word_46BBB4[4 * i] = i + 29;
        word_46BBB6[4 * i] = i + 36;
    }

    float v603 = 0.0;
    float v612 = 0.0525;
    for (i = 0; i < 49; ++i) {
        word_468258[4 * i] = i + 7;
        word_46825A[4 * i] = i + 12;
        word_46825C[4 * i] = i + 17;
        word_46825E[4 * i] = i + 22;
        flt_4679E0[i] = v603 * 0.8;
        v603 = v603 + v612;
        v612 = v612 * 0.95;
    }
    g_vibratingString_frame = VIBRATING_STRING_ANIM_SEQUENCE[0];
    g_vibratingString_frameIndex = 0;


    for (i = 0; i < 88; ++i) {
        g_pianoXylophoneOffset = g_pianoXylophoneOffset + KEY_WIDTHS[PIANO_KEY_SHAPE[i]];
    }
    g_pianoXylophoneOffset = g_pianoXylophoneOffset * 0.5;

    // WTF?
    g_pianokey_translation_x[1] = -0.20833333;
    g_pianokey_whiteBack_scale[1] = 0.58333331;
    g_pianokey_translation_x[2] = 0.0;
    g_pianokey_whiteBack_scale[2] = 0.5;
    g_pianokey_translation_x[3] = 0.20833333;
    g_pianokey_whiteBack_scale[3] = 0.58333331;
    g_pianokey_translation_x[4] = -0.25;
    g_pianokey_whiteBack_scale[4] = 0.5;
    g_pianokey_translation_x[5] = -0.083333336;
    g_pianokey_whiteBack_scale[5] = 0.5;
    g_pianokey_translation_x[6] = 0.083333336;
    g_pianokey_whiteBack_scale[6] = 0.5;
    g_pianokey_translation_x[7] = 0.25;
    g_pianokey_whiteBack_scale[7] = 0.5;
    g_pianokey_translation_x[8] = 0.0;
    g_pianokey_whiteBack_scale[8] = 1.0;
    g_pianokey_translation_x[9] = -0.125;
    g_pianokey_whiteBack_scale[9] = 1.0;
    g_pianokey_translation_x[10] = 0.125;
    g_pianokey_whiteBack_scale[10] = 1.0;
    g_pianokey_translation_x[11] = -0.25;
    g_pianokey_whiteBack_scale[11] = 1.0;
    g_pianokey_translation_x[12] = 0.0;
    g_pianokey_whiteBack_scale[12] = 1.0;
    g_pianokey_translation_x[13] = 0.25;
    g_pianokey_whiteBack_scale[13] = 1.0;

    g_windowCenterX = 0;
    g_windowCenterY = 0;
    g_framesSinceStart = 0;
    g_autoCamIdleTime = 0;
    g_fadeFactor = 1.0;
    g_isFadingIn = 1;
    g_isShuttingDown = 0;
    g_killApplication = 0;

    GetModuleFileNameA(nullptr, g_workingDirectory, 1000u);
    while (strlen(g_workingDirectory) > 0 && g_workingDirectory[strlen(g_workingDirectory) - 1] != '\\') {
        g_workingDirectory[strlen(g_workingDirectory) - 1] = '\0';
    }
    if (strlen(g_workingDirectory) > 0 && g_workingDirectory[strlen(g_workingDirectory) - 1] == '\\') {
        g_workingDirectory[strlen(g_workingDirectory) - 1] = '\0';
    }


    g_lastUnixEpochTime = GetUnixEpochTime(nullptr);

    char hwfPath[1000];
    sprintf(hwfPath, "%s\\MidiJam.HWF", g_workingDirectory);
    g_hwfStream = fopen(hwfPath, "rb");
    g_pHwfAppendix = ReadHwfAppendix(g_pHwfAppendix, g_hwfStream, &g_nHwfAppendixItems);

    // TODO: Maybe bad array access here?
    // memset(g_cloudTTL_whistle_long, 0, 8808u);

    const HINSTANCE hInstance = nullptr;
    if (MidiJamMain("MidiJam", hInstance, g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems) == 0x2B) {
        MessageBoxA(nullptr, "Video mode not supported, try running CONFIG.EXE first", "SHUTDOWN ERROR",
                    MB_ICONASTERISK);
        return 0;
    }

    gluLookAt(0.0, 50.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    constexpr GLfloat light_params_0_ambient[4] = {0.25, 0.25, 0.25, 1.0};
    constexpr GLfloat light_params_0_diffuse[4] = {1.0, 1.0, 1.0, 1.0};
    constexpr GLfloat light_params_0_position[4] = {15.0, 75.0, 5.0, 1.0};
    constexpr GLfloat light_params_1_ambient[4] = {0.025, 0.025, 0.25, 1.0};
    constexpr GLfloat light_params_1_diffuse[4] = {0.5, 0.5, 1.0, 1.0};
    constexpr GLfloat light_params_1_position[4] = {-25.0, 5.0, 25.0, 1.0};
    constexpr GLfloat light_params_2_ambient[4] = {0.25, 0.25, 0.25, 1.0};
    constexpr GLfloat light_params_2_diffuse[4] = {1.0, 1.0, 1.0, 1.0};
    constexpr GLfloat light_params_2_position[4] = {-15.0, 45.0, 35.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_params_0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_params_0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_params_0_position);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_params_1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_params_1_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light_params_1_position);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light_params_2_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_params_2_diffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, light_params_2_position);
    glEnable(GL_LIGHT2);
    g_normalizedWindowScale = g_windowWidth / 1024.0;
    g_windowCenterX = g_windowCenter_X;
    g_windowCenterY = g_windowCenter_Y;

    LOAD_MS3D_BUNDLE(g_drumSet_Stick_ms3d, "DrumSet_Stick.ms3d");
    LOAD_MS3D_BUNDLE(g_screenGradient_ms3d, "ScreenGradient.ms3d");
    LOAD_MS3D_BUNDLE(g_stage_ms3d, "Stage.ms3d");
    LOAD_MS3D_BUNDLE(g_songFillbar_ms3d, "SongFillbar.ms3d");
    LOAD_MS3D_BUNDLE(g_songFillbarBox_ms3d, "SongFillbarBox.ms3d");
    LOAD_MS3D_BUNDLE(g_pianoShadow_ms3d, "PianoShadow.ms3d");
    LOAD_MS3D_BUNDLE(g_xylophoneShadow_ms3d, "XylophoneShadow.ms3d");
    LOAD_MS3D_BUNDLE(g_pianoStand_ms3d, "PianoStand.ms3d");

    // Bass
    LOAD_MS3D_BUNDLE(g_bass_ms3d, "Bass.ms3d");
    LOAD_MS3D_BUNDLE(g_bassString_ms3d, "BassString.ms3d");
    g_bassStringBottomX_ms3d = new Ms3dBundle *[5];
    LOAD_MS3D_BUNDLE(g_bassStringBottomX_ms3d[0], "BassStringBottom0.ms3d");
    LOAD_MS3D_BUNDLE(g_bassStringBottomX_ms3d[1], "BassStringBottom1.ms3d");
    LOAD_MS3D_BUNDLE(g_bassStringBottomX_ms3d[2], "BassStringBottom2.ms3d");
    LOAD_MS3D_BUNDLE(g_bassStringBottomX_ms3d[3], "BassStringBottom3.ms3d");
    LOAD_MS3D_BUNDLE(g_bassStringBottomX_ms3d[4], "BassStringBottom4.ms3d");
    LOAD_MS3D_BUNDLE(g_bassNoteFinger_ms3d, "BassNoteFinger.ms3d");

    // Stage Horn
    LOAD_MS3D_BUNDLE(g_stageHorn_ms3d, "StageHorn.ms3d");

    // Stage String
    LOAD_MS3D_BUNDLE(g_stageString_ms3d, "StageString.ms3d");
    LOAD_MS3D_BUNDLE(g_stageStringHolder_ms3d, "StageStringHolder.ms3d");
    LOAD_MS3D_BUNDLE(g_stageStringBow_ms3d, "StageStringBow.ms3d");
    LOAD_MS3D_BUNDLE(g_stageStringBottomX_ms3d[0], "StageStringBottom0.ms3d");
    LOAD_MS3D_BUNDLE(g_stageStringBottomX_ms3d[1], "StageStringBottom1.ms3d");
    LOAD_MS3D_BUNDLE(g_stageStringBottomX_ms3d[2], "StageStringBottom2.ms3d");
    LOAD_MS3D_BUNDLE(g_stageStringBottomX_ms3d[3], "StageStringBottom3.ms3d");
    LOAD_MS3D_BUNDLE(g_stageStringBottomX_ms3d[4], "StageStringBottom4.ms3d");

    // Accordion
    LOAD_MS3D_BUNDLE(g_accordionKeyWhiteBack_ms3d, "AccordianKeyWhiteBack.ms3d");
    LOAD_MS3D_BUNDLE(g_accordionKeyWhiteFront_ms3d, "AccordianKeyWhiteFront.ms3d");
    LOAD_MS3D_BUNDLE(g_accordionKeyWhiteBack_Down_ms3d, "AccordianKeyWhiteBack.ms3d");
    g_accordionKeyWhiteBack_Down_ms3d->ReplaceTexture("AccordianKey.bmp", "AccordianKeyDown.bmp");
    LOAD_MS3D_BUNDLE(g_accordionKeyWhiteFront_Down_ms3d, "AccordianKeyWhiteFront.ms3d");
    g_accordionKeyWhiteFront_Down_ms3d->ReplaceTexture("AccordianKey.bmp", "AccordianKeyDown.bmp");
    LOAD_MS3D_BUNDLE(g_accordionKeyBlack_ms3d, "AccordianKeyBlack.ms3d");
    LOAD_MS3D_BUNDLE(g_accordionKeyBlack_Down_ms3d, "AccordianKeyBlack.ms3d");
    g_accordionKeyBlack_Down_ms3d->ReplaceTexture("AccordianKeyBlack.bmp", "AccordianKeyBlackDown.bmp");
    LOAD_MS3D_BUNDLE(g_accordionFold_ms3d, "AccordianFold.ms3d");
    LOAD_MS3D_BUNDLE(g_accordionLeftHand_ms3d, "AccordianLeftHand.ms3d");
    LOAD_MS3D_BUNDLE(g_accordionRightHand_ms3d, "AccordianRightHand.ms3d");

    // Xylophone
    LOAD_MS3D_BUNDLE(g_xylophoneMalletWhite_ms3d, "XylophoneMalletWhite.ms3d");
    LOAD_MS3D_BUNDLE(g_malletHitShadow_ms3d, "MalletHitShadow.ms3d");
    LOAD_MS3D_BUNDLE(g_xylophoneLegs_ms3d, "XylophoneLegs.ms3d");
    LOAD_MS3D_BUNDLE(g_xylophoneCase_ms3d, "XylophoneCase.ms3d");
    g_xylophoneModels_ms3d_arr = new XylophoneModels *[4];
    for (i = 0; i < 4; ++i) {
        g_xylophoneModels_ms3d_arr[i] = new XylophoneModels();
        LOAD_MS3D_BUNDLE(g_xylophoneModels_ms3d_arr[i]->xylophoneWhiteBar, "XylophoneWhiteBar.ms3d");
        LOAD_MS3D_BUNDLE(g_xylophoneModels_ms3d_arr[i]->xylophoneWhiteBarDown, "XylophoneWhiteBarDown.ms3d");
        LOAD_MS3D_BUNDLE(g_xylophoneModels_ms3d_arr[i]->xylophoneBlackBar, "XylophoneBlackBar.ms3d");
        LOAD_MS3D_BUNDLE(g_xylophoneModels_ms3d_arr[i]->xylophoneBlackBarDown, "XylophoneBlackBarDown.ms3d");
    }
    g_xylophoneModels_ms3d_arr[1]->xylophoneWhiteBar->ReplaceTexture("XylophoneBar.bmp", "GlockenspielBar.bmp");
    g_xylophoneModels_ms3d_arr[1]->xylophoneWhiteBarDown->ReplaceTexture("XylophoneBar.bmp", "GlockenspielBar.bmp");
    g_xylophoneModels_ms3d_arr[1]->xylophoneBlackBar->ReplaceTexture("XylophoneBar.bmp", "GlockenspielBar.bmp");
    g_xylophoneModels_ms3d_arr[1]->xylophoneBlackBarDown->ReplaceTexture("XylophoneBar.bmp", "GlockenspielBar.bmp");
    g_xylophoneModels_ms3d_arr[2]->xylophoneWhiteBar->ReplaceTexture("XylophoneBar.bmp", "VibesBar.bmp");
    g_xylophoneModels_ms3d_arr[2]->xylophoneWhiteBarDown->ReplaceTexture("XylophoneBar.bmp", "VibesBar.bmp");
    g_xylophoneModels_ms3d_arr[2]->xylophoneBlackBar->ReplaceTexture("XylophoneBar.bmp", "VibesBar.bmp");
    g_xylophoneModels_ms3d_arr[2]->xylophoneBlackBarDown->ReplaceTexture("XylophoneBar.bmp", "VibesBar.bmp");
    g_xylophoneModels_ms3d_arr[3]->xylophoneWhiteBar->ReplaceTexture("XylophoneBar.bmp", "MarimbaBar.bmp");
    g_xylophoneModels_ms3d_arr[3]->xylophoneWhiteBarDown->ReplaceTexture("XylophoneBar.bmp", "MarimbaBar.bmp");
    g_xylophoneModels_ms3d_arr[3]->xylophoneBlackBar->ReplaceTexture("XylophoneBar.bmp", "MarimbaBar.bmp");
    g_xylophoneModels_ms3d_arr[3]->xylophoneBlackBarDown->ReplaceTexture("XylophoneBar.bmp", "MarimbaBar.bmp");


    // Piano
    g_pianoModels_ms3d_arr = new PianoModels *[4];
    for (i = 0; i < 4; ++i) {
        g_pianoModels_ms3d_arr[i] = new PianoModels();
        LOAD_MS3D_BUNDLE(g_pianoModels_ms3d_arr[i]->pianoCase, "PianoCase.ms3d");
        LOAD_MS3D_BUNDLE(g_pianoModels_ms3d_arr[i]->pianoKeyBlack, "PianoKeyBlack.ms3d");
        LOAD_MS3D_BUNDLE(g_pianoModels_ms3d_arr[i]->pianoKeyBlackDown, "PianoKeyBlackDown.ms3d");
        LOAD_MS3D_BUNDLE(g_pianoModels_ms3d_arr[i]->pianoKeyWhiteFront, "PianoKeyWhiteFront.ms3d");
        LOAD_MS3D_BUNDLE(g_pianoModels_ms3d_arr[i]->pianoKeyWhiteBack, "PianoKeyWhiteBack.ms3d");
        LOAD_MS3D_BUNDLE(g_pianoModels_ms3d_arr[i]->pianoKeyWhiteFrontDown, "PianoKeyWhiteFrontDown.ms3d");
        LOAD_MS3D_BUNDLE(g_pianoModels_ms3d_arr[i]->pianoKeyWhiteBackDown, "PianoKeyWhiteBackDown.ms3d");
    }
    g_pianoModels_ms3d_arr[1]->pianoCase->ReplaceTexture("PianoSkin.bmp", "PianoSkin_Wood.bmp");
    g_pianoModels_ms3d_arr[1]->pianoKeyBlack->ReplaceTexture("PianoSkin.bmp", "PianoSkin_Wood.bmp");
    g_pianoModels_ms3d_arr[1]->pianoKeyBlackDown->ReplaceTexture("PianoSkin.bmp", "PianoSkin_Wood.bmp");
    g_pianoModels_ms3d_arr[1]->pianoKeyWhiteFront->ReplaceTexture("PianoSkin.bmp", "PianoSkin_Wood.bmp");
    g_pianoModels_ms3d_arr[1]->pianoKeyWhiteBack->ReplaceTexture("PianoSkin.bmp", "PianoSkin_Wood.bmp");
    g_pianoModels_ms3d_arr[1]->pianoKeyWhiteFrontDown->ReplaceTexture("PianoSkin.bmp", "PianoSkin_Wood.bmp");
    g_pianoModels_ms3d_arr[1]->pianoKeyWhiteBackDown->ReplaceTexture("PianoSkin.bmp", "PianoSkin_Wood.bmp");
    g_pianoModels_ms3d_arr[2]->pianoCase->ReplaceTexture("PianoSkin.bmp", "SynthSkin.bmp");
    g_pianoModels_ms3d_arr[2]->pianoKeyBlack->ReplaceTexture("PianoSkin.bmp", "SynthSkin.bmp");
    g_pianoModels_ms3d_arr[2]->pianoKeyBlackDown->ReplaceTexture("PianoSkin.bmp", "SynthSkin.bmp");
    g_pianoModels_ms3d_arr[2]->pianoKeyWhiteFront->ReplaceTexture("PianoSkin.bmp", "SynthSkin.bmp");
    g_pianoModels_ms3d_arr[2]->pianoKeyWhiteBack->ReplaceTexture("PianoSkin.bmp", "SynthSkin.bmp");
    g_pianoModels_ms3d_arr[2]->pianoKeyWhiteFrontDown->ReplaceTexture("PianoSkin.bmp", "SynthSkin.bmp");
    g_pianoModels_ms3d_arr[2]->pianoKeyWhiteBackDown->ReplaceTexture("PianoSkin.bmp", "SynthSkin.bmp");
    g_pianoModels_ms3d_arr[3]->pianoCase->ReplaceTexture("PianoSkin.bmp", "HarpsichordSkin.bmp");
    g_pianoModels_ms3d_arr[3]->pianoKeyBlack->ReplaceTexture("PianoSkin.bmp", "HarpsichordSkin.bmp");
    g_pianoModels_ms3d_arr[3]->pianoKeyBlackDown->ReplaceTexture("PianoSkin.bmp", "HarpsichordSkin.bmp");
    g_pianoModels_ms3d_arr[3]->pianoKeyWhiteFront->ReplaceTexture("PianoSkin.bmp", "HarpsichordSkin.bmp");
    g_pianoModels_ms3d_arr[3]->pianoKeyWhiteBack->ReplaceTexture("PianoSkin.bmp", "HarpsichordSkin.bmp");
    g_pianoModels_ms3d_arr[3]->pianoKeyWhiteFrontDown->ReplaceTexture("PianoSkin.bmp", "HarpsichordSkin.bmp");
    g_pianoModels_ms3d_arr[3]->pianoKeyWhiteBackDown->ReplaceTexture("PianoSkin.bmp", "HarpsichordSkin.bmp");

    // TODO UnixEpochTime
    MidiJamInitialize();

    MS3D_APPLY_TEXTURES(g_drumSet_Stick_ms3d);
    MS3D_APPLY_TEXTURES(g_screenGradient_ms3d);
    MS3D_APPLY_TEXTURES(g_stage_ms3d);
    MS3D_APPLY_TEXTURES(g_songFillbar_ms3d);
    MS3D_APPLY_TEXTURES(g_songFillbarBox_ms3d);
    MS3D_APPLY_TEXTURES(g_pianoShadow_ms3d);
    MS3D_APPLY_TEXTURES(g_xylophoneShadow_ms3d);
    MS3D_APPLY_TEXTURES(g_pianoStand_ms3d);

    MS3D_APPLY_TEXTURES(g_bass_ms3d);
    MS3D_APPLY_TEXTURES(g_bassString_ms3d);
    MS3D_APPLY_TEXTURES(g_bassStringBottomX_ms3d[0]);
    MS3D_APPLY_TEXTURES(g_bassStringBottomX_ms3d[1]);
    MS3D_APPLY_TEXTURES(g_bassStringBottomX_ms3d[2]);
    MS3D_APPLY_TEXTURES(g_bassStringBottomX_ms3d[3]);
    MS3D_APPLY_TEXTURES(g_bassStringBottomX_ms3d[4]);
    MS3D_APPLY_TEXTURES(g_bassNoteFinger_ms3d);

    // Stage Horn
    MS3D_APPLY_TEXTURES(g_stageHorn_ms3d);

    // Stage String
    MS3D_APPLY_TEXTURES(g_stageString_ms3d);
    MS3D_APPLY_TEXTURES(g_stageStringHolder_ms3d);
    MS3D_APPLY_TEXTURES(g_stageStringBow_ms3d);
    MS3D_APPLY_TEXTURES(g_stageStringBottomX_ms3d[0]);
    MS3D_APPLY_TEXTURES(g_stageStringBottomX_ms3d[1]);
    MS3D_APPLY_TEXTURES(g_stageStringBottomX_ms3d[2]);
    MS3D_APPLY_TEXTURES(g_stageStringBottomX_ms3d[3]);
    MS3D_APPLY_TEXTURES(g_stageStringBottomX_ms3d[4]);

    MS3D_APPLY_TEXTURES(g_accordionKeyWhiteBack_ms3d);
    MS3D_APPLY_TEXTURES(g_accordionKeyWhiteFront_ms3d);
    MS3D_APPLY_TEXTURES(g_accordionKeyWhiteBack_Down_ms3d);
    MS3D_APPLY_TEXTURES(g_accordionKeyWhiteFront_Down_ms3d);
    MS3D_APPLY_TEXTURES(g_accordionKeyBlack_ms3d);
    MS3D_APPLY_TEXTURES(g_accordionKeyBlack_Down_ms3d);
    MS3D_APPLY_TEXTURES(g_accordionFold_ms3d);
    MS3D_APPLY_TEXTURES(g_accordionLeftHand_ms3d);
    MS3D_APPLY_TEXTURES(g_accordionRightHand_ms3d);

    // Xylophone
    MS3D_APPLY_TEXTURES(g_xylophoneMalletWhite_ms3d);
    MS3D_APPLY_TEXTURES(g_malletHitShadow_ms3d);
    MS3D_APPLY_TEXTURES(g_xylophoneLegs_ms3d);
    MS3D_APPLY_TEXTURES(g_xylophoneCase_ms3d);
    for (i = 0; i < 4; ++i) {
        MS3D_APPLY_TEXTURES(g_xylophoneModels_ms3d_arr[i]->xylophoneWhiteBar);
        MS3D_APPLY_TEXTURES(g_xylophoneModels_ms3d_arr[i]->xylophoneWhiteBarDown);
        MS3D_APPLY_TEXTURES(g_xylophoneModels_ms3d_arr[i]->xylophoneBlackBar);
        MS3D_APPLY_TEXTURES(g_xylophoneModels_ms3d_arr[i]->xylophoneBlackBarDown);
    }

    for (i = 0; i < 4; ++i) {
        MS3D_APPLY_TEXTURES(g_pianoModels_ms3d_arr[i]->pianoCase);
        MS3D_APPLY_TEXTURES(g_pianoModels_ms3d_arr[i]->pianoKeyBlack);
        MS3D_APPLY_TEXTURES(g_pianoModels_ms3d_arr[i]->pianoKeyBlackDown);
        MS3D_APPLY_TEXTURES(g_pianoModels_ms3d_arr[i]->pianoKeyWhiteFront);
        MS3D_APPLY_TEXTURES(g_pianoModels_ms3d_arr[i]->pianoKeyWhiteBack);
        MS3D_APPLY_TEXTURES(g_pianoModels_ms3d_arr[i]->pianoKeyWhiteFrontDown);
        MS3D_APPLY_TEXTURES(g_pianoModels_ms3d_arr[i]->pianoKeyWhiteBackDown);
    }


    int v613 = 0;
    g_timerEventId = timeSetEvent(5u, 0, UpdateMidiJamMM, 0, TIME_PERIODIC);
    RECOIL_SCALE_FACTOR = 5;

    auto lpCmdLine = __argv[1];
    if (lpCmdLine && strlen(lpCmdLine) > 4) {
        int m = 0, n = 0;
        size_t len = strlen(lpCmdLine);

        // Copy MIDI file name up to but not including ".MID" or ".RMI"
        for (m = 0;
             m < (int) (len - 4) &&
             !(lpCmdLine[m] == '.' &&
               ((lpCmdLine[m + 1] == 'M' && lpCmdLine[m + 2] == 'I' && lpCmdLine[m + 3] == 'D') ||
                (lpCmdLine[m + 1] == 'R' && lpCmdLine[m + 2] == 'M' && lpCmdLine[m + 3] == 'I')));
             m++) {
            g_midiFileName[m] = lpCmdLine[m];
        }

        // Copy extension
        g_midiFileName[m] = lpCmdLine[m];
        g_midiFileName[m + 1] = lpCmdLine[m + 1];
        g_midiFileName[m + 2] = lpCmdLine[m + 2];
        g_midiFileName[m + 3] = lpCmdLine[m + 3];
        g_midiFileName[m + 4] = '\0';

        // Remove quotes if present
        if (g_midiFileName[0] == '"') {
            for (n = 0; g_midiFileName[n]; n++)
                g_midiFileName[n] = g_midiFileName[n + 1]; // shift left by 1

            if (g_midiFileName[n - 1] == '"')
                g_midiFileName[n - 1] = '\0';
        }
    }

    if (!lpCmdLine || strlen(lpCmdLine) <= 4) {
        // no midi file passed, load default song
        if (LoadAndPlayMidiFile(const_cast<char *>("rocky_1.mid")) == 1)
            g_killApplication_0 = 1;
    } else if (LoadAndPlayMidiFile(reinterpret_cast<char *>(&g_midiFileName)) == 1) {
        g_killApplication_0 = 1;
    }

    g_worldReady = 1;

    tagMSG Msg;
    while (!g_killApplication_0) {
        if (PeekMessageA(&Msg, 0, 0, 0, 1u)) {
            if (Msg.message == WM_QUIT) {
                g_killApplication_0 = 1;
            } else {
                TranslateMessage(&Msg);
                DispatchMessageA(&Msg);
            }
        } else if (!g_isWindowActive || UpdateMidiJam()) {
            // SwapBuffers_0();
            SwapBuffers(g_hdc);
            ++v613;
            // run once per second
            if (GetUnixEpochTime(0) > g_lastUnixEpochTime) {
                g_framesPerSecond = v613;
                v613 = 0;
                g_lastUnixEpochTime = GetUnixEpochTime(0);
            }
        } else {
            g_killApplication_0 = 1;
        }
    }

    /* TODO */
}
