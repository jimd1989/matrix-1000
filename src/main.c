#include <err.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_POLL_FDS 1
#define IX_STDIN 0
#define SIZE_BUFFER 4096

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
      if (n > 0)       { warnx("!");            }
      else if (n == 0) { return;                } /* EOF */
      else             { errx(1, "read error"); }
    } 
  }
}

int main() {
  repl();
  return 0;
}
