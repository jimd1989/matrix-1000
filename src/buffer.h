#pragma once

#include <stdint.h>
#include <unistd.h>

#include "config.h"

#define BUFFER_SIZE 4096
#define WRITE_BUFFER_SIZE (2 * BUFFER_SIZE)

typedef enum ReadBufferStats {
  READ_BUFFER_EOF = 1,
  READ_BUFFER_OK,
  READ_BUFFER_ERROR
} ReadBufferStatus;

typedef struct ReadBuffer {
  ReadBufferStatus  status;
  ssize_t           pos;
  ssize_t           bytesRead; 
  ssize_t           maxSize;
  char              data[BUFFER_SIZE];
} ReadBuffer;

typedef enum WriteBufferStatus {
  WRITE_BUFFER_EMPTY = 1,
  WRITE_BUFFER_POPULATED,
  WRITE_BUFFER_WRITING,
  WRITE_BUFFER_ERROR
} WriteBufferStatus;

typedef struct WriteBuffer {
  MidiChannel       chan;
  WriteBufferStatus status;
  WriteBufferTarget target;
  ssize_t           parsePos; /* position of stdin → bytes */
  ssize_t           writePos; /* position of bytes → output */
  ssize_t           bytesToWrite;
  ssize_t           maxSize;
  uint8_t           data[WRITE_BUFFER_SIZE];
} WriteBuffer;

void readBuffer(ReadBuffer *);
void writeBuffer(WriteBuffer *, MidiChannel, WriteBufferTarget);
void readToBuffer(ReadBuffer *);
void writeFromBufferToStdout(WriteBuffer *);
