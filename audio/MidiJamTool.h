//
// Created by Jacob on 3/18/2026.
//

#ifndef MIDIJAM_MIDIJAMTOOL_H
#define MIDIJAM_MIDIJAMTOOL_H

#include <dmusici.h>
#include <dmplugin.h>
#include "../instruments/instrument_ids.h"

// ---------------------------------------------------------------------------
// MidiJamTool — IDirectMusicTool implementation
// Routes DMUS_PMSGs through IMidiJamTool__ProcessPMsg.
// sizeof = 0x24 (vtable ptr + 8 DWORD fields)
// ---------------------------------------------------------------------------
struct MidiJamTool : IDirectMusicTool
{
    // vtable inherited from IDirectMusicTool

    // IUnknown
    HRESULT __stdcall QueryInterface(REFIID riid, void** ppv) override;
    ULONG __stdcall AddRef() override;
    ULONG __stdcall Release() override;

    // IDirectMusicTool
    HRESULT __stdcall Init(IDirectMusicGraph* pGraph) override;
    HRESULT __stdcall GetMsgDeliveryType(DWORD* pdwDeliveryType) override;
    HRESULT __stdcall GetMediaTypeArraySize(DWORD* pdwNumElements) override;
    HRESULT __stdcall GetMediaTypes(DWORD** padwMediaTypes, DWORD dwNumElements) override;
    HRESULT __stdcall ProcessPMsg(IDirectMusicPerformance* pPerf, DMUS_PMSG* pPMSG) override;
    HRESULT __stdcall Flush(IDirectMusicPerformance* pPerf, DMUS_PMSG* pPMSG, REFERENCE_TIME rtTime) override;

    LONG refCount;
};

HRESULT __stdcall IMidiJamTool__ProcessPMsg(
    int a1,
    IDirectMusicPerformance8* pPerf,
    DMUS_PMSG* pPMSG);

bool IsGmPercussionSupported(GM_PERCUSSION patch);

#endif // MIDIJAM_MIDIJAMTOOL_H
