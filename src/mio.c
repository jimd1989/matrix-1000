#include <stdbool.h>
#include <err.h>
#include <poll.h>
#include <sndio.h>
#include <stdlib.h>

#include "config.h"
#include "mio.h"

void mio(Mio *m, Config *c) {
  m->input = NULL;
  m->output = NULL;
  if (c->writeTarget == WRITE_BUFFER_TARGET_MIO) {
    /* Reads default MIDI_DEVICE for output. Blocking write. */
    m->output = mio_open(MIO_PORTANY, MIO_OUT, false);
  } else {
    warnx("no output MIDIDEVICE found; writing binary MIDI data to stdout");
  }
  if (c->input != NULL) {
    /* Reads "midi/n" string for input. Non-blocking read. */
    m->input = mio_open(c->input, MIO_IN, true);
    mio_pollfd(m->input, &m->inputPollFd, POLLIN);
  } else {
    warnx("no -input 'midi/n' device specified; only taking stdin commands");
  }
}
