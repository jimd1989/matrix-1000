#include <stdint.h>

#include "config.h"
#include "midi.h"

#define MIDI_CC 176
#define MIDI_NRPN_PARAM_MSB 99
#define MIDI_NRPN_PARAM_LSB 98
#define MIDI_NRPN_VALUE_MSB 6
#define MIDI_NRPN_VALUE_LSB 38

static uint8_t MIDI_NRPN_MESSAGE[MIDI_NRPN_MSG_SIZE] = {
  MIDI_CC, MIDI_NRPN_PARAM_MSB, 0,
  MIDI_CC, MIDI_NRPN_PARAM_LSB, 0, 
  MIDI_CC, MIDI_NRPN_VALUE_MSB, 0, 
  MIDI_CC, MIDI_NRPN_VALUE_LSB, 0
};

void midi(MidiChannel chan) {
  MIDI_NRPN_MESSAGE[0] |= chan;
  MIDI_NRPN_MESSAGE[3] |= chan;
  MIDI_NRPN_MESSAGE[6] |= chan;
  MIDI_NRPN_MESSAGE[9] |= chan;
}

uint8_t * nrpnMsg(uint8_t param, uint8_t value) {
  MIDI_NRPN_MESSAGE[5]  = param & 127;
  MIDI_NRPN_MESSAGE[11] = value & 127;
  return MIDI_NRPN_MESSAGE;
}
