(import (chicken bitwise) (chicken condition) (chicken file posix) 
        (chicken foreign) (chicken io) (chicken string) (srfi 4))

#>
#include <err.h>
#include <poll.h>
#include <sndio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1024 /* hard-locked to 1024 in OpenBSD kernel */
#define STDIN_IX 0
#define MIDI_IN_IX 1
static char BUFFER[BUFSIZE] = {0};
static struct pollfd POLL_FDS[2] = {0};
static struct mio_hdl *MIDI_OUT;
static struct mio_hdl *MIDI_IN; /* optional */

void init_midi(char *midiIn) {
  POLL_FDS[STDIN_IX].fd = STDIN_FILENO;
  POLL_FDS[STDIN_IX].events = POLLIN;
  MIDI_OUT = mio_open(MIO_PORTANY, MIO_OUT, false);
  if (MIDI_OUT == NULL) { errx(1, "MIDI output not found: %s", MIO_PORTANY); }
  if (midiIn != NULL) {
    MIDI_IN = mio_open(midiIn, MIO_IN, true);
    if (MIDI_IN == NULL) { errx(1, "MIDI input not found: %s", midiIn); }
    mio_pollfd(MIDI_IN, &POLL_FDS[MIDI_IN_IX], POLLIN);
  }
}

int poll_input() {
  int ready_fds = 0;
  poll(POLL_FDS, 2, -1); 
  ready_fds |= POLL_FDS[STDIN_IX].revents & POLLIN;
  ready_fds |= (POLL_FDS[MIDI_IN_IX].revents & POLLIN) << 1;
  POLL_FDS[STDIN_IX].revents = 0;
  POLL_FDS[MIDI_IN_IX].revents = 0;
  return ready_fds;
}

void forward_midi() {
  int bytes_read = mio_read(MIDI_IN, BUFFER, BUFSIZE);
  mio_write(MIDI_OUT, BUFFER, bytes_read);
}

void close_midi() {
  mio_close(MIDI_OUT);
  if (MIDI_IN != NULL) { mio_close(MIDI_IN); }
}
<#

(define-syntax λ (syntax-rules () ((_ . ω) (lambda . ω))))
(define-syntax ? (syntax-rules () ((_ . ω) (if . ω))))
(define-syntax ← (syntax-rules () ((_ . ω) (define . ω))))
(define-syntax ∃ (syntax-rules () ((_ . ω) (let* . ω))))
(define-syntax maybe (syntax-rules ()
  ((_ f ...) (∃ ((ω (condition-case ((λ () f ...)) ((exn) 'NONE))))
               (? (eq? 'NONE ω) 'NONE `(SOME ,ω))))))
(define-syntax for (syntax-rules (⇐ ⇒)
  ((_ (⇐ α β) ω ...) (>>= (λ (α) (for ω ...)) β))
  ((_ (⇒ α β) ω ...) (>>= (λ (α) (for ω ...)) (some β)))
  ((_ ω) (some ω))))

(← NRPN-CMDS '((p1 0   0 63 64)
               (l1 1 -63 63 64)
               (y  2   0  3 64)
               (q1 3   0 63 64)))

(← init-midi (foreign-lambda void "init_midi" c-string))
(← poll-input (foreign-lambda int "poll_input"))
(← forward-midi (foreign-lambda void "forward_midi"))
(← close-midi (foreign-lambda void "close_midi"))
(← ρ length) (← ∘ compose) (← ↑ car) (← ↑↓ cadr) (← ↓ cdr) (← ◇ conc)
(← $ apply) (← ⊂ cons) (← ∀ map) (← s⊥xs string-split) (← s⊥n string->number)
(← s⊥x string->symbol)
(← (K ω) (λ (α) ω))
(← (C f) (λ (α) (λ (ω) (f α ω))))
(← (some ω) `(SOME ,ω))
(← none 'NONE)
(← (none? ω) (eq? none ω))
(← (maybe? ω) (? (or (none? ω) (and (list ω) (= 2 (ρ ω)))) ω none))
(← (some? ω) (and (maybe? ω) (not (none? ω))))
(← (⊙ f Fω) (∃ ((Fα (maybe? Fω))) (? (none? Fα) none (some (f (↑↓ Fα))))))
(← (⊥ Fω) (maybe (↑↓ (↑↓ Fω))))
(← (>>= f Fω) (⊥ (⊙ f Fω)))
(← (● Ff Fω) (>>= (λ (ω) (⊙ (λ (f) (f ω)) Ff)) Fω))
(← (lift f Fα Fω) (● (⊙ (C f) Fα) Fω))
(← (sequence ω) (foldr (λ (x acc) (lift ⊂ x acc)) (some '()) ω))
(← (traverse f ω) (sequence (map f ω)))
(← (guard p) (? p (some 'UNIT) none))
(← (ι n ω) (maybe (list-ref ω n)))
(← (stdin-ready? n) (? (eq? (bitwise-and 1 n) 1) #t #f))
(← (midi-ready? n) (? (eq? (bitwise-and 1 (arithmetic-shift n -1)) 1) #t #f))
(← (to-number ω) (⊙ (K (s⊥n ω)) (guard (s⊥n ω))))
(← (∈ ω α) (∃ ((c (assoc α ω))) (? c (maybe (↓ c)) none)))
(← (to-nrpn ω) (for (⇐ cmd (ι 0 ω))
                    (⇐ valid-cmd (∈ NRPN-CMDS (s⊥x cmd)))
                    (⇐ cmd-byte (ι 0 valid-cmd))
                    (⇐ min (ι 1 valid-cmd))
                    (⇐ max (ι 2 valid-cmd))
                    (⇐ offset (ι 3 valid-cmd))
                    (⇐ val (>>= to-number (ι 1 ω)))
                    (⇐ _ (guard (>= val min)))
                    (⇐ _ (guard (<= val max)))
                    `(,cmd-byte ,(+ offset val))))
(← (read-stdin) (traverse to-nrpn (∀ s⊥xs (s⊥xs (read-line) ";"))))

; main
(init-midi "midi/1")
(← (midi-repl)
  (let ((signals (poll-input)))
    (? (stdin-ready? signals) (print (read-stdin)))
    (? (midi-ready? signals) (forward-midi)))
(← (stdin-ready? n) (? (eq? (bitwise-and 1 n) 1) #t #f))
  (midi-repl))

(midi-repl)
(close-midi)
