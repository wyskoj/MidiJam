//
// Created by Jacob on 11/16/2025.
//

#include "midi.h"

#include <iostream>

#include "DirectMusicSystem.h"
#include "dmusici.h"
#include "globals.h"
#include "MidiJam.h"

int g_midiFile_duration;

int LoadAndPlayMidiFile(char *midiFilePath) {
    // [esp+Ch] [ebp-Ch]
    __int16 i; // [esp+14h] [ebp-4h]

    if (g_DirectMusicSegmentWrapper) {
        // unsure what, but this is never called!
    }
    g_DirectMusicSystem->CollectLoaderGarbage();
    sprintf(g_midiFileNameDisp, "%s", midiFilePath);
    // Extract filename for display
    for (i = strlen(midiFilePath) - 1; i >= 0; --i) {
        if (midiFilePath[i] == '\\') // Found a blackslash, copy everything after it
        {
            sprintf(g_midiFileNameDisp, "%s", &midiFilePath[i + 1]);
            break;
        }
    }
    if (g_DirectMusicSystem->LoadMidiSegmentFromPath(&g_DirectMusicSegmentWrapper, midiFilePath, 1, 1) < 0) {
        return 1; // Bad, kill app
    }

    IDirectMusicPerformance8 *pPerformance = g_DirectMusicSegmentWrapper->pPerformance;
    pPerformance->GetPrepareTime(reinterpret_cast<DWORD *>(&g_midiFile_duration));
    return 0; // Good!
}
