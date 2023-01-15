#ifndef INDCPA_H
#define INDCPA_H

#include "params.h"

#if (SECRET_KEY_VARIANT==3) || (SECRET_KEY_VARIANT==6) || (SECRET_KEY_VARIANT==7)
void genPkFromSeeds(unsigned char *out,const unsigned char *sk, const unsigned char *publicseed, const unsigned char *noiseseed);
#endif
#if SECRET_KEY_VARIANT==5
void genPkFromSeeds_and_dec(unsigned char *out,const unsigned char *sk, const unsigned char *publicseed, const unsigned char *noiseseed, unsigned char *m, const unsigned char *c);
#endif
#if(SECRET_KEY_VARIANT==6) || (SECRET_KEY_VARIANT==7)
void genSKFromSeed(unsigned char *sk, const unsigned char *noiseseed);
#endif
void indcpa_keypair(unsigned char *pk,unsigned char *sk);

void indcpa_enc(unsigned char *c,const unsigned char *m,const unsigned char *pk,const unsigned char *coins);

#if(SECRET_KEY_VARIANT==1)
unsigned char indcpa_enc_cmp(const unsigned char *c,
                             const unsigned char *m,
                             const unsigned char *pk,
                             const unsigned char *coins,
                             const unsigned char *a
                             );
#else
unsigned char indcpa_enc_cmp(const unsigned char *ct,const unsigned char *m,const unsigned char *pk,const unsigned char *coins);
#endif

#if (SECRET_KEY_VARIANT==4) || (SECRET_KEY_VARIANT==5)
#include <stdint.h>
#include "polyvec.h"
void linebreakPackAndShift(const int16_t inputValue,uint8_t *firstOutput, uint8_t *secondOutput, int freeBitsInFirstOutput);
void linebreakUnpackAndShift(int16_t *output,uint8_t *firstInput, uint8_t *secondInput, int indikatorFirstOutput);
void simplePackAndShift(const int16_t inputValue,uint8_t *outputValue, int shift);
void simpleUnpackAndShift(uint8_t *inputValue, int16_t *ouputvalue, int shift);
void sk_serialization(const polyvec skpv, unsigned char *sk);
void sk_deserialization(polyvec *skpv, const unsigned char *sk);
#endif

#if SECRET_KEY_VARIANT==5
void  indcpa_dec(unsigned char *m,const unsigned char *c,const polyvec *sk);
#else
void indcpa_dec(unsigned char *m,const unsigned char *c,const unsigned char *sk);
#endif

#endif
