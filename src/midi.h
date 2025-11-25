#pragma once

#include <stdint.h>

#include "config.h"

#define MIDI_NRPN_MSG_SIZE 12

/* move to .c eventually */
#define MIDI_SYSEX_START 240
#define MIDI_SYSEX_MANUFACTURER_ID 16
#define MIDI_SYSEX_DEVICE_ID 6
#define MIDI_SYSEX_CMD_PARAMETER 6
#define MIDI_SYSEX_CMD_MATRIX 11
#define MIDI_SYSEX_NIBBLE_MASK 15
#define MIDI_SYSEX_CHECKSUM_END 127
#define MIDI_SYSEX_END 247

void midi(MidiChannel);
uint8_t * nrpnMsg(uint8_t, uint8_t);
