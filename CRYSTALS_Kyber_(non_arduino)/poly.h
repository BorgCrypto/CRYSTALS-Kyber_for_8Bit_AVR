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
#ifndef POLY_H
#define POLY_H

#include "params.h"
#include "ntt.h"
#include "symmetric.h"
#include "reduce.h"
#include <stdint.h>

#define poly_getnoise_eta1(p, seed, nonce) poly_noise_eta1(p, seed, nonce, 0)
#define poly_getnoise_eta2(p, seed, nonce) poly_noise_eta2(p, seed, nonce, 0)
#define poly_addnoise_eta1(p, seed, nonce) poly_noise_eta1(p, seed, nonce, 1)
#define poly_addnoise_eta2(p, seed, nonce) poly_noise_eta2(p, seed, nonce, 1)


/*
 * Elements of R_q = Z_q[X]/(X^n + 1). Represents polynomial
 * coeffs[0] + X*coeffs[1] + X^2*xoeffs[2] + ... + X^{n-1}*coeffs[n-1]
 */
typedef struct {
    int16_t coeffs[KYBER_N];
} poly;

void poly_compress(unsigned char *r, poly *a);
void poly_decompress(poly *r, const unsigned char *a);

void poly_packcompress(unsigned char *r, poly *a, int i);
void poly_unpackdecompress(poly *r, const unsigned char *a, int i);

int cmp_poly_compress(const unsigned char *r, poly *a);
int cmp_poly_packcompress(const unsigned char *r, poly *a, int i);

void poly_tobytes(unsigned char *r, poly *a);
void poly_frombytes(poly *r, const unsigned char *a);
void poly_frombytes_mul(poly *r, const poly *b, const unsigned char *a);
void poly_frombytes_mul_acc(poly *r, const poly *b, const unsigned char *a);

void poly_frommsg(poly *r, const unsigned char msg[KYBER_SYMBYTES]);
void poly_tomsg(unsigned char msg[KYBER_SYMBYTES], poly *a);

void poly_noise_eta1(poly *r, const unsigned char *seed, unsigned char nonce, int add);
void poly_noise_eta2(poly *r, const unsigned char *seed, unsigned char nonce, int add);

void poly_ntt(poly *r);
void poly_invntt(poly *r);
void poly_basemul(poly *r, const poly *a, const poly *b);
void poly_basemul_acc(poly *r, const poly *a, const poly *b);
void poly_frommont(poly *r);

void poly_reduce(poly *r);

void poly_add(poly *r, const poly *a, const poly *b);
void poly_add_pointwise(int16_t *r, const int16_t *a, const int16_t *b);
void poly_sub(poly *r, const poly *a, const poly *b);

void poly_zeroize(poly *p);

#endif
