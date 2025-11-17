//
// Created by Jacob on 11/16/2025.
//

#ifndef MIDIJAM_IMIDIJAMTOOL_H
#define MIDIJAM_IMIDIJAMTOOL_H
#include <atlbase.h>
#include <atlcom.h>
#include "dmplugin.h"


// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
// This is intentionally NOT IDirectMusicTool8
class IMidiJamTool : public IDirectMusicTool {
    LONG refCount = 1;
public:
    HRESULT __stdcall QueryInterface(REFIID riid, void **ppv) override;
    ULONG __stdcall AddRef() override;
    ULONG __stdcall Release() override;

    HRESULT __stdcall Init(IDirectMusicGraph *pGraph) override;

    HRESULT __stdcall GetMsgDeliveryType(DWORD *pdwDeliveryType) override;

    HRESULT __stdcall GetMediaTypeArraySize(DWORD *pdwNumElements) override;

    HRESULT __stdcall GetMediaTypes(DWORD **padwMediaTypes, DWORD dwNumElements) override;

    HRESULT __stdcall ProcessPMsg(IDirectMusicPerformance *pPerf, DMUS_PMSG *pPMSG) override;

    HRESULT __stdcall Flush(IDirectMusicPerformance *pPerf, DMUS_PMSG *pPMSG, REFERENCE_TIME rtTime) override;
};


#endif //MIDIJAM_IMIDIJAMTOOL_H
