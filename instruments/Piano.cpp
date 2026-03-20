//
// Created by Jacob on 3/17/2026.
//

#include "Piano.h"

#include "model/Ms3dBundle.h"
#include "render/texture.h"

#include <GL/gl.h>


// ---------------------------------------------------------------------------
// Key shape lookup table.
// Maps each of the 88 piano keys to a shape index (1-13).
// Shapes 1-7: white keys with back extension scaling.
// Shape 8: white key (last octave, no back scaling).
// Shapes 9-13: black keys.
// TODO: IDA reports 86 entries but the loop iterates 88 — verify array bounds.
// ---------------------------------------------------------------------------
static const short PIANO_KEY_SHAPE[88] = {
    1, 12, 3, 1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5, 12, 6, 13, 7,
    1, 9, 2, 10, 3, 4, 11, 5,
    12, 6, 13
};

// Per-key horizontal advance widths indexed by shape.
// TODO: KEY_WIDTHS[0] = 0x80007 — IDA value looks suspect, verify at runtime.
static const GLfloat KEY_WIDTHS[14] = {
    0.0f, // 0 — suspect value, original is 0x80007h
    0.5f, // 1
    0.5f, // 2
    1.0f, // 3
    0.5f, // 4
    0.5f, // 5
    0.5f, // 6
    1.0f, // 7
    1.0f, // 8
    0.5f, // 9
    0.5f, // 10
    0.5f, // 11
    0.5f, // 12
    0.5f, // 13
};

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------

extern I_DS_Piano* g_ds_piano;
extern short g_ialloc_piano;
extern PianoModels g_pianoModels_ms3d[4];
extern Ms3dBundle* g_pianoStand_ms3d;
extern GLfloat g_pianoXylophoneOffset;
extern GLfloat g_pianokey_translation_x[14];
extern GLfloat g_xylophoneLegs_offsets[14];
extern MUSIC_TIME g_time_global_current;
extern short g_inst_visible_piano;

// FUNCTION: MIDIJAM 0x411CD0
void I_Piano()
{
    short visibleCount = 0;

    for (short i = 0; i < g_ialloc_piano; ++i)
    {
        if (g_ds_piano[i].isActive != 1)
            continue;

        GLboolean wasTexturingEnabled =
            g_pianoModels_ms3d[g_ds_piano[i].materialIndex].pianoCase->ApplyMaterial();

        glPushMatrix();

        GLfloat z = visibleCount * -5.75f;
        GLfloat y = visibleCount * 3.0f;
        glTranslatef(0.0f, y, z);

        g_pianoModels_ms3d[g_ds_piano[i].materialIndex].pianoCase->Render();

        glTranslatef(-g_pianoXylophoneOffset, 0.0f, 0.0f);

        for (short key = 0; key < 88; ++key)
        {
            glPushMatrix();
            glRotatef(g_ds_piano[i].keyAngles[key], 1.0f, 0.0f, 0.0f);

            short shape = PIANO_KEY_SHAPE[key];

            if (shape >= 1 && shape <= 7)
            {
                // White key with back extension.
                if (g_ds_piano[i].keyAngles[key] <= 0.0f)
                    g_pianoModels_ms3d[g_ds_piano[i].materialIndex].pianoKeyWhiteFront->Render();
                else
                    g_pianoModels_ms3d[g_ds_piano[i].materialIndex].pianoKeyWhiteFrontDown->Render();

                glTranslatef(g_pianokey_translation_x[shape], 0.0f, 0.0f);
                glScalef(g_xylophoneLegs_offsets[shape], 1.0f, 1.0f);

                if (g_ds_piano[i].keyAngles[key] <= 0.0f)
                    g_pianoModels_ms3d[g_ds_piano[i].materialIndex].pianoKeyWhiteBack->Render();
                else
                    g_pianoModels_ms3d[g_ds_piano[i].materialIndex].pianoKeyWhiteBackDown->Render();
            }
            else if (shape == 8)
            {
                // White key without back scaling (last octave).
                if (g_ds_piano[i].keyAngles[key] <= 0.0f)
                {
                    g_pianoModels_ms3d[g_ds_piano[i].materialIndex].pianoKeyWhiteFront->Render();
                    g_pianoModels_ms3d[g_ds_piano[i].materialIndex].pianoKeyWhiteBack->Render();
                }
                else
                {
                    g_pianoModels_ms3d[g_ds_piano[i].materialIndex].pianoKeyWhiteFrontDown->Render();
                    g_pianoModels_ms3d[g_ds_piano[i].materialIndex].pianoKeyWhiteBackDown->Render();
                }
            }
            else if (shape >= 9 && shape <= 13)
            {
                // Black key.
                glTranslatef(g_pianokey_translation_x[shape], 0.0f, 0.0f);

                if (g_ds_piano[i].keyAngles[key] <= 0.0f)
                    g_pianoModels_ms3d[g_ds_piano[i].materialIndex].pianoKeyBlack->Render();
                else
                    g_pianoModels_ms3d[g_ds_piano[i].materialIndex].pianoKeyBlackDown->Render();
            }

            glPopMatrix();
            glTranslatef(KEY_WIDTHS[shape], 0.0f, 0.0f);
        }

        glPopMatrix();
        ++visibleCount;

        // Restore texture state to what it was before ApplyMaterial.
        SetTextureState(wasTexturingEnabled);
    }

    g_inst_visible_piano = 0;
    if (visibleCount > 0)
    {
        g_inst_visible_piano = visibleCount;
        g_pianoStand_ms3d->RenderModel();
    }
}

// FUNCTION: MIDIJAM 0x424780
bool I_Piano_MM(MUSIC_TIME pmtNow)
{
    short bAnyPianoActive = 0;

    for (short piano = 0; piano < g_ialloc_piano; ++piano)
    {
        short bAnyKeyActive = 0;

        for (short key = 0; key < 88; ++key)
        {
            // Process queued note-on events.
            for (short slot = 0; slot < 16; ++slot)
            {
                if (g_ds_piano[piano].timeDeltas[key][slot] > 0)
                {
                    bAnyPianoActive = 1;
                    g_ds_piano[piano].timeDeltas[key][slot] -=
                        static_cast<int16_t>(pmtNow - g_time_global_current);

                    if (g_ds_piano[piano].timeDeltas[key][slot] <= 0)
                    {
                        g_ds_piano[piano].timeDeltas[key][slot] = 0;
                        g_ds_piano[piano].keyAngles[key] =
                            g_ds_piano[piano].velocities[key][slot] * 8.0f / 128.0f + 1.75f;
                        g_ds_piano[piano].durations[key] =
                            g_ds_piano[piano].queueDurations[key][slot];
                        g_ds_piano[piano].queueDurations[key][slot] = 0;
                    }

                    g_ds_piano[piano].isActive = 1;
                    g_ds_piano[piano].timeToLive = 1000;
                    bAnyKeyActive = 1;
                }
            }

            // Process active note duration.
            if (g_ds_piano[piano].durations[key] > 0)
            {
                g_ds_piano[piano].durations[key] -=
                    static_cast<int>(pmtNow - g_time_global_current);

                if (g_ds_piano[piano].durations[key] <= 0)
                    g_ds_piano[piano].durations[key] = 0;

                g_ds_piano[piano].isActive = 1;
                g_ds_piano[piano].timeToLive = 1000;
                bAnyKeyActive = 1;
            }

            // Return key to rest position after note ends.
            if (g_ds_piano[piano].keyAngles[key] > 0.0f
                && !g_ds_piano[piano].durations[key])
            {
                bAnyPianoActive = 1;
                g_ds_piano[piano].keyAngles[key] -= 0.5f;

                if (g_ds_piano[piano].keyAngles[key] > 0.0f)
                {
                    g_ds_piano[piano].isActive = 1;
                    g_ds_piano[piano].timeToLive = 1000;
                    bAnyKeyActive = 1;
                }
                else
                {
                    g_ds_piano[piano].keyAngles[key] = 0.0f;
                }
            }
        }

        // Deactivate piano if no keys are active and timeToLive expires.
        if (!bAnyKeyActive
            && g_ds_piano[piano].isActive == 1
            && --g_ds_piano[piano].timeToLive <= 0)
        {
            g_ds_piano[piano].timeToLive = 0;
            g_ds_piano[piano].isActive = 0;
        }
    }

    return bAnyPianoActive == 1;
}
