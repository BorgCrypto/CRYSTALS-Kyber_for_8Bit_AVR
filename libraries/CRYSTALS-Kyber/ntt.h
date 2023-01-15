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
 * 
*/
#ifndef NTT_H
#define NTT_H

#include <stdint.h>

extern const int16_t zetas[128];
void ntt(int16_t *poly);
void invntt(int16_t *poly);
void basemul(int16_t r[2], const int16_t a[2], const int16_t b[2], int16_t zeta);

#endif
