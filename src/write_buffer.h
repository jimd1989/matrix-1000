#pragma once

#include <stdbool.h>
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
  ssize_t parsePos; /* position of stdin → bytes */
  ssize_t writePos; /* position of bytes → output */
  ssize_t bytesToWrite;
  uint8_t data[WRITE_BUFFER_SIZE];
} WriteBuffer;

void writeBuffer(WriteBuffer *, WriteBufferTarget);
WriteBufferStatus deriveWriteBufferStatus(WriteBuffer *);
bool canWriteStdout(WriteBuffer *);
bool isWriting(WriteBuffer *);
bool isWriteBufferError (WriteBuffer *);
void writeNrpnMsg(WriteBuffer *, uint8_t, uint8_t);
void rollbackWriteBuffer(WriteBuffer *, int);
void writeFromBufferToStdout(WriteBuffer *);
