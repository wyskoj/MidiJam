//
// Created by Jacob on 11/16/2025.
//

#include "IMidiJamTool.h"

#include <iostream>

#include "globals.h"
#include "instrument/Accordion.h"
#include "instrument/Bass.h"
#include "instrument/Piano.h"
#include "instrument/StageHorn.h"
#include "instrument/StageString.h"

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
    unsigned __int16 v99;
    unsigned __int16 v140;
    unsigned __int16 v148;
    unsigned __int16 v135;
    __int16 n;
    __int16 ii;
    __int16 i5;
    __int16 jj;

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
                        case BASS:
                            v148 = msg->wMusicValue - 21;
                            if (v148 < 0x58u) {
                                for (n = 0; g_ds_bass[g_bass_assignment[msg->dwPChannel]].field_2CC[v148][n] && n <
                                            16; ++n);
                                if (n < 16) {
                                    g_ds_bass[g_bass_assignment[msg->dwPChannel]].field_2CC[v148][n] = msg->mtDuration;
                                    g_ds_bass[g_bass_assignment[msg->dwPChannel]].field_2CC[v148][n] =
                                            g_ds_bass[g_bass_assignment[msg->dwPChannel]].field_2CC[v148][n]
                                            - g_currentTempo_scaleFactor0_5;
                                    if (g_ds_bass[g_bass_assignment[msg->dwPChannel]].field_2CC[v148][n] < 0)
                                        g_ds_bass[g_bass_assignment[msg->dwPChannel]].field_2CC[v148][n] = 10;
                                    pPerf->GetTime(&rtNow, &mtNow);
                                    g_ds_bass[g_bass_assignment[msg->dwPChannel]].field_18CC[v148][n] =
                                            msg->mtTime - mtNow;
                                    g_ds_bass[g_bass_assignment[msg->dwPChannel]].field_18CC[v148][n] -=
                                            g_currentTempo_scaleFactor0_9;
                                    if (g_ds_bass[g_bass_assignment[msg->dwPChannel]].field_18CC[v148][n] <= 0)
                                        g_ds_bass[g_bass_assignment[msg->dwPChannel]].field_18CC[v148][n] = 1;
                                }
                            }
                            break;
                        case XYLOPHONE:
                            v140 = msg->wMusicValue - 21;
                            if (v140 < 0x58u) {
                                for (ii = 0;
                                     g_ds_xylophone[g_xylophone_assignment[msg->dwPChannel]].field_2C6[v140][ii] && ii <
                                     16;
                                     ++ii) {
                                    ;
                                }
                                if (ii < 16) {
                                    g_ds_xylophone[g_xylophone_assignment[msg->dwPChannel]].field_2C6[v140][ii] = msg->
                                            mtDuration;
                                    g_ds_xylophone[g_xylophone_assignment[msg->dwPChannel]].field_18C6[v140][ii] = msg->
                                            bVelocity;
                                    g_ds_xylophone[g_xylophone_assignment[msg->dwPChannel]].field_2C6[v140][ii] =
                                            g_ds_xylophone[g_xylophone_assignment[msg->dwPChannel]].field_2C6[v140][ii]
                                            - g_currentTempo_scaleFactor0_5;
                                    if (g_ds_xylophone[g_xylophone_assignment[msg->dwPChannel]].field_2C6[v140][ii] < 0)
                                        g_ds_xylophone[g_xylophone_assignment[msg->dwPChannel]].field_2C6[v140][ii] =
                                                10;
                                    (pPerf->GetTime)(&rtNow, &mtNow);
                                    g_ds_xylophone[g_xylophone_assignment[msg->dwPChannel]].queue[v140][ii] =
                                            msg->mtTime - mtNow;
                                    g_ds_xylophone[g_xylophone_assignment[msg->dwPChannel]].queue[v140][ii] -=
                                            g_currentTempo_scaleFactor0_9;
                                    if (g_ds_xylophone[g_xylophone_assignment[msg->dwPChannel]].queue[v140][ii] <= 0)
                                        g_ds_xylophone[g_xylophone_assignment[msg->dwPChannel]].queue[v140][ii] = 1;
                                }
                            }
                            break;
                        case STAGE_HORN:
                            v135 = (msg->wMusicValue + 3) % 12;
                            for ( jj = 0; g_ds_stageHorn[g_stageHorn_assignment[msg->dwPChannel]].field_64[v135][jj] && jj < 16; ++jj )
                                ;
                            if ( jj < 16 )
                            {
                                g_ds_stageHorn[g_stageHorn_assignment[msg->dwPChannel]].field_64[v135][jj] = msg->mtDuration;
                                if ( g_ds_stageHorn[g_stageHorn_assignment[msg->dwPChannel]].field_64[v135][jj] < 0 )
                                    g_ds_stageHorn[g_stageHorn_assignment[msg->dwPChannel]].field_64[v135][jj] = 10;
                                (pPerf->GetTime)(&rtNow, &mtNow);
                                g_ds_stageHorn[g_stageHorn_assignment[msg->dwPChannel]].field_364[v135][jj] = msg->mtTime
                                                                                                                 - mtNow;
                                g_ds_stageHorn[g_stageHorn_assignment[msg->dwPChannel]].field_364[v135][jj] -= g_currentTempo_scaleFactor0_9;
                                if ( g_ds_stageHorn[g_stageHorn_assignment[msg->dwPChannel]].field_364[v135][jj] <= 0 )
                                    g_ds_stageHorn[g_stageHorn_assignment[msg->dwPChannel]].field_364[v135][jj] = 1;
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
                        case STAGE_STRINGS:
                            v99 = (msg->wMusicValue + 3) % 12;
                            for ( i5 = 0;
                                  g_ds_stageString[g_stageString_assignment[msg->dwPChannel]].field_64[v99][i5] && i5 < 16;
                                  ++i5 )
                            {
                                ;
                            }
                            if ( i5 < 16 )
                            {
                                g_ds_stageString[g_stageString_assignment[msg->dwPChannel]].field_64[v99][i5] = msg->mtDuration;
                                if ( g_ds_stageString[g_stageString_assignment[msg->dwPChannel]].field_64[v99][i5] < 0 )
                                    g_ds_stageString[g_stageString_assignment[msg->dwPChannel]].field_64[v99][i5] = 10;
                                pPerf->GetTime(&rtNow, &mtNow);
                                g_ds_stageString[g_stageString_assignment[msg->dwPChannel]].field_364[v99][i5] = msg->mtTime - mtNow;
                                g_ds_stageString[g_stageString_assignment[msg->dwPChannel]].field_364[v99][i5] -= g_currentTempo_scaleFactor0_9;
                                if ( g_ds_stageString[g_stageString_assignment[msg->dwPChannel]].field_364[v99][i5] <= 0 )
                                    g_ds_stageString[g_stageString_assignment[msg->dwPChannel]].field_364[v99][i5] = 1;
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
                            case BASS:
                                if (g_ds_bass) {
                                    g_ds_bass = static_cast<I_DS_Bass *>(realloc(
                                        g_ds_bass, sizeof(I_DS_Bass) * (g_ialloc_bass + 1)));
                                    memset(&g_ds_bass[g_ialloc_bass], 0, sizeof(I_DS_Bass));
                                } else {
                                    g_ds_bass = static_cast<I_DS_Bass *>(malloc(sizeof(I_DS_Bass)));
                                    memset(g_ds_bass, 0, sizeof(I_DS_Bass));
                                }
                                g_bass_assignment[pPMSG->dwPChannel] = g_ialloc_bass++;
                                break;
                            case XYLOPHONE:
                                if (g_ds_xylophone) {
                                    g_ds_xylophone = static_cast<I_DS_Xylophone *>(realloc(g_ds_xylophone, sizeof(I_DS_Xylophone) * (g_ialloc_xylophone + 1)));
                                    memset(&g_ds_xylophone[g_ialloc_xylophone], 0, sizeof(I_DS_Xylophone));
                                } else {
                                    g_ds_xylophone = static_cast<I_DS_Xylophone *>(malloc(sizeof(I_DS_Xylophone)));
                                    memset(g_ds_xylophone, 0, sizeof(I_DS_Xylophone));
                                }
                                g_xylophone_assignment[pPMSG->dwPChannel] = g_ialloc_xylophone;
                                switch (reinterpret_cast<DMUS_PATCH_PMSG *>(pPMSG)->byInstrument) {
                                    case 9:
                                        g_xylophone_types[g_ialloc_xylophone] = 1;
                                        break;
                                    case 11:
                                        g_xylophone_types[g_ialloc_xylophone] = 2;
                                        break;
                                    case 12:
                                        g_xylophone_types[g_ialloc_xylophone] = 3;
                                        break;
                                    default:
                                        g_xylophone_types[g_ialloc_xylophone] = 0;
                                        break;
                                }
                                ++g_ialloc_xylophone;
                                break;
                            case STAGE_HORN:
                                if ( g_ds_stageHorn )
                                {
                                    g_ds_stageHorn = static_cast<I_DS_StageHorn *>(realloc(g_ds_stageHorn, sizeof(I_DS_StageHorn) * (g_ialloc_stageHorn + 1)));
                                    memset(&g_ds_stageHorn[g_ialloc_stageHorn], 0, sizeof(I_DS_StageHorn));
                                }
                                else
                                {
                                    g_ds_stageHorn = static_cast<I_DS_StageHorn *>(malloc(sizeof(I_DS_StageHorn)));
                                    memset(g_ds_stageHorn, 0, sizeof(I_DS_StageHorn));
                                }
                                g_stageHorn_assignment[pPMSG->dwPChannel] = g_ialloc_stageHorn++;
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
                            case STAGE_STRINGS:
                                if ( g_ds_stageString )
                                {
                                    g_ds_stageString = static_cast<I_DS_StageString *>(realloc(g_ds_stageString, sizeof(I_DS_StageString) * (g_ialloc_stageString + 1)));
                                    memset(&g_ds_stageString[g_ialloc_stageString], 0, sizeof(I_DS_StageString));
                                }
                                else
                                {
                                    g_ds_stageString = static_cast<I_DS_StageString *>(malloc(sizeof(I_DS_StageString)));
                                    memset(g_ds_stageString, 0, sizeof(I_DS_StageString));
                                }
                                g_stageString_assignment[pPMSG->dwPChannel] = g_ialloc_stageString++;
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
