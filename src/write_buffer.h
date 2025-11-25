#pragma once

#include <stdint.h>

#include "config.h"

#define WRITE_BUFFER_SIZE 8192

typedef enum WriteBufferStatus {
  WRITE_BUFFER_EMPTY = 1,
  WRITE_BUFFER_POPULATED,
  WRITE_BUFFER_WRITING,
  WRITE_BUFFER_ERROR
} WriteBufferStatus;

typedef struct WriteBuffer {
  WriteBufferStatus status;
  WriteBufferTarget target;
  ssize_t           parsePos; /* position of stdin → bytes */
  ssize_t           writePos; /* position of bytes → output */
  ssize_t           bytesToWrite;
  ssize_t           maxSize;
  uint8_t           data[WRITE_BUFFER_SIZE];
} WriteBuffer;

void writeBuffer(WriteBuffer *, WriteBufferTarget);
void writeFromBufferToStdout(WriteBuffer *);
