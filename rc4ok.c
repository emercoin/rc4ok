#include <rc4ok.h>

/*-----------------------------------------------------------------------------*/
// Key Scheduling Algorithm
// Inits rc4ok context [ctx] with byte-string [p] length [n]
void rc4ok_ksa(rc4ok *ctx, const uint8_t *p, int n) {
    uint8_t i = 0;
    uint8_t j = 0;
    ctx->i = ctx->j32 = 0;
    do {
        j += 233;
        ctx->S[i] = j;
    } while(++i);
    // After 256 iterations here, both (i, j) == 0 again
    do {
        j += ctx->S[i] + p[i % n];
        uint8_t x = ctx->S[i]; ctx->S[i] = ctx->S[j]; ctx->S[j] = x;
    } while(++i);
    i = ctx->S[j ^ 0x55]; // Randomize i
    uint8_t dummy[0x100]; // 256 empty iterations for remix S-block
    rc4ok_prng(ctx,  dummy, sizeof(dummy));
} // rc4ok_ksa

/*-----------------------------------------------------------------------------*/
// Preudo-Random Numbers generator
// Based on [ctx], generates sequence of pdeudo-random bytes length[n],
// and deploy it by pointer [p]
void rc4ok_prng(rc4ok *ctx, uint8_t *p, int n) {
    while(n--) {
        uint8_t x = ctx->S[ctx->i += 11];
        ctx->j32 = ((ctx->j32 << 1) | (ctx->j32 >> 31)) + x;
        uint8_t j = ctx->j32;
        uint8_t y = ctx->S[j];
        ctx->S[j] = x;
        ctx->S[ctx->i] = y;
        x += y;
        *p++ = ctx->S[x];
    } // while
} // rc4ok_prng

/*-----------------------------------------------------------------------------*/
// Entropy mixer
// Adds 16-bit physical entrpy value [x] to rc4ok context [ctx]
void rc4ok_addentropy(rc4ok *ctx, uint16_t x) {
    // ROL high half-word of j32, and add entropy
#if   __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint16_t *pj16 = (uint16_t *)&ctx->j32 + 1; // Ptr to high half-word of j32
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    uint16_t *pj16 = (uint16_t *)&ctx->j32;     // Ptr to high half-word of j32
#else
#error "Unknown Endian, please correct"
#endif
    *pj16 = ((*pj16 << 1) | (*pj16 >> 15)) + x;
} // rc4ok_addentropy

