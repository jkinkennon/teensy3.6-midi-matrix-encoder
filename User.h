// Name:       User.h for MIDI_Matrix_Allen.ino
// Created:    11/30/2019
// Author:     John Kinkennon

#include <arduino.h>

#pragma once

//#define USE_ALLEN_CAPTURE_SUPPLY    // comment out if not using
#define INVERT_SPI_DATA             // comment out for 12V keying

#define NUM_CHANNELS        4       // max number of input boards to read,
                                    //   limit to about 6 for clock fanout
#define NUM_KEYS            64      // organs typically use 61 keys, pianos 88
#define NUM_SWITCHES        (NUM_CHANNELS * NUM_KEYS)
#define NUM_COLUMNS         11      // number of key matrix columns
#define NUM_ROWS            6       // number of key matrix rows

//#define HWSERIAL1 Serial1
//#define HWSERIAL2 Serial4

// typedefs that allow accessing tables by channel or by offset

typedef union {
  uint8_t C[NUM_CHANNELS][NUM_KEYS];    // channel access
  uint8_t O[NUM_SWITCHES];              // offset access
} keyTable8_t;

typedef union {
  bool C[NUM_CHANNELS][NUM_KEYS];       // channel access
  bool O[NUM_SWITCHES];                 // offset access
} keyTableB_t;

typedef enum {
  KEYBD_CH,
  PEDAL_CH,
  PISTONS_CH,
  STOPS_CH,
  UNUSED_CH
} midiChannelType;

midiChannelType channelTable[NUM_CHANNELS] {
  KEYBD_CH,     // MIDI channel 1
  KEYBD_CH,     // 2
  KEYBD_CH,     // 3
  KEYBD_CH,     // 4
  //UNUSED_CH,    // 5
  //UNUSED_CH     // 6
};
