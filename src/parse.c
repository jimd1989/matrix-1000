#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "alphabet.h"
#include "midi.h"
#include "parse.h"
#include "read_buffer.h"
#include "write_buffer.h"

static bool isEnd(ReadBuffer *);
static bool isComment(ReadBuffer *);
static void eatWhitespace(ReadBuffer *);
static bool breakAt(ReadBuffer *, char *);
static void parseNrpnCmd(ReadBuffer *, WriteBuffer *);

static bool isEnd(ReadBuffer *r) {
  return (r->data[r->pos] == '\0') || (r->data[r->pos] == '\n');
}

static bool isComment(ReadBuffer *r) {
  return r->data[r->pos] == '#';
}

static void eatWhitespace(ReadBuffer *r) {
  while (r->data[r->pos] == ' ' || r->data[r->pos] == '\t') { r->pos++; }
}

static bool breakAt(ReadBuffer *r, char *cs) {
  char *s = strpbrk(&r->data[r->pos], cs);
  if (s == NULL) { return false; } /* no delimiter found */
  r->pos          = s - r->data;
  r->data[r->pos] = '\0';
  r->pos++; /* resume reading after delimiter (should always be ok?) */
  return true;
}

static void parseNrpnCmd(ReadBuffer *r, WriteBuffer *w) {
  int i        = 0;
  int n        = -128;
  char *s      = NULL;
  uint8_t *msg = NULL;
  NrpnCmd cmd = {0};
  eatWhitespace(r); /* Move higher */
  s = &r->data[r->pos];
  if (!breakAt(r, " ")) {
    warnx("expected a space after cmd %s", s);
    r->status = READ_BUFFER_ERROR;
    return;
  }
  cmd = lookupNrpnCmd(s);
  if (cmd.name == NULL) {
    warnx("cmd %s not found", s);
    r->status = READ_BUFFER_ERROR;
    return;
  }
  eatWhitespace(r);
  s = &r->data[r->pos];
  if (!breakAt(r, ";\n")) {
    warnx("expected argument to %s; got '%s'", cmd.name, s);
    r->status = READ_BUFFER_ERROR;
    return;
  }
  n = atoi(s);
  if (n < cmd.min || n > cmd.max) {
    warnx("cmd %s expects n ∈ [%d, %d]; got %d", cmd.name, cmd.min, cmd.max, n);
    r->status = READ_BUFFER_ERROR;
    return;
  }
  n += cmd.offset;
  if ((n & 127) != n) {
    warnx("cmd %s expects 7 bit parameter; got %d", cmd.name, n);
    r->status = READ_BUFFER_ERROR;
    return;
  }
  if (w->parsePos + MIDI_NRPN_MSG_SIZE >= WRITE_BUFFER_SIZE) {
    warnx("input too long");
    w->status = WRITE_BUFFER_ERROR;
    return;
  }
  msg = nrpnMsg(cmd.param, n);
  for (; i < MIDI_NRPN_MSG_SIZE ; i++, w->parsePos++, w->bytesToWrite++) {
    w->data[w->parsePos] = msg[i];
  }
}

void parseCmds(ReadBuffer *r, WriteBuffer *w) {
  int oldWritePos             = w->parsePos; /* to rollback bad input */
  WriteBufferStatus oldStatus = w->status;
  while (r->pos < r->bytesRead) {
    eatWhitespace(r);
    if (isEnd(r) || isComment(r)) { return; } /* skip blank/commented input */
    parseNrpnCmd(r, w);
    if (r->status == READ_BUFFER_ERROR) {
      r->status   = READ_BUFFER_OK;
      w->status   = oldStatus;
      w->parsePos = oldWritePos;
      return;
    }
    if (w->status == WRITE_BUFFER_ERROR) {
      w->status   = oldStatus;
      w->parsePos = oldWritePos;
      return;
    }
  }
  /* since reading can only take place when EMPTY | POPULATED this is fine */
  if (w->bytesToWrite > 0) { w->status = WRITE_BUFFER_POPULATED; }
}
