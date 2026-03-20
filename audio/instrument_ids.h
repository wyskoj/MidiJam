//
// Created by Jacob on 3/18/2026.
//

#ifndef MIDIJAM_INSTRUMENT_IDS_H
#define MIDIJAM_INSTRUMENT_IDS_H

#include <windows.h>

/**
 * MidiJam instrument identifiers assigned to each MIDI channel.
 * Values are assigned when a DMUS_PMSGT_PATCH message is received.
 * Composite values (e.g. AGOGOS|GUITAR = 0x28) are used as
 * additional piano variant identifiers.
 */
enum MidiJamInstrumentId : short
{
    PIANO             = 0x01,
    PERCUSSION        = 0x02,
    BASS              = 0x03,
    XYLOPHONE         = 0x04,
    STAGE_HORN        = 0x05,
    STAGE_STRINGS     = 0x06,
    HARP              = 0x07,
    GUITAR            = 0x08,
    STAGE_CHOIR       = 0x09,
    TROMBONE          = 0x0A,
    TRUMPET           = 0x0B,
    MUTED_TRUMPET     = 0x0C,
    TUBA              = 0x0D,
    FRENCH_HORN       = 0x0E,
    TIMPANI           = 0x0F,
    ALTO_SAX          = 0x10,
    SAPRANO_SAX       = 0x11,
    TENOR_SAX         = 0x12,
    BARITONE_SAX      = 0x13,
    ACCORDION         = 0x14,
    PIZZICATO_STRINGS = 0x15,
    TUBULAR_BELLS     = 0x16,
    TAIKO             = 0x17,
    MUSIC_BOX         = 0x18,
    MELODIC_TOM       = 0x19,
    SYNTH_DRUM        = 0x1A,
    STEEL_DRUM        = 0x1B,
    OCARINA           = 0x1C,
    WHISTLES          = 0x1D,
    PAN_PIPE          = 0x1E,
    POP_BOTTLES       = 0x1F,
    AGOGOS            = 0x20,
    WOODBLOCKS        = 0x21,
    HARMONICA         = 0x22,
    VIOLIN            = 0x23,
    VIOLA             = 0x24,
    CELLO             = 0x25,
    DOUBLE_BASS       = 0x26,
    RECORDER          = 0x27,
    FLUTE             = 0x29,
    PICCOLO           = 0x2A,
    TELEPHONE         = 0x2B,
    ELECTRIC_PIANO    = 0x28,  // GM patches 15-20 — piano variant, material index 1
    HONKY_TONK_PIANO  = 0x2C,  // GM patches 80-81, 83-84, 86-90, 93-97, 99-103 — piano variant, material index 2
    HARPSICHORD       = 0x2D,  // GM patch 6 — piano variant, material index 3
};

enum GM_PERCUSSION : short
{
    HIGH_Q = 0x1B,
    SLAP = 0x1C,
    SCRATCH_PUSH = 0x1D,
    SCRATCH_PULL = 0x1E,
    STICKS = 0x1F,
    SQUARE_CLICK = 0x20,
    METRONOME_CLICK = 0x21,
    METRONOME_BELL = 0x22,
    ACOUSTIC_BASS_DRUM = 0x23,
    ELECTRIC_BASS_DRUM = 0x24,
    SIDE_STICK = 0x25,
    ACOUSTIC_SNARE = 0x26,
    HAND_CLAP = 0x27,
    ELECTRIC_SNARE = 0x28,
    LOW_FLOOR_TOM = 0x29,
    CLOSED_HI_HAT = 0x2A,
    HIGH_FLOOR_TOM = 0x2B,
    PEDAL_HI_HAT = 0x2C,
    LOW_TOM = 0x2D,
    OPEN_HI_HAT = 0x2E, // XREF: .data:g_cymbal_patches/s
    LOW_MID_TOM = 0x2F,
    HIGH_MID_TOM = 0x30,
    CRASH_CYMBAL_1 = 0x31, // XREF: .data:g_cymbal_patches/s
    HIGH_TOM = 0x32,
    RIDE_CYMBAL_1 = 0x33, // XREF: .data:g_cymbal_patches/s
    CHINESE_CYMBAL = 0x34, // XREF: .data:g_cymbal_patches/s
    RIDE_BELL = 0x35,
    TAMBOURINE = 0x36,
    SPLASH_CYMBAL = 0x37, // XREF: .data:g_cymbal_patches/s
    COWBELL = 0x38,
    CRASH_CYMBAL_2 = 0x39, // XREF: .data:g_cymbal_patches/s
    VIBRASLAP = 0x3A,
    RIDE_CYMBAL_2 = 0x3B, // XREF: .data:g_cymbal_patches/s
    HIGH_BONGO = 0x3C,
    LOW_BONGO = 0x3D,
    MUTE_HIGH_CONGA = 0x3E,
    OPEN_HIGH_CONGA = 0x3F,
    LOW_CONGA = 0x40,
    HIGH_TIMBALE = 0x41,
    LOW_TIMBALE = 0x42,
    HIGH_AGOGO = 0x43,
    LOW_AGOGO = 0x44,
    CABASA = 0x45,
    MARACAS = 0x46,
    SHORT_WHISTLE = 0x47,
    LONG_WHISTLE = 0x48,
    SHORT_GUIRO = 0x49,
    LONG_GUIRO = 0x4A,
    CLAVES = 0x4B,
    HIGH_WOODBLOCK = 0x4C,
    LOW_WOODBLOCK = 0x4D, // XREF: IsGmPercussionSupported+28F/s
    MUTE_CUICA = 0x4E,
    OPEN_CUICA = 0x4F,
    MUTE_TRIANGLE = 0x50, // XREF: IsGmPercussionSupported+282/s
    OPEN_TRIANGLE = 0x51,
    SHAKER = 0x52,
    JINGLE_BELL = 0x53,
    BELLTREE = 0x54,
    CASTANETS = 0x55,
    SURDO_MUTE = 0x56,
    SURDO_OPEN = 0x57,
};

/**
 * Maps GM patch numbers (0-127) to MidiJam instrument IDs.
 * 0 = no instrument assigned for that patch.
 */
static const short MIDIJAM_INSTRUMENT_IDS[128] = {
    // 0-7: Pianos
    1, 1, 1, 1, 1, 1, 0x2D, 1,
    // 8-15: Chromatic perc
    0x16, 4, 0x18, 4, 4, 4, 0x16, 0x28,
    // 16-23: Organs
    0x28, 0x28, 0x28, 0x28, 0x28, 0x14, 0x22, 0x14,
    // 24-31: Guitars
    8, 8, 8, 8, 8, 8, 8, 8,
    // 32-39: Bass
    0x26, 3, 3, 3, 3, 3, 3, 3,
    // 40-47: Strings
    0x23, 0x24, 0x25, 0x26, 6, 0x15, 7, 0xF,
    // 48-55: Ensemble
    6, 6, 6, 6, 9, 9, 9, 0x28,
    // 56-63: Brass
    0xB, 0xA, 0xD, 0xC, 0xE, 5, 5, 5,
    // 64-71: Reed
    0x11, 0x10, 0x12, 0x13, 0, 0, 0, 0,
    // 72-79: Pipe
    0x2A, 0x29, 0x27, 0x1E, 0x1F, 0, 0x1D, 0x1C,
    // 80-87: Synth lead
    0x2C, 0x2C, 0x1E, 0x2C, 0x2C, 9, 0x2C, 0x2C,
    // 88-95: Synth pad
    0x2C, 0x2C, 0x2C, 9, 6, 0x2C, 0x2C, 0x2C,
    // 96-103: Synth effects
    0x2C, 0x2C, 0x16, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, // TODO: verify 96-97
    // 104-111: Ethnic
    0, 0, 0, 0, 0, 0, 0x23, 0,
    // 112-119: Percussive
    0x16, 0x20, 0x1B, 0x21, 0x17, 0x19, 0x1A, 0,
    // 120-127: Sound effects
    8, 9, 0, 0, 0x2B, 0, 9, 0,
};

#endif // MIDIJAM_INSTRUMENT_IDS_H