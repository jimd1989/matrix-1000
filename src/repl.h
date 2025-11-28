#pragma once

#include <poll.h>

#include "config.h"
#include "mio.h"
#include "read_buffer.h"
#include "write_buffer.h"

#define REPL_POLL_FDS_N 3
#define REPL_POLL_FDS_STDIN_IX 0
#define REPL_POLL_FDS_STDOUT_IX 1
#define REPL_POLL_FDS_MIDI_IN_IX 2

typedef struct Repl {
  Mio           mio;
  struct pollfd fds[REPL_POLL_FDS_N];
  ReadBuffer    readBuffer;
  WriteBuffer   writeBuffer;
} Repl;

void repl(Repl *, Config *);
