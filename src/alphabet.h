#pragma once

#define SYSEX_START 240
#define SYSEX_MANUFACTURER_ID 16
#define SYSEX_DEVICE_ID 6
#define SYSEX_CMD_PARAMETER 6
#define SYSEX_CMD_MATRIX 11
#define SYSEX_NIBBLE_MASK 15
#define SYSEX_CHECKSUM_END 127
#define SYSEX_END 247


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
7   b1 64 .. 67  dco1 pitch bend/vibrato engaged (bitfield)
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

filter → CAPS
20  _  64 .. 127 balance between dco1/2 pre-filter
21  _  0  .. 127 vcf cutoff
22  _  1  .. 127 vcf mod by env
23  _  1  .. 127 vcf mod by channel aftertouch
24  _  63 .. 127 vcf resonance
25  _  64 .. 67  vcf bend/vibrato engaged (bitfield)
26  _  64 .. 67  vcf portamento/keyboard track engaged (bitfield)
27  _  64 .. 127 vca1 level
28  _  65 .. 127 vca1 mod by velocity
29  _  65 .. 127 vca2 mod by env2
30  _  64 .. 127 vcf fm
31  _  1  .. 127 vcf fm mod by env3
32  _  1  .. 127 vcf fm mod by channel aftertouch

control → @
33  _  65 .. 84  tracking generator source
34  _  64 .. 127 tracking point 1
35  _  64 .. 127 tracking point 2
36  _  64 .. 127 tracking point 3
38  _  64 .. 127 tracking point 4
38  _  64 .. 127 tracking point 5
39  _
40  _  64 .. 127 ramp1 rate
41  _  64 .. 66  ramp1 mode
42  _  64 .. 127 ramp2 rate
43  _  64 .. 66  ramp2 mode
44  _  64 .. 127 portamento
45  _  1  .. 127 portamento mod by velocity
47  _  64 .. 65  portamento legato engaged
48  _  64 .. 67  voice assign mode
49  _

env → .
50  _  64 .. 127 env1 delay
51  _  64 .. 127 env1 attack
52  _  64 .. 127 env1 decay
53  _  64 .. 127 env1 sustain
54  _  64 .. 127 env1 release
55  _  64 .. 127 env1 amplitude
56  _  1  .. 127 env1 amplitude mod by velocity
57  _  64 .. 67  env1 multi-trigger mode
58  _  64 .. 67  env1 mode
59  _  64 .. 67  env1 trigger mode
60  _  64 .. 127 env2 delay
61  _  64 .. 127 env2 attack
62  _  64 .. 127 env2 decay
63  _  64 .. 127 env2 sustain
64  _  64 .. 127 env2 release
65  _  64 .. 127 env2 amplitude
66  _  1  .. 127 env2 amplitude mod by velocity
67  _  64 .. 67  env2 multi-trigger mode
68  _  64 .. 67  env2 mode
69  _  64 .. 67  env2 trigger mode
70  _  64 .. 127 env3 delay
71  _  64 .. 127 env3 attack
72  _  64 .. 127 env3 decay
73  _  64 .. 127 env3 sustain
74  _  64 .. 127 env3 release
75  _  64 .. 127 env3 amplitude
76  _  1  .. 127 env3 amplitude mod by velocity
77  _  64 .. 67  env3 multi-trigger mode
78  _  64 .. 67  env3 mode
79  _  64 .. 67  env3 trigger mode

lfo → :
80  _  64 .. 127 lfo1 freq
81  _  1  .. 127 lfo1 mod by channel aftertouch
82  _  64 .. 71  lfo1 waveform
83  _  64 .. 95  lfo1 retrigger point
84  _  64 .. 127 lfo1 amplitude
85  _  1  .. 127 lfo1 amplitude modulation by ramp1
86  _  64 .. 66  lfo1 trigger mode
87  _  64 .. 65  lfo1 lag engaged
88  _  65 .. 84  lfo1 sample and hold source
89  _ 
90  _  64 .. 127 lfo2 freq
91  _  1  .. 127 lfo2 mod by keyboard (?)
92  _  64 .. 71  lfo2 waveform
93  _  64 .. 95  lfo2 retrigger point
94  _  64 .. 127 lfo2 amplitude
95  _  1  .. 127 lfo2 amplitude modulation by ramp2
96  _  64 .. 66  lfo2 trigger mode
97  _  64 .. 65  lfo2 lag engaged
98  _  65 .. 84  lfo2 sample and hold source

modulation sources

0 
1  env1
2  env2
3  env3
4  lfo1
5  lfo2
6  vibrato
7  ramp1
8  ramp2
9  keyboard
10 portamento
11 tracking generator
12 keyboard gate
13 velocity
14 release velocity
15 channel aftertouch
16 pedal1
17 pedal2
18 pitch bend
19 CC A (user configurable, usually mod wheel)
20 CC B (user configuarable, usually breath)


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
