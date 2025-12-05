(import (chicken bitwise) (chicken condition) (chicken file posix) 
        (chicken foreign) (chicken io) (chicken process-context)
        (chicken string) (srfi 1) (srfi 4))

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

void init_midi(char *out, char *in, uint8_t chan) {
  POLL_FDS[STDIN_IX].fd = STDIN_FILENO;
  POLL_FDS[STDIN_IX].events = POLLIN;
  MIDI_OUT = mio_open(out, MIO_OUT, false);
  if (MIDI_OUT == NULL) { errx(1, "MIDI output not found: %s", out); }
  if (in != NULL) {
    MIDI_IN = mio_open(in, MIO_IN, true);
    if (MIDI_IN == NULL) { errx(1, "MIDI input not found: %s", in); }
    mio_pollfd(MIDI_IN, &POLL_FDS[MIDI_IN_IX], POLLIN);
  }
  warnx("chan: %d\tin: %s\tout: %s", chan + 1, in != NULL ? in : "∅", out);
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
(define-syntax either
  (syntax-rules ()
    ((_ f ...)
     (∃ ((ω (condition-case ((λ () f ...))
              (e (exn) (left (get-condition-property e 'exn 'message))))))
       (? (left? ω) ω (right ω))))))
(define-syntax for
  (syntax-rules (←)
    ((_ (← α β) ω Ω ...) (>>= (λ (α) (for ω Ω ...)) β))
    ((_ (  α β) ω Ω ...) (>>= (λ (α) (for ω Ω ...)) (right β)))
    ((_               ω) (right ω))))

(← ↑ car) (← ↓ cdr) (← ↑↓ cadr) (← ∘ compose) (← ≡ equal?) (← ∅ '()) 
(← ∅? null?) (← ρ length) (← ◇ conc) (← ⊂ cons) (← ∀ map) (← ∀∀ for-each)
(← $ apply)
(← (∧ ω α) (and ω α))
(← (∨ ω α) (or ω α))
(← (I ω) ω)
(← (K ω) (λ (α) ω))
(← (C f) (λ (ω) (λ (α) (f ω α))))
(← (S g f) (λ (ω) (g ω (f ω))))
(← (J h g f) (λ (ω) (h (g ω) (f ω))))
(← (right ω) `(R ,ω))
(← (left ω) `(L ,ω))
(← (right? ω) (? (list? ω) (? (≡ 2 (ρ ω)) (≡ 'R (↑ ω)) #f) #f))
(← (left? ω) (? (list? ω) (? (≡ 2 (ρ ω)) (≡ 'L (↑ ω)) #f) #f))
(← (either? ω) (? ((J ∨ left? right?) ω) ω (left (◇ "not an either: " ω))))
(← (get Fω) (? (right? Fω) (↑↓ Fω) (error (↑↓ Fω))))
(← (get-or ω Fω) (? (right? Fω) (↑↓ Fω) ω))
(← (⊙ f Fω) (∃ ((α (either? Fω))) (? (right? α) (right (f (↑↓ α))) α)))
(← (_⊙ f Fω) (∃ ((α (either? Fω))) (? (left? α) (left (f (↑↓ α))) α)))
(← (⊙⊙ f g Fω) (⊙ f (_⊙ g Fω)))
(← (⊥ Fω) (either (get (get Fω))))
(← (>>= f Fω) (⊥ (⊙ f Fω)))
(← (● Ff Fω) (>>= (λ (ω) (⊙ (λ (f) (f ω)) Ff)) Fω))
(← (*> Fω Fα) (>>= (K Fα) Fω))
(← ($> Fω α) (*> Fω (right α)))
(← (lift2 f Fα Fω) (● (⊙ (C f) Fα) Fω))
(← (sequence Fω) (foldr (λ (x acc) (lift2 ⊂ x acc)) (right ∅) Fω))
(← (brk △ Fω) (? (left? Fω) (△ Fω) Fω))
(← (traverse f Fω) (call/cc (λ (△) (sequence (∀ (∘ ((C brk) △) f) Fω)))))
(← (ι n Fω) (either (list-ref Fω n)))
(← (ensure p e ω) (? p (right ω) (left e)))
(← (s⊥ f e ω) (>>= (λ (α) (ensure α (◇ e ": " ω) α)) (either (f ω))))
(← (s⊥n ω) (s⊥ string->number "not number" ω))
(← (s⊥x ω) (s⊥ string->symbol "not symbol" ω))
(← (∈ ω k) (∃ ((α (assoc k ω))) (⊙ ↓ (ensure α (◇ "cmd not found: " k) α))))
(← (group n Fω) (? (∅? Fω) ∅ (⊂ `(,(take Fω n)) (group n (drop Fω n)))))
(← init-midi (foreign-lambda void "init_midi" c-string c-string int))
(← (init) (for (← ω (either (group 2 (command-line-arguments))))
               (chan-str (get-or "1" (∈ ω "chan")))
               (output-str (get-or "midi/0" (∈ ω "output")))
               (input-str (get-or #f (∈ ω "input")))
               (← chan (⊙ ((C +) -1) (s⊥n chan-str)))
               (init-midi output-str input-str chan)))
(← poll-input (foreign-lambda int "poll_input"))
(← forward-midi (foreign-lambda void "forward_midi"))
(← close-midi (foreign-lambda void "close_midi"))
(← (stdin-ready? n) (? (eq? (bitwise-and 1 n) 1) #t #f))
(← (midi-ready? n) (? (eq? (bitwise-and 1 (arithmetic-shift n -1)) 1) #t #f))
(← (read-stdin) (print (↑↓ (either (eval (read))))))
(← (midi-repl)
  (let ((signals (poll-input)))
    (? (stdin-ready? signals) (read-stdin))
    (? (midi-ready? signals) (forward-midi)))
  (midi-repl))

(define-syntax nrpn-cmds
  (syntax-rules ()
    ((_ ((cmd byte min max offset) ...))
     (begin (← (cmd ω) (for (← _ (ensure (>= ω min) (◇ ω " < " min) ∅))
                            (← _ (ensure (<= ω max) (◇ ω " > " max) ∅))
                            `(,byte ,(+ offset ω)))) ...))))

(nrpn-cmds ((p1 0   0 63 64)
            (l1 1 -63 63 64)
            (y  2   0  3 64)
            (q1 3   0 63 64)))

; main
(init)
(midi-repl)
(close-midi)
