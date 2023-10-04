#pragma once
#include <stdint.h>

// Cryptographic-secure PRNG, based on RC4 algotithm, and improved by Oleg Khovayko
// Contais additinal functional: lock-free adding physical randomness (entropy)
// into PRNG state.

typedef struct {
    uint8_t           S[0x100]; // S-block
    volatile uint32_t j32;      // RC4 "j" and extra 1+2 bytes - entropy buffer
    uint8_t           i;        // RC4 "i"

} rc4ok;

// Key Scheduling Algorithm
// Inits rc4ok context [ctx] with byte-string [p] length [n]
void rc4ok_ksa(rc4ok *ctx, const uint8_t *p, uint32_t n);

// Preudo-Random Numbers generator
// Based on [ctx], generates sequence of pdeudo-random bytes length[n],
// and deploy it by pointer [p]
void rc4ok_prng(rc4ok *ctx, uint8_t *p, uint32_t n);

// Entropy mixer
// Adds 16-bit physical entrpy value [x] to rc4ok context [ctx]
void rc4ok_addentropy(rc4ok *ctx, uint16_t x);

