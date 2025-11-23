//
// Created by Jacob on 11/22/2025.
//

#include "Harp.h"

#include "../globals.h"
#include "../Ms3dBundle.h"

DEF_INST(harp, I_DS_Harp)
float HARP_STRING_SCALE[47] =
{
    69.010002,
    68.606003,
    68.093002,
    67.471001,
    66.876999,
    65.849998,
    64.686996,
    63.417,
    62.335999,
    61.146999,
    60.012001,
    59.066002,
    58.362999,
    57.551998,
    56.958,
    56.417,
    55.876999,
    55.417,
    55.039001,
    54.687,
    54.282001,
    53.903999,
    53.551998,
    53.174,
    52.876999,
    52.632999,
    52.335999,
    52.201,
    52.066002,
    51.958,
    51.931,
    51.985001,
    52.034,
    52.094002,
    52.194,
    52.313999,
    52.414001,
    52.694,
    52.973999,
    53.254002,
    53.613998,
    54.153999,
    54.594002,
    54.894001,
    55.133999,
    55.113998,
    55.113998
};

__int16 word_45EC60[12] = {
    0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6
};

HARP_STRING_COLOR g_harpStringColors[48];
GLfloat g_harp_string_y[47];
GLfloat g_harp_string_z[47];
GLfloat g_harp_string_scale[47];

void I_Harp() {
    __int16 j; // [esp+0h] [ebp-Ch]
    __int16 i; // [esp+4h] [ebp-8h]
    __int16 v2; // [esp+8h] [ebp-4h]

    v2 = 0;
    for (i = 0; i < g_ialloc_harp; ++i) {
        if (g_ds_harp[i].isActive == 1) {
            g_harp_ms3d->RenderModel();
            for (j = 0; j < 47; ++j) {
                glPushMatrix();
                glTranslatef(0.0, g_harp_string_y[j], g_harp_string_z[j]);
                glRotatef(4.0, 1.0, 0.0, 0.0);
                glScalef(1.0, g_harp_string_scale[j], 1.0);
                if (g_ds_harp[i].field_4[j] <= 0)
                    g_harpString_ms3d[g_harpStringColors[j]]->RenderModel();
                else
                    g_harpStringPlayingX_ms3d[g_harpStringColors[j]][g_vibratingString_frame]->RenderModel();
                glPopMatrix();
            }
            glTranslatef(16.0, 0.0, 0.0);
            ++v2;
        }
    }
    g_inst_visible_harp = 0;
    if (v2 > 0)
        g_inst_visible_harp = v2;
}

bool __cdecl I_Harp_MM(MUSIC_TIME mtNow) {
    __int16 v2; // [esp+4h] [ebp-14h]
    __int16 i; // [esp+8h] [ebp-10h]
    __int16 k; // [esp+Ch] [ebp-Ch]
    __int16 j; // [esp+10h] [ebp-8h]
    __int16 v6; // [esp+14h] [ebp-4h]

    v6 = 0;
    for (i = 0; i < g_ialloc_harp; ++i) {
        v2 = 0;
        for (j = 0; j < 47; ++j) {
            for (k = 0; k < 16; ++k) {
                if (g_ds_harp[i].field_D3C[j][k] > 0) {
                    v6 = 1;
                    g_ds_harp[i].field_D3C[j][k] -= mtNow - g_time_global_current;
                    if (g_ds_harp[i].field_D3C[j][k] <= 0) {
                        g_ds_harp[i].field_D3C[j][k] = 0;
                        g_ds_harp[i].field_4[j] = g_ds_harp[i].field_17C[j][k];
                        g_ds_harp[i].field_C0[j] = g_ds_harp[i].field_17C[j][k];
                        g_ds_harp[i].field_17C[j][k] = 0;
                    }
                    g_ds_harp[i].isActive = 1;
                    g_ds_harp[i].timeToLive = 1000;
                    v2 = 1;
                }
            }
            if (g_ds_harp[i].field_4[j] > 0) {
                g_ds_harp[i].field_4[j] -= mtNow - g_time_global_current;
                if (g_ds_harp[i].field_4[j] > 0) {
                    g_ds_harp[i].isActive = 1;
                    g_ds_harp[i].timeToLive = 1000;
                    v2 = 1;
                    v6 = 1;
                } else {
                    g_ds_harp[i].field_4[j] = 0;
                }
            }
        }
        if (!v2 && g_ds_harp[i].isActive == 1 && --g_ds_harp[i].timeToLive <= 0) {
            g_ds_harp[i].timeToLive = 0;
            g_ds_harp[i].isActive = 0;
        }
    }
    return v6 == 1;
}
