//
// Created by Jacob on 3/16/2026.
//

#include "DlsCollectionWrapper.h"

#include <dmusici.h>

// FUNCTION: MIDIJAM 0x401DD0
DlsCollectionWrapper::DlsCollectionWrapper(
    IDirectMusicPerformance8* pPerformance,
    IDirectMusicLoader8*      pLoader,
    IDirectMusicScript*       pScript)
{
    this->pPerformance = pPerformance;
    this->pLoader      = pLoader;
    this->pScript      = pScript;
}

// FUNCTION: MIDIJAM 0x401FA0
DlsCollectionWrapper::~DlsCollectionWrapper()
{
    Cleanup();
}

// FUNCTION: MIDIJAM 0x401E10
void DlsCollectionWrapper::Cleanup()
{
    if (pLoader)
    {
        pLoader->ReleaseObjectByUnknown(pScript);
        pLoader = nullptr;
    }

    if (pScript)
    {
        pScript->Release();
        pScript = nullptr;
    }

    pPerformance = nullptr;
}