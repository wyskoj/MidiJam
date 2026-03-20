//
// Created by Jacob on 3/17/2026.
//

#include "Piano.h"

#include "keys.h"
#include "model/Ms3dBundle.h"
#include "render/texture.h"
#include "scene/scene.h"

DEF_INST(piano, PianoState)

PianoModels g_pianoModels[4];
Ms3dBundle* g_pianoStand_ms3d;
GLfloat g_pianoKeyOffsetX[14];
GLfloat g_pianoKeyBackScale[14];

// FUNCTION: MIDIJAM 0x411CD0
void RenderPiano()
{
    short nVisible = 0;
    for (short i = 0; i < g_pianoCount; ++i)
    {
        const PianoState& piano = g_piano[i];
        if (piano.isActive != 1)
            continue;

        const PianoModels& models = g_pianoModels[piano.materialIndex];
        const GLboolean prevTextureState = models.pianoCase->ApplyMaterial();

        glPushMatrix();

        // --- Phase 1: Position and render case ---
        glTranslatef(0.0f, nVisible * 3.0f, nVisible * -5.75f);
        models.pianoCase->Render();

        // --- Phase 2: Render keys ---
        glTranslatef(-g_keysOffset, 0.0f, 0.0f);
        for (short key = 0; key < 88; ++key)
        {
            const short shape = KEY_SHAPE_INDEX[key];
            const bool pressed = piano.keyAngles[key] > 0.0f;

            glPushMatrix();
            glRotatef(piano.keyAngles[key], 1.0f, 0.0f, 0.0f);

            if (shape >= 1 && shape <= 7)
            {
                // White key (split back/front)
                pressed
                    ? models.pianoKeyWhiteFrontDown->Render()
                    : models.pianoKeyWhiteFront->Render();
                glTranslatef(g_pianoKeyOffsetX[shape], 0.0f, 0.0f);
                glScalef(g_pianoKeyBackScale[shape], 1.0f, 1.0f);
                pressed
                    ? models.pianoKeyWhiteBackDown->Render()
                    : models.pianoKeyWhiteBack->Render();
            }
            else if (shape == 8)
            {
                // Full-width white key (last key only)
                pressed
                    ? models.pianoKeyWhiteFrontDown->Render()
                    : models.pianoKeyWhiteFront->Render();
                pressed
                    ? models.pianoKeyWhiteBackDown->Render()
                    : models.pianoKeyWhiteBack->Render();
            }
            else if (shape >= 9 && shape <= 13)
            {
                // Black key
                glTranslatef(g_pianoKeyOffsetX[shape], 0.0f, 0.0f);
                pressed
                    ? models.pianoKeyBlackDown->Render()
                    : models.pianoKeyBlack->Render();
            }

            glPopMatrix();
            glTranslatef(KEY_WIDTHS[shape], 0.0f, 0.0f);
        }

        glPopMatrix();
        SetTextureState(prevTextureState);
        ++nVisible;
    }

    g_pianoVisible = nVisible > 0 ? nVisible : 0;
    if (nVisible > 0)
        g_pianoStand_ms3d->RenderModel();
}

// FUNCTION: MIDIJAM 0x424780
bool UpdatePiano(const MUSIC_TIME pmtNow)
{
    const MUSIC_TIME elapsed = pmtNow - g_currentGlobalTime;
    bool anyPianoActive = false;

    for (short p = 0; p < g_pianoCount; ++p)
    {
        PianoState& piano = g_piano[p];
        bool anyKeyActive = false;

        for (short key = 0; key < 88; ++key)
        {
            // --- Phase 1: Advance queued notes ---
            for (short slot = 0; slot < 16; ++slot)
            {
                if (piano.timeDeltas[key][slot] <= 0)
                    continue;

                anyPianoActive = true;
                piano.timeDeltas[key][slot] -= elapsed;

                if (piano.timeDeltas[key][slot] <= 0)
                {
                    // Note is ready to trigger — dequeue and apply
                    piano.timeDeltas[key][slot] = 0;
                    piano.keyAngles[key] = piano.velocities[key][slot] * 8.0f / 128.0f + 1.75f;
                    piano.durations[key] = piano.queue[key][slot];
                    piano.queue[key][slot] = 0;
                }

                piano.isActive = 1;
                piano.timeToLive = 1000;
                anyKeyActive = true;
            }

            // --- Phase 2: Count down note duration ---
            if (piano.durations[key] > 0)
            {
                piano.durations[key] -= elapsed;
                if (piano.durations[key] <= 0)
                    piano.durations[key] = 0;

                piano.isActive = 1;
                piano.timeToLive = 1000;
                anyKeyActive = true;
            }

            // --- Phase 3: Return key to rest position ---
            if (piano.keyAngles[key] > 0.0f && !piano.durations[key])
            {
                anyPianoActive = true;
                piano.keyAngles[key] -= 0.5f;

                if (piano.keyAngles[key] > 0.0f)
                {
                    piano.isActive = 1;
                    piano.timeToLive = 1000;
                    anyKeyActive = true;
                }
                else
                {
                    piano.keyAngles[key] = 0.0f;
                }
            }
        }

        // --- Phase 4: Deactivate idle piano ---
        if (!anyKeyActive && piano.isActive == 1 && --piano.timeToLive <= 0)
        {
            piano.timeToLive = 0;
            piano.isActive = 0;
        }
    }

    return anyPianoActive;
}
