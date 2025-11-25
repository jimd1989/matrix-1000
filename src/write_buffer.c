#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "config.h"
#include "midi.h"
#include "write_buffer.h"

static void clearEmptyWriteBuffer(WriteBuffer *);

static WriteBufferTarget WRITE_TARGET = WRITE_BUFFER_TARGET_MIO;
static WriteBufferStatus WRITE_STATUS = WRITE_BUFFER_EMPTY;

void writeBuffer(WriteBuffer *w, WriteBufferTarget target) {
  WRITE_TARGET = target;
  w->parsePos = 0;
  w->writePos = 0;
}

WriteBufferStatus deriveWriteBufferStatus(WriteBuffer *w) {
  /* only used to derive status from buffer pop; resetting should be local
   * and more explicit. */
  switch (WRITE_STATUS) {
    case WRITE_BUFFER_ERROR:
      return WRITE_BUFFER_ERROR;
    case WRITE_BUFFER_WRITING:
      return WRITE_BUFFER_WRITING;
    case WRITE_BUFFER_POPULATED:
    case WRITE_BUFFER_EMPTY:
      if (w->bytesToWrite > 0) {
        WRITE_STATUS = WRITE_BUFFER_POPULATED; 
      } else {
        WRITE_STATUS = WRITE_BUFFER_EMPTY;
      }
  }
  return WRITE_STATUS;
}

bool canWriteStdout(WriteBuffer *w) {
  WriteBufferStatus s = deriveWriteBufferStatus(w);
  bool statusOk = s == WRITE_BUFFER_POPULATED || s == WRITE_BUFFER_WRITING;
  bool isStdout = WRITE_TARGET == WRITE_BUFFER_TARGET_STDOUT;
  return statusOk && isStdout;
}

bool isWriting (WriteBuffer *w) {
  return WRITE_STATUS == WRITE_BUFFER_WRITING;
}

bool isWriteBufferError (WriteBuffer *w) {
  return WRITE_STATUS == WRITE_BUFFER_ERROR;
}

void writeNrpnMsg(WriteBuffer *w, uint8_t param, uint8_t val) {
  int i = 0;
  uint8_t *msg = nrpnMsg(param, val);
  if (w->parsePos + MIDI_NRPN_MSG_SIZE >= WRITE_BUFFER_SIZE) {
    WRITE_STATUS = WRITE_BUFFER_ERROR;
    warnx("input too long");
    return;
  }
  for (; i < MIDI_NRPN_MSG_SIZE ; i++, w->parsePos++, w->bytesToWrite++) {
    w->data[w->parsePos] = msg[i];
  }
}

void rollbackWriteBuffer(WriteBuffer *w, int oldPos) {
  if (WRITE_STATUS == WRITE_BUFFER_WRITING) { return; } /* illegal state */
  WRITE_STATUS = WRITE_BUFFER_EMPTY;
  w->parsePos  = oldPos;
  (void)deriveWriteBufferStatus(w);
}

static void clearEmptyWriteBuffer(WriteBuffer *w) {
  if (w->bytesToWrite > 0) { return; }
  w->parsePos = 0;
  w->writePos = 0;
  WRITE_STATUS = WRITE_BUFFER_EMPTY;
}

void writeFromBufferToStdout(WriteBuffer *w) {
  int bytesWritten = 0;
  uint8_t *blob    = &w->data[w->writePos];
  WRITE_STATUS     = WRITE_BUFFER_WRITING;
  bytesWritten     = write(STDOUT_FILENO, blob, w->bytesToWrite);
  if (bytesWritten == -1) { errx(1, "error writing to stdout"); }
  w->writePos     += bytesWritten;
  w->bytesToWrite -= bytesWritten;
  clearEmptyWriteBuffer(w);
}
