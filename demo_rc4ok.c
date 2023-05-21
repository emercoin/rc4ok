#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <rc4ok.h>

/*-----------------------------------------------------------------------------*/
// Add entropy to rc4ok PGNG, every ~1s
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
    if(argc < 4) {
        fprintf(stderr, "RC4OK bytestream PRNG\n"
                "Run:\n\t%s gen_size_mb=\\d+ prng-seed-key=\\s+ use_entropy={-1(no output),0(no entropy),1(add entropy)}\n", argv[0]);
        return 0;
    }

    rc4ok prng;
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
    for(uint32_t mb_no = 0; mb_no < num_mb; mb_no++)
        for(int kb_no = 0; kb_no < 1024; kb_no++) {
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

