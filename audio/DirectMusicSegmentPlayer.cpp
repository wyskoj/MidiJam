//
// Created by Jacob on 3/16/2026.
//

#include "DirectMusicSegmentPlayer.h"

#include <dmusici.h>

// FUNCTION: MIDIJAM 0x401960
DirectMusicSegmentPlayer::DirectMusicSegmentPlayer(IDirectMusicPerformance8* performance, IDirectMusicLoader8* loader,
                                                   IDirectMusicSegment8* segment) {
    this->performance = performance;
    this->loader = loader;
    this->segment = segment;
    this->audioPath = nullptr;
    this->downloaded = false;

    IUnknown* pSourceConfig = nullptr;
    if (SUCCEEDED(this->segment->GetAudioPathConfig(&pSourceConfig))) {
        this->performance->CreateAudioPath(pSourceConfig, TRUE, &this->audioPath);
        if (pSourceConfig) {
            pSourceConfig->Release();
            pSourceConfig = nullptr;
        }
    }
}

// FUNCTION: MIDIJAM 0x401F70
DirectMusicSegmentPlayer::~DirectMusicSegmentPlayer() {
    Cleanup();
}

// FUNCTION: MIDIJAM 0x401B70
HRESULT DirectMusicSegmentPlayer::Download(IUnknown* overrideAudioPath) {
    if (!segment)
        return CO_E_NOTINITIALIZED;

    HRESULT hr;
    if (overrideAudioPath)
        hr = segment->Download(overrideAudioPath);
    else if (this->audioPath)
        hr = segment->Download(this->audioPath);
    else
        hr = segment->Download(performance);

    if (SUCCEEDED(hr))
        downloaded = true;

    return hr;
}

// FUNCTION: MIDIJAM 0x401A10
void DirectMusicSegmentPlayer::Cleanup() {
    if (segment) {
        if (loader)
            loader->ReleaseObjectByUnknown(segment);

        if (downloaded) {
            if (audioPath)
                segment->Unload(audioPath);
            else
                segment->Unload(performance);
        }

        if (audioPath) {
            audioPath->Release();
            audioPath = nullptr;
        }

        // ReSharper disable once CppDFAConstantConditions - Faithful transcription
        if (segment) {
            segment->Release();
            segment = nullptr;
        }
    }

    performance = nullptr;
}

// FUNCTION: MIDIJAM 0x401AF0
HRESULT DirectMusicSegmentPlayer::Play(const int flags, IDirectMusicAudioPath* overrideAudioPath) const {
    if (!segment || !performance)
        return CO_E_NOTINITIALIZED;

    if (!downloaded)
        return E_FAIL;

    if (!overrideAudioPath && this->audioPath)
        overrideAudioPath = this->audioPath;

    return performance->PlaySegmentEx(
        segment,
        nullptr,
        nullptr,
        flags,
        0,
        nullptr,
        nullptr,
        overrideAudioPath
    );
}

// FUNCTION: MIDIJAM 0x401D00
HRESULT DirectMusicSegmentPlayer::Stop(const int flags) const {
    if (segment && performance)
        return performance->Stop(segment, nullptr, 0, flags);

    return CO_E_NOTINITIALIZED;
}

// FUNCTION: MIDIJAM 0x401CB0
BOOL DirectMusicSegmentPlayer::IsPlaying() const {
    if (segment && performance)
        return performance->IsPlaying(segment, nullptr) == S_OK;

    return CO_E_NOTINITIALIZED;
}

// FUNCTION: MIDIJAM 0x401D50
HRESULT DirectMusicSegmentPlayer::SetRepeatCount(const int count) const {
    if (segment)
        return segment->SetRepeats(count);

    return CO_E_NOTINITIALIZED;
}
