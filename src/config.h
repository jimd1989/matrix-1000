#pragma once

#include <stdint.h>

typedef uint8_t MidiChannel;

typedef enum WriteBufferTarget {
  WRITE_BUFFER_TARGET_STDOUT = 0,
  WRITE_BUFFER_TARGET_MIO
} WriteBufferTarget;

typedef struct Config {
  MidiChannel       chan;
  WriteBufferTarget writeTarget;
} Config;


void config(Config *, int, char **);
