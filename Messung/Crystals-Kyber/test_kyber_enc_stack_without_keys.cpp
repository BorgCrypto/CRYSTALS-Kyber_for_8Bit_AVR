#include <stdint.h>
#include "print.h"
#include "crystalskyber.h"
#include "RNG.h"
#include <avr/pgmspace.h>
#include "test_values_for_enc_bench.h"
unsigned int ctr=0,newctr;
unsigned char canary=0xFE;
volatile unsigned char *p;
extern unsigned char _end; 
extern unsigned char __stack; 

static unsigned int stack_count(unsigned char canary)
{
  const unsigned char *p = &_end;
  unsigned int c = 0;
  while(*p == canary && p <= &__stack)
  {
    p++;
    c++;
  }
  return c;
} 

#define WRITE_CANARY(X) {p=X;while(p>= &_end) *(p--) = canary;}

  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t key[CRYPTO_BYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];

main(void)
{ volatile unsigned char a; /* Mark the beginning of the stack */
  WRITE_CANARY(&a);
  print("K:");printllu(KYBER_K);print("\t");
  print("SK-Variant:");printllu(SECRET_KEY_VARIANT);print("\t");
  print("RNG:");printllu(Use_Deterministic_RNG);print("\n");
  RNG.begin("Test");
  for (int i = 0; i < 20; i++)
  {
    memcpy_P(pk,(uint8_t*)pgm_read_word(&(values[i])),CRYPTO_PUBLICKEYBYTES);
    CrystalsKyber::crypto_kem_enc(ct,key,pk);
    newctr =(unsigned int)&a - (unsigned int)&_end - stack_count(canary);
    ctr = (newctr>ctr)?newctr:ctr;
  }
  print_stack("encaps:",-1,ctr);
  serial_write(4);
  while(1) {};
  return 0;
}