//
// Created by Jacob on 3/16/2026.
//

#include "playback.h"

#include "DirectMusicSystem.h"
#include "DirectMusicSegmentPlayer.h"

#include <dmusici.h>
#include <cstdio>
#include <cstring>

// ---------------------------------------------------------------------------
// Globals referenced by playback functions.
// These are defined elsewhere in the application and declared here for access.
// ---------------------------------------------------------------------------

extern DirectMusicSegmentPlayer* g_directMusicSegmentPlayer;
extern DirectMusicSystem* g_directMusicSystem;
extern IDirectMusicPerformance8* g_DirectMusicPerformance;
extern char g_midiFileNameDisp[];
extern MUSIC_TIME g_midiFileDuration;
extern REFERENCE_TIME g_prtStart;
extern MUSIC_TIME g_mtStart;

// FUNCTION: MIDIJAM 0x438C80
int __cdecl LoadAndPlayMidiFile(const char *midiFilePath)
{
    IDirectMusicPerformance *pPerformance;
    size_t len;
    int i;

    // Release any previously loaded MIDI segment
    if (g_directMusicSegmentPlayer != NULL) {
        // g_directMusicSegmentPlayer->Release(1); // vtable offset 0, param 1 = force release
    }

    // Clean up loader garbage (releases unused resources)
    g_directMusicSystem->CollectLoaderGarbage();

    // Store the full path in the display buffer
    sprintf(g_midiFileNameDisp, "%s", midiFilePath);

    // Extract just the filename (after the last backslash) for display
    len = strlen(midiFilePath);
    i = (int)len - 1;
    while (i >= 0)
    {
        if (midiFilePath[i] == '\\')
        {
            sprintf(g_midiFileNameDisp, "%s", &midiFilePath[i + 1]);
            break;
        }
        i--;
    }

    // Load the MIDI file as a DirectMusic segment
    // Parameters: system, output segment wrapper, file path, download instruments, set as standard MIDI
    HRESULT hr = g_directMusicSystem->LoadMidiSegmentFromPath(
        &g_directMusicSegmentPlayer,
        midiFilePath,
        1,   // downloadInstruments = TRUE
        1    // setAsStandardMidiFile = TRUE
    );
    if (hr < 0)
        return 1;   // loading failed

    // Obtain the performance object from the segment wrapper
    IDirectMusicSegment *pSegment = g_directMusicSegmentPlayer->segment;
    pSegment->GetLength(&g_midiFileDuration);

    return 0;   // success
}

// FUNCTION: MIDIJAM 0x438D90
// MATCH: EXACT
HRESULT PlaySegment() {
    DWORD dwRepeatsFlag = 0;
    HRESULT hr;

    if (dwRepeatsFlag) {
        hr = g_directMusicSegmentPlayer->SetRepeatCount(-1);
        if (hr < 0)
            return hr;
    }
    else {
        hr = g_directMusicSegmentPlayer->SetRepeatCount(0);
        if (hr < 0)
            return hr;
    }

    g_DirectMusicPerformance->GetTime(&g_prtStart, &g_mtStart);
    hr = g_directMusicSegmentPlayer->Play(0x800000, 0);

    // Match original branch pattern
    if (hr < 0)
        return hr;
    return 0;
}
