#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <rc4ok.h>

/*-----------------------------------------------------------------------------*/
// Test key and output reference vector
const char G_algotest_key[] = "rc4ok-is-the-best";
const uint8_t G_algotest_refvec[] = {
    0x10, 0x4a, 0x1e, 0x8e, 0x59, 0xb3, 0x03, 0x67, 0x99, 0x33, 0x96, 0xb4, 0x60, 0x60, 0x16, 0x5a,
    0x7f, 0xd9, 0xe2, 0x71, 0xe8, 0x6e, 0x07, 0xf5, 0xa5, 0x18, 0xee, 0x40, 0x81, 0x96, 0x58, 0x4c,
    0x35, 0x67, 0x50, 0xbd, 0x3f, 0x17, 0x87, 0x40, 0x6d, 0x0f, 0x06, 0xcd, 0x8a, 0x0e, 0x82, 0x76,
    0x80, 0xba, 0xf8, 0x23, 0x2d, 0xf4, 0x6a, 0xcc, 0xfa, 0xce, 0x40, 0x1a, 0x95, 0x50, 0xe6, 0x92
};

/*-----------------------------------------------------------------------------*/
void *entropy(void *p) {
    uint16_t x = 0;
    rc4ok *q;
    while((q = *(rc4ok **)p) != NULL) {
        ++x;
        rc4ok_addentropy(q, x);
        usleep(x + 1000000); // every ~1s
    }
    return NULL;
} // entropy

/*-----------------------------------------------------------------------------*/
int main(int argc, char **argv) {
    rc4ok prng;
    if(argc < 4) {
        if(argc == 2 && strcmp(argv[1], "algotest") == 0) {
            rc4ok_ksa(&prng, (const uint8_t *)G_algotest_key, strlen(G_algotest_key));
            uint8_t buf[sizeof(G_algotest_refvec)];
            rc4ok_prng(&prng, buf, sizeof(buf));
            printf("%s: Algotest %s\n", argv[0], memcmp(G_algotest_refvec, buf, sizeof(G_algotest_refvec))? "FAILED!" : "Passed");
        } else
            fprintf(stderr, "RC4OK bytestream PRNG\n"
                "Run:\n\t%s gen_size_mb=\\d+ prng-seed-key=\\s+ use_entropy={-1(no output),0(no entropy),1(add entropy)}\n", argv[0]);
        return 0;
    }

    uint32_t num_mb = atoi(argv[1]); // Megabytes to out
    rc4ok_ksa(&prng, (const uint8_t *)argv[2], strlen(argv[2]));
    int use_entropy = atoi(argv[3]);

    pthread_t thr; // entropy anync thread
    rc4ok *prng_ent = &prng; // entropy anync thread ptr ro rc4ok instance

    if(use_entropy) {
        // Start entropy thread
        // create a second thread which executes entropy()
        if(pthread_create(&thr, NULL, entropy, &prng_ent)) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    } // if(use_entropy) - 1

    // Generete prng output num_mb megabytes
    uint32_t mb_no, kb_no;
    for(mb_no = 0; mb_no < num_mb; mb_no++)
        for(kb_no = 0; kb_no < 1024; kb_no++) {
            uint8_t outbuf[1024];
           rc4ok_prng(&prng, outbuf, sizeof(outbuf));
            if(use_entropy >= 0)
                fwrite(outbuf, sizeof(outbuf), 1, stdout);
        }

    if(use_entropy) {
        // Shutdown entropy thread
        prng_ent = NULL; // Signal to thread to stop
        /* wait for the second thread to finish */
        if(pthread_join(thr, NULL)) {
            fprintf(stderr, "Error joining thread\n");
            return 2;
        }
    } // if(use_entropy) - 2

    return 0;
} // main

