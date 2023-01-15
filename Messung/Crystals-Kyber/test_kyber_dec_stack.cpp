#include <stdint.h>
#include "print.h"
#include "crystalskyber.h"
#include "RNG.h"
#include <avr/pgmspace.h>
#include "test_values_for_dec_bench.h"
unsigned int ctr=0,newctr;
unsigned char canary=0xFF;
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


main(void)
{ volatile unsigned char a; /* Mark the beginning of the stack */
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t key[CRYPTO_BYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  #if SECRET_KEY_VARIANT!=1
  print("K:");printllu(KYBER_K);print("\t");
  print("SK-Variant:");printllu(SECRET_KEY_VARIANT);print("\t");
  print("RNG:");printllu(Use_Deterministic_RNG);print("\n");
  for (int i = 0; i < 30; i+=2)
  {
  #else
  int i=0;
  #endif
    WRITE_CANARY(&a);
    RNG.begin("Test");
    memcpy_P(sk,(uint8_t*)pgm_read_word(&(values[i])),CRYPTO_SECRETKEYBYTES);
    memcpy_P(ct,(uint8_t*)pgm_read_word(&(values[i+1])),CRYPTO_CIPHERTEXTBYTES);
    CrystalsKyber::crypto_kem_dec(key,ct,sk);
    newctr =(unsigned int)&a - (unsigned int)&_end - stack_count(canary);
    ctr = (newctr>ctr)?newctr:ctr;
    #if SECRET_KEY_VARIANT!=1
  }
  print_stack("dec:",-1,ctr);
  #else
  printllu(ctr);
  #endif
  print("\n");
  serial_write(4);
  while(1) {};
  return 0;
}