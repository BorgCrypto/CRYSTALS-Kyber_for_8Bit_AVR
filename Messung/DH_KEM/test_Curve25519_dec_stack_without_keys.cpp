#include <stdint.h>
#include "Curve25519KEM.h"
#include "print.h"
#include "avr.h"
#include "RNG.h"
#include "DH_KEM_Curve25519_test_values_for_bench.h"
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
  uint8_t sk[32];
  uint8_t key[32];
  uint8_t ct[32];

main(void)
{ volatile unsigned char a; /* Mark the beginning of the stack */

  print("Curve25519_KEM dec (with RNG, without keys): \n");
  for (int i = 0; i < 15; i++)
  {
    WRITE_CANARY(&a);
    RNG.begin("Test");
    memcpy_P(sk,(uint8_t*)pgm_read_word(&(indexes[i*3+1])),32);
    memcpy_P(ct,(uint8_t*)pgm_read_word(&(indexes[i*3+2])),32);
    Curve25519KEM::decap(key,ct,sk);
    newctr =(unsigned int)&a - (unsigned int)&_end - stack_count(canary);
    ctr = (newctr>ctr)?newctr:ctr;
  }
  print_stack("dec",-1,ctr);
  serial_write(4);
  while(1) {};
  return 0;
}