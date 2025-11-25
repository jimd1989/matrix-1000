#include <err.h>
#include <stddef.h>
#include <stdint.h>

#include "alphabet.h"

#define NRPN_CMD_TABLE_SIZE 256

static Hash fnv1(char *);
static void insertNrpnCmd(NrpnCmd);

static NrpnCmd NRPN_CMDS[NRPN_CMD_TABLE_SIZE] = {0};

static Hash fnv1a(char *s) {
  uint32_t offset = 2166136261;
  while (*s != '\0') {
    offset ^= (uint8_t)*s;
    offset *= 16777619;
    s++;
  }
  /* extra low bit scramble */
  offset ^= offset >> 16;
  offset *= 2246822507;
  offset ^= offset >> 13;
  return offset;
}

static void insertNrpnCmd(NrpnCmd c) {
  Hash    hash = fnv1a(c.name) & 255;
  NrpnCmd slot = NRPN_CMDS[hash];
  if (slot.name != NULL) {
    errx(1, "NRPN cmd slot for %s collides with %s", c.name, slot.name);
  }
  NRPN_CMDS[hash] = c;
}

void nrpnCmds() {
  insertNrpnCmd((struct NrpnCmd){"p1", 0,   0,  63, 64});
  insertNrpnCmd((struct NrpnCmd){"l1", 1, -63,  63, 64});
  insertNrpnCmd((struct NrpnCmd){ "y", 2,   0,   3, 64});
  insertNrpnCmd((struct NrpnCmd){"q1", 3,   0,  63, 64});
  insertNrpnCmd((struct NrpnCmd){"x1", 4, -63,  63, 64});
  insertNrpnCmd((struct NrpnCmd){"z1", 5,   0,  63, 64});
  insertNrpnCmd((struct NrpnCmd){"w1", 6,   0,   3, 64});
  insertNrpnCmd((struct NrpnCmd){"v1", 7,   0,   3, 64});
  insertNrpnCmd((struct NrpnCmd){"m1", 8,   0,   3, 64});
  insertNrpnCmd((struct NrpnCmd){"k1", 9,   0,   1, 64});
}

NrpnCmd lookupNrpnCmd(char *s) {
  Hash hash = fnv1a(s) & (NRPN_CMD_TABLE_SIZE - 1);
  return NRPN_CMDS[hash];
}

void hashTest() {
  int mod = NRPN_CMD_TABLE_SIZE;
  warnx("%d", fnv1a("p1") % mod);
  warnx("%d", fnv1a("l1") % mod);
  warnx("%d", fnv1a("y") % mod);
  warnx("%d", fnv1a("q1") % mod);
  warnx("%d", fnv1a("x1") % mod);
  warnx("%d", fnv1a("z1") % mod);
  warnx("%d", fnv1a("w1") % mod);
  warnx("%d", fnv1a("v1") % mod);
  warnx("%d", fnv1a("m1") % mod);
  warnx("%d", fnv1a("k1") % mod);
  warnx("%d", fnv1a("p2") % mod);
  warnx("%d", fnv1a("l2") % mod);
  warnx("%d", fnv1a("t" ) % mod);
  warnx("%d", fnv1a("q2") % mod);
  warnx("%d", fnv1a("x2") % mod);
  warnx("%d", fnv1a("z2") % mod);
  warnx("%d", fnv1a("w2") % mod);
  warnx("%d", fnv1a("b2") % mod);
  warnx("%d", fnv1a("m2") % mod);
  warnx("%d", fnv1a("k2") % mod);
  warnx("%d", fnv1a("B") % mod);
  warnx("%d", fnv1a("C") % mod);
  warnx("%d", fnv1a("E") % mod);
  warnx("%d", fnv1a("A") % mod);
  warnx("%d", fnv1a("R") % mod);
  warnx("%d", fnv1a("V") % mod);
  warnx("%d", fnv1a("M") % mod);
  warnx("%d", fnv1a("I") % mod);
  warnx("%d", fnv1a("U") % mod);
  warnx("%d", fnv1a("F") % mod);
  warnx("%d", fnv1a("Q") % mod);
  warnx("%d", fnv1a("X") % mod);
  warnx("%d", fnv1a("T") % mod);
  warnx("%d", fnv1a("s@") % mod);
  warnx("%d", fnv1a("t@1") % mod);
  warnx("%d", fnv1a("t@2") % mod);
  warnx("%d", fnv1a("t@3") % mod);
  warnx("%d", fnv1a("t@4") % mod);
  warnx("%d", fnv1a("t@5") % mod);
  warnx("%d", fnv1a("r@1") % mod);
  warnx("%d", fnv1a("n@1") % mod);
  warnx("%d", fnv1a("r@2") % mod);
  warnx("%d", fnv1a("n@1") % mod);
  warnx("%d", fnv1a("m@") % mod);
  warnx("%d", fnv1a("v@") % mod);
  warnx("%d", fnv1a("l@") % mod);
  warnx("%d", fnv1a("a@") % mod);
  warnx("%d", fnv1a("b.1") % mod);
  warnx("%d", fnv1a("a.1") % mod);
  warnx("%d", fnv1a("d.1") % mod);
  warnx("%d", fnv1a("s.1") % mod);
  warnx("%d", fnv1a("r.1") % mod);
  warnx("%d", fnv1a("i.1") % mod);
  warnx("%d", fnv1a("v.1") % mod);
  warnx("%d", fnv1a("u.1") % mod);
  warnx("%d", fnv1a("m.1") % mod);
  warnx("%d", fnv1a("t.1") % mod);
  warnx("%d", fnv1a("b.2") % mod);
  warnx("%d", fnv1a("a.2") % mod);
  warnx("%d", fnv1a("d.2") % mod);
  warnx("%d", fnv1a("s.2") % mod);
  warnx("%d", fnv1a("r.2") % mod);
  warnx("%d", fnv1a("i.2") % mod);
  warnx("%d", fnv1a("v.2") % mod);
  warnx("%d", fnv1a("u.2") % mod);
  warnx("%d", fnv1a("m.2") % mod);
  warnx("%d", fnv1a("t.2") % mod);
  warnx("%d", fnv1a("b.3") % mod);
  warnx("%d", fnv1a("a.3") % mod);
  warnx("%d", fnv1a("d.3") % mod);
  warnx("%d", fnv1a("s.3") % mod);
  warnx("%d", fnv1a("r.3") % mod);
  warnx("%d", fnv1a("i.3") % mod);
  warnx("%d", fnv1a("v.3") % mod);
  warnx("%d", fnv1a("u.3") % mod);
  warnx("%d", fnv1a("m.3") % mod);
  warnx("%d", fnv1a("t.3") % mod);
  warnx("%d", fnv1a("f:1") % mod);
  warnx("%d", fnv1a("a:1") % mod);
  warnx("%d", fnv1a("w:1") % mod);
  warnx("%d", fnv1a("p:1") % mod);
  warnx("%d", fnv1a("i:1") % mod);
  warnx("%d", fnv1a("r:1") % mod);
  warnx("%d", fnv1a("t:1") % mod);
  warnx("%d", fnv1a("l:1") % mod);
  warnx("%d", fnv1a("s:1") % mod);
  warnx("%d", fnv1a("f:2") % mod);
  warnx("%d", fnv1a("k:2") % mod);
  warnx("%d", fnv1a("w:2") % mod);
  warnx("%d", fnv1a("p:2") % mod);
  warnx("%d", fnv1a("i:2") % mod);
  warnx("%d", fnv1a("r:2") % mod);
  warnx("%d", fnv1a("t:2") % mod);
  warnx("%d", fnv1a("l:2") % mod);
  warnx("%d", fnv1a("s:2") % mod);
  for (int i = 0; i < 256; i++) {
    warnx("%d\t%s", i, NRPN_CMDS[i].name);
  }
}
