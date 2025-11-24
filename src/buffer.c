#include <stdio.h>
#include <unistd.h>

#include "buffer.h"

void readBuffer(ReadBuffer *r) {
  r->status    = READ_BUFFER_OK;
  r->pos       = 0;
  r->bytesRead = 0;
  r->maxSize   = BUFFER_SIZE;
}

void readToBuffer(ReadBuffer *r) {
  r->pos       = 0;
  r->bytesRead = read(STDIN_FILENO, r->data, sizeof(r->data) - 1);
  if      (r->bytesRead > 0)  { r->status = READ_BUFFER_OK;    }
  else if (r->bytesRead == 0) { r->status = READ_BUFFER_EOF;   }
  else                        { r->status = READ_BUFFER_ERROR; }
}

void writeBuffer(WriteBuffer *w, WriteBufferTarget target) {
  w->status   = WRITE_BUFFER_EMPTY;
  w->target   = target;
  w->parsePos = 0;
  w->writePos = 0;
  w->maxSize  = WRITE_BUFFER_SIZE;
}

void writeFromBufferToStdout(WriteBuffer *w) {
  int bytesWritten = 0;
  uint8_t *blob    = &w->data[w->writePos];
  w->status        = WRITE_BUFFER_WRITING;
  bytesWritten     = write(STDOUT_FILENO, blob, w->bytesToWrite);
  if (bytesWritten == -1) { 
    w->status = WRITE_BUFFER_ERROR;
    return;
  }
  w->writePos     += bytesWritten;
  w->bytesToWrite -= bytesWritten;
  if (w->bytesToWrite == 0) {
    /* only reset status and position if full buffer has been written */
    w->status   = WRITE_BUFFER_EMPTY;
    w->parsePos = 0;
    w->writePos = 0;
  }
}
