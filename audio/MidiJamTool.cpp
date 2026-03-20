//
// Created by Jacob on 3/17/2026.
//

#include "MidiJamTool.h"
#include "../instruments/Piano.h"

#include <cstring>
#include <cstdlib>

extern MidiJamInstrumentId g_midiJamInstrumentIds[1000];
extern I_DS_Piano* g_ds_piano;
extern short g_ialloc_piano;
extern short g_piano_assignment[1000];
extern int g_currentTempo_scaleFactor0_5;
extern int g_currentTempo_scaleFactor0_9;
extern int g_currentTempo_scaleFactor1_15;
extern double g_currentTempo;

ULONG __stdcall MidiJamTool::AddRef() { return InterlockedIncrement(&refCount); }

HRESULT __stdcall MidiJamTool::Flush(IDirectMusicPerformance*, DMUS_PMSG*, REFERENCE_TIME) { return E_NOTIMPL; }

HRESULT __stdcall MidiJamTool::GetMediaTypeArraySize(DWORD* pdwNumElements)
{
    *pdwNumElements = 4;
    return S_OK;
}

HRESULT __stdcall MidiJamTool::GetMediaTypes(DWORD** padwMediaTypes, const DWORD dwNumElements)
{
    if (dwNumElements != 4)
        return E_FAIL;
    (*padwMediaTypes)[0] = DMUS_PMSGT_NOTE;
    (*padwMediaTypes)[1] = DMUS_PMSGT_MIDI;
    (*padwMediaTypes)[2] = DMUS_PMSGT_PATCH;
    (*padwMediaTypes)[3] = DMUS_PMSGT_TEMPO;
    return S_OK;
}

HRESULT __stdcall MidiJamTool::GetMsgDeliveryType(DWORD* pdwDeliveryType)
{
    *pdwDeliveryType = DMUS_PMSGF_TOOL_IMMEDIATE;
    return S_OK;
}

HRESULT __stdcall MidiJamTool::Init(IDirectMusicGraph*) { return E_NOTIMPL; }

HRESULT __stdcall MidiJamTool::QueryInterface(REFIID riid, void** ppv)
{
    if (IsEqualGUID(riid, IID_IUnknown) || IsEqualGUID(riid, IID_IDirectMusicTool))
    {
        *ppv = this;
        AddRef();
        return S_OK;
    }

    *ppv = nullptr;
    return E_NOINTERFACE;
}

ULONG __stdcall MidiJamTool::Release()
{
    if (InterlockedDecrement(&refCount))
        return refCount;
    if (this)
        delete this;
    return 0;
}

HRESULT __stdcall MidiJamTool::ProcessPMsg(IDirectMusicPerformance* pPerf, DMUS_PMSG* pPMSG)
{
    REFERENCE_TIME rtNow;
    MUSIC_TIME mtNow;
    LPVOID* QueryInterfaceResult = nullptr;

    if (!pPMSG->pGraph
        || pPMSG->pGraph->StampPMsg(pPMSG) < 0)
        return DMUS_S_FREE;

    if (pPerf->QueryInterface(IID_IDirectMusicPerformance8,
                              reinterpret_cast<void**>(&QueryInterfaceResult)) < 0)
        return DMUS_S_REQUEUE;

    switch (pPMSG->dwType)
    {
    case DMUS_PMSGT_NOTE:
    {
        const DMUS_NOTE_PMSG* noteMsg = reinterpret_cast<DMUS_NOTE_PMSG*>(pPMSG);
        const MUSIC_TIME msgMtTime = pPMSG->mtTime;
        const DWORD msgChannel = pPMSG->dwPChannel;

        if (!noteMsg->bVelocity || msgChannel > 999)
            break;

        switch (g_midiJamInstrumentIds[msgChannel])
        {
        case PIANO:
        case ELECTRIC_PIANO:
        case HONKY_TONK_PIANO:
        case HARPSICHORD:
        {
            const unsigned short keyboardKeyIndex = noteMsg->wMusicValue - 21;
            if (keyboardKeyIndex >= 88u)
                break;

            short slot = 0;
            const int pi = g_piano_assignment[msgChannel];
            while (g_ds_piano[pi].queueDurations[keyboardKeyIndex][slot]
                && slot < 16)
                ++slot;

            if (slot >= 16)
                break;

            g_ds_piano[pi].queueDurations[keyboardKeyIndex][slot] =
                noteMsg->mtDuration - g_currentTempo_scaleFactor0_5;
            if (g_ds_piano[pi].queueDurations[keyboardKeyIndex][slot] < 0)
                g_ds_piano[pi].queueDurations[keyboardKeyIndex][slot] = 10;

            g_ds_piano[pi].velocities[keyboardKeyIndex][slot] =
                noteMsg->bVelocity;

            pPerf->GetTime(&rtNow, &mtNow);
            g_ds_piano[pi].timeDeltas[keyboardKeyIndex][slot] =
                static_cast<int16_t>(msgMtTime - mtNow)
                - static_cast<int16_t>(g_currentTempo_scaleFactor0_9);
            if (g_ds_piano[pi].timeDeltas[keyboardKeyIndex][slot] <= 0)
                g_ds_piano[pi].timeDeltas[keyboardKeyIndex][slot] = 1;
            break;
        }

        default:
            break;
        }
        break;
    }

    case DMUS_PMSGT_PATCH:
    {
        if (pPMSG->dwPChannel > 999)
            break;

        const DMUS_PATCH_PMSG* patchMsg = reinterpret_cast<DMUS_PATCH_PMSG*>(pPMSG);

        if (pPMSG->dwPChannel == 9)
        {
            g_midiJamInstrumentIds[pPMSG->dwPChannel] = PERCUSSION;
            break;
        }

        if (patchMsg->byInstrument > 127u)
            break;

        g_midiJamInstrumentIds[pPMSG->dwPChannel] =
            static_cast<MidiJamInstrumentId>(
                MIDIJAM_INSTRUMENT_IDS[patchMsg->byInstrument]);

        switch (g_midiJamInstrumentIds[pPMSG->dwPChannel])
        {
        case PIANO:
        case ELECTRIC_PIANO:
        case HONKY_TONK_PIANO:
        case HARPSICHORD:
        {
            if (g_ds_piano)
                g_ds_piano = static_cast<I_DS_Piano*>(
                    realloc(g_ds_piano, sizeof(I_DS_Piano) * (g_ialloc_piano + 1)));
            else
                g_ds_piano = static_cast<I_DS_Piano*>(
                    malloc(sizeof(I_DS_Piano)));
            memset(&g_ds_piano[g_ialloc_piano], 0, sizeof(I_DS_Piano));

            switch (g_midiJamInstrumentIds[pPMSG->dwPChannel])
            {
            case (MidiJamInstrumentId)40:
                g_ds_piano[g_ialloc_piano].materialIndex = 1;
                break;
            case (MidiJamInstrumentId)44:
                g_ds_piano[g_ialloc_piano].materialIndex = 2;
                break;
            case (MidiJamInstrumentId)45:
                g_ds_piano[g_ialloc_piano].materialIndex = 3;
                break;
            default:
                break;
            }
            g_piano_assignment[pPMSG->dwPChannel] = g_ialloc_piano++;
            break;
        }

        default:
            break;
        }
        break;
    }

    case DMUS_PMSGT_TEMPO:
    {
        g_currentTempo = reinterpret_cast<DMUS_TEMPO_PMSG*>(pPMSG)->dblTempo;
        g_currentTempo_scaleFactor0_5 = static_cast<int>(g_currentTempo * 0.5);
        g_currentTempo_scaleFactor0_9 = static_cast<int>(g_currentTempo * 0.8999999761581421);
        g_currentTempo_scaleFactor1_15 = static_cast<int>(g_currentTempo * 1.149999976158142);
        break;
    }
    }

    return DMUS_S_REQUEUE;
}

// FUNCTION: MIDIJAM 0x43C2D0
bool IsGmPercussionSupported(const GM_PERCUSSION patch)
{
    switch (patch)
    {
    case ACOUSTIC_SNARE:
    case ELECTRIC_SNARE:
    case SIDE_STICK:
    case LOW_MID_TOM:
    case LOW_TOM:
    case HIGH_FLOOR_TOM:
    case ACOUSTIC_BASS_DRUM:
    case ELECTRIC_BASS_DRUM:
    case OPEN_HI_HAT:
    case PEDAL_HI_HAT:
    case CLOSED_HI_HAT:
    case RIDE_BELL:
    case RIDE_CYMBAL_1:
    case RIDE_CYMBAL_2:
    case CRASH_CYMBAL_2:
    case CRASH_CYMBAL_1:
    case CHINESE_CYMBAL:
    case SPLASH_CYMBAL:
    case COWBELL:
    case HAND_CLAP:
    case LOW_TIMBALE:
    case HIGH_TIMBALE:
    case SHORT_WHISTLE:
    case LONG_WHISTLE:
    case LOW_BONGO:
    case HIGH_BONGO:
    case LOW_CONGA:
    case OPEN_HIGH_CONGA:
    case MUTE_HIGH_CONGA:
    case TAMBOURINE:
    case STICKS:
    case CLAVES:
    case CASTANETS:
    case JINGLE_BELL:
    case SHAKER:
    case HIGH_Q:
    case SQUARE_CLICK:
    case METRONOME_CLICK:
    case METRONOME_BELL:
    case MARACAS:
    case CABASA:
    case HIGH_WOODBLOCK:
    case OPEN_TRIANGLE:
    case MUTE_TRIANGLE:
    case LOW_WOODBLOCK:
    case HIGH_AGOGO:
    case LOW_AGOGO:
        return true;
    default:
        return false;
    }
}
