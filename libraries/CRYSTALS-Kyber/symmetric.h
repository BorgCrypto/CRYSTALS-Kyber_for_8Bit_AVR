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
#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include "params.h"
#include <stddef.h>
#include <SHAKE.h>
#include <SHA3.h>

typedef SHAKE128 xof_state;

void kyber_shake128_absorb(SHAKE128 *s, const unsigned char *input, unsigned char x, unsigned char y);
void kyber_shake128_squeezeblocks(unsigned char *output, size_t nblocks, SHAKE128 *s);
void shake256_prf(unsigned char *output, size_t outlen, const unsigned char *key, unsigned char nonce);
void shake256(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen);
void sha3_256(uint8_t h[32], const uint8_t *in, size_t inlen);
void sha3_512(uint8_t h[64], const uint8_t *in, size_t inlen);

#define hash_h(OUT, IN, INBYTES) sha3_256(OUT, IN, INBYTES)
#define hash_g(OUT, IN, INBYTES) sha3_512(OUT, IN, INBYTES)
#define xof_absorb(STATE, IN, X, Y) kyber_shake128_absorb(STATE, IN, X, Y)
#define xof_squeezeblocks(OUT, OUTBLOCKS, STATE) kyber_shake128_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define prf(OUT, OUTBYTES, KEY, NONCE) shake256_prf(OUT, OUTBYTES, KEY, NONCE)
#define kdf(OUT, IN, INBYTES) shake256(OUT, KYBER_SSBYTES, IN, INBYTES)
#define XOF_BLOCKBYTES 168

#endif /* SYMMETRIC_H */
