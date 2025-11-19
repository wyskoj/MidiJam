//
// Created by Jacob on 11/16/2025.
//

#ifndef MIDIJAM_MIDI_H
#define MIDIJAM_MIDI_H
#include "dmusici.h"

extern MUSIC_TIME g_midiFile_duration;

int __cdecl LoadAndPlayMidiFile(char *midiFilePath);

#endif //MIDIJAM_MIDI_H