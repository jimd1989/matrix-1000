#pragma once

#include <poll.h>

#include "config.h"
#include "read_buffer.h"
#include "write_buffer.h"

#define REPL_POLL_FDS_N 2
#define REPL_POLL_FDS_STDIN_IX 0
#define REPL_POLL_FDS_STDOUT_IX 1

typedef struct Repl {
  struct pollfd fds[REPL_POLL_FDS_N];
  ReadBuffer    readBuffer;
  WriteBuffer   writeBuffer;
} Repl;

void repl(Repl *, Config *);
