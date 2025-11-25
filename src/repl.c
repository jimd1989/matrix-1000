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
static void readStdin(Repl *);
static void writeStdout(Repl *);

static bool isStdinReady(Repl *r) {
  bool isNotWriting = r->writeBuffer.status != WRITE_BUFFER_WRITING;
  bool isPollin     = r->fds[REPL_POLL_FDS_STDIN_IX].revents & POLLIN;
  return isNotWriting && isPollin;
}

static bool isStdoutReady(Repl *r) {
  WriteBufferStatus s = r->writeBuffer.status;
  bool isTarget  = r->writeBuffer.target == WRITE_BUFFER_TARGET_STDOUT;
  bool canWrite  = s == WRITE_BUFFER_POPULATED || s == WRITE_BUFFER_WRITING;
  bool isPollout = r->fds[REPL_POLL_FDS_STDOUT_IX].revents & POLLOUT;
  return isTarget && canWrite && isPollout;
}

static void fds(Repl *r) {
  r->fds[REPL_POLL_FDS_STDIN_IX].fd      = STDIN_FILENO;
  r->fds[REPL_POLL_FDS_STDIN_IX].events  = POLLIN;
  r->fds[REPL_POLL_FDS_STDOUT_IX].fd     = STDOUT_FILENO;
  r->fds[REPL_POLL_FDS_STDOUT_IX].events = POLLOUT;
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
}

static void writeStdout(Repl *r) {
  writeFromBufferToStdout(&r->writeBuffer);
  switch(r->writeBuffer.status) {
    case WRITE_BUFFER_ERROR:
      errx(1, "error writing to stdout");
    case WRITE_BUFFER_WRITING:
    case WRITE_BUFFER_POPULATED:
    case WRITE_BUFFER_EMPTY:
      return;
  }
}

void repl(Repl *r, Config *c) {
  fds(r);
  readBuffer(&r->readBuffer);
  writeBuffer(&r->writeBuffer, c->writeTarget);
  warnx("awaiting commands");
  while (1) {
    pollFds(r);
    if (isStdinReady(r))  { readStdin(r);   }
    if (isStdoutReady(r)) { writeStdout(r); }
  }
}
