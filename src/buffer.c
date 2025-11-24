#include <stdio.h>

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
