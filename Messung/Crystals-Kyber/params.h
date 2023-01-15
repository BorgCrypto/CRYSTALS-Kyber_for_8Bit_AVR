/**
 * This Code is based on and contains parts of:
 * 
 * The Reference Implementation of CRYSTALS-KYBER
 * URL: https://github.com/pq-crystals/kyber/tree/round3
 * License of used code: Public Domain (https://creativecommons.org/share-your-work/public-domain/cc0/)
 * 
 * Post-quantum crypto library for the ARM Cortex-M4
 * By Matthias J. Kannwischer and Richard Petri and Joost Rijneveld and Peter Schwabe and Ko Stoffelen
 * URL: https://github.com/mupq/pqm4
 * License of used code: CC0 1.0 Universal (CC0 1.0) Public Domain Dedication (https://creativecommons.org/publicdomain/zero/1.0/)
 * 
 * and own code (adjustments and extensions) under MIT-License.
*/
#ifndef PARAMS_H
#define PARAMS_H

#ifndef KYBER_K
#define KYBER_K 2	/* Change this for different security strengths */
#endif

#ifndef SECRET_KEY_VARIANT
#define SECRET_KEY_VARIANT 0 //change to use different Secret-Key-Variants
#endif

#ifndef Use_Deterministic_RNG
#define Use_Deterministic_RNG 0 //change to indicate if a deterministic RNG shall be used
#endif

/* Don't change parameters below this line */
#define KYBER_N 256
#define KYBER_Q 3329

#define KYBER_SYMBYTES 32   /* size in bytes of hashes, and seeds */
#define KYBER_SSBYTES  32   /* size in bytes of shared key */

#define KYBER_POLYBYTES              384
#define KYBER_POLYVECBYTES           (KYBER_K * KYBER_POLYBYTES)

#if KYBER_K == 2
#define KYBER_ETA1 3
#define KYBER_POLYCOMPRESSEDBYTES    128
#define KYBER_POLYVECCOMPRESSEDBYTES (KYBER_K * 320)
#elif KYBER_K == 3
#define KYBER_ETA1 2
#define KYBER_POLYCOMPRESSEDBYTES    128
#define KYBER_POLYVECCOMPRESSEDBYTES (KYBER_K * 320)
#elif KYBER_K == 4
#define KYBER_ETA1 2
#define KYBER_POLYCOMPRESSEDBYTES    160
#define KYBER_POLYVECCOMPRESSEDBYTES (KYBER_K * 352)
#endif

#define KYBER_ETA2 2

#define KYBER_INDCPA_MSGBYTES       KYBER_SYMBYTES
#define KYBER_INDCPA_PUBLICKEYBYTES (KYBER_POLYVECBYTES + KYBER_SYMBYTES)
#define KYBER_INDCPA_BYTES          (KYBER_POLYVECCOMPRESSEDBYTES + KYBER_POLYCOMPRESSEDBYTES)

#define KYBER_PUBLICKEYBYTES  (KYBER_INDCPA_PUBLICKEYBYTES)
#define KYBER_CIPHERTEXTBYTES  KYBER_INDCPA_BYTES

#if (SECRET_KEY_VARIANT==0)
#define KYBER_INDCPA_SECRETKEYBYTES (KYBER_POLYVECBYTES)
#define KYBER_SECRETKEYBYTES  (KYBER_INDCPA_SECRETKEYBYTES +  KYBER_INDCPA_PUBLICKEYBYTES + 2*KYBER_SYMBYTES) /* 32 bytes of additional space to save H(pk) */
#elif (SECRET_KEY_VARIANT==1)
#define KYBER_INDCPA_SECRETKEYBYTES (KYBER_POLYVECBYTES)
#define KYBER_SECRETKEYBYTES    (KYBER_INDCPA_SECRETKEYBYTES + KYBER_POLYVECBYTES* KYBER_K + KYBER_INDCPA_PUBLICKEYBYTES + 2* KYBER_SYMBYTES)
#elif (SECRET_KEY_VARIANT==2)
#define KYBER_INDCPA_SECRETKEYBYTES (KYBER_POLYVECBYTES)
#define KYBER_SECRETKEYBYTES    (KYBER_INDCPA_SECRETKEYBYTES + KYBER_INDCPA_PUBLICKEYBYTES + KYBER_SYMBYTES)
#elif (SECRET_KEY_VARIANT==3)
#define KYBER_INDCPA_SECRETKEYBYTES (KYBER_POLYVECBYTES)
#define KYBER_SECRETKEYBYTES  (KYBER_INDCPA_SECRETKEYBYTES + 3 * KYBER_SYMBYTES)
#elif (SECRET_KEY_VARIANT==4)
#define KYBER_INDCPA_SECRETKEYBYTES (KYBER_K*96)
#define KYBER_SECRETKEYBYTES (KYBER_INDCPA_SECRETKEYBYTES + KYBER_INDCPA_PUBLICKEYBYTES + KYBER_SYMBYTES)
#elif (SECRET_KEY_VARIANT==5)
#define KYBER_INDCPA_SECRETKEYBYTES (KYBER_K*96)
#define KYBER_SECRETKEYBYTES (KYBER_INDCPA_SECRETKEYBYTES + 3 * KYBER_SYMBYTES)
#elif (SECRET_KEY_VARIANT==6)
#define KYBER_INDCPA_SECRETKEYBYTES KYBER_SYMBYTES
#define KYBER_SECRETKEYBYTES (KYBER_SYMBYTES)
#elif (SECRET_KEY_VARIANT==7)
#define KYBER_INDCPA_SECRETKEYBYTES KYBER_SYMBYTES
#define KYBER_SECRETKEYBYTES (2 * KYBER_SYMBYTES)
#elif (SECRET_KEY_VARIANT==8)
#define KYBER_INDCPA_SECRETKEYBYTES KYBER_SYMBYTES
#define KYBER_SECRETKEYBYTES (2 * KYBER_SYMBYTES + KYBER_INDCPA_PUBLICKEYBYTES)
#endif 
#endif
