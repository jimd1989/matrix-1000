#include <err.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "alphabet.h"

#define N_POLL_FDS 1
#define IX_STDIN 0
#define SIZE_BUFFER 4096


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

void repl() {
  int n                         = 0;
  int events                    = -1;
  char buffer[SIZE_BUFFER]      = {0};
  struct pollfd fds[N_POLL_FDS] = {0};
  fds[IX_STDIN].fd              = STDIN_FILENO;
  fds[IX_STDIN].events          = POLLIN;
  while (1) {
    events = poll(fds, N_POLL_FDS, -1);
    if      (events == -1) { errx(1, "poll error");}
    else if (fds[IX_STDIN].revents & POLLIN) { 
      n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
      if (n > 0)       { vca(atoi(buffer));     }
      else if (n == 0) { return;                } /* EOF */
      else             { errx(1, "read error"); }
    } 
  }
}

int main() {
  repl();
  return 0;
}
