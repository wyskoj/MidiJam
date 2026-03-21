//
// Created by Jacob on 3/16/2026.
//

#ifndef MIDIJAM_DIRECTMUSICSYSTEM_H
#define MIDIJAM_DIRECTMUSICSYSTEM_H

#include <windows.h>
#include <dmusici.h>

class DlsCollectionWrapper;
class DirectMusicSegmentPlayer;

/**
 * Top-level DirectMusic system object. Owns the IDirectMusicLoader8 and
 * IDirectMusicPerformance8 COM interfaces for the lifetime of the application.
 *
 * Initialized via Init(), which calls CoCreateInstance for both interfaces
 * and calls InitAudio on the performance object to set up the audio graph.
 *
 * Original struct layout (confirmed from ctor):
 *   +0x00  IDirectMusicLoader8*      pLoader
 *   +0x04  IDirectMusicPerformance8* pPerformance
 */
class DirectMusicSystem
{
public:
    IDirectMusicLoader8*      pLoader;
    IDirectMusicPerformance8* pPerformance;

    /**
     * Initializes pLoader and pPerformance to null and calls CoInitialize.
     */
    DirectMusicSystem();

    /**
     * Creates the DirectMusic loader and performance COM objects and initializes
     * the audio graph via InitAudio.
     * @param hWnd         Window handle passed to InitAudio.
     * @param audioParams1 Passed as dwDefaultPathType to InitAudio.
     * @param audioParams2 Passed as dwPChannelCount to InitAudio.
     * @return             S_OK on success, or the failing HRESULT.
     */
    HRESULT Init(HWND hWnd, int audioParams1, int audioParams2);

    /**
     * Returns the IDirectMusicPerformance8 pointer.
     */
    IDirectMusicPerformance8* GetPerformance() const;

    /**
     * Returns the default audio path from the performance object.
     * Returns null if pPerformance is null.
     */
    IDirectMusicAudioPath* GetDefaultAudioPath() const;

    /**
     * Calls CollectGarbage on the loader if pLoader is non-null.
     */
    void CollectLoaderGarbage() const;

    /**
     * Loads a MIDI segment from a file path and wraps it in a DirectMusicSegmentWrapper.
     * Optionally sets the standard MIDI file parameter and downloads instruments.
     * @param ppSegmentWrapper      Output wrapper object.
     * @param midiFilePath          ANSI path to the MIDI file.
     * @param downloadInstruments   If TRUE, calls Download on the wrapper after loading.
     * @param setAsStandardMidiFile If TRUE, sets GUID_StandardMIDIFile on the segment.
     * @return                      S_OK on success, or the failing HRESULT.
     */
    HRESULT LoadMidiSegmentFromPath(
        DirectMusicSegmentPlayer** ppSegmentWrapper,
        LPCCH midiFilePath,
        BOOL downloadInstruments,
        BOOL setAsStandardMidiFile) const;

    /**
     * Loads a MIDI segment from a Win32 resource and wraps it in a DirectMusicSegmentWrapper.
     * Optionally sets the standard MIDI file parameter and downloads instruments.
     * @param ppSegmentWrapper      Output wrapper object.
     * @param lpName                Resource name.
     * @param lpType                Resource type.
     * @param downloadInstruments   If TRUE, calls Download on the wrapper after loading.
     * @param setAsStandardMidiFile If TRUE, sets GUID_StandardMIDIFile on the segment.
     * @return                      S_OK on success, or the failing HRESULT.
     */
    HRESULT LoadMidiSegmentFromResource(
        DirectMusicSegmentPlayer** ppSegmentWrapper,
        LPCSTR lpName,
        LPCSTR lpType,
        BOOL downloadInstruments,
        BOOL setAsStandardMidiFile) const;

    /**
     * Loads a DirectMusic script object and wraps it in a DlsCollectionWrapper.
     *
     * TODO: verify function name — body loads a IDirectMusicScript, not a DLS collection.
     * Either the name is wrong or IDA's CLSID identification is incorrect.
     *
     * @param ppCollection  Output wrapper object.
     * @param dlsFilePath   ANSI path to the script/DLS file.
     * @return              S_OK on success, or the failing HRESULT.
     */
    HRESULT LoadDlsCollection(DlsCollectionWrapper** ppCollection, LPCCH dlsFilePath) const;

    /**
     * Loads a generic DirectMusic object from a file path.
     *
     * TODO: verify function name — body uses CLSID_DirectMusicStyle but
     * takes a void** suggesting it may be used for multiple object types.
     *
     * @param ppObject        Output object pointer.
     * @param objectFilePath  ANSI path to the file.
     * @return                HRESULT from LoadObjectFromFile.
     */
    HRESULT LoadStyle(void** ppObject, LPCCH objectFilePath) const;
};

static_assert(sizeof(DirectMusicSystem) == 8, "DirectMusicSystem layout mismatch");

#endif // MIDIJAM_DIRECTMUSICSYSTEM_H