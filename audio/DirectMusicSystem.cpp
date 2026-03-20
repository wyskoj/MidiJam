//
// Created by Jacob on 3/16/2026.
//

#define INITGUID
#include "DirectMusicSystem.h"

#include "DirectMusicSegmentWrapper.h"
#include "DlsCollectionWrapper.h"

#include <dmusici.h>
#include <cstring>

// Defined in dmusicf.h which is not available.
// Value taken from DirectX SDK documentation.
#ifndef DMUS_E_LOADER_FAILEDOPEN
#define DMUS_E_LOADER_FAILEDOPEN 0x80044004
#endif

void AnsiToUnicode(LPWSTR lpWideCharStr, LPCCH lpMultiByteStr, int a3);

// FUNCTION: MIDIJAM 0x401000
DirectMusicSystem::DirectMusicSystem()
{
    pLoader = nullptr;
    pPerformance = nullptr;
    CoInitialize(nullptr);
}

// FUNCTION: MIDIJAM 0x4010C0
HRESULT DirectMusicSystem::Init(const HWND hWnd, const int audioParams1, const int audioParams2)
{
    HRESULT hr = CoCreateInstance(
        CLSID_DirectMusicLoader,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IDirectMusicLoader8,
        reinterpret_cast<void**>(&pLoader));
    if (FAILED(hr))
        return hr;

    hr = CoCreateInstance(
        CLSID_DirectMusicPerformance,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IDirectMusicPerformance8,
        reinterpret_cast<void**>(&pPerformance));
    if (FAILED(hr))
        return hr;

    hr = pPerformance->InitAudio(
        nullptr,
        nullptr,
        hWnd,
        audioParams2,
        audioParams1,
        DMUS_AUDIOF_ALL,
        nullptr);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

// FUNCTION: MIDIJAM 0x438EA0
IDirectMusicPerformance8* DirectMusicSystem::GetPerformance() const
{
    return pPerformance;
}

// FUNCTION: MIDIJAM 0x401160
IDirectMusicAudioPath* DirectMusicSystem::GetDefaultAudioPath() const
{
    IDirectMusicAudioPath* pAudioPath = nullptr;
    if (!pPerformance)
        return nullptr;
    pPerformance->GetDefaultAudioPath(&pAudioPath);
    return pAudioPath;
}

// FUNCTION: MIDIJAM 0x4011A0
void DirectMusicSystem::CollectLoaderGarbage() const
{
    if (pLoader)
    {
        pLoader->CollectGarbage();
    }
}

// FUNCTION: MIDIJAM 0x401260
HRESULT DirectMusicSystem::LoadMidiSegmentFromPath(
    DirectMusicSegmentWrapper** ppSegmentWrapper,
    const LPCCH midiFilePath,
    const BOOL downloadInstruments,
    const BOOL setAsStandardMidiFile) const
{
    IDirectMusicSegment8* pSegment = nullptr;
    WCHAR midiFilePathUnicode[260];

    AnsiToUnicode(midiFilePathUnicode, midiFilePath, -1);

    HRESULT hr = pLoader->LoadObjectFromFile(
        CLSID_DirectMusicSegment,
        IID_IDirectMusicSegment8,
        midiFilePathUnicode,
        reinterpret_cast<void**>(&pSegment));

    if (SUCCEEDED(hr))
    {
        DirectMusicSegmentWrapper* pBlock = new DirectMusicSegmentWrapper(
            pPerformance,
            pLoader,
            pSegment);

        *ppSegmentWrapper = pBlock;

        if (*ppSegmentWrapper)
        {
            if (setAsStandardMidiFile)
            {
                hr = pSegment->SetParam(
                    GUID_StandardMIDIFile,
                    0xFFFFFFFF,
                    0,
                    0,
                    nullptr);
                if (FAILED(hr))
                    return hr;
            }

            if (downloadInstruments)
            {
                hr = (*ppSegmentWrapper)->Download(nullptr);
                if (FAILED(hr))
                    return hr;
            }

            return S_OK;
        }
        else
        {
            return E_OUTOFMEMORY;
        }
    }
    else if (hr == static_cast<HRESULT>(DMUS_E_LOADER_FAILEDOPEN))
    {
        return static_cast<HRESULT>(DMUS_E_LOADER_FAILEDOPEN);
    }
    else
    {
        return hr;
    }
}

// FUNCTION: MIDIJAM 0x401400
HRESULT DirectMusicSystem::LoadMidiSegmentFromResource(
    DirectMusicSegmentWrapper** ppSegmentWrapper,
    const LPCSTR lpName,
    const LPCSTR lpType,
    const BOOL downloadInstruments,
    const BOOL setAsStandardMidiFile) const
{
    IDirectMusicSegment8* pSegment = nullptr;

    HRSRC hResInfo = FindResourceA(nullptr, lpName, lpType);
    if (!hResInfo)
        return E_FAIL;

    HGLOBAL hResource = LoadResource(nullptr, hResInfo);
    if (!hResource)
        return E_FAIL;

    DWORD resourceSize = SizeofResource(nullptr, hResInfo);

    DMUS_OBJECTDESC objectDesc = {};
    objectDesc.dwSize = sizeof(DMUS_OBJECTDESC);
    objectDesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_MEMORY;
    objectDesc.guidClass = CLSID_DirectMusicSegment;
    objectDesc.llMemLength = static_cast<LONGLONG>(resourceSize);
    objectDesc.pbMemData = static_cast<BYTE*>(LockResource(hResource));
    objectDesc.pStream = nullptr;

    HRESULT hr = pLoader->GetObject(
        &objectDesc,
        IID_IDirectMusicSegment8,
        reinterpret_cast<void**>(&pSegment));

    if (SUCCEEDED(hr))
    {
        DirectMusicSegmentWrapper* pBlock = new DirectMusicSegmentWrapper(
            pPerformance,
            pLoader,
            pSegment);

        *ppSegmentWrapper = pBlock;

        if (*ppSegmentWrapper)
        {
            if (setAsStandardMidiFile)
            {
                hr = pSegment->SetParam(
                    GUID_StandardMIDIFile,
                    static_cast<DWORD>(-1),
                    0,
                    0,
                    nullptr);
                if (FAILED(hr))
                    return hr;
            }

            if (downloadInstruments)
            {
                hr = (*ppSegmentWrapper)->Download(nullptr);
                if (FAILED(hr))
                    return hr;
            }

            return S_OK;
        }
        else
        {
            return E_OUTOFMEMORY;
        }
    }
    else if (hr == static_cast<HRESULT>(DMUS_E_LOADER_FAILEDOPEN))
    {
        return static_cast<HRESULT>(DMUS_E_LOADER_FAILEDOPEN);
    }
    else
    {
        return hr;
    }
}

// FUNCTION: MIDIJAM 0x401680
// TODO: verify function name — body loads a IDirectMusicScript, not a DLS collection.
HRESULT DirectMusicSystem::LoadDlsCollection(
    DlsCollectionWrapper** ppCollection,
    const LPCCH dlsFilePath) const
{
    IDirectMusicScript* pScript = nullptr;
    WCHAR widePath[260];

    AnsiToUnicode(widePath, dlsFilePath, -1);

    HRESULT hr = pLoader->LoadObjectFromFile(
        CLSID_DirectMusicScript,
        IID_IDirectMusicScript,
        widePath,
        reinterpret_cast<void**>(&pScript));
    if (FAILED(hr))
        return hr;

    hr = pScript->Init(pPerformance, nullptr);
    if (FAILED(hr))
        return hr;

    *ppCollection = new DlsCollectionWrapper(pPerformance, pLoader, pScript);
    if (!*ppCollection)
        return E_OUTOFMEMORY;

    return S_OK;
}

// FUNCTION: MIDIJAM 0x401840
// TODO: verify function name — may load other object types besides styles.
HRESULT DirectMusicSystem::LoadStyle(void** ppObject, const LPCCH objectFilePath) const
{
    WCHAR widePath[260];
    AnsiToUnicode(widePath, objectFilePath, -1);
    return pLoader->LoadObjectFromFile(
        CLSID_DirectMusicStyle,
        IID_IDirectMusicStyle8,
        widePath,
        ppObject);
}
