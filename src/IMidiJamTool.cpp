//
// Created by Jacob on 11/16/2025.
//

#include "IMidiJamTool.h"

HRESULT IMidiJamTool::QueryInterface(const IID &riid, void **ppv) {
    if (IsEqualGUID(riid, IID_IUnknown) || IsEqualGUID(riid, IID_IDirectMusicTool)) {
        *ppv = this;
        AddRef();
        return S_OK;
    }

    *ppv = nullptr;
    return E_NOINTERFACE;
}

ULONG IMidiJamTool::AddRef() {
    return InterlockedIncrement(&refCount);
}

ULONG IMidiJamTool::Release() {
    ULONG r = InterlockedDecrement(&refCount);
    if (r == 0) delete this;
    return r;
}

HRESULT IMidiJamTool::Init(IDirectMusicGraph *pGraph) {
    return E_NOTIMPL;
}

HRESULT IMidiJamTool::GetMsgDeliveryType(DWORD *pdwDeliveryType) {
    *pdwDeliveryType = DMUS_PMSGF_TOOL_IMMEDIATE;
    return S_OK;
}

HRESULT IMidiJamTool::GetMediaTypeArraySize(DWORD *pdwNumElements) {
    *pdwNumElements = 4;
    return S_OK;
}

HRESULT IMidiJamTool::GetMediaTypes(DWORD **padwMediaTypes, DWORD dwNumElements) {
    if (dwNumElements != 4)
        return E_FAIL;
    (*padwMediaTypes)[0] = DMUS_PMSGT_NOTE;
    (*padwMediaTypes)[1] = DMUS_PMSGT_MIDI;
    (*padwMediaTypes)[2] = DMUS_PMSGT_PATCH;
    (*padwMediaTypes)[3] = DMUS_PMSGT_TEMPO;
    return S_OK;
}

HRESULT IMidiJamTool::ProcessPMsg(IDirectMusicPerformance *pPerf, DMUS_PMSG *pPMSG) {
    if ( !pPMSG->pGraph || pPMSG->pGraph->StampPMsg(pPMSG) < 0 )
        return DMUS_S_FREE;
    /* TODO */
    return DMUS_S_REQUEUE;
}

HRESULT IMidiJamTool::Flush(IDirectMusicPerformance *pPerf, DMUS_PMSG *pPMSG, REFERENCE_TIME rtTime) {
    return E_NOTIMPL;
}
