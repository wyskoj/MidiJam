//
// Created by Jacob on 3/17/2026.
//

#include "MidiJamTool.h"
#include "../instruments/Piano.h"

#include <cstring>
#include <cstdlib>

#include "instruments/Accordion.h"
#include "instruments/Bass.h"
#include "instruments/Harp.h"
#include "instruments/StageHorn.h"
#include "instruments/StageString.h"

// Finds the first empty slot and executes body with slot variable in scope.
// Slot field is the expression used to test for empty (== 0).
#define FIND_SLOT(slotTest, maxSlots, slotVar, body) \
do { \
short slotVar = 0; \
while ((slotTest) && (slotVar) < (maxSlots)) ++(slotVar); \
if ((slotVar) < (maxSlots)) { body } \
} while(0)

#define QUEUE_NOTE(state, noteIndex, maxNotes, duration, velocity, timeDelta) \
do { \
short _slot = 0; \
while ((state).queue[(noteIndex)][_slot] && _slot < 16) \
++_slot; \
if (_slot < 16) \
{ \
(state).queue[(noteIndex)][_slot] = (duration); \
(state).velocities[(noteIndex)][_slot]     = (velocity); \
(state).timeDeltas[(noteIndex)][_slot]     = (timeDelta); \
} \
} while(0)

#define MIDI_NOTE_INDEX(wMusicValue, offset, max, outIndex) \
const unsigned short outIndex = (wMusicValue) - (offset); \
if ((outIndex) >= (unsigned short)(max)) break;

#define ALLOC_INST(name, type) \
do { \
if (g_##name) \
g_##name = static_cast<type*>( \
realloc(g_##name, sizeof(type) * (g_##name##Count + 1))); \
else \
g_##name = static_cast<type*>(malloc(sizeof(type))); \
memset(&g_##name[g_##name##Count], 0, sizeof(type)); \
g_##name##Channel[pPMSG->dwPChannel] = g_##name##Count++; \
} while(0)

__int16 word_45EC60[12] = {
    0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6
};

extern MidiJamInstrumentId g_midiJamInstrumentIds[1000];
extern int g_currentTempo_scaleFactor0_5;
extern int g_currentTempo_scaleFactor0_9;
extern int g_currentTempo_scaleFactor1_15;
extern double g_currentTempo;

ULONG __stdcall MidiJamTool::AddRef() { return InterlockedIncrement(&refCount); }

HRESULT __stdcall MidiJamTool::Flush(IDirectMusicPerformance*, DMUS_PMSG*, REFERENCE_TIME) { return E_NOTIMPL; }

HRESULT __stdcall MidiJamTool::GetMediaTypeArraySize(DWORD* pdwNumElements) {
    *pdwNumElements = 4;
    return S_OK;
}

HRESULT __stdcall MidiJamTool::GetMediaTypes(DWORD** padwMediaTypes, const DWORD dwNumElements) {
    if (dwNumElements != 4)
        return E_FAIL;
    (*padwMediaTypes)[0] = DMUS_PMSGT_NOTE;
    (*padwMediaTypes)[1] = DMUS_PMSGT_MIDI;
    (*padwMediaTypes)[2] = DMUS_PMSGT_PATCH;
    (*padwMediaTypes)[3] = DMUS_PMSGT_TEMPO;
    return S_OK;
}

HRESULT __stdcall MidiJamTool::GetMsgDeliveryType(DWORD* pdwDeliveryType) {
    *pdwDeliveryType = DMUS_PMSGF_TOOL_IMMEDIATE;
    return S_OK;
}

HRESULT __stdcall MidiJamTool::Init(IDirectMusicGraph*) { return E_NOTIMPL; }

HRESULT __stdcall MidiJamTool::QueryInterface(REFIID riid, void** ppv) {
    if (IsEqualGUID(riid, IID_IUnknown) || IsEqualGUID(riid, IID_IDirectMusicTool)) {
        *ppv = this;
        AddRef();
        return S_OK;
    }

    *ppv = nullptr;
    return E_NOINTERFACE;
}

ULONG __stdcall MidiJamTool::Release() {
    if (InterlockedDecrement(&refCount))
        return refCount;
    if (this)
        delete this;
    return 0;
}

HRESULT __stdcall MidiJamTool::ProcessPMsg(IDirectMusicPerformance* pPerf, DMUS_PMSG* pPMSG) {
    REFERENCE_TIME rtNow;
    MUSIC_TIME mtNow;
    LPVOID* QueryInterfaceResult = nullptr;

    if (!pPMSG->pGraph
        || pPMSG->pGraph->StampPMsg(pPMSG) < 0)
        return DMUS_S_FREE;

    if (pPerf->QueryInterface(IID_IDirectMusicPerformance8,
                              reinterpret_cast<void**>(&QueryInterfaceResult)) < 0)
        return DMUS_S_REQUEUE;

    switch (pPMSG->dwType) {
        case DMUS_PMSGT_NOTE: {
            const DMUS_NOTE_PMSG* noteMsg = reinterpret_cast<DMUS_NOTE_PMSG*>(pPMSG);
            const MUSIC_TIME msgMtTime = pPMSG->mtTime;
            const DWORD msgChannel = pPMSG->dwPChannel;

            if (!noteMsg->bVelocity || msgChannel > 999)
                break;

            switch (g_midiJamInstrumentIds[msgChannel]) {
                case PIANO: {
                    MIDI_NOTE_INDEX(noteMsg->wMusicValue, 21, 88, keyIndex);
                    PianoState& inst = g_piano[g_pianoChannel[msgChannel]];

                    pPerf->GetTime(&rtNow, &mtNow);
                    int duration = noteMsg->mtDuration - g_currentTempo_scaleFactor0_5;
                    if (duration < 0) duration = 10;
                    short timeDelta = static_cast<short>(msgMtTime - mtNow)
                        - static_cast<short>(g_currentTempo_scaleFactor0_9);
                    if (timeDelta <= 0) timeDelta = 1;

                    FIND_SLOT(
                        inst.queue[keyIndex][slot], 16, slot,
                        {
                        inst.queue[keyIndex][slot] = duration;
                        inst.velocities[keyIndex][slot] = noteMsg->bVelocity;
                        inst.timeDeltas[keyIndex][slot] = timeDelta;
                        }
                    );
                    break;
                }

                case BASS: {
                    MIDI_NOTE_INDEX(noteMsg->wMusicValue, 21, 88, bassNote);
                    BassState& inst = g_bass[g_bassChannel[msgChannel]];

                    pPerf->GetTime(&rtNow, &mtNow);
                    int duration = noteMsg->mtDuration - g_currentTempo_scaleFactor0_5;
                    if (duration < 0) duration = 10;
                    short timeDelta = static_cast<short>(msgMtTime - mtNow)
                        - static_cast<short>(g_currentTempo_scaleFactor0_9);
                    if (timeDelta <= 0) timeDelta = 1;

                    FIND_SLOT(
                        inst.queue[bassNote][slot], 16, slot,
                        {
                        inst.queue[bassNote][slot] = duration;
                        inst.field_18CC[bassNote][slot] = timeDelta;
                        }
                    );
                    break;
                }

                case ACCORDION: {
                    const short noteIndex = (noteMsg->wMusicValue + 12) % 24 + 1;
                    AccordionState& inst = g_accordion[g_accordionChannel[msgChannel]];

                    pPerf->GetTime(&rtNow, &mtNow);
                    int duration = noteMsg->mtDuration - g_currentTempo_scaleFactor0_5;
                    if (duration < 0) duration = 10;
                    short timeDelta = static_cast<short>(msgMtTime - mtNow)
                        - static_cast<short>(g_currentTempo_scaleFactor0_9);
                    if (timeDelta <= 0) timeDelta = 1;

                    FIND_SLOT(
                        inst.queue[noteIndex][slot], 16, slot,
                        {
                        inst.queue[noteIndex][slot] = duration;
                        inst.timeDeltas[noteIndex][slot] = timeDelta;
                        }
                    );
                    break;
                }

                case HARP: {
                    const auto v60 = noteMsg->wMusicValue - 24;
                    if (v60 < 0x50u) {
                        const auto v59 = word_45EC60[v60 % 12] + 7 * (v60 / 12);
                        if (v59 < 0x2Fu) {
                            auto i15 = 0;
                            while (g_harp[g_harpChannel[noteMsg->dwPChannel]].queue[v59][i15] && i15 < 16) ++i15;
                            if (i15 < 16) {
                                g_harp[g_harpChannel[noteMsg->dwPChannel]].queue[v59][i15] = noteMsg->mtDuration;
                                g_harp[g_harpChannel[noteMsg->dwPChannel]].queue[v59][i15] = g_harp[g_harpChannel[
                                        noteMsg->dwPChannel]].queue[v59][i15]
                                    - g_currentTempo_scaleFactor0_5;
                                if (g_harp[g_harpChannel[noteMsg->dwPChannel]].queue[v59][i15] < 0)
                                    g_harp[g_harpChannel[noteMsg->dwPChannel]].queue[v59][i15] = 10;
                                pPerf->GetTime(&rtNow, &mtNow);
                                g_harp[g_harpChannel[noteMsg->dwPChannel]].timeDeltas[v59][i15] = noteMsg->mtTime -
                                    mtNow;
                                g_harp[g_harpChannel[noteMsg->dwPChannel]].timeDeltas[v59][i15] -=
                                    g_currentTempo_scaleFactor0_9;
                                if (g_harp[g_harpChannel[noteMsg->dwPChannel]].timeDeltas[v59][i15] <= 0)
                                    g_harp[g_harpChannel[noteMsg->dwPChannel]].timeDeltas[v59][i15] = 1;
                            }
                        }
                    }
                    break;
                }

                case STAGE_HORN: {
                    const auto v135 = (noteMsg->wMusicValue + 3) % 12;
                    auto jj = 0;
                    while (g_stageHorn[g_stageHornChannel[noteMsg->dwPChannel]].queue[v135][jj] && jj < 16) ++jj;
                    if (jj < 16) {
                        g_stageHorn[g_stageHornChannel[noteMsg->dwPChannel]].queue[v135][jj] = noteMsg->mtDuration;
                        if (g_stageHorn[g_stageHornChannel[noteMsg->dwPChannel]].queue[v135][jj] < 0)
                            g_stageHorn[g_stageHornChannel[noteMsg->dwPChannel]].queue[v135][jj] = 10;
                        pPerf->GetTime(&rtNow, &mtNow);
                        g_stageHorn[g_stageHornChannel[noteMsg->dwPChannel]].timeDeltas[v135][jj] = noteMsg->mtTime -
                            mtNow;
                        g_stageHorn[g_stageHornChannel[noteMsg->dwPChannel]].timeDeltas[v135][jj] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_stageHorn[g_stageHornChannel[noteMsg->dwPChannel]].timeDeltas[v135][jj] <= 0)
                            g_stageHorn[g_stageHornChannel[noteMsg->dwPChannel]].timeDeltas[v135][jj] = 1;
                    }
                    break;
                }
                    
                case STAGE_STRINGS: {
                    const auto v99 = (noteMsg->wMusicValue + 3) % 12;
                    int i5 = 0;
                    while (g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_64[v99][i5] && i5 < 16) ++i5;
                    if ( i5 < 16 )
                    {
                        g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_64[v99][i5] = noteMsg->mtDuration;
                        if ( g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_64[v99][i5] < 0 )
                            g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_64[v99][i5] = 10;
                        (pPerf->GetTime)(&rtNow, &mtNow);
                        g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_364[v99][i5] = noteMsg->mtTime - mtNow;
                        g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_364[v99][i5] -= g_currentTempo_scaleFactor0_9;
                        if ( g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_364[v99][i5] <= 0 )
                            g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_364[v99][i5] = 1;
                    }
                }

                default:
                    break;
            }
            break;
        }

        case DMUS_PMSGT_PATCH: {
            if (pPMSG->dwPChannel > 999) {
                break;
            }
            const DMUS_PATCH_PMSG* patchMsg = reinterpret_cast<DMUS_PATCH_PMSG*>(pPMSG);
            const bool isPercussionChannel = pPMSG->dwPChannel == 9;
            if (isPercussionChannel) {
                g_midiJamInstrumentIds[pPMSG->dwPChannel] = PERCUSSION;
                break;
            }
            const auto isPatchInRange = patchMsg->byInstrument > 127u;
            if (isPatchInRange) {
                break;
            }
            g_midiJamInstrumentIds[pPMSG->dwPChannel] =
                static_cast<MidiJamInstrumentId>(MIDIJAM_INSTRUMENT_IDS[patchMsg->byInstrument]);

            switch (g_midiJamInstrumentIds[pPMSG->dwPChannel]) {
                case PIANO: {
                    ALLOC_INST(piano, PianoState);
                    break;
                }

                case BASS: {
                    ALLOC_INST(bass, BassState);
                    break;
                }

                case ACCORDION: {
                    ALLOC_INST(accordion, AccordionState);
                    g_accordion[g_accordionCount - 1].squeezeAngle = 4.0f;
                    break;
                }

                case HARP: {
                    ALLOC_INST(harp, HarpState);
                    break;
                }

                case STAGE_HORN: {
                    ALLOC_INST(stageHorn, StageHornState);
                    break;
                }

                case STAGE_STRINGS: {
                    ALLOC_INST(stageString, StageStringState);
                }

                default:
                    break;
            }
            break;
        }

        case DMUS_PMSGT_TEMPO: {
            g_currentTempo = reinterpret_cast<DMUS_TEMPO_PMSG*>(pPMSG)->dblTempo;
            g_currentTempo_scaleFactor0_5 = static_cast<int>(g_currentTempo * 0.5);
            g_currentTempo_scaleFactor0_9 = static_cast<int>(g_currentTempo * 0.9);
            g_currentTempo_scaleFactor1_15 = static_cast<int>(g_currentTempo * 1.15);
            break;
        }
        default:
            break;
    }

    return DMUS_S_REQUEUE;
}

// FUNCTION: MIDIJAM 0x43C2D0
bool IsGmPercussionSupported(const GM_PERCUSSION patch) {
    switch (patch) {
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
