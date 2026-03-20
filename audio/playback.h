//
// Created by Jacob on 3/16/2026.
//

#ifndef MIDIJAM_PLAYBACK_H
#define MIDIJAM_PLAYBACK_H

#include <windows.h>

/**
 * Loads a MIDI file from disk, destroys any previously loaded segment,
 * and extracts the filename for display in the UI.
 * @param midiFilePath  Full path to the MIDI file to load.
 * @return              0 on success, 1 on failure.
 */
int LoadAndPlayMidiFile(const char* midiFilePath);

/**
 * Plays the currently loaded MIDI segment as a secondary segment.
 * Captures the start time before playback begins.
 * @return  S_OK on success, or the failing HRESULT.
 */
HRESULT PlaySegment();

#endif // MIDIJAM_PLAYBACK_H