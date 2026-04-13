//
// Created by Jacob on 4/13/2026.
//

#include "particles.h"

#include <cstdlib>
extern float g_recoil_whistle_long;
extern float g_recoil_whistle_short;
extern DS_Particles g_particles[2];

bool UpdateSteamPuffers() {
    __int16 i; // [esp+18h] [ebp-Ch]
    __int16 j; // [esp+1Ch] [ebp-8h]
    __int16 v3; // [esp+20h] [ebp-4h]

    v3 = 0;
    for (i = 0; i < 2; ++i) {
        if (!i && g_recoil_whistle_long > 0.0 || i == 1 && g_recoil_whistle_short > 0.0) {
            v3 = 1;
            if (g_particles[i].field_1132 <= 0) {
                g_particles[i].field_0[g_particles[i].field_1130] = 0.1;
                g_particles[i].field_320[g_particles[i].field_1130] = 0.25;
                g_particles[i].field_4B0[g_particles[i].field_1130] = 0.0;
                g_particles[i].field_640[g_particles[i].field_1130] = 0.0;
                g_particles[i].field_7D0[g_particles[i].field_1130] = 0.0;
                g_particles[i].field_960[g_particles[i].field_1130] = 0.0;
                g_particles[i].field_AF0[g_particles[i].field_1130] = ((double)(50 * rand() / 0x8000) - 25.0) * 0.1;
                g_particles[i].field_C80[g_particles[i].field_1130] = ((double)(50 * rand() / 0x8000) - 25.0) * 0.1;
                g_particles[i].field_E10[g_particles[i].field_1130] = ((double)(25 * rand() / 0x8000) - 12.5) * 0.001;
                g_particles[i].field_FA0[g_particles[i].field_1130] = ((double)(25 * rand() / 0x8000) - 12.5) * 0.001
                    + 0.1;
                g_particles[i].field_190[g_particles[i].field_1130] = 0.0;
                if (i)
                    g_particles[i].field_190[g_particles[i].field_1130] = g_recoil_whistle_short;
                else
                    g_particles[0].field_190[g_particles[0].field_1130] = g_recoil_whistle_long;
                if (++g_particles[i].field_1130 >= 100)
                    g_particles[i].field_1130 = 0;
                g_particles[i].field_1132 = 2;
            }
            else {
                --g_particles[i].field_1132;
            }
        }
        for (j = 0; j < 100; ++j) {
            if (g_particles[i].field_0[j] <= 0.0) {
                g_particles[i].field_0[j] = 0.0;
            }
            else {
                g_particles[i].field_0[j] = g_particles[i].field_0[j] + g_particles[i].field_320[j];
                g_particles[i].field_320[j] = g_particles[i].field_320[j] * 0.95999998;
                g_particles[i].field_4B0[j] = g_particles[i].field_4B0[j] + g_particles[i].field_AF0[j];
                g_particles[i].field_640[j] = g_particles[i].field_640[j] + g_particles[i].field_C80[j];
                g_particles[i].field_7D0[j] = g_particles[i].field_7D0[j] + g_particles[i].field_E10[j];
                g_particles[i].field_960[j] = g_particles[i].field_960[j] + g_particles[i].field_FA0[j];
                if (g_particles[i].field_320[j] < 0.0099999998) {
                    g_particles[i].field_0[j] = 0.0;
                    g_particles[i].field_320[j] = 0.0;
                }
            }
        }
    }
    return v3 == 1;
}
