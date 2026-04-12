//
// Created by Jacob on 4/12/2026.
//

#ifndef MIDIJAM_PARTICLES_H
#define MIDIJAM_PARTICLES_H

struct DS_Particles {
    float field_0[100];
    float field_190[100];
    float field_320[100];
    float field_4B0[100];
    float field_640[100];
    float field_7D0[100];
    float field_960[100];
    float field_AF0[100];
    float field_C80[100];
    float field_E10[100];
    float field_FA0[100];
    __int16 field_1130;
    __int16 field_1132;
};

static_assert(sizeof(DS_Particles) == 0x1134, "DS_Particles layout mismatch");

#endif //MIDIJAM_PARTICLES_H
