//
// Created by Jacob on 11/16/2025.
//

#include "DirectMusicSegmentWrapper.h"

DirectMusicSegmentWrapper::DirectMusicSegmentWrapper(IDirectMusicPerformance8 *p_performance,
                                                     IDirectMusicLoader8 *p_loader, IDirectMusicSegment8 *p_segment) {
    IUnknown *pSourceConfig; // [esp+4h] [ebp-4h] BYREF

    this->pPerformance = p_performance;
    this->pLoader = p_loader;
    this->pSegment = p_segment;
    this->pAudioPath = nullptr;
    this->field_14 = 0;
    pSourceConfig = nullptr;
    if (this->pSegment->GetAudioPathConfig(&pSourceConfig) >= 0) {
        this->pPerformance->CreateAudioPath(pSourceConfig, 1, &this->pAudioPath);
        if (pSourceConfig) {
            (pSourceConfig->Release)();
            pSourceConfig = nullptr;
        }
    }
}

DirectMusicSegmentWrapper::~DirectMusicSegmentWrapper() {
    Init();
}

HRESULT DirectMusicSegmentWrapper::Download(IUnknown *pAudioPathParam) {
    HRESULT hr; // [esp+4h] [ebp-4h]

    if (!this->pSegment)
        return CO_E_NOTINITIALIZED;
    if (pAudioPathParam) {
        hr = this->pSegment->Download(pAudioPathParam);
    } else if (this->pAudioPath) {
        hr = this->pSegment->Download(this->pAudioPath);
    } else {
        hr = this->pSegment->Download(this->pPerformance);
    }
    if (hr >= 0)
        this->field_14 = 1;
    return hr;
}

void DirectMusicSegmentWrapper::Init() {
    if (this->pSegment) {
        if (this->pLoader)
            this->pLoader->ReleaseObjectByUnknown(this->pSegment);
        if (this->field_14) {
            if (this->pAudioPath)
                this->pSegment->Unload(this->pAudioPath);
            else
                this->pSegment->Unload(this->pPerformance);
        }
        if (this->pAudioPath) {
            this->pAudioPath->Release();
            this->pAudioPath = nullptr;
        }
        if (this->pSegment) {
            this->pSegment->Release();
            this->pSegment = nullptr;
        }
    }
    this->pPerformance = nullptr;
}

HRESULT DirectMusicSegmentWrapper::IsPlaying() const {
    if (this->pSegment && this->pPerformance)
        return (this->pPerformance->IsPlaying)(this->pSegment, 0) == 0;
    return CO_E_NOTINITIALIZED;
}

HRESULT DirectMusicSegmentWrapper::PlaySegment(int dwFlags, IDirectMusicAudioPath8 *pAudioPathParam) {
    if (!this->pSegment || !this->pPerformance)
        return CO_E_NOTINITIALIZED;
    if (!this->field_14)
        return E_FAIL;
    if (!pAudioPathParam) {
        if (this->pAudioPath)
            pAudioPathParam = this->pAudioPath;
    }
    return this->pPerformance->PlaySegmentEx(
        this->pSegment, 0, 0, dwFlags, 0, nullptr, nullptr, pAudioPathParam
    );
}

HRESULT DirectMusicSegmentWrapper::SetRepeats(const int dwRepeats) const {
    if (this->pSegment)
        return this->pSegment->SetRepeats(dwRepeats);
    return CO_E_NOTINITIALIZED;
}

HRESULT DirectMusicSegmentWrapper::Stop(const int dwFlags) const {
    if (this->pSegment && this->pPerformance)
        return this->pPerformance->Stop(this->pSegment, nullptr, 0, dwFlags);
    return CO_E_NOTINITIALIZED;
}
