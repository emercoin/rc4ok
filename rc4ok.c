#include <rc4ok.h>

/*-----------------------------------------------------------------------------*/
// Key Scheduling Algorithm
// Inits rc4ok context [ctx] with byte-string [p] length [n]
void rc4ok_ksa(rc4ok *ctx, const uint8_t *p, uint32_t n) {
    uint8_t i = 0;
    uint8_t j = 0;
    do {
        j += 233;
        ctx->S[i] = j;
    } while(++i);
    // After 256 iterations here, both (i, j) == 0 again
    // We must process both: entire S-block[256] and entire key
    int k, k_len = (n > 256)? n : 256;
    for(k = 0; k < k_len; k++) {
        j += ctx->S[i] + p[k % n];
        uint8_t x = ctx->S[i]; ctx->S[i] = ctx->S[j]; ctx->S[j] = x;
        i++;
    }
    // Init (i, j) fpr PRNG context
    ctx->i = ctx->S[j ^ 0x55]; // Randomize PRNG i
    ctx->j32 = 0;              // Clear PRNG j
    // 256 empty iterations for initial remix S-block
    uint8_t dummy[0x100];
    rc4ok_prng(ctx,  dummy, sizeof(dummy));
} // rc4ok_ksa

/*-----------------------------------------------------------------------------*/
// Preudo-Random Numbers generator
// Based on [ctx], generates sequence of pdeudo-random bytes length[n],
// and deploys it by pointer [p]
void rc4ok_prng(rc4ok *ctx, uint8_t *p, uint32_t n) {
    uint8_t *p_end = p + n;
    while(p < p_end) {
        uint8_t    i = ctx->i += 11;
        uint32_t j32 = ctx->j32;
        uint8_t x = ctx->S[i];
        j32 = ((j32 << 1) | (j32 >> 31)) + x;
        ctx->j32  = j32;
        uint8_t j = j32;
        uint8_t y = ctx->S[j];
        ctx->S[j] = x;
        ctx->S[i] = y;
        *p++ = ctx->S[(uint8_t)(x + y)];
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

