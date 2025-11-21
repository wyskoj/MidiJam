//
// Created by Jacob on 11/16/2025.
//

#include "IMidiJamTool.h"

#include <iostream>

#include "globals.h"
#include "instrument/Accordion.h"
#include "instrument/Piano.h"

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
    DMUS_NOTE_PMSG *msg;
    LPVOID *QueryInterfaceResult = nullptr;
    unsigned __int16 keyboardKeyIndex;
    __int16 slot;
    __int16 slot_a;
    BYTE v165[12];
    REFERENCE_TIME rtNow;
    MUSIC_TIME mtNow;
    REFERENCE_TIME rtNow_1;
    MUSIC_TIME mtNow_1;
    unsigned __int16 wrappedNoteValue;

    if (!pPMSG->pGraph || pPMSG->pGraph->StampPMsg(pPMSG) < 0)
        return DMUS_S_FREE;
    if (pPerf->QueryInterface(IID_IDirectMusicPerformance8, QueryInterfaceResult) >= 0 || true) {
        switch (pPMSG->dwType) {
            case DMUS_PMSGT_NOTE:
                msg = reinterpret_cast<DMUS_NOTE_PMSG *>(pPMSG);
                if (reinterpret_cast<DMUS_NOTE_PMSG *>(pPMSG)->bFlags & DMUS_NOTEF_NOTEON) {
                    switch (g_midiJamInstrumentIds[msg->dwPChannel]) {
                        case PIANO:
                            keyboardKeyIndex = msg->wMusicValue - 21;
                            if (keyboardKeyIndex < 88u) {
                                for (slot = 0;
                                     g_ds_piano[g_piano_assignment[msg->dwPChannel]].queueDurations[
                                         keyboardKeyIndex][slot]
                                     && slot < 16;
                                     ++slot) {
                                    ;
                                }
                                if (slot < 16) {
                                    g_ds_piano[g_piano_assignment[msg->dwPChannel]].queueDurations[
                                        keyboardKeyIndex][slot] = msg->mtDuration;
                                    g_ds_piano[g_piano_assignment[msg->dwPChannel]].velocities[keyboardKeyIndex][
                                        slot] = msg->bVelocity;
                                    g_ds_piano[g_piano_assignment[msg->dwPChannel]].queueDurations[
                                                keyboardKeyIndex][slot] =
                                            g_ds_piano[g_piano_assignment[msg->dwPChannel]].queueDurations[
                                                keyboardKeyIndex][slot] - g_currentTempo_scaleFactor0_5;
                                    if (g_ds_piano[g_piano_assignment[msg->dwPChannel]].queueDurations[
                                            keyboardKeyIndex][slot] < 0)
                                        g_ds_piano[g_piano_assignment[msg->dwPChannel]].queueDurations[
                                            keyboardKeyIndex][slot] = 10;
                                    pPerf->GetTime(&rtNow, &mtNow);
                                    g_ds_piano[g_piano_assignment[msg->dwPChannel]].timeDeltas[keyboardKeyIndex][
                                        slot] = msg->mtTime - mtNow;
                                    g_ds_piano[g_piano_assignment[msg->dwPChannel]].timeDeltas[keyboardKeyIndex][
                                        slot] -= g_currentTempo_scaleFactor0_9;
                                    if (g_ds_piano[g_piano_assignment[msg->dwPChannel]].timeDeltas[
                                            keyboardKeyIndex][slot] <= 0)
                                        g_ds_piano[g_piano_assignment[msg->dwPChannel]].timeDeltas[
                                            keyboardKeyIndex][slot] = 1;
                                }
                            }
                            break;
                        case ACCORDION:
                            wrappedNoteValue = (msg->wMusicValue + 12) % 24 + 1;
                            for (slot_a = 0;
                                 g_ds_accordion[g_accordion_assignment[msg->dwPChannel]].queueDurations[
                                     wrappedNoteValue][slot_a]
                                 && slot_a < 16;
                                 ++slot_a) {
                                ;
                            }
                            if (slot_a < 16) {
                                g_ds_accordion[g_accordion_assignment[msg->dwPChannel]].queueDurations[
                                    wrappedNoteValue][slot_a] = msg->mtDuration;
                                g_ds_accordion[g_accordion_assignment[msg->dwPChannel]].queueDurations[
                                            wrappedNoteValue][slot_a] =
                                        g_ds_accordion[g_accordion_assignment[msg->dwPChannel]].queueDurations[
                                            wrappedNoteValue][slot_a] - g_currentTempo_scaleFactor0_5;
                                if (g_ds_accordion[g_accordion_assignment[msg->dwPChannel]].queueDurations[
                                        wrappedNoteValue][slot_a] < 0)
                                    g_ds_accordion[g_accordion_assignment[msg->dwPChannel]].queueDurations[
                                        wrappedNoteValue][slot_a] = 10;
                                pPerf->GetTime(&rtNow_1, &mtNow_1);
                                g_ds_accordion[g_accordion_assignment[msg->dwPChannel]].timeDeltas[
                                    wrappedNoteValue][slot_a] = msg->mtTime - mtNow_1;
                                g_ds_accordion[g_accordion_assignment[msg->dwPChannel]].timeDeltas[
                                    wrappedNoteValue][slot_a] -= g_currentTempo_scaleFactor0_9;
                                if (g_ds_accordion[g_accordion_assignment[msg->dwPChannel]].timeDeltas[
                                        wrappedNoteValue][slot_a] <= 0)
                                    g_ds_accordion[g_accordion_assignment[msg->dwPChannel]].timeDeltas[
                                        wrappedNoteValue][slot_a] = 1;
                            }
                            break;
                    }
                }
                break;
            case DMUS_PMSGT_PATCH:
                if (pPMSG->dwPChannel <= 999) {
                    if (pPMSG->dwPChannel == 9) {
                        g_midiJamInstrumentIds[pPMSG->dwPChannel] = PERCUSSION;
                    } else if (reinterpret_cast<DMUS_PATCH_PMSG *>(pPMSG)->byInstrument <= 127u) {
                        g_midiJamInstrumentIds[pPMSG->dwPChannel] = MIDIJAM_INSTRUMENT_IDS[reinterpret_cast<
                            DMUS_PATCH_PMSG *>(pPMSG)->byInstrument];
                        switch (g_midiJamInstrumentIds[pPMSG->dwPChannel]) {
                            case PIANO:
                                if (g_ds_piano) {
                                    g_ds_piano = static_cast<I_DS_Piano *>(realloc(
                                        g_ds_piano, sizeof(I_DS_Piano) * (g_ialloc_piano + 1)));
                                    memset(&g_ds_piano[g_ialloc_piano], 0, sizeof(I_DS_Piano));
                                } else {
                                    g_ds_piano = static_cast<I_DS_Piano *>(malloc(sizeof(I_DS_Piano)));
                                    memset(g_ds_piano, 0, sizeof(I_DS_Piano));
                                }
                                switch (g_midiJamInstrumentIds[pPMSG->dwPChannel]) {
                                    case 40:
                                        g_ds_piano[g_ialloc_piano].materialIndex = 1;
                                        break;
                                    case 44:
                                        g_ds_piano[g_ialloc_piano].materialIndex = 2;
                                        break;
                                    case 45:
                                        g_ds_piano[g_ialloc_piano].materialIndex = 3;
                                        break;
                                }
                                g_piano_assignment[pPMSG->dwPChannel] = g_ialloc_piano++;
                                break;
                            case ACCORDION:
                                if (g_ds_accordion) {
                                    g_ds_accordion = static_cast<I_DS_Accordion *>(realloc(
                                        g_ds_accordion, sizeof(I_DS_Accordion) * (g_ialloc_accordion + 1)));
                                    memset(&g_ds_accordion[g_ialloc_accordion], 0, sizeof(I_DS_Accordion));
                                    g_ds_accordion[g_ialloc_accordion].squeezeAngle = 4.0;
                                } else {
                                    g_ds_accordion = static_cast<I_DS_Accordion *>(malloc(sizeof(I_DS_Accordion)));
                                    memset(g_ds_accordion, 0, sizeof(I_DS_Accordion));
                                    g_ds_accordion->squeezeAngle = 4.0;
                                }
                                g_accordion_assignment[pPMSG->dwPChannel] = g_ialloc_accordion++;
                                break;
                        }
                    }
                }
                break;
            case DMUS_PMSGT_TEMPO:
                g_currentTempo = reinterpret_cast<DMUS_TEMPO_PMSG *>(pPMSG)->dblTempo;
                g_currentTempo_scaleFactor0_5 = (g_currentTempo * 0.5);
                g_currentTempo_scaleFactor0_9 = (g_currentTempo * 0.8999999761581421);
                g_currentTempo_scaleFactor1_15 = (g_currentTempo * 1.149999976158142);
                break;
        }
    }
    /* TODO */
    return DMUS_S_REQUEUE;
}

HRESULT IMidiJamTool::Flush(IDirectMusicPerformance *pPerf, DMUS_PMSG *pPMSG, REFERENCE_TIME rtTime) {
    return E_NOTIMPL;
}
