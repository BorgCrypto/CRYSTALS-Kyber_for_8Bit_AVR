#include <stdint.h>
#include "print.h"
#include "crystalskyber.h"
#include "RNG.h"

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

  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
main(void)
{ volatile unsigned char a; /* Mark the beginning of the stack */
  #if SECRET_KEY_VARIANT !=1
  print("K:");printllu(KYBER_K);print("\t");
  print("SK-Variant:");printllu(SECRET_KEY_VARIANT);print("\t");
  print("RNG:");printllu(Use_Deterministic_RNG);print("\n");
  #endif
  for (int i = 0; i < 15; i++)
  {
    WRITE_CANARY(&a);
    RNG.begin("Test");
    CrystalsKyber::crypto_kem_keypair(pk,sk);
    newctr =(unsigned int)&a - (unsigned int)&_end - stack_count(canary);
    ctr = (newctr>ctr)?newctr:ctr;
  }
  #if SECRET_KEY_VARIANT !=1
  print_stack("Keygen:",-1,newctr);
  #else
  printllu(ctr);
  print("\n");
  #endif
  serial_write(4);
  while(1) {};
  return 0;
}