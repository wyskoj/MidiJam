//
// Created by Jacob on 11/16/2025.
//

#ifndef MIDIJAM_DIRECTMUSICSYSTEM_H
#define MIDIJAM_DIRECTMUSICSYSTEM_H

#include "dmusici.h"
#include "DirectMusicSegmentWrapper.h"

class DlsCollectionWrapper // sizeof=0x10
{
public:
    DlsCollectionWrapper(IDirectMusicPerformance8 *p_performance, IDirectMusicLoader *p_loader,
                         IDirectMusicScript *p_script)
        : pScript(p_script),
          pLoader(p_loader),
          pPerformance(p_performance) {
    }

    IDirectMusicScript *pScript;
    IDirectMusicLoader *pLoader;
    IDirectMusicPerformance8 *pPerformance;
};

class DirectMusicSystem {
public:
    IDirectMusicLoader8 *pLoader;
    IDirectMusicPerformance8 *pPerformance;

    DirectMusicSystem();

    void CollectLoaderGarbage() const;

    IDirectMusicAudioPath8 *GetDefaultAudioPath() const;

    IDirectMusicPerformance8 *GetPerformance() const;

    HRESULT Init(HWND hWnd, int audioParams1, int audioParams2);

    HRESULT LoadDlsCollection(DlsCollectionWrapper **ppCollection, LPCCH dlsFilePath);

    HRESULT LoadMidiSegmentFromPath(DirectMusicSegmentWrapper **ppSegmentWrapper, LPCCH midiFilePath,
                                    BOOL downloadInstruments, BOOL setAsStandardMidiFile);

    HRESULT LoadMidiSegmentFromResource(DirectMusicSegmentWrapper **ppSegmentWrapper,
                                        LPCSTR lpName,
                                        LPCSTR lpType,
                                        BOOL downloadInstruments,
                                        BOOL setAsStandardMidiFile);

    HRESULT LoadStyle(void **ppObject, LPCCH objectFilePath) const;
};


#endif //MIDIJAM_DIRECTMUSICSYSTEM_H
