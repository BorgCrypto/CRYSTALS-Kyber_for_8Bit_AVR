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
#include "symmetric.h"
/*************************************************
* Name:        kyber_shake128_absorb
*
* Description: Absorb step of the SHAKE128 specialized for the Kyber context.
*
* Arguments:   - shake128ctx *s:                  pointer to (uninitialized) output Keccak state
*              - const unsigned char *input:      pointer to KYBER_SYMBYTES input to be absorbed into s
*              - unsigned char i                  additional byte of input
*              - unsigned char j                  additional byte of input
**************************************************/
void kyber_shake128_absorb(SHAKE128 *s, const unsigned char *input, unsigned char x, unsigned char y) {
    unsigned char extseed[KYBER_SYMBYTES + 2];
    int i;
    
    for (i = 0; i < KYBER_SYMBYTES; i++) {
        extseed[i] = input[i];
    }
    extseed[i++] = x;
    extseed[i]   = y;
    s->update(extseed,sizeof(extseed));
}

/*************************************************
* Name:        kyber_shake128_squeezeblocks
*
* Description: Squeeze step of SHAKE128 XOF. Squeezes full blocks of SHAKE128_RATE bytes each.
*              Modifies the state. Can be called multiple times to keep squeezing,
*              i.e., is incremental.
*
* Arguments:   - unsigned char *output:      pointer to output blocks
*              - size_t nblocks:             number of blocks to be squeezed (written to output)
*              - shake128ctx *s:            pointer to in/output Keccak state
**************************************************/
void kyber_shake128_squeezeblocks(unsigned char *output, size_t nblocks, SHAKE128 *s) {
    s->extend(output,XOF_BLOCKBYTES*nblocks);
}

/*************************************************
* Name:        shake256_prf
*
* Description: Usage of SHAKE256 as a PRF, concatenates secret and public input
*              and then generates outlen bytes of SHAKE256 output
*
* Arguments:   - unsigned char *output:      pointer to output
*              - size_t outlen:              number of requested output bytes
*              - const unsigned char * key:  pointer to the key (of length KYBER_SYMBYTES)
*              - const unsigned char nonce:  single-byte nonce (public PRF input)
**************************************************/
void shake256_prf(unsigned char *output, size_t outlen, const unsigned char *key, unsigned char nonce) {
    SHAKE256 s;
    unsigned char extkey[KYBER_SYMBYTES + 1];
    size_t i;

    for (i = 0; i < KYBER_SYMBYTES; i++) {
        extkey[i] = key[i];
    }
    extkey[i] = nonce;
    s.update(extkey,sizeof(extkey));
    s.extend(output,outlen);
}

void shake256(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen){
    SHAKE256 s;
    s.update(in,inlen);
    s.extend(out,outlen);
}

void sha3_256(uint8_t h[32], const uint8_t *in, size_t inlen){
    SHA3_256 s;
    s.update(in,inlen);
    s.finalize(h,32);
}

void sha3_512(uint8_t h[64], const uint8_t *in, size_t inlen){
    SHA3_512 s;
    s.update(in,inlen);
    s.finalize(h,64);
}