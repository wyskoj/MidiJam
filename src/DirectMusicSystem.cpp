//
// Created by Jacob on 11/16/2025.
//

#define INITGUID
#include "DirectMusicSystem.h"

void __cdecl AnsiToUnicodeImpl(LPWSTR lpWideCharStr, LPCCH lpMultiByteStr, int a3) {
    if (lpWideCharStr && lpMultiByteStr) {
        if (a3 == -1)
            a3 = strlen(lpMultiByteStr) + 1;
        MultiByteToWideChar(0, 0, lpMultiByteStr, -1, lpWideCharStr, a3 - 1);
        lpWideCharStr[a3 - 1] = 0;
    }
}

void AnsiToUnicode(LPWSTR lpWideCharStr, LPCCH lpMultiByteStr, int a3) {
    if (lpWideCharStr) {
        if (lpMultiByteStr)
            AnsiToUnicodeImpl(lpWideCharStr, lpMultiByteStr, a3);
    }
}

DirectMusicSystem::DirectMusicSystem() {
    this->pLoader = nullptr;
    this->pPerformance = nullptr;
    CoInitialize(nullptr);
}

void DirectMusicSystem::CollectLoaderGarbage() const {
    if (this->pLoader) {
        pLoader->CollectGarbage();
    }
}

IDirectMusicAudioPath8 *DirectMusicSystem::GetDefaultAudioPath() const {
    IDirectMusicAudioPath *pAudioPath; // [esp+4h] [ebp-4h] BYREF

    pAudioPath = nullptr;
    if (!this->pPerformance)
        return nullptr;
    pPerformance->GetDefaultAudioPath(&pAudioPath);
    return pAudioPath;
}

IDirectMusicPerformance8 *DirectMusicSystem::GetPerformance() const {
    return this->pPerformance;
}

HRESULT DirectMusicSystem::Init(HWND hWnd, int audioParams1, int audioParams2) {
    int hloaderResult; // [esp+4h] [ebp-4h]
    int hPerformanceResult; // [esp+4h] [ebp-4h]
    HRESULT hInitResult; // [esp+4h] [ebp-4h]

    hloaderResult = CoCreateInstance(CLSID_DirectMusicLoader, nullptr, 3u, IID_IDirectMusicLoader8,
                                     (LPVOID *) &this->pLoader);
    if (hloaderResult < 0)
        return hloaderResult;
    hPerformanceResult = CoCreateInstance(
        CLSID_DirectMusicPerformance,
        nullptr,
        3u,
        IID_IDirectMusicPerformance8,
        (LPVOID *) &this->pPerformance);
    if (hPerformanceResult < 0)
        return hPerformanceResult;

    hInitResult = pPerformance->InitAudio(nullptr, nullptr, hWnd, audioParams2, audioParams1, 63, nullptr);
    if (hInitResult >= 0)
        return 0;

    return hInitResult;
}

HRESULT DirectMusicSystem::LoadDlsCollection(DlsCollectionWrapper **ppCollection, LPCCH dlsFilePath) {
    IDirectMusicScript *pScript; // [esp+14h] [ebp-218h] BYREF
    WCHAR widePath[260]; // [esp+18h] [ebp-214h] BYREF

    pScript = nullptr;
    AnsiToUnicode(widePath, dlsFilePath, -1);
    const HRESULT hlLoadObjectFromFile = pLoader->LoadObjectFromFile(
        CLSID_DirectMusicScript,
        IID_IDirectMusicScript,
        widePath,
        reinterpret_cast<void **>(&pScript));
    if (hlLoadObjectFromFile < 0)
        return hlLoadObjectFromFile;
    // hrScriptInit = (pScript->vtable->Init)(pScript, this->pPerformance, 0);
    const HRESULT hrScriptInit = pScript->Init(this->pPerformance, nullptr);
    if (hrScriptInit < 0)
        return hrScriptInit;
    const auto pDlsCollectionWrapper = new DlsCollectionWrapper(this->pPerformance, this->pLoader, pScript);
    *ppCollection = pDlsCollectionWrapper;
    if (*ppCollection)
        return hrScriptInit;
    return E_OUTOFMEMORY;
}

HRESULT DirectMusicSystem::LoadMidiSegmentFromPath(DirectMusicSegmentWrapper **ppSegmentWrapper, LPCCH midiFilePath,
                                                   BOOL downloadInstruments, BOOL setAsStandardMidiFile) {
    DirectMusicSegmentWrapper *pDirectMusicSegmentWrapper; // [esp+0h] [ebp-22Ch]
    DirectMusicSegmentWrapper *pBlock; // [esp+8h] [ebp-224h]
    HRESULT hrLoadObjectFromFile; // [esp+10h] [ebp-21Ch]
    HRESULT hrSetParam; // [esp+10h] [ebp-21Ch]
    int hrDownload; // [esp+10h] [ebp-21Ch]
    IDirectMusicSegment8 *pDirectMusicSegment; // [esp+14h] [ebp-218h] BYREF
    WCHAR midiFilePathUnicode[260]; // [esp+18h] [ebp-214h] BYREF
    int _; // [esp+228h] [ebp-4h]

    pDirectMusicSegment = nullptr;
    // Convert ANSI path to Unicode for DirectMusic
    AnsiToUnicode(midiFilePathUnicode, midiFilePath, -1);
    // Load the MIDI file as a DirectMusic segment
    hrLoadObjectFromFile = pLoader->LoadObjectFromFile(
        CLSID_DirectMusicSegment,
        IID_IDirectMusicSegment8,
        midiFilePathUnicode,
        reinterpret_cast<void **>(&pDirectMusicSegment));
    if (hrLoadObjectFromFile >= 0) {
        pBlock = static_cast<DirectMusicSegmentWrapper *>(operator new(sizeof(DirectMusicSegmentWrapper)));
        _ = 0;
        if (pBlock)
            pDirectMusicSegmentWrapper = new DirectMusicSegmentWrapper(
                this->pPerformance,
                this->pLoader,
                pDirectMusicSegment);
        else
            pDirectMusicSegmentWrapper = NULL;
        _ = -1;
        *ppSegmentWrapper = pDirectMusicSegmentWrapper;
        if (*ppSegmentWrapper) {
            if (setAsStandardMidiFile
                && (hrSetParam = pDirectMusicSegment->SetParam(
                        GUID_StandardMIDIFile, // rguidType
                        0xFFFFFFFF, // dwGroupBits (0xFFFFFFFF = all groups)
                        0, // dwIndex
                        0, // mtTime (immediate)
                        nullptr), // pParam (no additional params)
                    hrSetParam < 0)) {
                return hrSetParam;
            } else if (downloadInstruments
                       && (hrDownload = (*ppSegmentWrapper)->Download(nullptr), hrDownload < 0)) {
                return hrDownload;
            } else {
                return S_OK;
            }
        } else {
            return E_OUTOFMEMORY;
        }
    } else if (hrLoadObjectFromFile == DMUS_E_LOADER_FAILEDOPEN) {
        return DMUS_E_LOADER_FAILEDOPEN;
    } else {
        return hrLoadObjectFromFile;
    }
}

HRESULT DirectMusicSystem::LoadMidiSegmentFromResource(DirectMusicSegmentWrapper **ppSegmentWrapper, LPCSTR lpName,
                                                       LPCSTR lpType, BOOL downloadInstruments,
                                                       BOOL setAsStandardMidiFile) {
    void *pDirectMusicSegmentWrapper; // [esp+0h] [ebp-380h]
    DirectMusicSegmentWrapper *pWrapperBlock; // [esp+8h] [ebp-378h]
    HRESULT hrGetObject; // [esp+10h] [ebp-370h]
    HRESULT hr; // [esp+10h] [ebp-370h]
    int hrDownload; // [esp+10h] [ebp-370h]
    HRSRC hResInfo; // [esp+14h] [ebp-36Ch]
    IDirectMusicSegment8 *pSegment; // [esp+18h] [ebp-368h] BYREF
    DWORD resourceSize; // [esp+1Ch] [ebp-364h]
    HGLOBAL Resource; // [esp+20h] [ebp-360h]
    DMUS_OBJECTDESC objectDesc; // [esp+24h] [ebp-35Ch] BYREF
    int _; // [esp+37Ch] [ebp-4h]

    pSegment = 0;
    Resource = nullptr;
    resourceSize = 0;
    hResInfo = FindResourceA(nullptr, lpName, lpType);
    if (!hResInfo)
        return E_FAIL;
    Resource = LoadResource(nullptr, hResInfo);
    if (!Resource)
        return E_FAIL;
    resourceSize = SizeofResource(nullptr, hResInfo);
    memset(&objectDesc, 0, sizeof(objectDesc));
    objectDesc.dwSize = 848;
    objectDesc.dwValidData = 0x402;
    objectDesc.guidClass = CLSID_DirectMusicSegment;
    objectDesc.llMemLength = resourceSize;
    objectDesc.pbMemData = nullptr;
    objectDesc.pStream = static_cast<IStream *>(Resource);
    hrGetObject = pLoader->GetObjectA(&objectDesc, IID_IDirectMusicSegment8, reinterpret_cast<void **>(&pSegment));
    if (hrGetObject >= 0) {
        pWrapperBlock = static_cast<DirectMusicSegmentWrapper *>(operator new(sizeof(DirectMusicSegmentWrapper)));
        _ = 0;
        if (pWrapperBlock)
            pDirectMusicSegmentWrapper = new DirectMusicSegmentWrapper(
                this->pPerformance,
                this->pLoader,
                pSegment);
        else
            pDirectMusicSegmentWrapper = nullptr;
        _ = -1;
        *ppSegmentWrapper = static_cast<DirectMusicSegmentWrapper *>(pDirectMusicSegmentWrapper);
        if (*ppSegmentWrapper) {
            if (setAsStandardMidiFile
                && (hr = (pSegment->SetParam)(GUID_StandardMIDIFile, -1, 0, 0, 0), hr < 0)) {
                return hr;
            } else if (downloadInstruments
                       && (hrDownload = (*ppSegmentWrapper)->Download(nullptr), hrDownload < 0)) {
                return hrDownload;
            } else {
                return 0;
            }
        } else {
            return E_OUTOFMEMORY;
        }
    } else if (hrGetObject == DMUS_E_LOADER_FAILEDOPEN) {
        return DMUS_E_LOADER_FAILEDOPEN;
    } else {
        return hrGetObject;
    }
}

HRESULT DirectMusicSystem::LoadStyle(void **ppObject, LPCCH objectFilePath) const {
    WCHAR widePath[260]; // [esp+4h] [ebp-208h] BYREF
    AnsiToUnicode(widePath, objectFilePath, -1);
    return pLoader->LoadObjectFromFile(CLSID_DirectMusicStyle, IID_IDirectMusicStyle8, widePath, ppObject);
}
