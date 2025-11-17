//
// Created by Jacob on 11/16/2025.
//

#ifndef MIDIJAM_DIRECTMUSICSEGMENTWRAPPER_H
#define MIDIJAM_DIRECTMUSICSEGMENTWRAPPER_H
#include "dmusici.h"


class DirectMusicSegmentWrapper {
public:
    IDirectMusicSegment8 *pSegment;
    IDirectMusicLoader8 *pLoader;
    IDirectMusicPerformance8 *pPerformance;
    IDirectMusicAudioPath8 *pAudioPath;
    int field_14;

    DirectMusicSegmentWrapper(IDirectMusicPerformance8 *p_performance, IDirectMusicLoader8 *p_loader,
                              IDirectMusicSegment8 *p_segment);

    ~DirectMusicSegmentWrapper();

    HRESULT Download(IUnknown *pAudioPathParam);
    void Init();
    HRESULT IsPlaying() const;
    HRESULT PlaySegment(int dwFlags, IDirectMusicAudioPath8* pAudioPathParam);
    HRESULT SetRepeats(int dwRepeats) const;
    HRESULT Stop(int dwFlags) const;
};


#endif //MIDIJAM_DIRECTMUSICSEGMENTWRAPPER_H
