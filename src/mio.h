#pragma once

#include <poll.h>
#include <sndio.h>

#include "config.h"

typedef struct Mio {
  struct pollfd   inputPollFd;
  struct mio_hdl  * input;
  struct mio_hdl  * output;
} Mio;

void mio(Mio *, Config *);
