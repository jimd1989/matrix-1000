#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

static void parseMidiChannelConfig(Config *, char *);
static void parseWriteTargetConfig(Config *, char *);

static void parseMidiChannelConfig(Config *c, char *s) {
  int n = atoi(s);
  if (n > 16 || n < 1) { errx(1, "MIDI channel must be between 1 and 16"); }
  c->chan = n - 1;
}

static void parseWriteTargetConfig(Config *c, char *s) {
  if (strcmp(s, "stdout") == 0) { 
    c->writeTarget = WRITE_BUFFER_TARGET_STDOUT;
  } else if (strcmp(s, "midi") == 0) {
    c->writeTarget = WRITE_BUFFER_TARGET_MIO;
  } else {
    errx(1, "output should be 'stdout' or 'midi'");
  }
}

void config(Config *c, int argc, char **argv) {
  int pos        = 1;
  c->chan        = 0;
  c->writeTarget = WRITE_BUFFER_TARGET_STDOUT;
  while (pos < argc) {
    if (strcmp(argv[pos], "-chan") == 0) {
      parseMidiChannelConfig(c, argv[++pos]);
      pos++;
    } else if (strcmp(argv[pos], "-output") == 0) {
      parseWriteTargetConfig(c, argv[++pos]);
      pos++;
    } else { errx(1, "unknown option %s", argv[pos]); }
  }
}
