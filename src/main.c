#include <err.h>

#include "alphabet.h"
#include "config.h"
#include "midi.h"
#include "repl.h"

/*
void vca(uint8_t n) {
  uint32_t checksum   = 0;
  uint8_t volumeParam = 27;
  n = 64 + (n >> 1); 
  putchar(SYSEX_START);
  putchar(SYSEX_MANUFACTURER_ID);
  putchar(SYSEX_DEVICE_ID);
  putchar(SYSEX_CMD_PARAMETER);
  checksum += volumeParam;
  putchar(volumeParam & SYSEX_NIBBLE_MASK);
  putchar((volumeParam >> 4) & SYSEX_NIBBLE_MASK);
  checksum += n;
  putchar(n & SYSEX_NIBBLE_MASK);
  putchar((n >> 4) & SYSEX_NIBBLE_MASK);
  putchar(checksum & SYSEX_CHECKSUM_END);
  putchar(SYSEX_END);
}
*/

int main(int argc, char **argv) {
  Config c = {0};
  Repl r = {0};
  config (&c, argc, argv); /* eventually won't need MIDI channel */
  midi(c.chan);
  nrpnCmds();
  /* hashTest(); */
  repl(&r, &c);
  return 0;
}
