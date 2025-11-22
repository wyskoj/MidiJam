//
// Created by Jacob on 11/19/2025.
//

#include "instrument.h"

MidiJamInstrumentId MIDIJAM_INSTRUMENT_IDS[128] = {
    PIANO, // 1  - Acoustic Grand Piano
    PIANO, // 2  - Bright Acoustic Piano
    PIANO, // 3  - Electric Grand Piano
    PIANO, // 4  - Honky-tonk Piano
    PIANO, // 5  - Electric Piano 1
    PIANO, // 6  - Electric Piano 2
    PIANO, // 7  - Harpsichord
    PIANO, // 8  - Clavinet
    PIANO, // 9  - Celesta
    UNDEFINED,  // 10 - Glockenspiel
    UNDEFINED,  // 11 - Music Box
    UNDEFINED,  // 12 - Vibraphone
    UNDEFINED,  // 13 - Marimba
    UNDEFINED,  // 14 - Xylophone
    UNDEFINED,  // 15 - Tubular Bells
    UNDEFINED,  // 16 - Dulcimer
    UNDEFINED,  // 17 - Drawbar Organ
    UNDEFINED,  // 18 - Percussive Organ
    UNDEFINED,  // 19 - Rock Organ
    UNDEFINED,  // 20 - Church Organ
    UNDEFINED,  // 21 - Reed Organ
    ACCORDION,  // 22 - Accordion
    UNDEFINED,  // 23 - Harmonica
    ACCORDION,  // 24 - Tango Accordion
    UNDEFINED,  // 25 - Acoustic Guitar (nylon)
    UNDEFINED,  // 26 - Acoustic Guitar (steel)
    UNDEFINED,  // 27 - Electric Guitar (jazz)
    UNDEFINED,  // 28 - Electric Guitar (clean)
    UNDEFINED,  // 29 - Electric Guitar (muted)
    UNDEFINED,  // 30 - Overdriven Guitar
    UNDEFINED,  // 31 - Distortion Guitar
    UNDEFINED,  // 32 - Guitar harmonics
    BASS,  // 33 - Acoustic Bass
    BASS,  // 34 - Electric Bass (finger)
    BASS,  // 35 - Electric Bass (pick)
    BASS,  // 36 - Fretless Bass
    BASS,  // 37 - Slap Bass 1
    BASS,  // 38 - Slap Bass 2
    BASS,  // 39 - Synth Bass 1
    BASS,  // 40 - Synth Bass 2
    UNDEFINED,  // 41 - Violin
    UNDEFINED,  // 42 - Viola
    UNDEFINED,  // 43 - Cello
    UNDEFINED,  // 44 - Contrabass
    UNDEFINED,  // 45 - Tremolo Strings
    UNDEFINED,  // 46 - Pizzicato Strings
    UNDEFINED,  // 47 - Orchestral Harp
    UNDEFINED,  // 48 - Timpani
    UNDEFINED,  // 49 - String Ensemble 1
    UNDEFINED,  // 50 - String Ensemble 2
    UNDEFINED,  // 51 - SynthStrings 1
    UNDEFINED,  // 52 - SynthStrings 2
    UNDEFINED,  // 53 - Choir Aahs
    UNDEFINED,  // 54 - Voice Oohs
    UNDEFINED,  // 55 - Synth Voice
    UNDEFINED,  // 56 - Orchestra Hit
    UNDEFINED,  // 57 - Trumpet
    UNDEFINED,  // 58 - Trombone
    UNDEFINED,  // 59 - Tuba
    UNDEFINED,  // 60 - Muted Trumpet
    UNDEFINED,  // 61 - French Horn
    UNDEFINED,  // 62 - Brass Section
    UNDEFINED,  // 63 - SynthBrass 1
    UNDEFINED,  // 64 - SynthBrass 2
    UNDEFINED,  // 65 - Soprano Sax
    UNDEFINED,  // 66 - Alto Sax
    UNDEFINED,  // 67 - Tenor Sax
    UNDEFINED,  // 68 - Baritone Sax
    UNDEFINED,  // 69 - Oboe
    UNDEFINED,  // 70 - English Horn
    UNDEFINED,  // 71 - Bassoon
    UNDEFINED,  // 72 - Clarinet
    UNDEFINED,  // 73 - Piccolo
    UNDEFINED,  // 74 - Flute
    UNDEFINED,  // 75 - Recorder
    UNDEFINED,  // 76 - Pan Flute
    UNDEFINED,  // 77 - Blown Bottle
    UNDEFINED,  // 78 - Shakuhachi
    UNDEFINED,  // 79 - Whistle
    UNDEFINED,  // 80 - Ocarina
    UNDEFINED,  // 81 - Lead 1 (square)
    UNDEFINED,  // 82 - Lead 2 (sawtooth)
    UNDEFINED,  // 83 - Lead 3 (calliope)
    UNDEFINED,  // 84 - Lead 4 (chiff)
    UNDEFINED,  // 85 - Lead 5 (charang)
    UNDEFINED,  // 86 - Lead 6 (voice)
    UNDEFINED,  // 87 - Lead 7 (fifths)
    UNDEFINED,  // 88 - Lead 8 (bass + lead)
    UNDEFINED,  // 89 - Pad 1 (new age)
    UNDEFINED,  // 90 - Pad 2 (warm)
    UNDEFINED,  // 91 - Pad 3 (polysynth)
    UNDEFINED,  // 92 - Pad 4 (choir)
    UNDEFINED,  // 93 - Pad 5 (bowed)
    UNDEFINED,  // 94 - Pad 6 (metallic)
    UNDEFINED,  // 95 - Pad 7 (halo)
    UNDEFINED,  // 96 - Pad 8 (sweep)
    UNDEFINED,  // 97 - FX 1 (rain)
    UNDEFINED,  // 98 - FX 2 (soundtrack)
    UNDEFINED,  // 99 - FX 3 (crystal)
    UNDEFINED,  // 100 - FX 4 (atmosphere)
    UNDEFINED,  // 101 - FX 5 (brightness)
    UNDEFINED,  // 102 - FX 6 (goblins)
    UNDEFINED,  // 103 - FX 7 (echoes)
    UNDEFINED,  // 104 - FX 8 (sci-fi)
    UNDEFINED,  // 105 - Sitar
    UNDEFINED,  // 106 - Banjo
    UNDEFINED,  // 107 - Shamisen
    UNDEFINED,  // 108 - Koto
    UNDEFINED,  // 109 - Kalimba
    UNDEFINED,  // 110 - Bag pipe
    UNDEFINED,  // 111 - Fiddle
    UNDEFINED,  // 112 - Shanai
    UNDEFINED,  // 113 - Tinkle Bell
    UNDEFINED,  // 114 - Agogo
    UNDEFINED,  // 115 - Steel Drums
    UNDEFINED,  // 116 - Woodblock
    UNDEFINED,  // 117 - Taiko Drum
    UNDEFINED,  // 118 - Melodic Tom
    UNDEFINED,  // 119 - Synth Drum
    UNDEFINED,  // 120 - Reverse Cymbal
    UNDEFINED,  // 121 - Guitar Fret Noise
    UNDEFINED,  // 122 - Breath Noise
    UNDEFINED,  // 123 - Seashore
    UNDEFINED,  // 124 - Bird Tweet
    UNDEFINED,  // 125 - Telephone Ring
    UNDEFINED,  // 126 - Helicopter
    UNDEFINED,  // 127 - Applause
    UNDEFINED   // 128 - Gunshot
};
