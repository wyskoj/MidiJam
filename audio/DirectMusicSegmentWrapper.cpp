//
// Created by Jacob on 3/16/2026.
//

#include "DirectMusicSegmentWrapper.h"

#include <dmusici.h>

// FUNCTION: MIDIJAM 0x401960
DirectMusicSegmentWrapper::DirectMusicSegmentWrapper(
    IDirectMusicPerformance8* pPerformance,
    IDirectMusicLoader8* pLoader,
    IDirectMusicSegment8* pSegment)
{
    this->pPerformance = pPerformance;
    this->pLoader = pLoader;
    this->pSegment = pSegment;
    this->pAudioPath = nullptr;
    this->isDownloaded = 0;

    IUnknown* pSourceConfig = nullptr;
    if (SUCCEEDED(this->pSegment->GetAudioPathConfig(&pSourceConfig)))
    {
        this->pPerformance->CreateAudioPath(pSourceConfig, TRUE, &this->pAudioPath);
        if (pSourceConfig)
        {
            pSourceConfig->Release();
            pSourceConfig = nullptr;
        }
    }
}

// FUNCTION: MIDIJAM 0x401F70
DirectMusicSegmentWrapper::~DirectMusicSegmentWrapper()
{
    Init();
}

// FUNCTION: MIDIJAM 0x401A10
// Note: named Init in the original — this is actually a cleanup/teardown function.
void DirectMusicSegmentWrapper::Init()
{
    if (pSegment)
    {
        if (pLoader)
            pLoader->ReleaseObjectByUnknown(pSegment);

        if (isDownloaded)
        {
            if (pAudioPath)
                pSegment->Unload(pAudioPath);
            else
                pSegment->Unload(pPerformance);
        }

        if (pAudioPath)
        {
            pAudioPath->Release();
            pAudioPath = nullptr;
        }

        // ReSharper disable once CppDFAConstantConditions
        if (pSegment)
        {
            pSegment->Release();
            pSegment = nullptr;
        }
    }

    pPerformance = nullptr;
}

// FUNCTION: MIDIJAM 0x401B70
HRESULT DirectMusicSegmentWrapper::Download(IUnknown* pAudioPathParam)
{
    if (!pSegment)
        return CO_E_NOTINITIALIZED;

    HRESULT hr;
    if (pAudioPathParam)
    {
        hr = pSegment->Download(pAudioPathParam);
    }
    else if (pAudioPath)
    {
        hr = pSegment->Download(pAudioPath);
    }
    else
    {
        hr = pSegment->Download(pPerformance);
    }

    if (SUCCEEDED(hr))
        isDownloaded = 1;

    return hr;
}

// FUNCTION: MIDIJAM 0x401AF0
HRESULT DirectMusicSegmentWrapper::PlaySegment(const int dwFlags, IDirectMusicAudioPath* pDirectMusicAudioPath) const
{
    if (!pSegment || !pPerformance)
        return CO_E_NOTINITIALIZED;

    if (!isDownloaded)
        return E_FAIL;

    if (!pDirectMusicAudioPath && this->pAudioPath)
        pDirectMusicAudioPath = this->pAudioPath;

    return pPerformance->PlaySegmentEx(
        pSegment,
        nullptr,
        nullptr,
        dwFlags,
        0,
        nullptr,
        nullptr,
        pDirectMusicAudioPath);
}

// FUNCTION: MIDIJAM 0x401CB0
BOOL DirectMusicSegmentWrapper::IsPlaying() const
{
    if (pSegment && pPerformance)
        return pPerformance->IsPlaying(pSegment, nullptr) == S_OK;
    else
        return CO_E_NOTINITIALIZED;
}

// FUNCTION: MIDIJAM 0x401D00
HRESULT DirectMusicSegmentWrapper::Stop(const int dwFlags) const
{
    if (pSegment && pPerformance)
        return pPerformance->Stop(pSegment, nullptr, 0, dwFlags);
    else
        return CO_E_NOTINITIALIZED;
}

// FUNCTION: MIDIJAM 0x401D50
HRESULT DirectMusicSegmentWrapper::SetRepeats(const int dwRepeats) const
{
    if (pSegment)
        return pSegment->SetRepeats(dwRepeats);
    else
        return CO_E_NOTINITIALIZED;
}
