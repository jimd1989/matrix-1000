#pragma once

#include <stdint.h>

#define SYSEX_START 240
#define SYSEX_MANUFACTURER_ID 16
#define SYSEX_DEVICE_ID 6
#define SYSEX_CMD_PARAMETER 6
#define SYSEX_CMD_MATRIX 11
#define SYSEX_NIBBLE_MASK 15
#define SYSEX_CHECKSUM_END 127
#define SYSEX_END 247

typedef uint32_t Hash;

typedef struct NrpnCmd {
  char    * name;  /* shell cmd */
  uint8_t   param;  /* matrix-1000 parameter number */
  int8_t    min;    /* user-facing min value (negative possible) */
  int8_t    max;    /* user-facing max value */
  int8_t    offset; /* offset needed to encode user-facing values ∈ [0, 127] */
} NrpnCmd;

void nrpnCmds();
NrpnCmd lookupNrpnCmd(char *);
void hashTest();

/* 

NRPN parameters

pitch → naked
0   p1 64 .. 127 dco1 freq
1   l1 1  .. 127 dco1 freq mod by lfo1
2   y  64 .. 67  dco sync
3   q1 64 .. 127 dco1 pulse width
4   x1 1  .. 127 dco1 pulse width mod by lfo2
5   z1 64 .. 127 dco1 triangle waveshape
6   w1 64 .. 67  dco1 waveform
7   v1 64 .. 67  dco1 pitch bend/vibrato engaged (bitfield)
8   m1 64 .. 67  dco1 portamento engaged (bitfield)
9   k1 64 .. 67  dco1 click engaged
10  p2 64 .. 127 dco2 freq
11  l2 1  .. 127 dco2 freq mod by lfo1
12  t  33 .. 95  dco2 detune
13  q2 64 .. 127 dco2 pulse width
14  x2 1  .. 127 dco2 pulse width mod by lfo2
15  z2 64 .. 127 dco2 triangle waveshape
16  w2 64 .. 67  dco2 waveform
17  b2 64 .. 67  dco2 pitch bend/vibrato engaged (bitfield)
18  m2 64 .. 67  dco2 portamento engaged (bitfield)
19  k2 64 .. 67  dco2 click engaged

filter/VCA → naked CAPS
20  B  64 .. 127 balance between dco1/2 pre-filter
21  C  0  .. 127 vcf cutoff
22  E  1  .. 127 vcf mod by env
23  A  1  .. 127 vcf mod by channel aftertouch
24  R  63 .. 127 vcf resonance
25  V  64 .. 67  vcf bend/vibrato engaged (bitfield)
26  M  64 .. 67  vcf portamento/keyboard track engaged (bitfield)
27  I  64 .. 127 vca1 level
28  U  65 .. 127 vca1 mod by velocity
29  F  65 .. 127 vca2 mod by env2
30  Q  64 .. 127 vcf fm
31  X  1  .. 127 vcf fm mod by env3
32  T  1  .. 127 vcf fm mod by channel aftertouch

control → @
33  s@  65 .. 84  tracking generator source
34  t@1 64 .. 127 tracking point 1
35  t@2 64 .. 127 tracking point 2
36  t@3 64 .. 127 tracking point 3
38  t@4 64 .. 127 tracking point 4
38  t@5 64 .. 127 tracking point 5
39  _
40  r@1 64 .. 127 ramp1 rate
41  n@1 64 .. 66  ramp1 mode
42  r@2 64 .. 127 ramp2 rate
43  n@1 64 .. 66  ramp2 mode
44  m@  64 .. 127 portamento
45  v@  1  .. 127 portamento mod by velocity
47  l@  64 .. 65  portamento legato engaged
48  a@  64 .. 67  voice assign mode
49  _

env → .
50  b.1 64 .. 127 env1 delay
51  a.1 64 .. 127 env1 attack
52  d.1 64 .. 127 env1 decay
53  s.1 64 .. 127 env1 sustain
54  r.1 64 .. 127 env1 release
55  i.1 64 .. 127 env1 amplitude
56  v.1 1  .. 127 env1 amplitude mod by velocity
57  u.1 64 .. 67  env1 multi-trigger mode
58  m.1 64 .. 67  env1 mode
59  t.1 64 .. 67  env1 trigger mode
60  b.2 64 .. 127 env2 delay
61  a.2 64 .. 127 env2 attack
62  d.2 64 .. 127 env2 decay
63  s.2 64 .. 127 env2 sustain
64  r.2 64 .. 127 env2 release
65  i.2 64 .. 127 env2 amplitude
66  v.2 1  .. 127 env2 amplitude mod by velocity
67  u.2 64 .. 67  env2 multi-trigger mode
68  m.2 64 .. 67  env2 mode
69  t.2 64 .. 67  env2 trigger mode
70  b.3 64 .. 127 env3 delay
71  a.3 64 .. 127 env3 attack
72  d.3 64 .. 127 env3 decay
73  s.3 64 .. 127 env3 sustain
74  r.3 64 .. 127 env3 release
75  i.3 64 .. 127 env3 amplitude
76  v.3 1  .. 127 env3 amplitude mod by velocity
77  u.3 64 .. 67  env3 multi-trigger mode
78  m.3 64 .. 67  env3 mode
79  t.3 64 .. 67  env3 trigger mode

lfo → :
80  f:1 64 .. 127 lfo1 freq
81  a:1 1  .. 127 lfo1 mod by channel aftertouch
82  w:1 64 .. 71  lfo1 waveform
83  p:1 64 .. 95  lfo1 retrigger point
84  i:1 64 .. 127 lfo1 amplitude
85  r:1 1  .. 127 lfo1 amplitude modulation by ramp1
86  t:1 64 .. 66  lfo1 trigger mode
87  l:1 64 .. 65  lfo1 lag engaged
88  s:1 65 .. 84  lfo1 sample and hold source
89  _ 
90  f:2 64 .. 127 lfo2 freq
91  k:2 1  .. 127 lfo2 mod by keyboard (?)
92  w:2 64 .. 71  lfo2 waveform
93  p:2 64 .. 95  lfo2 retrigger point
94  i:2 64 .. 127 lfo2 amplitude
95  r:2 1  .. 127 lfo2 amplitude modulation by ramp2
96  t:2 64 .. 66  lfo2 trigger mode
97  l:2 64 .. 65  lfo2 lag engaged
98  s:2 65 .. 84  lfo2 sample and hold source

modulation sources → $

0 
1  $e1 env1
2  $e2 env2
3  $e3 env3
4  $l1 lfo1
5  $l2 lfo2
6  $v  vibrato
7  $r1 ramp1
8  $r2 ramp2
9  $k  keyboard
10 $m  portamento
11 $t  tracking generator
12 $g  keyboard gate
13 $i  velocity
14 $r  release velocity
15 $a  channel aftertouch
16 $p1 pedal1
17 $p2 pedal2
18 $b  pitch bend
19 $c1 CC A (user configurable, usually mod wheel)
20 $c2 CC B (user configuarable, usually breath)

mod matrix sysex ­ rough notes
from http://www.synthzone.com/midi/oberheim/matrix1000/Oberheim-Matrix6R.txt
from https://youngmonkey.ca/nose/audio_tech/synth/Oberheim-Matrix1000.html

- All "data" in patch and parameter dumps is unpacked for transmission.
  The general algorithm for transmission is: 
  
  1: Transmit header and messages code(s).
  2: Clear checksum.
  3. Fetch next data byte to be transmitted.
  4: Add byte to checksum.
  5: Transmit (data & 0FH).
  6: Transmit (data/16 & 0FH).
  7: Repeat 3-6 for all data bytes.
  8: Transmit (checksum & 07FH).
  9: Transmit F7H.

F0H 240 sysex start
10H 16  oberheim id
06H 6   matrix 1000 id
0BH 11  matrix edit command
    0 .. 9  matrix slot
    0 .. 20 modulation source
    0 .. 127 modulation amount
    0 .. 32 modulation destination
F7H 247 sysex end

*/
