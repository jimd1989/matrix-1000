#include <err.h>
#include <poll.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "parse.h"
#include "read_buffer.h"
#include "repl.h"
#include "write_buffer.h"

static void fds(Repl *);
static void pollFds(Repl *);
static bool isStdinReady(Repl *);
static bool isStdoutReady(Repl *);
static bool isMidiInReady(Repl *);
static void readStdin(Repl *);

static bool isStdinReady(Repl *r) {
  bool isNotWriting = !isWriting(&r->writeBuffer);
  bool isPollin     = r->fds[REPL_POLL_FDS_STDIN_IX].revents & POLLIN;
  return isNotWriting && isPollin;
}

static bool isStdoutReady(Repl *r) {
  bool canWrite  = canWriteStdout(&r->writeBuffer);
  bool isPollout = r->fds[REPL_POLL_FDS_STDOUT_IX].revents & POLLOUT;
  return canWrite && isPollout;
}

static bool isMidiInReady(Repl *r) {
  if (r->mio.input == NULL) { return false; }
  return r->fds[REPL_POLL_FDS_MIDI_IN_IX].revents & POLLIN;
}

static void fds(Repl *r) {
  r->fds[REPL_POLL_FDS_STDIN_IX].fd      = STDIN_FILENO;
  r->fds[REPL_POLL_FDS_STDIN_IX].events  = POLLIN;
  r->fds[REPL_POLL_FDS_STDOUT_IX].fd     = STDOUT_FILENO;
  r->fds[REPL_POLL_FDS_STDOUT_IX].events = POLLOUT;
  r->fds[REPL_POLL_FDS_MIDI_IN_IX]       = r->mio.inputPollFd;
}

static void pollFds(Repl *r) {
  int events = poll(r->fds, REPL_POLL_FDS_N, -1);
  if (events == -1) { errx(1, "error polling IO"); }
}

static void readStdin(Repl *r) {
  readToBuffer(&r->readBuffer);
  switch (r->readBuffer.status) {
    case READ_BUFFER_ERROR:
      errx(1, "error reading from stdin");
    case READ_BUFFER_EOF:
      exit(0);
    case READ_BUFFER_OK:
      parseCmds(&r->readBuffer, &r->writeBuffer);
  }
  r->fds[REPL_POLL_FDS_STDIN_IX].revents = 0;
}

void repl(Repl *r, Config *c) {
  fds(r);
  readBuffer(&r->readBuffer);
  writeBuffer(&r->writeBuffer, c->writeTarget);
  warnx("writing MIDI on channel %d", c->chan + 1);
  while (1) {
    pollFds(r);
    if (isStdinReady(r))  { readStdin(r);                              }
    if (isStdoutReady(r)) { writeFromBufferToStdout(&r->writeBuffer);  }
    if (isMidiInReady(r)) { readMidiToBuffer(&r->readBuffer, &r->mio); } 
  }
}
