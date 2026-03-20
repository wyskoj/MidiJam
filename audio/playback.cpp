//
// Created by Jacob on 3/16/2026.
//

#include "playback.h"

#include "DirectMusicSystem.h"
#include "DirectMusicSegmentWrapper.h"

#include <dmusici.h>
#include <cstdio>
#include <cstring>

// ---------------------------------------------------------------------------
// Globals referenced by playback functions.
// These are defined elsewhere in the application and declared here for access.
// ---------------------------------------------------------------------------

extern DirectMusicSegmentWrapper* g_DirectMusicSegmentWrapper;
extern DirectMusicSystem* g_DirectMusicSystem;
extern IDirectMusicPerformance8* g_DirectMusicPerformance;
extern char g_midiFileNameDisp[];
extern MUSIC_TIME g_midiFile_duration;
extern REFERENCE_TIME g_prtStart;
extern MUSIC_TIME g_mtStart;

// FUNCTION: MIDIJAM 0x438C80
int LoadAndPlayMidiFile(const char* midiFilePath)
{
    // Destroy the existing segment wrapper if one is loaded.
    if (g_DirectMusicSegmentWrapper)
    {
        delete g_DirectMusicSegmentWrapper;
        g_DirectMusicSegmentWrapper = nullptr;
    }

    g_DirectMusicSystem->CollectLoaderGarbage();

    sprintf(g_midiFileNameDisp, "%s", midiFilePath);

    // Extract just the filename from the full path for display purposes.
    for (short i = static_cast<short>(strlen(midiFilePath)) - 1; i >= 0; --i)
    {
        if (midiFilePath[i] == '\\')
        {
            sprintf(g_midiFileNameDisp, "%s", &midiFilePath[i + 1]);
            break;
        }
    }

    if (FAILED(g_DirectMusicSystem->LoadMidiSegmentFromPath(
        &g_DirectMusicSegmentWrapper,
        midiFilePath,
        TRUE,
        TRUE)))
    {
        return 1;
    }

    // TODO: verify — IDA shows GetPerformance called on g_DirectMusicSegmentWrapper,
    // but DirectMusicSegmentWrapper has no GetPerformance method. This likely
    // accesses pSegment directly to retrieve the track length.
    IDirectMusicSegment8* pSegment = g_DirectMusicSegmentWrapper->pSegment;
    pSegment->GetLength(&g_midiFile_duration);

    return 0;
}

// FUNCTION: MIDIJAM 0x438D90
HRESULT PlaySegment()
{
    HRESULT hrSetRepeats;
    int hrPlaySegment;

    hrSetRepeats = g_DirectMusicSegmentWrapper->SetRepeats(0);
    if (hrSetRepeats < 0)
        return hrSetRepeats;
    g_DirectMusicPerformance->GetTime(&g_prtStart, &g_mtStart);
    hrPlaySegment = g_DirectMusicSegmentWrapper->PlaySegment(0x800000, 0);
    if (hrPlaySegment >= 0)
        return 0;
    return hrPlaySegment;
}
