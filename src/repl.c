#include <err.h>
#include <poll.h>
#include <stdbool.h>
#include <unistd.h>

#include "buffer.h"
#include "repl.h"

static bool isStdinReady(Repl *);
static bool isStdoutReady(Repl *);

static bool isStdinReady(Repl *r) {
  bool isNotWriting = r->writeBuffer.status != WRITE_BUFFER_WRITING;
  bool isPollin     = r->fds[REPL_POLL_FDS_STDIN_IX].revents & POLLIN;
  return isNotWriting && isPollin;
}

static bool isStdoutReady(Repl *r) {
  WriteBufferStatus s = r->writeBuffer.status;
  bool canWrite  = s == WRITE_BUFFER_POPULATED || s == WRITE_BUFFER_WRITING;
  bool isPollout = r->fds[REPL_POLL_FDS_STDOUT_IX].revents & POLLOUT;
  return canWrite && isPollout;
}

void repl(Repl *r) {
  int events                             = 0;
  r->fds[REPL_POLL_FDS_STDIN_IX].fd      = STDIN_FILENO;
  r->fds[REPL_POLL_FDS_STDIN_IX].events  = POLLIN;
  r->fds[REPL_POLL_FDS_STDOUT_IX].fd     = STDOUT_FILENO;
  r->fds[REPL_POLL_FDS_STDOUT_IX].events = POLLOUT;
  readBuffer(&r->readBuffer);
  writeBuffer(&r->writeBuffer, WRITE_BUFFER_TARGET_STDOUT);
  while (1) {
    events = poll(r->fds, REPL_POLL_FDS_N, -1);
    if (events == -1) { errx(1, "error polling IO"); }
    if (isStdinReady(r)) {
      readToBuffer(&r->readBuffer);
      switch (r->readBuffer.status) {
        case READ_BUFFER_ERROR:
          errx(1, "error reading from stdin");
          break;
        case READ_BUFFER_EOF:
          return;
        case READ_BUFFER_OK:
          warnx("");
      }
    }
    if (isStdoutReady(r)) {
      writeFromBufferToStdout(&r->writeBuffer);
      switch(r->writeBuffer.status) {
        case WRITE_BUFFER_ERROR:
          errx(1, "error writing to stdout");
          break;
        case WRITE_BUFFER_WRITING:
        case WRITE_BUFFER_POPULATED:
        case WRITE_BUFFER_EMPTY:
          break;
      }
    }
  }
}
