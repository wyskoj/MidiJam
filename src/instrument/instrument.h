//
// Created by Jacob on 11/18/2025.
//

#ifndef MIDIJAM_INSTRUMENT_H
#define MIDIJAM_INSTRUMENT_H

enum MidiJamInstrumentId : __int16 {
    UNDEFINED = 0x0,
    PIANO = 0x1,
    PERCUSSION = 0x2,
    BASS = 0x3,
    XYLOPHONE = 0x4,
    STAGE_HORN = 0x5,
    STAGE_STRINGS = 0x6,
    HARP = 0x7,
    GUITAR = 0x8,
    STAGE_CHOIR = 0x9,
    TROMBONE = 0xA,
    TRUMPET = 0xB,
    MUTED_TRUMPET = 0xC,
    TUBA = 0xD,
    FRENCH_HORN = 0xE,
    TIMPANI = 0xF,
    ALTO_SAX = 0x10,
    SAPRANO_SAX = 0x11,
    TENOR_SAX = 0x12,
    BARITONE_SAX = 0x13,
    ACCORDION = 0x14,
    PIZZICATO_STRINGS = 0x15,
    TUBULAR_BELLS = 0x16,
    TAIKO = 0x17,
    MUSIC_BOX = 0x18,
    MELODIC_TOM = 0x19,
    SYNTH_DRUM = 0x1A,
    STEEL_DRUM = 0x1B,
    OCARINA = 0x1C,
    WHISTLES = 0x1D,
    PAN_PIPE = 0x1E,
    POP_BOTTLES = 0x1F,
    AGOGOS = 0x20,
    WOODBLOCKS = 0x21,
    HARMONICA = 0x22,
    VIOLIN = 0x23,
    VIOLA = 0x24,
    CELLO = 0x25,
    DOUBLE_BASS = 0x26,
    RECORDER = 0x27,
    FLUTE = 0x29,
    PICCOLO = 0x2A,
    TELEPHONE = 0x2B,
};

extern MidiJamInstrumentId MIDIJAM_INSTRUMENT_IDS[128];

#endif //MIDIJAM_INSTRUMENT_H
