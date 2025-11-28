#pragma once

#include <stdint.h>

typedef uint8_t MidiChannel;
typedef char * MidiInput;

typedef enum WriteBufferTarget {
  WRITE_BUFFER_TARGET_STDOUT = 0,
  WRITE_BUFFER_TARGET_MIO
} WriteBufferTarget;

typedef struct Config {
  MidiChannel       chan;
  WriteBufferTarget writeTarget;
  MidiInput         input;
} Config;


void config(Config *, int, char **);
