//
// Created by Jacob on 3/19/2026.
//

#ifndef MIDIJAM_INSTRUMENT_H
#define MIDIJAM_INSTRUMENT_H

#define DECL_INST(name) \
extern short g_##name##Visible; \
extern short g_##name##Count; \
extern short g_##name##Channel[300];

#define DECL_INST_DS(name, type) \
DECL_INST(name) \
extern type* g_##name;

#define DEF_INST(name, type) \
short  g_##name##Visible      = 0; \
short  g_##name##Count        = 0; \
type*  g_##name               = nullptr; \
short  g_##name##Channel[300] = {};

#endif //MIDIJAM_INSTRUMENT_H
