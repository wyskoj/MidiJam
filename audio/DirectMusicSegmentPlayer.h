//
// Created by Jacob on 3/16/2026.
//

#ifndef MIDIJAM_DIRECTMUSICSEGMENTWRAPPER_H
#define MIDIJAM_DIRECTMUSICSEGMENTWRAPPER_H

#include <windows.h>
#include <dmusici.h>

class DirectMusicSegmentPlayer {
public:
    IDirectMusicSegment8* segment;
    IDirectMusicPerformance8* performance;
    IDirectMusicLoader8* loader;
    IDirectMusicAudioPath* audioPath;
    bool downloaded;

    DirectMusicSegmentPlayer(IDirectMusicPerformance8* performance,
                             IDirectMusicLoader8* loader,
                             IDirectMusicSegment8* segment);
    ~DirectMusicSegmentPlayer();

    // SETUP/TEARDOWN
    HRESULT Download(IUnknown* overrideAudioPath);
    void Cleanup();

    // PLAYBACK CONTROL
    HRESULT Play(int flags, IDirectMusicAudioPath* overrideAudioPath) const;
    HRESULT Stop(int flags) const;

    // QUERY/STATE
    BOOL IsPlaying() const;

    // CONFIGURATION
    HRESULT SetRepeatCount(int count) const;
};

#endif // MIDIJAM_DIRECTMUSICSEGMENTWRAPPER_H
