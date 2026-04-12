//
// Created by Jacob on 4/11/2026.
//

#include "strings.h"

float flt_46E3AC = 10.53;
float flt_46E3C0 = 44.55;
float flt_46E398 = 0.81;
float dword_45E7E4[18] = {
    0.0,
    0.0,
    0.0640000030,
    0.1200000048,
    0.1840000004,
    0.2480000108,
    0.3039999902,
    0.3599999845,
    0.4079999924,
    0.4480000138,
    0.4960000217,
    0.5360000134,
    0.5760000348,
    0.6159999967,
    0.6480000019,
    0.6800000072,
    0.7120000124,
    0.7360000014,
};

GLfloat g_stringInstrument_stringScale[4] = {};

Ms3dBundle* g_violinBow_ms3d = nullptr;
Ms3dBundle* g_violinString_ms3d = nullptr;
Ms3dBundle* g_violinFinger_ms3d = nullptr;
Ms3dBundle* g_violinStringPlayedX_ms3d[5] = {};