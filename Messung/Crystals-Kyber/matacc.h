#ifndef MATACC_H
#define MATACC_H
#include "poly.h"
#include "polyvec.h"
#include "symmetric.h"
#include "ntt.h"


void matacc_asm(int16_t *r, const int16_t *b, int16_t c[4], unsigned char buf[XOF_BLOCKBYTES+2], const int16_t zeta[128], xof_state *state);
void matacc_asm_acc(int16_t *r, const int16_t *b, int16_t c[4], unsigned char buf[XOF_BLOCKBYTES+2], const int16_t zeta[128], xof_state *state);
void matacc(poly* r, const polyvec *b, unsigned char i, const unsigned char *seed, int transposed);
#if(SECRET_KEY_VARIANT==1)
void matacc_save_A(poly* r,polyvec *a, const polyvec *b, unsigned char i, const unsigned char *seed, int transposed);
void matacc_asm_save_A(int16_t *coeffs,int16_t *a, const int16_t *b, int16_t c[4],unsigned char buf[XOF_BLOCKBYTES+2], const int16_t zeta[128], xof_state *state);
void matacc_asm_acc_save_A(int16_t *coeffs,int16_t *a, const int16_t *b, int16_t c[4], unsigned char buf[XOF_BLOCKBYTES+2], const int16_t zeta[128], xof_state *state);
#endif
#endif