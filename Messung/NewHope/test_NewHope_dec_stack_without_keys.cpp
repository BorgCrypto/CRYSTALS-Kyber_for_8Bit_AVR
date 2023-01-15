#include <stdint.h>
#include "NewHope.h"
#include "print.h"
#include "avr.h"
#include "RNG.h"
#include "KEM_NewHope_test_values_for_dec_bench.h"
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

  NewHopePrivateKey sk;
  uint8_t key[32];
  uint8_t ct[2048];

main(void)
{ volatile unsigned char a; /* Mark the beginning of the stack */
  print("NewHope dec (with RNG): \n");
  for (int i = 0; i < 15; i+=2)
  {
    WRITE_CANARY(&a);
    RNG.begin("Test");
    memcpy_P(sk.seed,(uint8_t*)pgm_read_word(&(indexes[i])),32);
    memcpy_P(ct,(uint8_t*)pgm_read_word(&(indexes[i+1])),2048);
    NewHope::shareda(key,sk,ct);
    NewHope::shareda(key,sk,ct);
    newctr =(unsigned int)&a - (unsigned int)&_end - stack_count(canary);
    ctr = (newctr>ctr)?newctr:ctr;
  }
  print_stack("dec:",-1,ctr);
  serial_write(4);
  while(1) {};
  return 0;
}