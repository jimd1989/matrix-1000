#pragma once

#include <stdint.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

typedef enum ReadBufferStats {
  READ_BUFFER_EOF = 0,
  READ_BUFFER_OK,
  READ_BUFFER_ERROR
} ReadBufferStatus;

typedef struct ReadBuffer {
  ReadBufferStatus  status;
  ssize_t           pos;
  ssize_t           bytesRead; 
  ssize_t           maxSize;
  uint8_t           data[BUFFER_SIZE];
} ReadBuffer;

void readBuffer(ReadBuffer *);
void readToBuffer(ReadBuffer *);
