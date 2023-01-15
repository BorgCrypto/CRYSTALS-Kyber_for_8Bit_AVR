#include <stdint.h>
#include "P521KEM.h"
#include "print.h"
#include "avr.h"
#include "RNG.h"
#include "DH_KEM_P521_test_values_for_bench.h"

unsigned int ctr=0, newctr;
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
uint8_t pk[132];
uint8_t key[64];
uint8_t ct[133];

main(void)
{ volatile unsigned char a; /* Mark the beginning of the stack */
  WRITE_CANARY(&a);
  print("P521_KEM enc (with RNG): \n");
  RNG.begin("Test");
  for (int i = 0; i < 15; i++)
  {
    memcpy_P(pk,(uint8_t*)pgm_read_word(&(indexes[i*3])),132);
    P521KEM::encap(key,ct,pk,0);
    newctr =(unsigned int)&a - (unsigned int)&_end - stack_count(canary);
    ctr = (newctr>ctr)?newctr:ctr;
  }
  print_stack("encaps:",-1,ctr);
  serial_write(4);
  while(1) {};
  return 0;
}