//
// Created by Jacob on 3/17/2026.
//

#include "MidiJamTool.h"
#include "../instruments/Piano.h"

#include <cstring>
#include <cstdlib>

#include "instruments/Accordion.h"
#include "instruments/Agogos.h"
#include "instruments/AltoSax.h"
#include "instruments/Bass.h"
#include "instruments/BaritoneSax.h"
#include "instruments/FrenchHorn.h"
#include "instruments/Guitar.h"
#include "instruments/Harp.h"
#include "instruments/MelodicTom.h"
#include "instruments/MusicBox.h"
#include "instruments/PizzicatoStrings.h"
#include "instruments/SapranoSax.h"
#include "instruments/StageChoir.h"
#include "instruments/StageHorn.h"
#include "instruments/StageString.h"
#include "instruments/SteelDrum.h"
#include "instruments/SynthDrum.h"
#include "instruments/Taiko.h"
#include "instruments/TenorSax.h"
#include "instruments/Timpani.h"
#include "instruments/Trombone.h"
#include "instruments/Trumpet.h"
#include "instruments/Tuba.h"
#include "instruments/TubularBells.h"
#include "instruments/Violin.h"
#include "instruments/Viola.h"
#include "instruments/Cello.h"
#include "instruments/DoubleBass.h"
#include "instruments/Flute.h"
#include "instruments/Ocarina.h"
#include "instruments/PanPipe.h"
#include "instruments/Harmonica.h"
#include "instruments/PopBottles.h"
#include "instruments/Recorder.h"
#include "instruments/Telephone.h"
#include "instruments/Whistles.h"
#include "instruments/Woodblocks.h"
#include "instruments/Xylophone.h"


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

extern MidiJamInstrumentId g_midiJamInstrumentIds[300];
extern short g_doubleBass_playingStyle[300];
extern int g_currentTempo_scaleFactor0_5;
extern int g_currentTempo_scaleFactor0_9;
extern int g_currentTempo_scaleFactor1_15;
extern double g_currentTempo;

// FUNCTION: MIDIJAM 0x43C470
// MATCH: EXACT
ULONG __stdcall MidiJamTool::AddRef() {
    return InterlockedIncrement(&refCount);
}

// FUNCTION: MIDIJAM 0x445A50
// MATCH: EXACT
HRESULT __stdcall MidiJamTool::Flush(IDirectMusicPerformance*, DMUS_PMSG*, REFERENCE_TIME) {
    return E_NOTIMPL;
}

// FUNCTION: MIDIJAM 0x43C510
// MATCH: EXACT
HRESULT __stdcall MidiJamTool::GetMediaTypeArraySize(DWORD* pdwNumElements) {
    *pdwNumElements = 4;
    return S_OK;
}

// FUNCTION: MIDIJAM 0x43C530
// MATCH: EXACT
HRESULT __stdcall MidiJamTool::GetMediaTypes(DWORD** padwMediaTypes, const DWORD dwNumElements) {
    if (dwNumElements == 4) {
        (*padwMediaTypes)[0] = DMUS_PMSGT_NOTE;
        (*padwMediaTypes)[1] = DMUS_PMSGT_MIDI;
        (*padwMediaTypes)[2] = DMUS_PMSGT_PATCH;
        (*padwMediaTypes)[3] = DMUS_PMSGT_TEMPO;
        return S_OK;
    }
    return E_FAIL;
}

// FUNCTION: MIDIJAM 0x43C4F0
// MATCH: EXACT
HRESULT __stdcall MidiJamTool::GetMsgDeliveryType(DWORD* pdwDeliveryType) {
    *pdwDeliveryType = DMUS_PMSGF_TOOL_IMMEDIATE;
    return S_OK;
}

// FUNCTION: MIDIJAM 0x43C4E0
HRESULT __stdcall MidiJamTool::Init(IDirectMusicGraph*) {
    return E_NOTIMPL;
}


// FUNCTION: MIDIJAM 0x43C410
HRESULT __stdcall MidiJamTool::QueryInterface(REFIID riid, void** ppv) {
    if (!(IsEqualGUID(riid, IID_IUnknown) || IsEqualGUID(riid, IID_IDirectMusicTool))) {
        *ppv = nullptr;
        return E_NOINTERFACE;
    }
    *ppv = this;
    AddRef();
    return S_OK;
}

// FUNCTION: MIDIJAM 0x43C490
ULONG __stdcall MidiJamTool::Release() {
    if (!InterlockedDecrement(&refCount)) {
        if (this)
            delete this;
        return 0;
    }
    return refCount;
}

// FUNCTION: MIDIJAM 0x43C580
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
                    const unsigned short keyIndex = (noteMsg->wMusicValue) - (21);
                    if ((keyIndex) >= (unsigned short)(88)) break;;
                    PianoState& inst = g_piano[g_pianoChannel[msgChannel]];

                    pPerf->GetTime(&rtNow, &mtNow);
                    int duration = noteMsg->mtDuration - g_currentTempo_scaleFactor0_5;
                    if (duration < 0) duration = 10;
                    short timeDelta = static_cast<short>(msgMtTime - mtNow)
                        - static_cast<short>(g_currentTempo_scaleFactor0_9);
                    if (timeDelta <= 0) timeDelta = 1;

                    short slot = 0;
                    while ((inst.queue[keyIndex][slot]) && (slot) < (16)) ++(slot);
                    if ((slot) < (16)) {
                        {
                            inst.queue[keyIndex][slot] = duration;
                            inst.velocities[keyIndex][slot] = noteMsg->bVelocity;
                            inst.timeDeltas[keyIndex][slot] = timeDelta;
                        }
                    }
                    break;
                }

                case BASS: {
                    const unsigned short bassNote = (noteMsg->wMusicValue) - (21);
                    if ((bassNote) >= (unsigned short)(88)) break;;
                    BassState& inst = g_bass[g_bassChannel[msgChannel]];

                    pPerf->GetTime(&rtNow, &mtNow);
                    int duration = noteMsg->mtDuration - g_currentTempo_scaleFactor0_5;
                    if (duration < 0) duration = 10;
                    short timeDelta = static_cast<short>(msgMtTime - mtNow)
                        - static_cast<short>(g_currentTempo_scaleFactor0_9);
                    if (timeDelta <= 0) timeDelta = 1;

                    short slot = 0;
                    while ((inst.queue[bassNote][slot]) && (slot) < (16)) ++(slot);
                    if ((slot) < (16)) {
                        {
                            inst.queue[bassNote][slot] = duration;
                            inst.field_18CC[bassNote][slot] = timeDelta;
                        }
                    }
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

                    short slot = 0;
                    while ((inst.queue[noteIndex][slot]) && (slot) < (16)) ++(slot);
                    if ((slot) < (16)) {
                        {
                            inst.queue[noteIndex][slot] = duration;
                            inst.timeDeltas[noteIndex][slot] = timeDelta;
                        }
                    }
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
                    if (i5 < 16) {
                        g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_64[v99][i5] = noteMsg->
                            mtDuration;
                        if (g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_64[v99][i5] < 0)
                            g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_64[v99][i5] = 10;
                        (pPerf->GetTime)(&rtNow, &mtNow);
                        g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_364[v99][i5] = noteMsg->mtTime -
                            mtNow;
                        g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_364[v99][i5] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_364[v99][i5] <= 0)
                            g_stageString[g_stageStringChannel[noteMsg->dwPChannel]].field_364[v99][i5] = 1;
                    }
                    break;
                }

                case STAGE_CHOIR: {
                    const auto v50 = (noteMsg->wMusicValue + 3) % 12;
                    int i17 = 0;
                    while (g_stageChoir[g_stageChoirChannel[noteMsg->dwPChannel]].field_64[v50][i17] && i17 < 16) ++i17;
                    if (i17 < 16) {
                        g_stageChoir[g_stageChoirChannel[noteMsg->dwPChannel]].field_64[v50][i17] = noteMsg->mtDuration;
                        if (g_stageChoir[g_stageChoirChannel[noteMsg->dwPChannel]].field_64[v50][i17] < 0)
                            g_stageChoir[g_stageChoirChannel[noteMsg->dwPChannel]].field_64[v50][i17] = 10;
                        (pPerf->GetTime)(&rtNow, &mtNow);
                        g_stageChoir[g_stageChoirChannel[noteMsg->dwPChannel]].field_364[v50][i17] = noteMsg->mtTime -
                            mtNow;
                        g_stageChoir[g_stageChoirChannel[noteMsg->dwPChannel]].field_364[v50][i17] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_stageChoir[g_stageChoirChannel[noteMsg->dwPChannel]].field_364[v50][i17] <= 0)
                            g_stageChoir[g_stageChoirChannel[noteMsg->dwPChannel]].field_364[v50][i17] = 1;
                    }
                    break;
                }

                case XYLOPHONE: {
                    const auto v140 = noteMsg->wMusicValue - 21;
                    if (v140 < 0x58u) {
                        int ii = 0;
                        while (g_xylophone[g_xylophoneChannel[noteMsg->dwPChannel]].field_2C6[v140][ii] && ii < 16)
                            ++
                                ii;
                        if (ii < 16) {
                            g_xylophone[g_xylophoneChannel[noteMsg->dwPChannel]].field_2C6[v140][ii] = noteMsg->
                                mtDuration;
                            g_xylophone[g_xylophoneChannel[noteMsg->dwPChannel]].field_18C6[v140][ii] = noteMsg->
                                bVelocity;
                            g_xylophone[g_xylophoneChannel[noteMsg->dwPChannel]].field_2C6[v140][ii] = g_xylophone[
                                    g_xylophoneChannel[noteMsg->dwPChannel]].field_2C6[v140][ii] -
                                g_currentTempo_scaleFactor0_5;
                            if (g_xylophone[g_xylophoneChannel[noteMsg->dwPChannel]].field_2C6[v140][ii] < 0)
                                g_xylophone[g_xylophoneChannel[noteMsg->dwPChannel]].field_2C6[v140][ii] = 10;
                            (pPerf->GetTime)(&rtNow, &mtNow);
                            g_xylophone[g_xylophoneChannel[noteMsg->dwPChannel]].queue[v140][ii] = noteMsg->mtTime -
                                mtNow;
                            g_xylophone[g_xylophoneChannel[noteMsg->dwPChannel]].queue[v140][ii] -=
                                g_currentTempo_scaleFactor0_9;
                            if (g_xylophone[g_xylophoneChannel[noteMsg->dwPChannel]].queue[v140][ii] <= 0)
                                g_xylophone[g_xylophoneChannel[noteMsg->dwPChannel]].queue[v140][ii] = 1;
                        }
                    }
                    break;
                }

                case TROMBONE: {
                    const auto v46 = (noteMsg->wMusicValue + 3) % 12;
                    int i18 = 0;
                    while (g_trombone[g_tromboneChannel[noteMsg->dwPChannel]].field_64[v46][i18] && i18 < 16) ++i18;
                    if (i18 < 16) {
                        g_trombone[g_tromboneChannel[noteMsg->dwPChannel]].field_64[v46][i18] = noteMsg->mtDuration;
                        if (g_trombone[g_tromboneChannel[noteMsg->dwPChannel]].field_64[v46][i18] < 0)
                            g_trombone[g_tromboneChannel[noteMsg->dwPChannel]].field_64[v46][i18] = 10;
                        (pPerf->GetTime)(&rtNow, &mtNow);
                        g_trombone[g_tromboneChannel[noteMsg->dwPChannel]].field_364[v46][i18] = noteMsg->mtTime -
                            mtNow;
                        g_trombone[g_tromboneChannel[noteMsg->dwPChannel]].field_364[v46][i18] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_trombone[g_tromboneChannel[noteMsg->dwPChannel]].field_364[v46][i18] <= 0)
                            g_trombone[g_tromboneChannel[noteMsg->dwPChannel]].field_364[v46][i18] = 1;
                    }
                    break;
                }

                case GUITAR: {
                    const auto v55 = noteMsg->wMusicValue - 21;
                    if (v55 < 0x58u) {
                        int i16 = 0;
                        while (g_guitar[g_guitarChannel[noteMsg->dwPChannel]].field_2D2[v55][i16] && i16 < 16) {
                            ++i16;
                        }
                        if (i16 < 16) {
                            g_guitar[g_guitarChannel[noteMsg->dwPChannel]].field_2D2[v55][i16] = noteMsg->mtDuration;
                            g_guitar[g_guitarChannel[noteMsg->dwPChannel]].field_2D2[v55][i16] = g_guitar[
                                    g_guitarChannel[noteMsg->dwPChannel]].field_2D2[v55][i16]
                                - g_currentTempo_scaleFactor0_5;
                            if (g_guitar[g_guitarChannel[noteMsg->dwPChannel]].field_2D2[v55][i16] < 0)
                                g_guitar[g_guitarChannel[noteMsg->dwPChannel]].field_2D2[v55][i16] = 10;
                            (pPerf->GetTime)(&rtNow, &mtNow);
                            g_guitar[g_guitarChannel[noteMsg->dwPChannel]].field_18D2[v55][i16] = noteMsg->mtTime -
                                mtNow;
                            g_guitar[g_guitarChannel[noteMsg->dwPChannel]].field_18D2[v55][i16] -=
                                g_currentTempo_scaleFactor0_9;
                            if (g_guitar[g_guitarChannel[noteMsg->dwPChannel]].field_18D2[v55][i16] <= 0)
                                g_guitar[g_guitarChannel[noteMsg->dwPChannel]].field_18D2[v55][i16] = 1;
                        }
                    }
                    break;
                }

                case AGOGOS: {
                    const auto v115 = (noteMsg->wMusicValue + 3) % 12;
                    int i2 = 0;
                    while (g_agogos[g_agogosChannel[noteMsg->dwPChannel]].quededHitDurationMs[v115][i2] && i2 < 16)
                        ++
                            i2;
                    if (i2 < 16) {
                        g_agogos[g_agogosChannel[noteMsg->dwPChannel]].quededHitDurationMs[v115][i2] = noteMsg->
                            mtDuration;
                        if (g_agogos[g_agogosChannel[noteMsg->dwPChannel]].quededHitDurationMs[v115][i2] < 0)
                            g_agogos[g_agogosChannel[noteMsg->dwPChannel]].quededHitDurationMs[v115][i2] = 10;
                        (pPerf->GetTime)(&rtNow, &mtNow);
                        g_agogos[g_agogosChannel[noteMsg->dwPChannel]].quededStartDelayMs[v115][i2] = noteMsg->mtTime -
                            mtNow;
                        g_agogos[g_agogosChannel[noteMsg->dwPChannel]].queuedVelocity[v115][i2] = noteMsg->bVelocity;
                        g_agogos[g_agogosChannel[noteMsg->dwPChannel]].quededStartDelayMs[v115][i2] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_agogos[g_agogosChannel[noteMsg->dwPChannel]].quededStartDelayMs[v115][i2] <= 0)
                            g_agogos[g_agogosChannel[noteMsg->dwPChannel]].quededStartDelayMs[v115][i2] = 1;
                    }
                    break;
                }

                case WOODBLOCKS: {
                    const auto v111 = (noteMsg->wMusicValue + 3) % 12;
                    int i3 = 0;
                    while (g_woodblocks[g_woodblocksChannel[noteMsg->dwPChannel]].quededHitDurationMs[v111][i3]
                        && i3 < 16)
                        i3++;
                    if (i3 < 16) {
                        g_woodblocks[g_woodblocksChannel[noteMsg->dwPChannel]].quededHitDurationMs[v111][i3] =
                            noteMsg->mtDuration;
                        if (g_woodblocks[g_woodblocksChannel[noteMsg->dwPChannel]].quededHitDurationMs[v111][i3]
                            < 0)
                            g_woodblocks[g_woodblocksChannel[noteMsg->dwPChannel]].quededHitDurationMs[v111][i3]
                                = 10;
                        (pPerf->GetTime)(&rtNow, &mtNow);
                        g_woodblocks[g_woodblocksChannel[noteMsg->dwPChannel]].quededStartDelayMs[v111][i3] =
                            noteMsg->mtTime - mtNow;
                        g_woodblocks[g_woodblocksChannel[noteMsg->dwPChannel]].queuedVelocity[v111][i3] = noteMsg->
                            bVelocity;
                        g_woodblocks[g_woodblocksChannel[noteMsg->dwPChannel]].quededStartDelayMs[v111][i3] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_woodblocks[g_woodblocksChannel[noteMsg->dwPChannel]].quededStartDelayMs[v111][i3]
                            <= 0)
                            g_woodblocks[g_woodblocksChannel[noteMsg->dwPChannel]].quededStartDelayMs[v111][i3]
                                = 1;
                    }
                    break;
                }

                case ALTO_SAX: {
                    const auto v19 = noteMsg->wMusicValue - 49;
                    if (v19 < 0x21u) {
                        int i25 = 0;
                        while (g_altoSax[g_altoSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] && i25 < 16)
                            i25++;
                        if (i25 < 16) {
                            g_altoSax[g_altoSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] = noteMsg->mtDuration;
                            if (g_altoSax[g_altoSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] < 0)
                                g_altoSax[g_altoSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] = 10;
                            (pPerf->GetTime)(&rtNow, &mtNow);
                            g_altoSax[g_altoSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] = noteMsg->mtTime -
                                mtNow;
                            g_altoSax[g_altoSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] -=
                                g_currentTempo_scaleFactor0_9;
                            if (g_altoSax[g_altoSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] <= 0)
                                g_altoSax[g_altoSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] = 1;
                        }
                    }
                    break;
                }

                case TENOR_SAX: {
                    const auto v19 = noteMsg->wMusicValue - 56;
                    if (v19 < 0x21u) {
                        int i25 = 0;
                        while (g_tenorSax[g_tenorSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] && i25 < 16)
                            i25++;
                        if (i25 < 16) {
                            g_tenorSax[g_tenorSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] = noteMsg->
                                mtDuration;
                            if (g_tenorSax[g_tenorSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] < 0)
                                g_tenorSax[g_tenorSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] = 10;
                            (pPerf->GetTime)(&rtNow, &mtNow);
                            g_tenorSax[g_tenorSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] = noteMsg->mtTime -
                                mtNow;
                            g_tenorSax[g_tenorSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] -=
                                g_currentTempo_scaleFactor0_9;
                            if (g_tenorSax[g_tenorSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] <= 0)
                                g_tenorSax[g_tenorSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] = 1;
                        }
                    }
                    break;
                }

                case BARITONE_SAX: {
                    const auto v19 = noteMsg->wMusicValue - 37;
                    if (v19 < 0x21u) {
                        int i25 = 0;
                        while (g_baritoneSax[g_baritoneSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] && i25 < 16)
                            i25++;
                        if (i25 < 16) {
                            g_baritoneSax[g_baritoneSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] = noteMsg->
                                mtDuration;
                            if (g_baritoneSax[g_baritoneSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] < 0)
                                g_baritoneSax[g_baritoneSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] = 10;
                            (pPerf->GetTime)(&rtNow, &mtNow);
                            g_baritoneSax[g_baritoneSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] = noteMsg->
                                mtTime -
                                mtNow;
                            g_baritoneSax[g_baritoneSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] -=
                                g_currentTempo_scaleFactor0_9;
                            if (g_baritoneSax[g_baritoneSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] <= 0)
                                g_baritoneSax[g_baritoneSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] = 1;
                        }
                    }
                    break;
                }

                case SAPRANO_SAX: {
                    const auto v19 = noteMsg->wMusicValue - 44;
                    if (v19 < 0x21u) {
                        int i25 = 0;
                        while (g_sapranoSax[g_sapranoSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] && i25 < 16)
                            i25++;
                        if (i25 < 16) {
                            g_sapranoSax[g_sapranoSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] = noteMsg->
                                mtDuration;
                            if (g_sapranoSax[g_sapranoSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] < 0)
                                g_sapranoSax[g_sapranoSaxChannel[noteMsg->dwPChannel]].field_10C[v19][i25] = 10;
                            (pPerf->GetTime)(&rtNow, &mtNow);
                            g_sapranoSax[g_sapranoSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] = noteMsg->mtTime
                                -
                                mtNow;
                            g_sapranoSax[g_sapranoSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] -=
                                g_currentTempo_scaleFactor0_9;
                            if (g_sapranoSax[g_sapranoSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] <= 0)
                                g_sapranoSax[g_sapranoSaxChannel[noteMsg->dwPChannel]].field_94C[v19][i25] = 1;
                        }
                    }
                    break;
                }

                case TRUMPET:
                case MUTED_TRUMPET: {
                    const auto v43 = noteMsg->wMusicValue - 54;
                    if (v43 < 0x36u) {
                        int i19 = 0;
                        while (g_trumpet[g_trumpetChannel[noteMsg->dwPChannel]].field_1B4[v43][i19] && i19 < 16)
                            i19++;
                        if (i19 < 16) {
                            g_trumpet[g_trumpetChannel[noteMsg->dwPChannel]].field_1B4[v43][i19] = noteMsg->mtDuration;
                            if (g_trumpet[g_trumpetChannel[noteMsg->dwPChannel]].field_1B4[v43][i19] < 0)
                                g_trumpet[g_trumpetChannel[noteMsg->dwPChannel]].field_1B4[v43][i19] = 10;
                            (pPerf->GetTime)(&rtNow, &mtNow);
                            g_trumpet[g_trumpetChannel[noteMsg->dwPChannel]].field_F34[v43][i19] = noteMsg->
                                mtTime - mtNow;
                            g_trumpet[g_trumpetChannel[noteMsg->dwPChannel]].field_F34[v43][i19] -=
                                g_currentTempo_scaleFactor0_9;
                            if (g_trumpet[g_trumpetChannel[noteMsg->dwPChannel]].field_F34[v43][i19] <= 0)
                                g_trumpet[g_trumpetChannel[noteMsg->dwPChannel]].field_F34[v43][i19] = 1;
                        }
                    }
                    break;
                }

                case TUBA: {
                    const auto v39 = noteMsg->wMusicValue - 16;
                    if (v39 < 0x2Cu) {
                        int i20 = 0;
                        while (g_tuba[g_tubaChannel[noteMsg->dwPChannel]].field_164[v39][i20] && i20 < 16)
                            i20++;
                        if (i20 < 16) {
                            g_tuba[g_tubaChannel[noteMsg->dwPChannel]].field_164[v39][i20] = noteMsg->mtDuration;
                            if (g_tuba[g_tubaChannel[noteMsg->dwPChannel]].field_164[v39][i20] < 0)
                                g_tuba[g_tubaChannel[noteMsg->dwPChannel]].field_164[v39][i20] = 10;
                            (pPerf->GetTime)(&rtNow, &mtNow);
                            g_tuba[g_tubaChannel[noteMsg->dwPChannel]].field_C64[v39][i20] = noteMsg->mtTime - mtNow;
                            g_tuba[g_tubaChannel[noteMsg->dwPChannel]].field_C64[v39][i20] -=
                                g_currentTempo_scaleFactor0_9;
                            if (g_tuba[g_tubaChannel[noteMsg->dwPChannel]].field_C64[v39][i20] <= 0)
                                g_tuba[g_tubaChannel[noteMsg->dwPChannel]].field_C64[v39][i20] = 1;
                        }
                    }
                    break;
                }

                case FRENCH_HORN: {
                    const auto v35 = noteMsg->wMusicValue - 36;
                    if (v35 < 0x24u) {
                        int i21 = 0;
                        while (g_frenchHorn[g_frenchHornChannel[noteMsg->dwPChannel]].field_124[v35][i21] && i21 < 16)
                            i21++;
                        if (i21 < 16) {
                            g_frenchHorn[g_frenchHornChannel[noteMsg->dwPChannel]].field_124[v35][i21] = noteMsg->
                                mtDuration;
                            if (g_frenchHorn[g_frenchHornChannel[noteMsg->dwPChannel]].field_124[v35][i21] < 0)
                                g_frenchHorn[g_frenchHornChannel[noteMsg->dwPChannel]].field_124[v35][i21] = 10;
                            (pPerf->GetTime)(&rtNow, &mtNow);
                            g_frenchHorn[g_frenchHornChannel[noteMsg->dwPChannel]].field_A24[v35][i21] = noteMsg->mtTime
                                - mtNow;
                            g_frenchHorn[g_frenchHornChannel[noteMsg->dwPChannel]].field_A24[v35][i21] -=
                                g_currentTempo_scaleFactor0_9;
                            if (g_frenchHorn[g_frenchHornChannel[noteMsg->dwPChannel]].field_A24[v35][i21] <= 0)
                                g_frenchHorn[g_frenchHornChannel[noteMsg->dwPChannel]].field_A24[v35][i21] = 1;
                        }
                    }
                    break;
                }

                case TIMPANI: {
                    const auto v79 = (noteMsg->wMusicValue + 3) % 12;
                    int i10 = 0;
                    while (g_timpani[g_timpaniChannel[noteMsg->dwPChannel]].field_68[v79][i10] && i10 < 32)
                        i10++;
                    if (i10 < 32) {
                        g_timpani[g_timpaniChannel[noteMsg->dwPChannel]].field_68[v79][i10] = noteMsg->mtDuration;
                        if (g_timpani[g_timpaniChannel[noteMsg->dwPChannel]].field_68[v79][i10] < 0)
                            g_timpani[g_timpaniChannel[noteMsg->dwPChannel]].field_68[v79][i10] = 10;
                        (pPerf->GetTime)(&rtNow, &mtNow);
                        g_timpani[g_timpaniChannel[noteMsg->dwPChannel]].field_668[v79][i10] = noteMsg->mtTime - mtNow;
                        g_timpani[g_timpaniChannel[noteMsg->dwPChannel]].field_998[v79][i10] = noteMsg->bVelocity;
                        g_timpani[g_timpaniChannel[noteMsg->dwPChannel]].field_668[v79][i10] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_timpani[g_timpaniChannel[noteMsg->dwPChannel]].field_668[v79][i10] <= 0)
                            g_timpani[g_timpaniChannel[noteMsg->dwPChannel]].field_668[v79][i10] = 1;
                    }
                    break;
                }

                case MELODIC_TOM: {
                    const auto v79 = (noteMsg->wMusicValue + 3) % 12;
                    int i10 = 0;
                    while (g_melodicTom[g_melodicTomChannel[noteMsg->dwPChannel]].field_68[v79][i10] && i10 < 32)
                        i10++;
                    if (i10 < 32) {
                        g_melodicTom[g_melodicTomChannel[noteMsg->dwPChannel]].field_68[v79][i10] = noteMsg->mtDuration;
                        if (g_melodicTom[g_melodicTomChannel[noteMsg->dwPChannel]].field_68[v79][i10] < 0)
                            g_melodicTom[g_melodicTomChannel[noteMsg->dwPChannel]].field_68[v79][i10] = 10;
                        (pPerf->GetTime)(&rtNow, &mtNow);
                        g_melodicTom[g_melodicTomChannel[noteMsg->dwPChannel]].field_668[v79][i10] = noteMsg->mtTime -
                            mtNow;
                        g_melodicTom[g_melodicTomChannel[noteMsg->dwPChannel]].field_998[v79][i10] = noteMsg->bVelocity;
                        g_melodicTom[g_melodicTomChannel[noteMsg->dwPChannel]].field_668[v79][i10] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_melodicTom[g_melodicTomChannel[noteMsg->dwPChannel]].field_668[v79][i10] <= 0)
                            g_melodicTom[g_melodicTomChannel[noteMsg->dwPChannel]].field_668[v79][i10] = 1;
                    }
                    break;
                }

                case SYNTH_DRUM: {
                    const auto v79 = (noteMsg->wMusicValue + 3) % 12;
                    int i10 = 0;
                    while (g_synthDrum[g_synthDrumChannel[noteMsg->dwPChannel]].field_68[v79][i10] && i10 < 32)
                        i10++;
                    if (i10 < 32) {
                        g_synthDrum[g_synthDrumChannel[noteMsg->dwPChannel]].field_68[v79][i10] = noteMsg->mtDuration;
                        if (g_synthDrum[g_synthDrumChannel[noteMsg->dwPChannel]].field_68[v79][i10] < 0)
                            g_synthDrum[g_synthDrumChannel[noteMsg->dwPChannel]].field_68[v79][i10] = 10;
                        (pPerf->GetTime)(&rtNow, &mtNow);
                        g_synthDrum[g_synthDrumChannel[noteMsg->dwPChannel]].field_668[v79][i10] = noteMsg->mtTime -
                            mtNow;
                        g_synthDrum[g_synthDrumChannel[noteMsg->dwPChannel]].field_998[v79][i10] = noteMsg->bVelocity;
                        g_synthDrum[g_synthDrumChannel[noteMsg->dwPChannel]].field_668[v79][i10] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_synthDrum[g_synthDrumChannel[noteMsg->dwPChannel]].field_668[v79][i10] <= 0)
                            g_synthDrum[g_synthDrumChannel[noteMsg->dwPChannel]].field_668[v79][i10] = 1;
                    }
                    break;
                }

                case TAIKO: {
                    const auto v79 = (noteMsg->wMusicValue + 3) % 12;
                    int i10 = 0;
                    while (g_taiko[g_taikoChannel[noteMsg->dwPChannel]].field_68[v79][i10] && i10 < 32)
                        i10++;
                    if (i10 < 32) {
                        g_taiko[g_taikoChannel[noteMsg->dwPChannel]].field_68[v79][i10] = noteMsg->mtDuration;
                        if (g_taiko[g_taikoChannel[noteMsg->dwPChannel]].field_68[v79][i10] < 0)
                            g_taiko[g_taikoChannel[noteMsg->dwPChannel]].field_68[v79][i10] = 10;
                        (pPerf->GetTime)(&rtNow, &mtNow);
                        g_taiko[g_taikoChannel[noteMsg->dwPChannel]].field_668[v79][i10] = noteMsg->mtTime - mtNow;
                        g_taiko[g_taikoChannel[noteMsg->dwPChannel]].field_998[v79][i10] = noteMsg->bVelocity;
                        g_taiko[g_taikoChannel[noteMsg->dwPChannel]].field_668[v79][i10] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_taiko[g_taikoChannel[noteMsg->dwPChannel]].field_668[v79][i10] <= 0)
                            g_taiko[g_taikoChannel[noteMsg->dwPChannel]].field_668[v79][i10] = 1;
                    }
                    break;
                }

                case PIZZICATO_STRINGS: {
                    const auto v103 = (noteMsg->wMusicValue + 3) % 12;
                    int i4 = 0;
                    while (g_pizzicatoStrings[g_pizzicatoStringsChannel[noteMsg->dwPChannel]].field_64[0][16 * v103 +
                        i4])
                        i4++;
                    if (i4 < 16) {
                        g_pizzicatoStrings[g_pizzicatoStringsChannel[noteMsg->dwPChannel]].field_64[0][16 * v103 + i4] =
                            noteMsg->mtDuration;
                        if (g_pizzicatoStrings[g_pizzicatoStringsChannel[noteMsg->dwPChannel]].field_64[0][16 * v103 +
                            i4] < 0)
                            g_pizzicatoStrings[g_pizzicatoStringsChannel[noteMsg->dwPChannel]].field_64[0][16 * v103 +
                                i4] = 10;
                        (pPerf->GetTime)(&rtNow, &mtNow);
                        g_pizzicatoStrings[g_pizzicatoStringsChannel[noteMsg->dwPChannel]].field_37C[v103][i4] = noteMsg
                            ->mtTime - mtNow;
                        g_pizzicatoStrings[g_pizzicatoStringsChannel[noteMsg->dwPChannel]].field_37C[v103][i4] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_pizzicatoStrings[g_pizzicatoStringsChannel[noteMsg->dwPChannel]].field_37C[v103][i4] <= 0)
                            g_pizzicatoStrings[g_pizzicatoStringsChannel[noteMsg->dwPChannel]].field_37C[v103][i4] = 1;
                    }
                    break;
                }

                case TUBULAR_BELLS: {
                    const auto v63 = (noteMsg->wMusicValue + 3) % 12;
                    int i14 = 0;
                    while (g_tubularBells[g_tubularBellsChannel[noteMsg->dwPChannel]].field_68[v63][i14] && i14 < 16)
                        i14++;
                    if (i14 < 16) {
                        g_tubularBells[g_tubularBellsChannel[noteMsg->dwPChannel]].field_68[v63][i14] = noteMsg->
                            mtDuration;
                        if (g_tubularBells[g_tubularBellsChannel[noteMsg->dwPChannel]].field_68[v63][i14] < 0)
                            g_tubularBells[g_tubularBellsChannel[noteMsg->dwPChannel]].field_68[v63][i14] = 10;
                        (pPerf->GetTime)(&rtNow, &mtNow);
                        g_tubularBells[g_tubularBellsChannel[noteMsg->dwPChannel]].field_368[v63][i14] = noteMsg->mtTime
                            - mtNow;
                        g_tubularBells[g_tubularBellsChannel[noteMsg->dwPChannel]].field_518[v63][i14] = noteMsg->
                            bVelocity;
                        g_tubularBells[g_tubularBellsChannel[noteMsg->dwPChannel]].field_368[v63][i14] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_tubularBells[g_tubularBellsChannel[noteMsg->dwPChannel]].field_368[v63][i14] <= 0)
                            g_tubularBells[g_tubularBellsChannel[noteMsg->dwPChannel]].field_368[v63][i14] = 1;
                    }
                    break;
                }

                case MUSIC_BOX: {
                    const auto v95 = (noteMsg->wMusicValue + 3) % 12;
                    int i6 = 0;
                    while (g_musicBox[g_musicBoxChannel[noteMsg->dwPChannel]].field_68[v95][i6] && i6 < 16)
                        ++i6;
                    if (i6 < 16) {
                        g_musicBox[g_musicBoxChannel[noteMsg->dwPChannel]].field_68[v95][i6] = noteMsg->mtDuration;
                        if (g_musicBox[g_musicBoxChannel[noteMsg->dwPChannel]].field_68[v95][i6] < 0)
                            g_musicBox[g_musicBoxChannel[noteMsg->dwPChannel]].field_68[v95][i6] = 10;
                        pPerf->GetTime(&rtNow, &mtNow);
                        g_musicBox[g_musicBoxChannel[noteMsg->dwPChannel]].field_368[v95][i6] = noteMsg->mtTime - mtNow;
                        g_musicBox[g_musicBoxChannel[noteMsg->dwPChannel]].field_818[v95][i6] = noteMsg->bVelocity;
                        g_musicBox[g_musicBoxChannel[noteMsg->dwPChannel]].field_368[v95][i6] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_musicBox[g_musicBoxChannel[noteMsg->dwPChannel]].field_368[v95][i6] <= 0)
                            g_musicBox[g_musicBoxChannel[noteMsg->dwPChannel]].field_368[v95][i6] = 1;
                    }
                    break;
                }

                case STEEL_DRUM: {
                    const auto v79 = (noteMsg->wMusicValue + 3) % 12;
                    int i10 = 0;
                    while (g_steelDrum[g_steelDrumChannel[noteMsg->dwPChannel]].field_68[v79][i10] && i10 < 32)
                        i10++;
                    if (i10 < 32) {
                        g_steelDrum[g_steelDrumChannel[noteMsg->dwPChannel]].field_68[v79][i10] = noteMsg->mtDuration;
                        if (g_steelDrum[g_steelDrumChannel[noteMsg->dwPChannel]].field_68[v79][i10] < 0)
                            g_steelDrum[g_steelDrumChannel[noteMsg->dwPChannel]].field_68[v79][i10] = 10;
                        (pPerf->GetTime)(&rtNow, &mtNow);
                        g_steelDrum[g_steelDrumChannel[noteMsg->dwPChannel]].field_668[v79][i10] = noteMsg->mtTime -
                            mtNow;
                        g_steelDrum[g_steelDrumChannel[noteMsg->dwPChannel]].field_998[v79][i10] = noteMsg->bVelocity;
                        g_steelDrum[g_steelDrumChannel[noteMsg->dwPChannel]].field_668[v79][i10] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_steelDrum[g_steelDrumChannel[noteMsg->dwPChannel]].field_668[v79][i10] <= 0)
                            g_steelDrum[g_steelDrumChannel[noteMsg->dwPChannel]].field_668[v79][i10] = 1;
                    }
                    break;
                }

                case VIOLIN: {
                    const auto v164 = noteMsg->wMusicValue - 21;
                    if (v164 < 0x58u) {
                        int i = 0;
                        while (g_violin[g_violinChannel[noteMsg->dwPChannel]].queue[v164][i] && i < 16) ++i;
                        if (i < 16) {
                            g_violin[g_violinChannel[noteMsg->dwPChannel]].queue[v164][i] = noteMsg->mtDuration;
                            g_violin[g_violinChannel[noteMsg->dwPChannel]].queue[v164][i] =
                                g_violin[g_violinChannel[noteMsg->dwPChannel]].queue[v164][i] -
                                g_currentTempo_scaleFactor0_5;
                            if (g_violin[g_violinChannel[noteMsg->dwPChannel]].queue[v164][i] < 0)
                                g_violin[g_violinChannel[noteMsg->dwPChannel]].queue[v164][i] = 10;
                            (pPerf->GetTime)(&rtNow, &mtNow);
                            g_violin[g_violinChannel[noteMsg->dwPChannel]].timeDeltas[v164][i] = noteMsg->mtTime -
                                mtNow;
                            g_violin[g_violinChannel[noteMsg->dwPChannel]].timeDeltas[v164][i] -=
                                g_currentTempo_scaleFactor0_9;
                            if (g_violin[g_violinChannel[noteMsg->dwPChannel]].timeDeltas[v164][i] <= 0)
                                g_violin[g_violinChannel[noteMsg->dwPChannel]].timeDeltas[v164][i] = 1;
                        }
                    }
                    break;
                }

                case VIOLA: {
                    const auto note = noteMsg->wMusicValue - 21;
                    if (note < 0x58u) {
                        int i = 0;
                        while (g_viola[g_violaChannel[noteMsg->dwPChannel]].queue[note][i] && i < 16) ++i;
                        if (i < 16) {
                            g_viola[g_violaChannel[noteMsg->dwPChannel]].queue[note][i] = noteMsg->mtDuration;
                            g_viola[g_violaChannel[noteMsg->dwPChannel]].queue[note][i] =
                                g_viola[g_violaChannel[noteMsg->dwPChannel]].queue[note][i] -
                                g_currentTempo_scaleFactor0_5;
                            if (g_viola[g_violaChannel[noteMsg->dwPChannel]].queue[note][i] < 0)
                                g_viola[g_violaChannel[noteMsg->dwPChannel]].queue[note][i] = 10;
                            pPerf->GetTime(&rtNow, &mtNow);
                            g_viola[g_violaChannel[noteMsg->dwPChannel]].timeDeltas[note][i] = noteMsg->mtTime - mtNow;
                            g_viola[g_violaChannel[noteMsg->dwPChannel]].timeDeltas[note][i] -=
                                g_currentTempo_scaleFactor0_9;
                            if (g_viola[g_violaChannel[noteMsg->dwPChannel]].timeDeltas[note][i] <= 0)
                                g_viola[g_violaChannel[noteMsg->dwPChannel]].timeDeltas[note][i] = 1;
                        }
                    }
                    break;
                }

                case CELLO: {
                    const auto note = noteMsg->wMusicValue - 21;
                    if (note < 0x58u) {
                        int i = 0;
                        while (g_cello[g_celloChannel[noteMsg->dwPChannel]].queue[note][i] && i < 16) ++i;
                        if (i < 16) {
                            g_cello[g_celloChannel[noteMsg->dwPChannel]].queue[note][i] = noteMsg->mtDuration;
                            g_cello[g_celloChannel[noteMsg->dwPChannel]].queue[note][i] =
                                g_cello[g_celloChannel[noteMsg->dwPChannel]].queue[note][i] -
                                g_currentTempo_scaleFactor0_5;
                            if (g_cello[g_celloChannel[noteMsg->dwPChannel]].queue[note][i] < 0)
                                g_cello[g_celloChannel[noteMsg->dwPChannel]].queue[note][i] = 10;
                            pPerf->GetTime(&rtNow, &mtNow);
                            g_cello[g_celloChannel[noteMsg->dwPChannel]].timeDeltas[note][i] = noteMsg->mtTime - mtNow;
                            g_cello[g_celloChannel[noteMsg->dwPChannel]].timeDeltas[note][i] -=
                                g_currentTempo_scaleFactor0_9;
                            if (g_cello[g_celloChannel[noteMsg->dwPChannel]].timeDeltas[note][i] <= 0)
                                g_cello[g_celloChannel[noteMsg->dwPChannel]].timeDeltas[note][i] = 1;
                        }
                    }
                    break;
                }

                case DOUBLE_BASS: {
                    const auto note = noteMsg->wMusicValue - 21;
                    if (note < 0x58u) {
                        g_doubleBass_playingStyle[g_doubleBassChannel[noteMsg->dwPChannel]] = 1;
                        int i = 0;
                        while (g_doubleBass[g_doubleBassChannel[noteMsg->dwPChannel]].queue[note][i] && i < 16) ++i;
                        if (i < 16) {
                            g_doubleBass[g_doubleBassChannel[noteMsg->dwPChannel]].queue[note][i] = noteMsg->mtDuration;
                            g_doubleBass[g_doubleBassChannel[noteMsg->dwPChannel]].queue[note][i] =
                                g_doubleBass[g_doubleBassChannel[noteMsg->dwPChannel]].queue[note][i] -
                                g_currentTempo_scaleFactor0_5;
                            if (g_doubleBass[g_doubleBassChannel[noteMsg->dwPChannel]].queue[note][i] < 0)
                                g_doubleBass[g_doubleBassChannel[noteMsg->dwPChannel]].queue[note][i] = 10;
                            pPerf->GetTime(&rtNow, &mtNow);
                            g_doubleBass[g_doubleBassChannel[noteMsg->dwPChannel]].timeDeltas[note][i] = noteMsg->mtTime
                                -
                                mtNow;
                            g_doubleBass[g_doubleBassChannel[noteMsg->dwPChannel]].timeDeltas[note][i] -=
                                g_currentTempo_scaleFactor0_9;
                            if (g_doubleBass[g_doubleBassChannel[noteMsg->dwPChannel]].timeDeltas[note][i] <= 0)
                                g_doubleBass[g_doubleBassChannel[noteMsg->dwPChannel]].timeDeltas[note][i] = 1;
                        }
                    }
                    break;
                }

                case TELEPHONE: {
                    const auto v67 = (noteMsg->wMusicValue + 3) % 12;
                    int i13 = 0;
                    while (g_telephone[g_telephoneChannel[noteMsg->dwPChannel]].field_68[v67][i13] && i13 < 16)
                        ++i13;
                    if (i13 < 16) {
                        g_telephone[g_telephoneChannel[noteMsg->dwPChannel]].field_68[v67][i13] = noteMsg->
                            mtDuration;
                        if (g_telephone[g_telephoneChannel[noteMsg->dwPChannel]].field_68[v67][i13] < 0)
                            g_telephone[g_telephoneChannel[noteMsg->dwPChannel]].field_68[v67][i13] = 10;
                        (pPerf->GetTime)(&rtNow, &mtNow);
                        g_telephone[g_telephoneChannel[noteMsg->dwPChannel]].field_368[v67][i13] = noteMsg->
                            mtTime -
                            mtNow;
                        g_telephone[g_telephoneChannel[noteMsg->dwPChannel]].field_518[v67][i13] = noteMsg->
                            bVelocity;
                        g_telephone[g_telephoneChannel[noteMsg->dwPChannel]].field_368[v67][i13] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_telephone[g_telephoneChannel[noteMsg->dwPChannel]].field_368[v67][i13] <= 0)
                            g_telephone[g_telephoneChannel[noteMsg->dwPChannel]].field_368[v67][i13] = 1;
                    }
                    break;
                }

                case OCARINA: {
                    const auto v75 = (noteMsg->wMusicValue + 3) % 12;
                    int i11 = 0;
                    while (g_ocarina[g_ocarinaChannel[noteMsg->dwPChannel]].field_64[v75][i11] && i11 < 16)
                        ++i11;
                    if (i11 < 16) {
                        g_ocarina[g_ocarinaChannel[noteMsg->dwPChannel]].field_64[v75][i11] = noteMsg->mtDuration;
                        if (g_ocarina[g_ocarinaChannel[noteMsg->dwPChannel]].field_64[v75][i11] < 0)
                            g_ocarina[g_ocarinaChannel[noteMsg->dwPChannel]].field_64[v75][i11] = 10;
                        pPerf->GetTime(&rtNow, &mtNow);
                        g_ocarina[g_ocarinaChannel[noteMsg->dwPChannel]].field_364[v75][i11] = noteMsg->mtTime - mtNow;
                        g_ocarina[g_ocarinaChannel[noteMsg->dwPChannel]].field_364[v75][i11] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_ocarina[g_ocarinaChannel[noteMsg->dwPChannel]].field_364[v75][i11] <= 0)
                            g_ocarina[g_ocarinaChannel[noteMsg->dwPChannel]].field_364[v75][i11] = 1;
                    }
                    break;
                }

                case WHISTLES: {
                    const auto v131 = (noteMsg->wMusicValue + 3) % 12;
                    int kk = 0;
                    while (g_whistles[g_whistlesChannel[noteMsg->dwPChannel]].field_64[v131][kk] && kk < 16)
                        ++kk;
                    if (kk < 16) {
                        g_whistles[g_whistlesChannel[noteMsg->dwPChannel]].field_64[v131][kk] = noteMsg->mtDuration;
                        if (g_whistles[g_whistlesChannel[noteMsg->dwPChannel]].field_64[v131][kk] < 0)
                            g_whistles[g_whistlesChannel[noteMsg->dwPChannel]].field_64[v131][kk] = 10;
                        pPerf->GetTime(&rtNow, &mtNow);
                        g_whistles[g_whistlesChannel[noteMsg->dwPChannel]].field_364[v131][kk] = noteMsg->mtTime -
                            mtNow;
                        g_whistles[g_whistlesChannel[noteMsg->dwPChannel]].field_364[v131][kk] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_whistles[g_whistlesChannel[noteMsg->dwPChannel]].field_364[v131][kk] <= 0)
                            g_whistles[g_whistlesChannel[noteMsg->dwPChannel]].field_364[v131][kk] = 1;
                    }
                    break;
                }

                case PAN_PIPE: {
                    const auto v127 = (noteMsg->wMusicValue + 3) % 12;
                    int mm = 0;
                    while (g_panPipe[g_panPipeChannel[noteMsg->dwPChannel]].field_64[v127][mm] && mm < 16)
                        ++mm;
                    if (mm < 16) {
                        g_panPipe[g_panPipeChannel[noteMsg->dwPChannel]].field_64[v127][mm] = noteMsg->mtDuration;
                        if (g_panPipe[g_panPipeChannel[noteMsg->dwPChannel]].field_64[v127][mm] < 0)
                            g_panPipe[g_panPipeChannel[noteMsg->dwPChannel]].field_64[v127][mm] = 10;
                        pPerf->GetTime(&rtNow, &mtNow);
                        g_panPipe[g_panPipeChannel[noteMsg->dwPChannel]].field_364[v127][mm] = noteMsg->mtTime - mtNow;
                        g_panPipe[g_panPipeChannel[noteMsg->dwPChannel]].field_364[v127][mm] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_panPipe[g_panPipeChannel[noteMsg->dwPChannel]].field_364[v127][mm] <= 0)
                            g_panPipe[g_panPipeChannel[noteMsg->dwPChannel]].field_364[v127][mm] = 1;
                    }
                    break;
                }

                case HARMONICA: {
                    const auto v123 = (noteMsg->wMusicValue + 3) % 12;
                    int nn = 0;
                    while (g_harmonica[g_harmonicaChannel[noteMsg->dwPChannel]].field_64[v123][nn] && nn < 16)
                        ++nn;
                    if (nn < 16) {
                        g_harmonica[g_harmonicaChannel[noteMsg->dwPChannel]].field_64[v123][nn] = noteMsg->mtDuration;
                        if (g_harmonica[g_harmonicaChannel[noteMsg->dwPChannel]].field_64[v123][nn] < 0)
                            g_harmonica[g_harmonicaChannel[noteMsg->dwPChannel]].field_64[v123][nn] = 10;
                        pPerf->GetTime(&rtNow, &mtNow);
                        g_harmonica[g_harmonicaChannel[noteMsg->dwPChannel]].field_364[v123][nn] = noteMsg->mtTime -
                            mtNow;
                        g_harmonica[g_harmonicaChannel[noteMsg->dwPChannel]].field_364[v123][nn] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_harmonica[g_harmonicaChannel[noteMsg->dwPChannel]].field_364[v123][nn] <= 0)
                            g_harmonica[g_harmonicaChannel[noteMsg->dwPChannel]].field_364[v123][nn] = 1;
                    }
                    break;
                }

                case POP_BOTTLES: {
                    const auto v119 = (noteMsg->wMusicValue + 3) % 12;
                    int i1 = 0;
                    while (g_popBottles[g_popBottlesChannel[noteMsg->dwPChannel]].field_64[v119][i1] && i1 < 16)
                        ++i1;
                    if (i1 < 16) {
                        g_popBottles[g_popBottlesChannel[noteMsg->dwPChannel]].field_64[v119][i1] = noteMsg->mtDuration;
                        if (g_popBottles[g_popBottlesChannel[noteMsg->dwPChannel]].field_64[v119][i1] < 0)
                            g_popBottles[g_popBottlesChannel[noteMsg->dwPChannel]].field_64[v119][i1] = 10;
                        pPerf->GetTime(&rtNow, &mtNow);
                        g_popBottles[g_popBottlesChannel[noteMsg->dwPChannel]].field_364[v119][i1] = noteMsg->mtTime -
                            mtNow;
                        g_popBottles[g_popBottlesChannel[noteMsg->dwPChannel]].field_364[v119][i1] -=
                            g_currentTempo_scaleFactor0_9;
                        if (g_popBottles[g_popBottlesChannel[noteMsg->dwPChannel]].field_364[v119][i1] <= 0)
                            g_popBottles[g_popBottlesChannel[noteMsg->dwPChannel]].field_364[v119][i1] = 1;
                    }
                    break;
                }

                case RECORDER: {
                    const auto note = noteMsg->wMusicValue - 36;
                    if (note < 0x3Du) {
                        int i26 = 0;
                        while (g_recorder[g_recorderChannel[noteMsg->dwPChannel]].field_1EC[note][i26] && i26 < 16) {
                            ++i26;
                        }
                        if (i26 < 16) {
                            g_recorder[g_recorderChannel[noteMsg->dwPChannel]].field_1EC[note][i26] = noteMsg->mtDuration;
                            if (g_recorder[g_recorderChannel[noteMsg->dwPChannel]].field_1EC[note][i26] < 0) {
                                g_recorder[g_recorderChannel[noteMsg->dwPChannel]].field_1EC[note][i26] = 10;
                            }
                            pPerf->GetTime(&rtNow, &mtNow);
                            g_recorder[g_recorderChannel[noteMsg->dwPChannel]].field_112C[note][i26] = noteMsg->mtTime - mtNow;
                            g_recorder[g_recorderChannel[noteMsg->dwPChannel]].field_112C[note][i26] -= g_currentTempo_scaleFactor0_9;
                            if (g_recorder[g_recorderChannel[noteMsg->dwPChannel]].field_112C[note][i26] <= 0) {
                                g_recorder[g_recorderChannel[noteMsg->dwPChannel]].field_112C[note][i26] = 1;
                            }
                        }
                    }
                    break;
                }

                case FLUTE: {
                    const auto note = noteMsg->wMusicValue - 59;
                    if (note < 0x26u) {
                        int i28 = 0;
                        while (g_flute[g_fluteChannel[noteMsg->dwPChannel]].field_134[note][i28] && i28 < 16)
                            ++i28;
                        if (i28 < 16) {
                            g_flute[g_fluteChannel[noteMsg->dwPChannel]].field_134[note][i28] = noteMsg->mtDuration;
                            if (g_flute[g_fluteChannel[noteMsg->dwPChannel]].field_134[note][i28] < 0)
                                g_flute[g_fluteChannel[noteMsg->dwPChannel]].field_134[note][i28] = 10;
                            pPerf->GetTime(&rtNow, &mtNow);
                            g_flute[g_fluteChannel[noteMsg->dwPChannel]].field_AB4[note][i28] = noteMsg->mtTime - mtNow;
                            g_flute[g_fluteChannel[noteMsg->dwPChannel]].field_AB4[note][i28] -= g_currentTempo_scaleFactor0_9;
                            if (g_flute[g_fluteChannel[noteMsg->dwPChannel]].field_AB4[note][i28] <= 0)
                                g_flute[g_fluteChannel[noteMsg->dwPChannel]].field_AB4[note][i28] = 1;
                        }
                    }
                    break;
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
                    break;
                }

                case STAGE_CHOIR: {
                    ALLOC_INST(stageChoir, StageChoirState);
                    break;
                }

                case XYLOPHONE: {
                    if (g_xylophone) {
                        g_xylophone = static_cast<XylophoneState*>(
                            realloc(g_xylophone, 10566 * (g_xylophoneCount + 1)));
                        memset(&g_xylophone[g_xylophoneCount], 0, sizeof(XylophoneState));
                    }
                    else {
                        g_xylophone = static_cast<XylophoneState*>(malloc(0x2946u));
                        memset(g_xylophone, 0, sizeof(XylophoneState));
                    }
                    g_xylophoneChannel[patchMsg->dwPChannel] = g_xylophoneCount;
                    switch (patchMsg->byInstrument) {
                        case 9u:
                            g_xylophone_types[g_xylophoneCount] = 1;
                            break;
                        case 0xBu:
                            g_xylophone_types[g_xylophoneCount] = 2;
                            break;
                        case 0xCu:
                            g_xylophone_types[g_xylophoneCount] = 3;
                            break;
                        default:
                            g_xylophone_types[g_xylophoneCount] = 0;
                            break;
                    }
                    ++g_xylophoneCount;
                    break;
                }

                case TROMBONE: {
                    ALLOC_INST(trombone, TromboneState);
                    break;
                }

                case GUITAR: {
                    ALLOC_INST(guitar, GuitarState);
                    break;
                }

                case AGOGOS: {
                    ALLOC_INST(agogos, AgogosState);
                    break;
                }

                case WOODBLOCKS: {
                    ALLOC_INST(woodblocks, WoodblocksState);
                    break;
                }

                case ALTO_SAX: {
                    ALLOC_INST(altoSax, AltoSaxState);
                    break;
                }

                case BARITONE_SAX: {
                    ALLOC_INST(baritoneSax, BaritoneSaxState);
                    break;
                }

                case TENOR_SAX: {
                    ALLOC_INST(tenorSax, TenorSaxState);
                    break;
                }

                case SAPRANO_SAX: {
                    ALLOC_INST(sapranoSax, SapranoSaxState);
                    break;
                }

                case TRUMPET:
                case MUTED_TRUMPET: {
                    if (g_trumpet)
                        g_trumpet = static_cast<TrumpetState*>(realloc(
                            g_trumpet, sizeof(TrumpetState) * (g_trumpetCount + 1)));
                    else g_trumpet = static_cast<TrumpetState*>(malloc(sizeof(TrumpetState)));
                    memset(&g_trumpet[g_trumpetCount], 0, sizeof(TrumpetState));
                    if (g_midiJamInstrumentIds[pPMSG->dwPChannel] == MUTED_TRUMPET)
                        g_trumpet[g_trumpetCount].field_15F6 = 1;
                    g_trumpetChannel[pPMSG->dwPChannel] = g_trumpetCount++;
                    break;
                }

                case TUBA: {
                    ALLOC_INST(tuba, TubaState);
                    break;
                }

                case FRENCH_HORN: {
                    ALLOC_INST(frenchHorn, FrenchHornState);
                    break;
                }

                case TIMPANI: {
                    ALLOC_INST(timpani, TimpaniState);
                    break;
                }

                case MELODIC_TOM: {
                    ALLOC_INST(melodicTom, MelodicTomState);
                    break;
                }

                case SYNTH_DRUM: {
                    ALLOC_INST(synthDrum, SynthDrumState);
                    break;
                }

                case TAIKO: {
                    ALLOC_INST(taiko, TaikoState);
                    break;
                }

                case PIZZICATO_STRINGS: {
                    ALLOC_INST(pizzicatoStrings, PizzicatoStringsState);
                    break;
                }

                case TUBULAR_BELLS: {
                    ALLOC_INST(tubularBells, TubularBellsState);
                    break;
                }

                case MUSIC_BOX: {
                    ALLOC_INST(musicBox, MusicBoxState);
                    break;
                }

                case STEEL_DRUM: {
                    ALLOC_INST(steelDrum, SteelDrumState);
                    break;
                }

                case VIOLIN: {
                    ALLOC_INST(violin, ViolinState);
                    break;
                }

                case VIOLA: {
                    ALLOC_INST(viola, ViolaState);
                    break;
                }

                case CELLO: {
                    ALLOC_INST(cello, CelloState);
                    break;
                }

                case DOUBLE_BASS: {
                    ALLOC_INST(doubleBass, DoubleBassState);
                    g_doubleBass_playingStyle[g_doubleBassCount - 1] = 1;
                    break;
                }

                case TELEPHONE: {
                    ALLOC_INST(telephone, TelephoneState);
                    break;
                }

                case OCARINA: {
                    ALLOC_INST(ocarina, OcarinaState);
                    break;
                }

                case WHISTLES: {
                    ALLOC_INST(whistles, WhistlesState);
                    break;
                }

                case PAN_PIPE: {
                    if (g_panPipe)
                        g_panPipe = static_cast<PanPipeState*>(realloc(
                            g_panPipe, sizeof(PanPipeState) * (g_panPipeCount + 1)));
                    else g_panPipe = static_cast<PanPipeState*>(malloc(sizeof(PanPipeState)));
                    memset(&g_panPipe[g_panPipeCount], 0, sizeof(PanPipeState));
                    g_panPipeChannel[pPMSG->dwPChannel] = g_panPipeCount;
                    g_isPanPipeCalliope[g_panPipeCount++] = patchMsg->byInstrument == 0x52;
                    break;
                }

                case HARMONICA: {
                    ALLOC_INST(harmonica, HarmonicaState);
                    break;
                }

                case POP_BOTTLES: {
                    ALLOC_INST(popBottles, PopBottlesState);
                    break;
                }

                case RECORDER: {
                    ALLOC_INST(recorder, RecorderState);
                    break;
                }

                case FLUTE: {
                    ALLOC_INST(flute, FluteState);
                    break;
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

// FUNCTION: MIDIJAM 0x445790
bool IsGmPercussionSupported(const GM_PERCUSSION patch) {
    switch (patch) {
        case ACOUSTIC_SNARE:
        case ELECTRIC_SNARE:
        case SIDE_STICK:
            return true;
        case LOW_MID_TOM:
            return true;
        case LOW_TOM:
            return true;
        case HIGH_FLOOR_TOM:
            return true;
        case ACOUSTIC_BASS_DRUM:
        case ELECTRIC_BASS_DRUM:
            return true;
        case OPEN_HI_HAT:
            return true;
        case PEDAL_HI_HAT:
            return true;
        case CLOSED_HI_HAT:
            return true;
        case RIDE_BELL:
        case RIDE_CYMBAL_1:
        case RIDE_CYMBAL_2:
            return true;
        case CRASH_CYMBAL_2:
            return true;
        case CRASH_CYMBAL_1:
            return true;
        case CHINESE_CYMBAL:
            return true;
        case SPLASH_CYMBAL:
            return true;
        case COWBELL:
            return true;
        case HAND_CLAP:
            return true;
        case LOW_TIMBALE:
            return true;
        case HIGH_TIMBALE:
            return true;
        case SHORT_WHISTLE:
            return true;
        case LONG_WHISTLE:
            return true;
        case LOW_BONGO:
            return true;
        case HIGH_BONGO:
            return true;
        case LOW_CONGA:
            return true;
        case OPEN_HIGH_CONGA:
            return true;
        case MUTE_HIGH_CONGA:
            return true;
        case TAMBOURINE:
            return true;
        case STICKS:
            return true;
        case CLAVES:
            return true;
        case CASTANETS:
            return true;
        case JINGLE_BELL:
            return true;
        case SHAKER:
            return true;
        case HIGH_Q:
            return true;
        case SQUARE_CLICK:
            return true;
        case METRONOME_CLICK:
            return true;
        case METRONOME_BELL:
            return true;
        case MARACAS:
            return true;
        case CABASA:
            return true;
        case HIGH_WOODBLOCK:
            return true;
        case OPEN_TRIANGLE:
            return true;
        case MUTE_TRIANGLE:
            return true;
        case LOW_WOODBLOCK:
            return true;
        case HIGH_AGOGO:
            return true;
    }
    return patch == LOW_AGOGO;
}
