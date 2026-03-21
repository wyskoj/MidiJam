//
// Created by Jacob on 03/21/2026.
//

#include "StageHorn.h"
#include "scene/scene.h"

DEF_INST(stageHorn, StageHornState)

GLfloat startAngle = 12.0f * (-202.5f / STAGE_BASE_Z);
GLfloat angleStep = -1.125f * (-202.5f / STAGE_BASE_Z);

extern MUSIC_TIME g_currentGlobalTime;
Ms3dBundle* g_stageHorn_ms3d = nullptr;

void RenderStageHorn() {
    short visibleCount = 0;

    glPushMatrix();
    glTranslatef(0.0f, 8.0f, -14.0f);

    for (short s = 0; s < g_stageHornCount; ++s) {
        const StageHornState& inst = g_stageHorn[s];
        if (inst.isActive != 1)
            continue;

        for (short horn = 0; horn < 12; ++horn) {
            const GLfloat angle = startAngle - horn * angleStep;
            const GLfloat z = STAGE_BASE_Z - visibleCount * 5.0f;
            const GLfloat y = inst.riseTimer[horn] > 0
                                  ? (1.0f - inst.riseTimer[horn] / static_cast<float>(inst.riseDuration[horn])) * -10.0f
                                  : -10.0f;

            glPushMatrix();
            glRotatef(angle, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.0f, y, z);
            g_stageHorn_ms3d->RenderModelShiny();
            glPopMatrix();
        }

        ++visibleCount;
    }

    glPopMatrix();
}

bool UpdateStageHorn(const MUSIC_TIME pmtNow) {
    __int16 anyStageHornActive = 0;
    for (__int16 i = 0; i < g_stageHornCount; ++i) {
        __int16 anyHornActive = 0;
        for (__int16 horn = 0; horn < 12; ++horn) {
            for (__int16 slot = 0; slot < 16; ++slot) {
                if (g_stageHorn[i].timeDeltas[horn][slot] > 0) {
                    anyStageHornActive = 1;
                    g_stageHorn[i].timeDeltas[horn][slot] -= pmtNow - g_currentGlobalTime;
                    if (g_stageHorn[i].timeDeltas[horn][slot] <= 0) {
                        g_stageHorn[i].timeDeltas[horn][slot] = 0;
                        g_stageHorn[i].riseTimer[horn] = g_stageHorn[i].queue[horn][slot];
                        g_stageHorn[i].riseDuration[horn] = g_stageHorn[i].queue[horn][slot];
                        g_stageHorn[i].queue[horn][slot] = 0;
                    }
                    g_stageHorn[i].isActive = 1;
                    g_stageHorn[i].timeToLive = 1000;
                    anyHornActive = 1;
                }
            }
            if (g_stageHorn[i].riseTimer[horn] > 0) {
                g_stageHorn[i].riseTimer[horn] -= pmtNow - g_currentGlobalTime;
                if (g_stageHorn[i].riseTimer[horn] > 0) {
                    g_stageHorn[i].isActive = 1;
                    g_stageHorn[i].timeToLive = 1000;
                    anyHornActive = 1;
                    anyStageHornActive = 1;
                }
                else {
                    g_stageHorn[i].riseTimer[horn] = 0;
                }
            }
        }
        if (!anyHornActive && g_stageHorn[i].isActive == 1 && --g_stageHorn[i].timeToLive <= 0) {
            g_stageHorn[i].timeToLive = 0;
            g_stageHorn[i].isActive = 0;
        }
    }
    return anyStageHornActive == 1;
}
