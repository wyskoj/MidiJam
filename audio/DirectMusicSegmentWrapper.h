//
// Created by Jacob on 3/16/2026.
//

#ifndef MIDIJAM_DIRECTMUSICSEGMENTWRAPPER_H
#define MIDIJAM_DIRECTMUSICSEGMENTWRAPPER_H

#include <windows.h>
#include <dmusici.h>

/**
 * Wraps an IDirectMusicSegment8 and its associated playback context.
 * Manages the lifetime of the segment, audio path, and download state.
 *
 * Original struct layout (sizeof = 0x18):
 *   +0x00  void*                     vtable
 *   +0x04  IDirectMusicSegment8*     pSegment
 *   +0x08  IDirectMusicLoader*       pLoader
 *   +0x0C  IDirectMusicPerformance8* pPerformance
 *   +0x10  IDirectMusicAudioPath*    pAudioPath
 *   +0x14  int                       isDownloaded
 */
class DirectMusicSegmentWrapper
{
public:
    IDirectMusicSegment8*     pSegment;
    IDirectMusicLoader8*      pLoader;
    IDirectMusicPerformance8* pPerformance;
    IDirectMusicAudioPath*    pAudioPath;
    int                       isDownloaded;

    /**
     * Initializes the wrapper, attempts to create an audio path from the
     * segment's audio path config via the performance object.
     * @param pPerformance        The DirectMusic performance object.
     * @param pLoader             The DirectMusic loader object.
     * @param pSegment            The segment to wrap.
     */
    DirectMusicSegmentWrapper(
        IDirectMusicPerformance8* pPerformance,
        IDirectMusicLoader8*      pLoader,
        IDirectMusicSegment8*     pSegment);

    /**
     * Destructor. Calls Cleanup() and optionally frees memory.
     * Note: the original takes a freeMemory bool parameter — this is a
     * compiler-generated two-phase destructor pattern from MSVC.
     */
    ~DirectMusicSegmentWrapper();

    /**
     * Releases all COM references and zeroes all pointers.
     * Despite the name, this is a cleanup/teardown function, not initialization.
     * Named Init in the original source — retained for matching purposes.
     */
    void Init();

    /**
     * Downloads the segment's instruments to the audio path or performance object.
     * Sets isDownloaded to 1 on success.
     * @param pAudioPathParam  Optional audio path to download to. If null, uses
     *                         pAudioPath if available, otherwise pPerformance.
     * @return                 S_OK on success, CO_E_NOTINITIALIZED if pSegment is null.
     */
    HRESULT Download(IUnknown* pAudioPathParam);

    /**
     * Plays the segment via PlaySegmentEx on the performance object.
     * Requires isDownloaded to be set — returns E_FAIL if not downloaded.
     * @param dwFlags     Playback flags passed to PlaySegmentEx.
     * @param pDirectMusicAudioPath  Optional audio path override. If null, uses pAudioPath.
     * @return            HRESULT from PlaySegmentEx, or error if not ready.
     */
    HRESULT PlaySegment(int dwFlags, IDirectMusicAudioPath* pDirectMusicAudioPath) const;

    /**
     * Returns TRUE if the segment is currently playing.
     * Returns CO_E_NOTINITIALIZED if pSegment or pPerformance is null.
     */
    BOOL IsPlaying() const;

    /**
     * Stops playback of the segment.
     * @param dwFlags  Stop flags passed to IDirectMusicPerformance8::Stop.
     * @return         HRESULT from Stop, or CO_E_NOTINITIALIZED if not ready.
     */
    HRESULT Stop(int dwFlags) const;

    /**
     * Sets the number of times the segment repeats.
     * @param dwRepeats  Repeat count passed to IDirectMusicSegment8::SetRepeats.
     * @return           HRESULT from SetRepeats, or CO_E_NOTINITIALIZED if pSegment is null.
     */
    HRESULT SetRepeats(int dwRepeats) const;
};

#endif // MIDIJAM_DIRECTMUSICSEGMENTWRAPPER_H